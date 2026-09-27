"""Shared event-lookup logic used by both the Discord dashboard and the web app."""

from datetime import datetime

import robotevents_api

DASHBOARD_EVENT_COUNT = 5


async def get_events_dashboard_data(cfg) -> dict:
    """Returns {region, team_number, events: [...], error: str|None}."""
    region = cfg["region"]
    team_number = (cfg["team_number"] or "").upper()
    result = {"region": region, "team_number": team_number, "events": [], "error": None}

    try:
        events = await robotevents_api.get_upcoming_events(region, limit=DASHBOARD_EVENT_COUNT)
    except robotevents_api.RobotEventsError as e:
        result["error"] = str(e)
        return result

    for event in events:
        try:
            teams = await robotevents_api.get_event_teams(event["id"])
        except robotevents_api.RobotEventsError:
            teams = []

        team_numbers = [t.get("number", "?") for t in teams]
        registered = team_number in (n.upper() for n in team_numbers) if team_number else False

        start = event.get("start") or ""
        try:
            date_display = datetime.fromisoformat(start).strftime("%a %b %d, %Y")
        except ValueError:
            date_display = start[:10]

        result["events"].append(
            {
                "id": event.get("id"),
                "name": event.get("name", "Unknown event"),
                "url": robotevents_api.event_url(event),
                "date": date_display,
                "start": start,
                "location": robotevents_api.format_location(event) or "location TBD",
                "registered": registered,
                "teams": team_numbers,
            }
        )

    return result
