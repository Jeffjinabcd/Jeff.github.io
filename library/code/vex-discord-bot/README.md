# VEX Penn-West Team Dashboard

A local web app + Discord bot, both reading/writing the same data, so they
always stay in sync:

1. **Web app** (`http://127.0.0.1:8787`, only reachable from your own
   machine) — the easy way to use everything. No slash commands: type in a
   box, click checkboxes, click delete. Auto-refreshes every few seconds.
2. **Discord bot** — the same data, visible to your whole team in a channel:
   a live events dashboard message and a combined todo board message, each
   auto-updated. Slash commands (`/vex`, `/todo`, `/todo-admin`, `/config`)
   still work too, for anyone who prefers Discord.

Both interfaces show:
- **Events**: upcoming VEX events for a tracked region (default
  `Pennsylvania - West`, i.e. Penn-West), each with a link, date, whether
  your tracked team is registered, and the full list of registered teams.
- **Todo**: an admin-managed list and a shared list. Undone items are
  labeled `X1, X2, ...`; done items are labeled `Y1, Y2, ...` with `Y1`
  being the most recently completed item.

Everything only runs — and only updates — while `bot.py` is running on your
laptop.

## 1. Set up Python

Your `python` on PATH points at an Inkscape-bundled interpreter that can't
build dependencies. Use your Python 3.9 install instead, either via its full
path or the `py` launcher:

```bash
py -3.9 -m pip install -r requirements.txt
```

(Already done once in this checkout — dependencies are installed under that
interpreter.)

## 2. Create the Discord bot

1. Go to https://discord.com/developers/applications -> **New Application**.
2. **Bot** tab -> **Reset Token** -> copy it. This is your `DISCORD_TOKEN`.
   No privileged intents (Message Content / Members) are needed.
3. **OAuth2 -> URL Generator**: scopes = `bot`, `applications.commands`.
   Bot permissions = `Send Messages`, `Embed Links`, `Read Message History`.
   Open the generated URL and invite the bot to your server.
4. In Discord, make sure the bot can actually see and post in whichever
   channels you'll use for the events dashboard / todo board — a channel
   can have its own permission overwrites that block the bot even though
   its server-wide invite permissions look fine. If a channel was locked to
   specific roles, add the bot's role to that channel's permissions
   (View Channel + Send Messages).

## 3. Get a RobotEvents API token

RobotEvents moved from robotevents.com to events.vex.com in September 2026;
the API key page isn't under the normal "My Account" menu.

1. Log in at https://events.vex.com
2. Go to https://events.vex.com/api/v2/accessRequest/create and submit the
   access request form (approved instantly).
3. You'll land on an **Access Tokens** page — click **Create New Token**,
   name it (e.g. "discord-bot"), and copy it immediately. It's only shown
   once. This is your `ROBOTEVENTS_API_KEY`.

## 4. Configure

```bash
cp .env.example .env
```

Fill in `DISCORD_TOKEN` and `ROBOTEVENTS_API_KEY`. Optionally set
`DEV_GUILD_ID` (your server's ID) while testing — Discord commands sync to
that one server instantly instead of waiting up to an hour for a global
sync. `WEBAPP_PORT` defaults to `8787`; change it only if that port is
already used by something else on your machine.

## 5. Run it

```bash
py -3.9 bot.py
```

Then open **http://127.0.0.1:8787** in your browser — that's the whole app.

## Using the web app

- **Events panel**: shows the soonest 5 events for the tracked region.
  ⚙️ opens settings to change the region or tracked team number. 🔄 forces
  an immediate refresh instead of waiting for the 5-minute auto-refresh.
- **Todo panels**: type in the box and press Enter (or click Add). Click
  the checkbox to toggle done. Click the text to edit it inline (Enter to
  save, Esc to cancel). Click 🗑️, then the "Delete?" button that appears,
  to remove an item.
- If the yellow banner at the top appears, the Discord side of things (the
  channel dashboard/board) hit a permissions problem — the web app itself
  still works fine regardless.

## Using Discord (optional)

- `/vex channel <#channel>` (admin) — post the live events dashboard there.
- `/vex team <number>` / `/vex region <name>` (admin) — same settings as
  the web app's ⚙️ panel.
- `/vex upcoming [count]` / `/vex next` — one-off lookups, no setup needed.
- `/config todoboard <#channel>` (admin) — post the persistent combined
  todo board there.
- `/todo-admin add|edit|remove|ids|list` (add/edit/remove need admin) /
  `/todo add|edit|remove|ids|list` (open to everyone) — the `<id>` these
  take is the item's underlying database id, not its `X#`/`Y#` label; run
  `/todo-admin ids` or `/todo ids` to look it up. Easier to just use the
  web app for this.
- `/config adminrole <role>` — let a specific role (e.g. `@Team Captain`)
  count as admin, in addition to Manage Server.
- `/config show` — see current region/team/channels/admin-role settings.

## Notes / known limitations

- Data is stored locally in `data/bot.sqlite3`. Back this file up if it
  matters — it's gitignored.
- Nothing updates while `bot.py` isn't running. Stopping and rerunning it
  resumes updates (editing the same Discord messages, not posting new
  ones) as long as those messages weren't deleted while it was offline.
- The events dashboard fetches each shown event's registered-teams list on
  every refresh, so very large regions may take a few seconds to update.
- The Discord checklist dropdowns can only show 25 items at once (a Discord
  platform limit); the web app has no such limit. If a list ever grows past
  25 items, use the web app to check off the overflow ones.
- The web app has no login — anyone who can reach `127.0.0.1:8787` on your
  machine can use it. That's only you, since it isn't exposed to your
  network, so this is intentional, not a bug.
