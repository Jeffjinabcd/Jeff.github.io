from typing import Optional

import discord
from discord import app_commands
from discord.ext import commands

import database
import permissions
import todo_view


class AdminConfig(commands.Cog):
    config_group = app_commands.Group(
        name="config", description="Bot configuration for this server", guild_only=True
    )

    def __init__(self, bot: commands.Bot):
        self.bot = bot

    @config_group.command(name="adminrole", description="[Admin] Set which role counts as 'admin' for bot commands")
    @app_commands.describe(role="Role to treat as admin (leave empty to clear and use Manage Server only)")
    async def adminrole(self, interaction: discord.Interaction, role: Optional[discord.Role] = None):
        if not permissions.is_admin(interaction.user):
            await interaction.response.send_message(
                "You need Manage Server permission to change this.", ephemeral=True
            )
            return
        database.set_admin_role(interaction.guild_id, role.id if role else None)
        if role:
            await interaction.response.send_message(
                f"✅ Members with the {role.mention} role (or Manage Server permission) are now bot admins."
            )
        else:
            await interaction.response.send_message(
                "✅ Cleared the admin role. Only members with Manage Server permission are bot admins now."
            )

    @config_group.command(
        name="todoboard",
        description="[Admin] Post a persistent, auto-synced combined todo board in a channel",
    )
    @app_commands.describe(channel="Channel to post the persistent todo board in")
    async def todoboard(self, interaction: discord.Interaction, channel: discord.TextChannel):
        if not permissions.is_admin(interaction.user):
            await interaction.response.send_message(
                "You need Manage Server permission (or the configured admin role) to do that.",
                ephemeral=True,
            )
            return
        await interaction.response.defer()
        try:
            await todo_view.post_new_board(channel, interaction.guild_id)
        except discord.HTTPException as e:
            await interaction.followup.send(
                f"⚠️ Couldn't post there: {e}. Check the bot has View Channel + Send Messages "
                f"in {channel.mention}."
            )
            return
        await interaction.followup.send(f"✅ Todo board will stay synced in {channel.mention}.")

    @config_group.command(name="show", description="Show this server's current bot configuration")
    async def show(self, interaction: discord.Interaction):
        cfg = database.get_guild_config(interaction.guild_id)
        events_channel = f"<#{cfg['announce_channel_id']}>" if cfg["announce_channel_id"] else "not set"
        todo_channel = f"<#{cfg['todo_channel_id']}>" if cfg["todo_channel_id"] else "not set"
        role = f"<@&{cfg['admin_role_id']}>" if cfg["admin_role_id"] else "none (Manage Server only)"
        embed = discord.Embed(title="Bot configuration", color=discord.Color.blue())
        embed.add_field(name="VEX region", value=cfg["region"], inline=False)
        embed.add_field(name="Tracked team number", value=cfg["team_number"] or "not set", inline=False)
        embed.add_field(name="Live events dashboard channel", value=events_channel, inline=False)
        embed.add_field(name="Persistent todo board channel", value=todo_channel, inline=False)
        embed.add_field(name="Extra admin role", value=role, inline=False)
        await interaction.response.send_message(embed=embed, ephemeral=True)


async def setup(bot: commands.Bot):
    await bot.add_cog(AdminConfig(bot))
