"""Combined todo board: one message showing both lists, with dropdowns to
check items off. Undone items are labeled X1, X2, ...; done items are
labeled Y1, Y2, ... with Y1 being the most recently completed item.

Anyone in the server may toggle any item from these dropdowns; who is
allowed to add/edit/remove items is enforced by each cog's slash commands.
"""

import logging
from datetime import datetime, timezone
from typing import Optional

import discord

import database

log = logging.getLogger("vex-bot.todo")

ADMIN_SELECT_ID = "todo_admin_toggle_select"
SHARED_SELECT_ID = "todo_shared_toggle_select"


def _compute_order(items: list) -> list:
    """Return items ordered undone-first (by id), then done (most recently done first)."""
    undone = [i for i in items if not i["done"]]
    done = sorted((i for i in items if i["done"]), key=lambda i: i["done_at"] or "", reverse=True)
    return undone, done


def _labels_for(undone: list, done: list) -> dict:
    labels = {}
    for idx, item in enumerate(undone, start=1):
        labels[item["id"]] = f"X{idx}"
    for idx, item in enumerate(done, start=1):
        labels[item["id"]] = f"Y{idx}"
    return labels


def get_labeled_items(admin: bool, guild_id: int) -> list[dict]:
    """Items ordered undone-then-done, each tagged with its X#/Y# display label."""
    items = database.list_todos(admin, guild_id)
    undone, done = _compute_order(items)
    labels = _labels_for(undone, done)
    ordered = undone + done
    return [
        {"id": i["id"], "label": labels[i["id"]], "text": i["text"], "done": bool(i["done"])}
        for i in ordered
    ]


def build_board_embed(guild_id: int, guild_name: str) -> discord.Embed:
    embed = discord.Embed(title="📋 Team Todo Board", color=discord.Color.purple())

    for admin, heading in (
        (True, "🔒 Admin List — admin adds/removes, anyone checks off"),
        (False, "🤝 Shared List — anyone can add, edit, remove, check off"),
    ):
        ordered = get_labeled_items(admin, guild_id)
        lines = [
            f"{'✅' if i['done'] else '⬜'} **{i['label']}** {i['text']}" for i in ordered
        ]
        value = "\n".join(lines) if lines else "_Nothing here yet._"
        if len(value) > 1024:
            value = value[:1000] + "\n… (truncated)"
        embed.add_field(name=heading, value=value, inline=False)

    embed.set_footer(text=f"{guild_name} · pick an item below to check it off or reopen it")
    embed.timestamp = datetime.now(timezone.utc)
    return embed


class _TodoSelect(discord.ui.Select):
    def __init__(self, admin: bool, guild_id: Optional[int]):
        self.admin = admin
        options = [discord.SelectOption(label="(no items yet)", value="0")]
        disabled = True

        if guild_id is not None:
            ordered = get_labeled_items(admin, guild_id)[:25]
            if ordered:
                disabled = False
                options = [
                    discord.SelectOption(
                        label=f"{i['label']} {i['text'][:90]}",
                        value=str(i["id"]),
                        emoji="✅" if i["done"] else "⬜",
                    )
                    for i in ordered
                ]

        super().__init__(
            placeholder=("Admin list" if admin else "Shared list") + ": toggle an item...",
            options=options,
            min_values=1,
            max_values=1,
            disabled=disabled,
            custom_id=ADMIN_SELECT_ID if admin else SHARED_SELECT_ID,
        )

    async def callback(self, interaction: discord.Interaction):
        item_id = int(self.values[0])
        database.toggle_todo(self.admin, interaction.guild_id, item_id, interaction.user.id)
        embed = build_board_embed(interaction.guild_id, interaction.guild.name)
        view = TodoBoardView(interaction.guild_id)
        await interaction.response.edit_message(embed=embed, view=view)
        database.set_todo_board(interaction.guild_id, interaction.channel_id, interaction.message.id)


class TodoBoardView(discord.ui.View):
    def __init__(self, guild_id: Optional[int] = None):
        super().__init__(timeout=None)
        self.add_item(_TodoSelect(True, guild_id))
        self.add_item(_TodoSelect(False, guild_id))


async def refresh_board(bot, guild_id: int) -> None:
    """Re-render the persistent board message for a guild, if one is configured."""
    cfg = database.get_guild_config(guild_id)
    if cfg["todo_channel_id"] is None or cfg["todo_message_id"] is None:
        return

    channel = bot.get_channel(cfg["todo_channel_id"])
    if channel is None:
        try:
            channel = await bot.fetch_channel(cfg["todo_channel_id"])
        except discord.HTTPException as e:
            log.error("Can't reach todo board channel %s for guild %s: %s", cfg["todo_channel_id"], guild_id, e)
            return

    try:
        message = await channel.fetch_message(cfg["todo_message_id"])
    except discord.HTTPException as e:
        log.error("Can't fetch todo board message for guild %s: %s", guild_id, e)
        return

    embed = build_board_embed(guild_id, channel.guild.name)
    view = TodoBoardView(guild_id)
    try:
        await message.edit(embed=embed, view=view)
    except discord.HTTPException as e:
        log.error("Can't edit todo board message for guild %s: %s", guild_id, e)


async def post_new_board(channel: discord.TextChannel, guild_id: int) -> discord.Message:
    embed = build_board_embed(guild_id, channel.guild.name)
    view = TodoBoardView(guild_id)
    message = await channel.send(embed=embed, view=view)
    database.set_todo_board(guild_id, channel.id, message.id)
    return message
