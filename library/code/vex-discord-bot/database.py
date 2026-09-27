import sqlite3
from datetime import datetime, timezone
from typing import Optional

import config

config.DB_PATH.parent.mkdir(parents=True, exist_ok=True)

_conn = sqlite3.connect(config.DB_PATH, check_same_thread=False)
_conn.row_factory = sqlite3.Row
_conn.execute("PRAGMA foreign_keys = ON")

SCHEMA = """
CREATE TABLE IF NOT EXISTS guild_config (
    guild_id INTEGER PRIMARY KEY,
    region TEXT NOT NULL DEFAULT 'Pennsylvania - West',
    announce_channel_id INTEGER,
    admin_role_id INTEGER
);

CREATE TABLE IF NOT EXISTS todo_admin_items (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    guild_id INTEGER NOT NULL,
    text TEXT NOT NULL,
    created_by INTEGER NOT NULL,
    created_at TEXT NOT NULL,
    done INTEGER NOT NULL DEFAULT 0,
    done_by INTEGER,
    done_at TEXT,
    edited_by INTEGER,
    edited_at TEXT
);

CREATE TABLE IF NOT EXISTS todo_shared_items (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    guild_id INTEGER NOT NULL,
    text TEXT NOT NULL,
    created_by INTEGER NOT NULL,
    created_at TEXT NOT NULL,
    done INTEGER NOT NULL DEFAULT 0,
    done_by INTEGER,
    done_at TEXT,
    edited_by INTEGER,
    edited_at TEXT
);
"""

_conn.executescript(SCHEMA)
_conn.commit()

# --- lightweight migration: add columns introduced after the initial release ---
def _ensure_columns(table: str, columns: dict) -> None:
    existing = {row["name"] for row in _conn.execute(f"PRAGMA table_info({table})")}
    for col, decl in columns.items():
        if col not in existing:
            _conn.execute(f"ALTER TABLE {table} ADD COLUMN {col} {decl}")


_ensure_columns(
    "guild_config",
    {
        "team_number": "TEXT DEFAULT '32767A'",
        "events_message_id": "INTEGER",
        "todo_channel_id": "INTEGER",
        "todo_message_id": "INTEGER",
    },
)
_ensure_columns("todo_admin_items", {"edited_by": "INTEGER", "edited_at": "TEXT"})
_conn.commit()


def now_iso() -> str:
    return datetime.now(timezone.utc).isoformat()


# ---------- guild_config ----------

def get_guild_config(guild_id: int) -> sqlite3.Row:
    row = _conn.execute(
        "SELECT * FROM guild_config WHERE guild_id = ?", (guild_id,)
    ).fetchone()
    if row is None:
        _conn.execute(
            "INSERT INTO guild_config (guild_id, region) VALUES (?, ?)",
            (guild_id, config.DEFAULT_VEX_REGION),
        )
        _conn.commit()
        row = _conn.execute(
            "SELECT * FROM guild_config WHERE guild_id = ?", (guild_id,)
        ).fetchone()
    return row


def set_guild_region(guild_id: int, region: str) -> None:
    get_guild_config(guild_id)
    _conn.execute(
        "UPDATE guild_config SET region = ? WHERE guild_id = ?", (region, guild_id)
    )
    _conn.commit()


def set_team_number(guild_id: int, team_number: str) -> None:
    get_guild_config(guild_id)
    _conn.execute(
        "UPDATE guild_config SET team_number = ? WHERE guild_id = ?",
        (team_number, guild_id),
    )
    _conn.commit()


def set_announce_channel(guild_id: int, channel_id: Optional[int]) -> None:
    get_guild_config(guild_id)
    _conn.execute(
        "UPDATE guild_config SET announce_channel_id = ?, events_message_id = NULL "
        "WHERE guild_id = ?",
        (channel_id, guild_id),
    )
    _conn.commit()


def set_events_message(guild_id: int, message_id: Optional[int]) -> None:
    get_guild_config(guild_id)
    _conn.execute(
        "UPDATE guild_config SET events_message_id = ? WHERE guild_id = ?",
        (message_id, guild_id),
    )
    _conn.commit()


def set_todo_board(guild_id: int, channel_id: Optional[int], message_id: Optional[int]) -> None:
    get_guild_config(guild_id)
    _conn.execute(
        "UPDATE guild_config SET todo_channel_id = ?, todo_message_id = ? WHERE guild_id = ?",
        (channel_id, message_id, guild_id),
    )
    _conn.commit()


def set_admin_role(guild_id: int, role_id: Optional[int]) -> None:
    get_guild_config(guild_id)
    _conn.execute(
        "UPDATE guild_config SET admin_role_id = ? WHERE guild_id = ?",
        (role_id, guild_id),
    )
    _conn.commit()


def all_guild_configs() -> list[sqlite3.Row]:
    return _conn.execute("SELECT * FROM guild_config").fetchall()


# ---------- shared todo helpers (used for both todo tables) ----------

def _table(admin: bool) -> str:
    return "todo_admin_items" if admin else "todo_shared_items"


def add_todo(admin: bool, guild_id: int, text: str, created_by: int) -> int:
    cur = _conn.execute(
        f"INSERT INTO {_table(admin)} (guild_id, text, created_by, created_at) "
        "VALUES (?, ?, ?, ?)",
        (guild_id, text, created_by, now_iso()),
    )
    _conn.commit()
    return cur.lastrowid


def list_todos(admin: bool, guild_id: int) -> list[sqlite3.Row]:
    return _conn.execute(
        f"SELECT * FROM {_table(admin)} WHERE guild_id = ? ORDER BY id ASC",
        (guild_id,),
    ).fetchall()


def get_todo(admin: bool, guild_id: int, item_id: int) -> Optional[sqlite3.Row]:
    return _conn.execute(
        f"SELECT * FROM {_table(admin)} WHERE guild_id = ? AND id = ?",
        (guild_id, item_id),
    ).fetchone()


def remove_todo(admin: bool, guild_id: int, item_id: int) -> bool:
    cur = _conn.execute(
        f"DELETE FROM {_table(admin)} WHERE guild_id = ? AND id = ?",
        (guild_id, item_id),
    )
    _conn.commit()
    return cur.rowcount > 0


def edit_todo(admin: bool, guild_id: int, item_id: int, text: str, editor_id: int) -> bool:
    cur = _conn.execute(
        f"UPDATE {_table(admin)} SET text = ?, edited_by = ?, edited_at = ? "
        "WHERE guild_id = ? AND id = ?",
        (text, editor_id, now_iso(), guild_id, item_id),
    )
    _conn.commit()
    return cur.rowcount > 0


def toggle_todo(admin: bool, guild_id: int, item_id: int, user_id: int) -> Optional[sqlite3.Row]:
    item = get_todo(admin, guild_id, item_id)
    if item is None:
        return None
    new_done = 0 if item["done"] else 1
    if new_done:
        _conn.execute(
            f"UPDATE {_table(admin)} SET done = 1, done_by = ?, done_at = ? "
            "WHERE guild_id = ? AND id = ?",
            (user_id, now_iso(), guild_id, item_id),
        )
    else:
        _conn.execute(
            f"UPDATE {_table(admin)} SET done = 0, done_by = NULL, done_at = NULL "
            "WHERE guild_id = ? AND id = ?",
            (guild_id, item_id),
        )
    _conn.commit()
    return get_todo(admin, guild_id, item_id)
