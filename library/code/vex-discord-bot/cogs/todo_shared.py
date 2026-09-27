import discord
from discord import app_commands
from discord.ext import commands

import database
import todo_view


class TodoShared(commands.Cog):
    """Shared todo list. Every member (admin or not) can add/edit/remove/check off items."""

    todo = app_commands.Group(
        name="todo",
        description="Shared todo list (everyone can add, edit, and check off)",
        guild_only=True,
    )

    def __init__(self, bot: commands.Bot):
        self.bot = bot

    @todo.command(name="add", description="Add an item to the shared todo list")
    @app_commands.describe(text="The task to add")
    async def add(self, interaction: discord.Interaction, text: str):
        database.add_todo(False, interaction.guild_id, text, interaction.user.id)
        await interaction.response.send_message(f"✅ Added: {text}")
        await todo_view.refresh_board(self.bot, interaction.guild_id)

    @todo.command(name="edit", description="Edit an item's text on the shared todo list")
    @app_commands.describe(item_id="The item's underlying # id (from /todo ids), not its X/Y label", text="New text")
    async def edit(self, interaction: discord.Interaction, item_id: int, text: str):
        edited = database.edit_todo(False, interaction.guild_id, item_id, text, interaction.user.id)
        if edited:
            await interaction.response.send_message(f"✏️ Updated item #{item_id}: {text}")
            await todo_view.refresh_board(self.bot, interaction.guild_id)
        else:
            await interaction.response.send_message(f"No item #{item_id} found.", ephemeral=True)

    @todo.command(name="remove", description="Remove an item from the shared todo list")
    @app_commands.describe(item_id="The item's underlying # id (from /todo ids), not its X/Y label")
    async def remove(self, interaction: discord.Interaction, item_id: int):
        removed = database.remove_todo(False, interaction.guild_id, item_id)
        if removed:
            await interaction.response.send_message(f"🗑️ Removed item #{item_id}.")
            await todo_view.refresh_board(self.bot, interaction.guild_id)
        else:
            await interaction.response.send_message(f"No item #{item_id} found.", ephemeral=True)

    @todo.command(name="ids", description="List shared todo items with their underlying # ids (for /todo edit and remove)")
    async def ids(self, interaction: discord.Interaction):
        items = database.list_todos(False, interaction.guild_id)
        if not items:
            await interaction.response.send_message("Shared list is empty.", ephemeral=True)
            return
        lines = [f"#{i['id']} — {'✅' if i['done'] else '⬜'} {i['text']}" for i in items]
        await interaction.response.send_message("\n".join(lines), ephemeral=True)

    @todo.command(name="list", description="Show the combined team todo board")
    async def list_items(self, interaction: discord.Interaction):
        embed = todo_view.build_board_embed(interaction.guild_id, interaction.guild.name)
        view = todo_view.TodoBoardView(interaction.guild_id)
        await interaction.response.send_message(embed=embed, view=view)
        message = await interaction.original_response()
        database.set_todo_board(interaction.guild_id, interaction.channel_id, message.id)


async def setup(bot: commands.Bot):
    await bot.add_cog(TodoShared(bot))
