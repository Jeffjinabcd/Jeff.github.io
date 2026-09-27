import logging
from datetime import datetime, timezone

import discord
from discord import app_commands
from discord.ext import commands, tasks

import config
import dashboard_data
import database
import permissions
import robotevents_api

log = logging.getLogger("vex-bot.events")

TEAM_LIST_CHAR_LIMIT = 300


def _format_event_line(event: dict) -> str:
    start = event.get("start") or ""
    try:
        date_str = datetime.fromisoformat(start).strftime("%a %b %d, %Y")
    except ValueError:
        date_str = start[:10]
    name = event.get("name", "Unknown event")
    url = robotevents_api.event_url(event)
    location = robotevents_api.format_location(event)
    return f"**[{name}]({url})**\n{date_str} — {location or 'location TBD'}"


def _format_team_list(numbers: list[str]) -> str:
    if not numbers:
        return "none registered yet"
    text = ", ".join(numbers)
    if len(text) <= TEAM_LIST_CHAR_LIMIT:
        return f"{len(numbers)} team(s): {text}"
    truncated = text[:TEAM_LIST_CHAR_LIMIT].rsplit(",", 1)[0]
    return f"{len(numbers)} team(s): {truncated}, ..."


async def _build_dashboard_embed(cfg) -> discord.Embed:
    data = await dashboard_data.get_events_dashboard_data(cfg)

    embed = discord.Embed(
        title=f"📅 Upcoming VEX Events — {data['region']}",
        color=discord.Color.blue(),
    )
    subtitle = "Auto-refreshes every 5 minutes while the bot is running."
    if data["team_number"]:
        subtitle += f" Tracking team **{data['team_number']}**."
    embed.description = subtitle
    embed.timestamp = datetime.now(timezone.utc)

    if data["error"]:
        embed.add_field(name="⚠️ Error", value=data["error"], inline=False)
        return embed

    if not data["events"]:
        embed.add_field(name="No events found", value="Nothing upcoming in this region.", inline=False)
        return embed

    for event in data["events"]:
        value_lines = [f"[Event page]({event['url']})", event["location"]]
        if data["team_number"]:
            reg = "✅ registered" if event["registered"] else "❌ not registered"
            value_lines.append(f"Team {data['team_number']}: {reg}")
        value_lines.append(_format_team_list(event["teams"]))

        embed.add_field(
            name=f"{event['name']} — {event['date']}",
            value="\n".join(value_lines),
            inline=False,
        )

    embed.set_footer(text="Last updated")
    return embed


class VexEvents(commands.Cog):
    vex = app_commands.Group(
        name="vex", description="VEX event tracking (Penn West region)", guild_only=True
    )

    def __init__(self, bot: commands.Bot):
        self.bot = bot
        self.last_error: dict[int, str] = {}
        self.refresh_dashboards.start()

    def cog_unload(self):
        self.refresh_dashboards.cancel()

    async def _refresh_guild_dashboard(self, guild_id: int) -> None:
        cfg = database.get_guild_config(guild_id)
        if cfg["announce_channel_id"] is None:
            return

        channel = self.bot.get_channel(cfg["announce_channel_id"])
        if channel is None:
            try:
                channel = await self.bot.fetch_channel(cfg["announce_channel_id"])
            except discord.HTTPException as e:
                msg = f"Can't reach the events channel: {e}"
                log.error("guild %s: %s", guild_id, msg)
                self.last_error[guild_id] = msg
                return

        try:
            embed = await _build_dashboard_embed(cfg)
        except Exception as e:
            log.exception("Failed building dashboard embed for guild %s", guild_id)
            self.last_error[guild_id] = f"Failed building dashboard: {e}"
            return

        message = None
        if cfg["events_message_id"] is not None:
            try:
                message = await channel.fetch_message(cfg["events_message_id"])
            except discord.NotFound:
                message = None
            except discord.HTTPException as e:
                msg = f"Can't fetch the dashboard message: {e}"
                log.error("guild %s: %s", guild_id, msg)
                self.last_error[guild_id] = msg
                return

        if message is not None:
            try:
                await message.edit(embed=embed)
                self.last_error.pop(guild_id, None)
                return
            except discord.HTTPException as e:
                log.error("Can't edit dashboard message for guild %s: %s", guild_id, e)

        try:
            new_message = await channel.send(embed=embed)
            database.set_events_message(guild_id, new_message.id)
            log.info("Posted new events dashboard message %s in guild %s", new_message.id, guild_id)
            self.last_error.pop(guild_id, None)
        except discord.HTTPException as e:
            msg = f"Can't post in the events channel ({e}). Check the bot has View Channel + Send Messages there."
            log.error("guild %s: %s", guild_id, msg)
            self.last_error[guild_id] = msg

    @vex.command(name="upcoming", description="List upcoming VEX events in this server's region")
    @app_commands.describe(count="How many events to show (default 5, max 15)")
    async def upcoming(self, interaction: discord.Interaction, count: int = 5):
        count = max(1, min(count, 15))
        cfg = database.get_guild_config(interaction.guild_id)
        await interaction.response.defer()
        try:
            events = await robotevents_api.get_upcoming_events(cfg["region"], limit=count)
        except robotevents_api.RobotEventsError as e:
            await interaction.followup.send(f"⚠️ {e}", ephemeral=True)
            return

        if not events:
            await interaction.followup.send(
                f"No upcoming events found for region **{cfg['region']}**."
            )
            return

        embed = discord.Embed(
            title=f"Upcoming VEX events — {cfg['region']}",
            color=discord.Color.blue(),
            description="\n\n".join(_format_event_line(e) for e in events),
        )
        await interaction.followup.send(embed=embed)

    @vex.command(name="next", description="Show the next upcoming VEX event in this server's region")
    async def next_event(self, interaction: discord.Interaction):
        cfg = database.get_guild_config(interaction.guild_id)
        await interaction.response.defer()
        try:
            events = await robotevents_api.get_upcoming_events(cfg["region"], limit=1)
        except robotevents_api.RobotEventsError as e:
            await interaction.followup.send(f"⚠️ {e}", ephemeral=True)
            return

        if not events:
            await interaction.followup.send(
                f"No upcoming events found for region **{cfg['region']}**."
            )
            return

        event = events[0]
        start = event.get("start") or ""
        try:
            dt = datetime.fromisoformat(start)
            days_away = (dt.date() - datetime.now(timezone.utc).date()).days
            countdown = f"in {days_away} day{'s' if days_away != 1 else ''}" if days_away > 0 else "today"
        except ValueError:
            countdown = ""

        embed = discord.Embed(
            title=event.get("name", "Unknown event"),
            url=robotevents_api.event_url(event),
            description=f"{robotevents_api.format_location(event)}\n{countdown}",
            color=discord.Color.green(),
        )
        await interaction.followup.send(embed=embed)

    @vex.command(name="region", description="[Admin] Set the RobotEvents region this server tracks")
    @app_commands.describe(region='RobotEvents region name, e.g. "Pennsylvania - West"')
    async def set_region(self, interaction: discord.Interaction, region: str):
        if not permissions.is_admin(interaction.user):
            await interaction.response.send_message(
                "You need Manage Server permission (or the configured admin role) to do that.",
                ephemeral=True,
            )
            return
        database.set_guild_region(interaction.guild_id, region)
        await interaction.response.send_message(f"✅ This server now tracks region **{region}**.")
        await self._refresh_guild_dashboard(interaction.guild_id)

    @vex.command(name="team", description="[Admin] Set which team number the events dashboard highlights")
    @app_commands.describe(number='Your team number, e.g. "32767A"')
    async def set_team(self, interaction: discord.Interaction, number: str):
        if not permissions.is_admin(interaction.user):
            await interaction.response.send_message(
                "You need Manage Server permission (or the configured admin role) to do that.",
                ephemeral=True,
            )
            return
        database.set_team_number(interaction.guild_id, number)
        await interaction.response.send_message(f"✅ Now tracking team **{number.upper()}**.")
        await self._refresh_guild_dashboard(interaction.guild_id)

    @vex.command(
        name="channel",
        description="[Admin] Post a live, auto-updating events dashboard in a channel",
    )
    @app_commands.describe(channel="Channel to post the live dashboard in")
    async def set_channel(self, interaction: discord.Interaction, channel: discord.TextChannel):
        if not permissions.is_admin(interaction.user):
            await interaction.response.send_message(
                "You need Manage Server permission (or the configured admin role) to do that.",
                ephemeral=True,
            )
            return
        database.set_announce_channel(interaction.guild_id, channel.id)
        await interaction.response.defer()
        await self._refresh_guild_dashboard(interaction.guild_id)
        error = self.last_error.get(interaction.guild_id)
        if error:
            await interaction.followup.send(
                f"⚠️ Set the channel, but couldn't post there yet: {error}"
            )
        else:
            await interaction.followup.send(f"✅ Live events dashboard posted in {channel.mention}.")

    @tasks.loop(minutes=config.EVENT_DASHBOARD_REFRESH_MINUTES)
    async def refresh_dashboards(self):
        for cfg in database.all_guild_configs():
            if cfg["announce_channel_id"] is None:
                continue
            try:
                await self._refresh_guild_dashboard(cfg["guild_id"])
            except Exception:
                log.exception("Dashboard refresh failed for guild %s", cfg["guild_id"])

    @refresh_dashboards.before_loop
    async def before_refresh_dashboards(self):
        await self.bot.wait_until_ready()


async def setup(bot: commands.Bot):
    await bot.add_cog(VexEvents(bot))
