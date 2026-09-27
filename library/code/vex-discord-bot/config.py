import os
from pathlib import Path

from dotenv import load_dotenv

load_dotenv()

DISCORD_TOKEN = os.environ.get("DISCORD_TOKEN", "")
ROBOTEVENTS_API_KEY = os.environ.get("ROBOTEVENTS_API_KEY", "")
DEV_GUILD_ID = os.environ.get("DEV_GUILD_ID") or None

BASE_DIR = Path(__file__).resolve().parent
DB_PATH = BASE_DIR / "data" / "bot.sqlite3"

DEFAULT_VEX_REGION = "Pennsylvania - West"
ROBOTEVENTS_BASE_URL = "https://events.vex.com/api/v2"
EVENT_DASHBOARD_REFRESH_MINUTES = 5

WEBAPP_PORT = int(os.environ.get("WEBAPP_PORT", "8787"))
