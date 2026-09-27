const POLL_MS = 4000;

let lastState = null;
let editingId = null; // `${list}-${id}` while an inline edit box is open
let confirmingId = null; // `${list}-${id}` while delete is awaiting a second click
let confirmingTimer = null;

const el = (id) => document.getElementById(id);
const toastEl = document.createElement("div");
toastEl.className = "toast";
document.body.appendChild(toastEl);

function toast(msg) {
  toastEl.textContent = msg;
  toastEl.classList.add("show");
  clearTimeout(toastEl._t);
  toastEl._t = setTimeout(() => toastEl.classList.remove("show"), 1800);
}

function setLive(ok) {
  el("liveDot").classList.toggle("offline", !ok);
  el("liveText").textContent = ok ? "live" : "reconnecting…";
}

function renderDiscordBanner(state) {
  const banner = el("discordBanner");
  const d = state.discord;
  if (!d || !d.events_error) {
    banner.classList.add("hidden");
    banner.innerHTML = "";
    return;
  }
  banner.classList.remove("hidden");
  banner.innerHTML = `<div class="inner">⚠️ Discord events dashboard isn't posting: ${escapeHtml(
    d.events_error
  )}</div>`;
}

async function api(path, options) {
  const res = await fetch(path, {
    headers: { "Content-Type": "application/json" },
    ...options,
  });
  if (!res.ok) {
    const body = await res.json().catch(() => ({}));
    throw new Error(body.error || `HTTP ${res.status}`);
  }
  return res.json();
}

function escapeHtml(s) {
  const div = document.createElement("div");
  div.textContent = s;
  return div.innerHTML;
}

// ---------- events ----------

function renderEvents(state) {
  const list = el("eventsList");
  const meta = el("eventsMeta");
  const data = state.events;

  if (!data) {
    list.innerHTML = '<p class="empty">Loading events…</p>';
    meta.textContent = "";
    return;
  }
  if (data.error) {
    list.innerHTML = `<div class="error-box">⚠️ ${escapeHtml(data.error)}</div>`;
    meta.textContent = "";
    return;
  }

  meta.textContent = `Region: ${data.region}${data.team_number ? " · Tracking team " + data.team_number : ""}`;

  if (!data.events.length) {
    list.innerHTML = '<p class="empty">No upcoming events found for this region.</p>';
    return;
  }

  list.innerHTML = data.events
    .map((ev) => {
      const regBadge = data.team_number
        ? `<span class="badge ${ev.registered ? "registered" : "not-registered"}">${
            ev.registered ? "✅ " + data.team_number + " registered" : "❌ " + data.team_number + " not registered"
          }</span>`
        : "";
      const teams = ev.teams.length
        ? `${ev.teams.length} team(s): ${ev.teams.map(escapeHtml).join(", ")}`
        : "No teams registered yet";
      return `
        <div class="event-card">
          <a href="${ev.url}" target="_blank" rel="noopener">${escapeHtml(ev.name)}</a>
          <div class="event-date">${escapeHtml(ev.date)}</div>
          <div class="event-location">${escapeHtml(ev.location)}</div>
          ${regBadge}
          <div class="team-list">${teams}</div>
        </div>`;
    })
    .join("");
}

// ---------- todos ----------

function renderTodoList(listName, items) {
  const ul = el(listName === "admin" ? "adminList" : "sharedList");
  if (!items.length) {
    ul.innerHTML = '<p class="empty">Nothing here yet — add one above.</p>';
    return;
  }

  ul.innerHTML = items
    .map((item) => {
      const key = `${listName}-${item.id}`;
      if (editingId === key) {
        return `
          <li class="todo-item" data-id="${item.id}" data-list="${listName}">
            <span class="todo-label">${item.label}</span>
            <input class="todo-text-input" type="text" value="${escapeHtml(item.text)}" maxlength="200" data-edit-input />
            <div class="todo-actions">
              <button class="btn icon" data-action="save-edit" title="Save">💾</button>
              <button class="btn icon" data-action="cancel-edit" title="Cancel">✖️</button>
            </div>
          </li>`;
      }
      const isConfirming = confirmingId === key;
      return `
        <li class="todo-item ${item.done ? "done" : ""}" data-id="${item.id}" data-list="${listName}">
          <input type="checkbox" class="todo-checkbox" ${item.done ? "checked" : ""} data-action="toggle" />
          <span class="todo-label">${item.label}</span>
          <span class="todo-text" data-action="start-edit" title="Click to edit">${escapeHtml(item.text)}</span>
          <div class="todo-actions">
            ${
              isConfirming
                ? `<button class="btn danger confirm" data-action="confirm-delete" title="Click again to confirm">Delete?</button>
                   <button class="btn icon" data-action="cancel-delete" title="Cancel">✖️</button>`
                : `<button class="btn danger" data-action="delete" title="Delete">🗑️</button>`
            }
          </div>
        </li>`;
    })
    .join("");
}

function renderTodos(state) {
  renderTodoList("admin", state.todos.admin);
  renderTodoList("shared", state.todos.shared);
}

async function toggleItem(listName, id) {
  const state = await api(`/api/todos/${id}/toggle`, {
    method: "POST",
    body: JSON.stringify({ list: listName }),
  });
  lastState.todos = state;
  renderTodos(lastState);
}

async function deleteItem(listName, id) {
  const state = await api(`/api/todos/${id}`, {
    method: "DELETE",
    body: JSON.stringify({ list: listName }),
  });
  lastState.todos = state;
  renderTodos(lastState);
  toast("Removed");
}

async function saveEdit(listName, id, text) {
  if (!text.trim()) return;
  const state = await api(`/api/todos/${id}`, {
    method: "PUT",
    body: JSON.stringify({ list: listName, text: text.trim() }),
  });
  editingId = null;
  lastState.todos = state;
  renderTodos(lastState);
}

async function addItem(listName) {
  const input = el(listName === "admin" ? "adminAddInput" : "sharedAddInput");
  const text = input.value.trim();
  if (!text) return;
  const state = await api("/api/todos", {
    method: "POST",
    body: JSON.stringify({ list: listName, text }),
  });
  input.value = "";
  lastState.todos = state;
  renderTodos(lastState);
}

function wireTodoList(listName) {
  const ul = el(listName === "admin" ? "adminList" : "sharedList");
  ul.addEventListener("click", async (e) => {
    const li = e.target.closest(".todo-item");
    if (!li) return;
    const id = Number(li.dataset.id);
    const action = e.target.dataset.action;

    try {
      if (action === "delete") {
        confirmingId = `${listName}-${id}`;
        renderTodos(lastState);
        clearTimeout(confirmingTimer);
        confirmingTimer = setTimeout(() => {
          confirmingId = null;
          renderTodos(lastState);
        }, 3000);
      } else if (action === "confirm-delete") {
        clearTimeout(confirmingTimer);
        confirmingId = null;
        await deleteItem(listName, id);
      } else if (action === "cancel-delete") {
        clearTimeout(confirmingTimer);
        confirmingId = null;
        renderTodos(lastState);
      } else if (action === "start-edit") {
        editingId = `${listName}-${id}`;
        renderTodos(lastState);
        const box = ul.querySelector("[data-edit-input]");
        if (box) {
          box.focus();
          box.select();
        }
      } else if (action === "save-edit") {
        const box = li.querySelector("[data-edit-input]");
        await saveEdit(listName, id, box.value);
      } else if (action === "cancel-edit") {
        editingId = null;
        renderTodos(lastState);
      }
    } catch (err) {
      toast("Error: " + err.message);
    }
  });

  ul.addEventListener("change", async (e) => {
    if (e.target.dataset.action !== "toggle") return;
    const li = e.target.closest(".todo-item");
    const id = Number(li.dataset.id);
    try {
      await toggleItem(listName, id);
    } catch (err) {
      toast("Error: " + err.message);
    }
  });

  ul.addEventListener("keydown", async (e) => {
    if (!e.target.matches("[data-edit-input]")) return;
    const li = e.target.closest(".todo-item");
    const id = Number(li.dataset.id);
    if (e.key === "Enter") {
      try {
        await saveEdit(listName, id, e.target.value);
      } catch (err) {
        toast("Error: " + err.message);
      }
    } else if (e.key === "Escape") {
      editingId = null;
      renderTodos(lastState);
    }
  });
}

// ---------- settings ----------

function wireSettings() {
  el("settingsBtn").addEventListener("click", () => {
    if (lastState) {
      el("regionInput").value = lastState.region || "";
      el("teamInput").value = lastState.team_number || "";
    }
    el("settingsForm").classList.toggle("hidden");
  });
  el("cancelSettingsBtn").addEventListener("click", () => {
    el("settingsForm").classList.add("hidden");
  });
  document.addEventListener("keydown", (e) => {
    if (e.key === "Escape" && !el("settingsForm").classList.contains("hidden")) {
      el("settingsForm").classList.add("hidden");
    }
  });
  el("saveSettingsBtn").addEventListener("click", async () => {
    const region = el("regionInput").value.trim();
    const team_number = el("teamInput").value.trim();
    try {
      await api("/api/config", { method: "POST", body: JSON.stringify({ region, team_number }) });
      el("settingsForm").classList.add("hidden");
      toast("Settings saved — refreshing events…");
      await refresh();
    } catch (err) {
      toast("Error: " + err.message);
    }
  });
  el("refreshEventsBtn").addEventListener("click", async () => {
    toast("Refreshing events…");
    try {
      await api("/api/events/refresh", { method: "POST" });
      await refresh();
      toast("Events updated");
    } catch (err) {
      toast("Error: " + err.message);
    }
  });
}

function wireAddButtons() {
  document.querySelectorAll(".add-btn").forEach((btn) => {
    btn.addEventListener("click", () => addItem(btn.dataset.list));
  });
  el("adminAddInput").addEventListener("keydown", (e) => {
    if (e.key === "Enter") addItem("admin");
  });
  el("sharedAddInput").addEventListener("keydown", (e) => {
    if (e.key === "Enter") addItem("shared");
  });
}

// ---------- polling ----------

async function refresh() {
  try {
    const state = await api("/api/state");
    lastState = state;
    setLive(true);
    el("guildName").textContent = state.guild_name || "";
    renderDiscordBanner(state);
    if (document.activeElement && document.activeElement.matches("[data-edit-input]")) {
      // don't clobber an in-progress edit while polling
      renderEvents(state);
      return;
    }
    renderEvents(state);
    renderTodos(state);
  } catch (err) {
    setLive(false);
  }
}

wireSettings();
wireAddButtons();
wireTodoList("admin");
wireTodoList("shared");
refresh();
setInterval(refresh, POLL_MS);
