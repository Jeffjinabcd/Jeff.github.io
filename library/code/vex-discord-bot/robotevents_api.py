"""Thin async client for the VEX Events API v2 (events.vex.com, formerly robotevents.com)."""

from datetime import datetime, timezone

import aiohttp

import config


class RobotEventsError(RuntimeError):
    pass


def _headers() -> dict:
    if not config.ROBOTEVENTS_API_KEY:
        raise RobotEventsError(
            "ROBOTEVENTS_API_KEY is not set. Get one at "
            "https://www.robotevents.com/api/v2 and add it to your .env file."
        )
    return {
        "Authorization": f"Bearer {config.ROBOTEVENTS_API_KEY}",
        "Accept": "application/json",
    }


async def get_upcoming_events(region: str, limit: int = 10) -> list[dict]:
    """Return upcoming events for a RobotEvents `region` string, soonest first."""
    today = datetime.now(timezone.utc).strftime("%Y-%m-%d")
    params = {
        "region": region,
        "start": today,
        "per_page": 50,
    }

    events: list[dict] = []
    url = f"{config.ROBOTEVENTS_BASE_URL}/events"

    async with aiohttp.ClientSession(headers=_headers()) as session:
        page_url = url
        page_params = params
        # Follow pagination until we've collected enough or run out of pages.
        for _ in range(10):  # hard cap so a bad response can't loop forever
            async with session.get(page_url, params=page_params) as resp:
                if resp.status == 401:
                    raise RobotEventsError(
                        "RobotEvents rejected the API key (401). Double-check "
                        "ROBOTEVENTS_API_KEY in your .env file."
                    )
                if resp.status != 200:
                    raise RobotEventsError(
                        f"RobotEvents API returned HTTP {resp.status}"
                    )
                payload = await resp.json()

            events.extend(payload.get("data", []))

            next_url = (payload.get("meta") or {}).get("next_page_url")
            if not next_url or len(events) >= 200:
                break
            page_url = next_url
            page_params = None  # next_page_url already includes query params

    events.sort(key=lambda e: e.get("start") or "")
    return events[:limit]


async def get_event_teams(event_id: int, limit: int = 100) -> list[dict]:
    """Return the teams registered for a given event."""
    teams: list[dict] = []
    url = f"{config.ROBOTEVENTS_BASE_URL}/events/{event_id}/teams"

    async with aiohttp.ClientSession(headers=_headers()) as session:
        page_url = url
        page_params = {"per_page": 100}
        for _ in range(5):
            async with session.get(page_url, params=page_params) as resp:
                if resp.status != 200:
                    raise RobotEventsError(f"RobotEvents API returned HTTP {resp.status}")
                payload = await resp.json()

            teams.extend(payload.get("data", []))

            next_url = (payload.get("meta") or {}).get("next_page_url")
            if not next_url or len(teams) >= limit:
                break
            page_url = next_url
            page_params = None

    return teams[:limit]


def event_url(event: dict) -> str:
    sku = event.get("sku", "")
    return f"https://events.vex.com/robot-competitions/vex-robotics-competition/{sku}.html"


def format_location(event: dict) -> str:
    loc = event.get("location") or {}
    parts = [loc.get("venue"), loc.get("city"), loc.get("region")]
    return ", ".join(p for p in parts if p)
