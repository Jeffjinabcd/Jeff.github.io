import discord
from discord import app_commands
from discord.ext import commands

import database
import permissions
import todo_view


class TodoAdmin(commands.Cog):
    """Admin-managed todo list. Only admins add/remove items; anyone can check them off."""

    todo_admin = app_commands.Group(
        name="todo-admin",
        description="Admin-managed todo list (anyone can check items off)",
        guild_only=True,
    )

    def __init__(self, bot: commands.Bot):
        self.bot = bot

    @todo_admin.command(name="add", description="[Admin] Add an item to the admin todo list")
    @app_commands.describe(text="The task to add")
    async def add(self, interaction: discord.Interaction, text: str):
        if not permissions.is_admin(interaction.user):
            await interaction.response.send_message(
                "Only admins can add items to this list. Try `/todo add` for the shared list.",
                ephemeral=True,
            )
            return
        database.add_todo(True, interaction.guild_id, text, interaction.user.id)
        await interaction.response.send_message(f"✅ Added: {text}")
        await todo_view.refresh_board(self.bot, interaction.guild_id)

    @todo_admin.command(name="edit", description="[Admin] Edit an item's text on the admin todo list")
    @app_commands.describe(item_id="The item's underlying # id (from /todo-admin ids), not its X/Y label", text="New text")
    async def edit(self, interaction: discord.Interaction, item_id: int, text: str):
        if not permissions.is_admin(interaction.user):
            await interaction.response.send_message(
                "Only admins can edit items on this list.", ephemeral=True
            )
            return
        edited = database.edit_todo(True, interaction.guild_id, item_id, text, interaction.user.id)
        if edited:
            await interaction.response.send_message(f"✏️ Updated item #{item_id}: {text}")
            await todo_view.refresh_board(self.bot, interaction.guild_id)
        else:
            await interaction.response.send_message(f"No item #{item_id} found.", ephemeral=True)

    @todo_admin.command(name="remove", description="[Admin] Remove an item from the admin todo list")
    @app_commands.describe(item_id="The item's underlying # id (from /todo-admin ids), not its X/Y label")
    async def remove(self, interaction: discord.Interaction, item_id: int):
        if not permissions.is_admin(interaction.user):
            await interaction.response.send_message(
                "Only admins can remove items from this list.", ephemeral=True
            )
            return
        removed = database.remove_todo(True, interaction.guild_id, item_id)
        if removed:
            await interaction.response.send_message(f"🗑️ Removed item #{item_id}.")
            await todo_view.refresh_board(self.bot, interaction.guild_id)
        else:
            await interaction.response.send_message(f"No item #{item_id} found.", ephemeral=True)

    @todo_admin.command(name="ids", description="List admin todo items with their underlying # ids (for /todo-admin remove)")
    async def ids(self, interaction: discord.Interaction):
        items = database.list_todos(True, interaction.guild_id)
        if not items:
            await interaction.response.send_message("Admin list is empty.", ephemeral=True)
            return
        lines = [f"#{i['id']} — {'✅' if i['done'] else '⬜'} {i['text']}" for i in items]
        await interaction.response.send_message("\n".join(lines), ephemeral=True)

    @todo_admin.command(name="list", description="Show the combined team todo board")
    async def list_items(self, interaction: discord.Interaction):
        embed = todo_view.build_board_embed(interaction.guild_id, interaction.guild.name)
        view = todo_view.TodoBoardView(interaction.guild_id)
        await interaction.response.send_message(embed=embed, view=view)
        message = await interaction.original_response()
        database.set_todo_board(interaction.guild_id, interaction.channel_id, message.id)


async def setup(bot: commands.Bot):
    await bot.add_cog(TodoAdmin(bot))
