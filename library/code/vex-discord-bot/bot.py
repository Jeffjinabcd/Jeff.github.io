import asyncio
import logging

import discord
from discord.ext import commands

import config
import webapp
from todo_view import TodoBoardView

logging.basicConfig(level=logging.INFO)
log = logging.getLogger("vex-bot")

INTENTS = discord.Intents.default()

COGS = [
    "cogs.vex_events",
    "cogs.todo_admin",
    "cogs.todo_shared",
    "cogs.admin_config",
]


class VexBot(commands.Bot):
    def __init__(self):
        super().__init__(command_prefix="!", intents=INTENTS)

    async def setup_hook(self):
        for cog in COGS:
            await self.load_extension(cog)
            log.info("Loaded %s", cog)

        # Register the todo board's dropdowns so they keep working on messages
        # posted before a restart (routing is by custom_id, not by message).
        self.add_view(TodoBoardView())

        if config.DEV_GUILD_ID:
            guild = discord.Object(id=int(config.DEV_GUILD_ID))
            self.tree.copy_global_to(guild=guild)
            synced = await self.tree.sync(guild=guild)
            log.info("Synced %d commands to dev guild %s", len(synced), config.DEV_GUILD_ID)
        else:
            synced = await self.tree.sync()
            log.info("Synced %d global commands (may take up to an hour to appear)", len(synced))

        self.web_runner = await webapp.start_webapp(self)
        self.loop.create_task(webapp.events_cache_loop(self))

    async def on_ready(self):
        log.info("Logged in as %s (id %s)", self.user, self.user.id)


def main():
    if not config.DISCORD_TOKEN:
        raise SystemExit(
            "DISCORD_TOKEN is not set. Copy .env.example to .env and fill it in."
        )
    bot = VexBot()
    bot.run(config.DISCORD_TOKEN)


if __name__ == "__main__":
    main()
