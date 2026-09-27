import discord

import database


def is_admin(member: discord.Member) -> bool:
    if member.guild_permissions.manage_guild:
        return True
    cfg = database.get_guild_config(member.guild.id)
    admin_role_id = cfg["admin_role_id"]
    if admin_role_id is None:
        return False
    return any(role.id == admin_role_id for role in member.roles)
