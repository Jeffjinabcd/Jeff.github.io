"""Local web dashboard: an app-like alternative to Discord slash commands.

Runs inside the same process/event loop as the Discord bot, on
127.0.0.1 only (not exposed to the network), and reads/writes the same
SQLite database — so changes made here show up on the Discord dashboard
and todo board too, and vice versa.
"""

import asyncio
import logging
from pathlib import Path

from aiohttp import web

import config
import dashboard_data
import database
import todo_view

log = logging.getLogger("vex-bot.web")

WEB_DIR = Path(__file__).resolve().parent / "web"

# Web-triggered edits aren't tied to a Discord user; store as this sentinel.
WEB_USER_ID = 0

_events_cache = {"data": None, "guild_id": None}


def _active_guild(bot):
    if config.DEV_GUILD_ID:
        guild = bot.get_guild(int(config.DEV_GUILD_ID))
        if guild:
            return guild
    return bot.guilds[0] if bot.guilds else None


def _todo_payload(guild_id: int) -> dict:
    return {
        "admin": todo_view.get_labeled_items(True, guild_id),
        "shared": todo_view.get_labeled_items(False, guild_id),
    }


async def _refresh_events_cache(guild) -> dict:
    cfg = database.get_guild_config(guild.id)
    data = await dashboard_data.get_events_dashboard_data(cfg)
    _events_cache["data"] = data
    _events_cache["guild_id"] = guild.id
    return data


def create_app(bot) -> web.Application:
    app = web.Application()

    async def index(request):
        return web.FileResponse(WEB_DIR / "index.html")

    async def get_state(request):
        guild = _active_guild(bot)
        if guild is None:
            return web.json_response(
                {"error": "The bot isn't in a Discord server yet. Invite it first."}, status=503
            )
        cfg = database.get_guild_config(guild.id)
        events = _events_cache["data"] if _events_cache["guild_id"] == guild.id else None

        vex_cog = bot.get_cog("VexEvents")
        events_error = vex_cog.last_error.get(guild.id) if vex_cog else None

        return web.json_response(
            {
                "guild_name": guild.name,
                "region": cfg["region"],
                "team_number": cfg["team_number"] or "",
                "events": events,
                "todos": _todo_payload(guild.id),
                "discord": {
                    "events_channel_configured": cfg["announce_channel_id"] is not None,
                    "events_error": events_error,
                    "todo_board_configured": cfg["todo_channel_id"] is not None,
                },
            }
        )

    async def refresh_events(request):
        guild = _active_guild(bot)
        if guild is None:
            return web.json_response({"error": "no guild"}, status=503)
        data = await _refresh_events_cache(guild)
        vex_cog = bot.get_cog("VexEvents")
        if vex_cog:
            asyncio.create_task(vex_cog._refresh_guild_dashboard(guild.id))
        return web.json_response(data)

    async def update_config(request):
        guild = _active_guild(bot)
        if guild is None:
            return web.json_response({"error": "no guild"}, status=503)
        body = await request.json()
        region = (body.get("region") or "").strip()
        team_number = (body.get("team_number") or "").strip()
        if region:
            database.set_guild_region(guild.id, region)
        if team_number:
            database.set_team_number(guild.id, team_number)
        asyncio.create_task(refresh_events(request))
        return web.json_response({"ok": True})

    async def add_todo(request):
        guild = _active_guild(bot)
        if guild is None:
            return web.json_response({"error": "no guild"}, status=503)
        body = await request.json()
        admin = body.get("list") == "admin"
        text = (body.get("text") or "").strip()
        if not text:
            return web.json_response({"error": "text required"}, status=400)
        database.add_todo(admin, guild.id, text, WEB_USER_ID)
        await todo_view.refresh_board(bot, guild.id)
        return web.json_response(_todo_payload(guild.id))

    async def toggle_todo(request):
        guild = _active_guild(bot)
        if guild is None:
            return web.json_response({"error": "no guild"}, status=503)
        item_id = int(request.match_info["item_id"])
        body = await request.json()
        admin = body.get("list") == "admin"
        result = database.toggle_todo(admin, guild.id, item_id, WEB_USER_ID)
        if result is None:
            return web.json_response({"error": "item not found"}, status=404)
        await todo_view.refresh_board(bot, guild.id)
        return web.json_response(_todo_payload(guild.id))

    async def edit_todo(request):
        guild = _active_guild(bot)
        if guild is None:
            return web.json_response({"error": "no guild"}, status=503)
        item_id = int(request.match_info["item_id"])
        body = await request.json()
        admin = body.get("list") == "admin"
        text = (body.get("text") or "").strip()
        if not text:
            return web.json_response({"error": "text required"}, status=400)
        edited = database.edit_todo(admin, guild.id, item_id, text, WEB_USER_ID)
        if not edited:
            return web.json_response({"error": "item not found"}, status=404)
        await todo_view.refresh_board(bot, guild.id)
        return web.json_response(_todo_payload(guild.id))

    async def delete_todo(request):
        guild = _active_guild(bot)
        if guild is None:
            return web.json_response({"error": "no guild"}, status=503)
        item_id = int(request.match_info["item_id"])
        body = await request.json()
        admin = body.get("list") == "admin"
        removed = database.remove_todo(admin, guild.id, item_id)
        if not removed:
            return web.json_response({"error": "item not found"}, status=404)
        await todo_view.refresh_board(bot, guild.id)
        return web.json_response(_todo_payload(guild.id))

    app.router.add_get("/", index)
    app.router.add_get("/api/state", get_state)
    app.router.add_post("/api/events/refresh", refresh_events)
    app.router.add_post("/api/config", update_config)
    app.router.add_post("/api/todos", add_todo)
    app.router.add_post("/api/todos/{item_id}/toggle", toggle_todo)
    app.router.add_put("/api/todos/{item_id}", edit_todo)
    app.router.add_delete("/api/todos/{item_id}", delete_todo)
    app.router.add_static("/static/", WEB_DIR, show_index=False)

    return app


async def start_webapp(bot):
    app = create_app(bot)
    runner = web.AppRunner(app)
    await runner.setup()
    site = web.TCPSite(runner, "127.0.0.1", config.WEBAPP_PORT)
    try:
        await site.start()
    except OSError as e:
        log.error(
            "Couldn't start the web dashboard on port %s (%s). Is another copy of the bot "
            "already running? Set WEBAPP_PORT in .env to use a different port.",
            config.WEBAPP_PORT, e,
        )
        await runner.cleanup()
        return None
    log.info("Web dashboard running at http://127.0.0.1:%s", config.WEBAPP_PORT)
    return runner


async def events_cache_loop(bot):
    """Keep the events cache warm in the background so page loads are instant."""
    await bot.wait_until_ready()
    while True:
        guild = _active_guild(bot)
        if guild is not None:
            try:
                await _refresh_events_cache(guild)
            except Exception:
                log.exception("Background events cache refresh failed")
        await asyncio.sleep(config.EVENT_DASHBOARD_REFRESH_MINUTES * 60)
