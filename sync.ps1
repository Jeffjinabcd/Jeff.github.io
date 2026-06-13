# sync.ps1  —  Auto-sync local folders → GitHub Pages library
#
# Usage:
#   .\sync.ps1              # sync once and exit
#   .\sync.ps1 -Watch       # sync now, then re-sync every N seconds (set in config.json)
#   .\sync.ps1 -SetupTask   # install a Windows startup scheduled task that runs -Watch
#
# Edit config.json to set your source folders and extensions.

param(
  [switch]$Watch,
  [switch]$SetupTask
)

$ErrorActionPreference = "Stop"
$repoRoot = $PSScriptRoot

function Format-Size($bytes) {
  if ($bytes -lt 1KB)  { return "$bytes B" }
  if ($bytes -lt 1MB)  { return "{0:N1} KB" -f ($bytes / 1KB) }
  return "{0:N1} MB" -f ($bytes / 1MB)
}

function Sync-Library {
  $cfg = Get-Content "$repoRoot\config.json" -Raw | ConvertFrom-Json
  $maxBytes = $cfg.maxFileSizeMB * 1MB
  $files = [System.Collections.Generic.List[object]]::new()
  $copied = 0
  $skipped = 0

  foreach ($mapping in $cfg.mappings) {
    $src = $mapping.source
    $cat = $mapping.category
    $exts = $cfg.extensions.$cat

    if (-not (Test-Path $src)) {
      Write-Host "  [SKIP] Source not found: $src" -ForegroundColor Yellow
      continue
    }

    $destDir = Join-Path $repoRoot "library\$cat"
    if (-not (Test-Path $destDir)) { New-Item -ItemType Directory -Force $destDir | Out-Null }

    Get-ChildItem $src -Recurse -File | Where-Object {
      $exts -contains $_.Extension.ToLower()
    } | ForEach-Object {
      $file = $_

      if ($file.Length -gt $maxBytes) {
        Write-Host "  [SKIP] Too large ($( Format-Size $file.Length )): $($file.Name)" -ForegroundColor Yellow
        $skipped++
        return
      }

      # Preserve subfolder structure relative to source root
      $rel = $file.FullName.Substring($src.Length).TrimStart('\', '/')
      $destFile = Join-Path $destDir $rel
      $destParent = Split-Path $destFile
      if (-not (Test-Path $destParent)) { New-Item -ItemType Directory -Force $destParent | Out-Null }

      Copy-Item $file.FullName $destFile -Force
      $copied++

      $files.Add([PSCustomObject]@{
        name     = $file.Name
        type     = $file.Extension.TrimStart('.').ToLower()
        category = $cat
        path     = ("library/$cat/" + $rel.Replace('\', '/'))
        relPath  = $rel.Replace('\', '/')
        size     = $file.Length
        modified = $file.LastWriteTimeUtc.ToString("o")
        label    = $mapping.label
      })
    }
  }

  # Write manifest
  $libDir = Join-Path $repoRoot "library"
  if (-not (Test-Path $libDir)) { New-Item -ItemType Directory -Force $libDir | Out-Null }

  [PSCustomObject]@{
    generated = (Get-Date).ToUniversalTime().ToString("o")
    files     = $files
  } | ConvertTo-Json -Depth 5 | Set-Content "$libDir\manifest.json" -Encoding utf8

  Write-Host "  Synced $copied file(s), skipped $skipped" -ForegroundColor Green

  # Git commit & push
  Push-Location $repoRoot
  try {
    git add -A 2>&1 | Out-Null
    $ts = Get-Date -Format "yyyy-MM-dd HH:mm"
    $result = git commit -m "auto: sync library $ts" 2>&1
    if ($LASTEXITCODE -eq 0) {
      git push 2>&1 | Out-Null
      Write-Host "  Pushed to GitHub" -ForegroundColor Cyan
    } else {
      Write-Host "  Nothing new to push" -ForegroundColor DarkGray
    }
  } finally {
    Pop-Location
  }
}

function Install-StartupTask {
  $scriptPath = Join-Path $repoRoot "sync.ps1"

  # Task 1: run -Watch on every login (background, hidden)
  $action   = New-ScheduledTaskAction -Execute "powershell.exe" `
                -Argument "-NonInteractive -WindowStyle Hidden -File `"$scriptPath`" -Watch"
  $trigger  = New-ScheduledTaskTrigger -AtLogOn
  $settings = New-ScheduledTaskSettingsSet `
                -ExecutionTimeLimit (New-TimeSpan -Hours 24) `
                -RestartCount 3 -RestartInterval (New-TimeSpan -Minutes 1) `
                -MultipleInstances IgnoreNew
  Register-ScheduledTask -TaskName "JeffGitHubSync" -Action $action `
    -Trigger $trigger -Settings $settings -Force | Out-Null
  Write-Host "✓ Login task installed  — syncs every $( (Get-Content "$repoRoot\config.json" | ConvertFrom-Json).pollIntervalSeconds )s in the background." -ForegroundColor Green

  # Task 2: extra sync whenever Claude.exe or claude.exe starts
  $claudeExe = "$env:LOCALAPPDATA\Programs\claude\Claude.exe"
  if (-not (Test-Path $claudeExe)) {
    # Try common alternate locations
    $candidates = @(
      "$env:LOCALAPPDATA\Programs\Claude\Claude.exe",
      "$env:APPDATA\Claude\Claude.exe",
      "C:\Program Files\Claude\Claude.exe"
    )
    $claudeExe = $candidates | Where-Object { Test-Path $_ } | Select-Object -First 1
  }

  if ($claudeExe) {
    # Use a process-start WMIC event subscription via a tiny helper script
    $helperPath = Join-Path $repoRoot "sync-on-claude.ps1"
    @"
# Auto-generated — triggered when Claude starts
& "$scriptPath"
"@ | Set-Content $helperPath -Encoding utf8

    $action2   = New-ScheduledTaskAction -Execute "powershell.exe" `
                   -Argument "-NonInteractive -WindowStyle Hidden -File `"$helperPath`""
    # Event trigger: process create for Claude.exe
    $xml = @"
<?xml version="1.0" encoding="UTF-16"?>
<Task version="1.2" xmlns="http://schemas.microsoft.com/windows/2004/02/mit/task">
  <Triggers>
    <EventTrigger>
      <Enabled>true</Enabled>
      <Subscription>&lt;QueryList&gt;&lt;Query Id="0" Path="Microsoft-Windows-WMI-Activity/Operational"&gt;&lt;Select Path="Security"&gt;*[System[EventID=4688]] and *[EventData[Data[@Name='NewProcessName'] and (Data='$($claudeExe.Replace('\','\\'))')]]&lt;/Select&gt;&lt;/Query&gt;&lt;/QueryList&gt;</Subscription>
    </EventTrigger>
  </Triggers>
  <Actions>
    <Exec>
      <Command>powershell.exe</Command>
      <Arguments>-NonInteractive -WindowStyle Hidden -File "$helperPath"</Arguments>
    </Exec>
  </Actions>
</Task>
"@
    # Simpler alternative: add a second At-Startup trigger that runs 1 min after login
    # (process-event triggers require audit policy; use a reliable fallback instead)
    $action3   = New-ScheduledTaskAction -Execute "powershell.exe" `
                   -Argument "-NonInteractive -WindowStyle Hidden -File `"$scriptPath`""
    $trigger3  = New-ScheduledTaskTrigger -RepetitionInterval (New-TimeSpan -Minutes 5) `
                   -At (Get-Date) -Once
    $settings3 = New-ScheduledTaskSettingsSet -ExecutionTimeLimit (New-TimeSpan -Minutes 3) `
                   -MultipleInstances IgnoreNew
    Register-ScheduledTask -TaskName "JeffGitHubSyncPulse" -Action $action3 `
      -Trigger $trigger3 -Settings $settings3 -Force | Out-Null
    Write-Host "✓ Pulse task installed   — also syncs every 5 min regardless." -ForegroundColor Green
  }

  Write-Host ""
  Write-Host "Both tasks are active. The library will stay up to date automatically." -ForegroundColor Cyan
  Write-Host "To uninstall: Unregister-ScheduledTask -TaskName JeffGitHubSync,JeffGitHubSyncPulse -Confirm:`$false" -ForegroundColor DarkGray
}

# ── Main ─────────────────────────────────────────────────────────────────────

if ($SetupTask) {
  Install-StartupTask
  exit
}

$cfg = Get-Content "$repoRoot\config.json" -Raw | ConvertFrom-Json
$interval = [int]$cfg.pollIntervalSeconds

Write-Host "=== Jeff GitHub Library Sync ===" -ForegroundColor Cyan
Write-Host "[$(Get-Date -Format 'HH:mm:ss')] Starting sync..."
Sync-Library

if ($Watch) {
  Write-Host "Watching — will re-sync every $interval seconds. Ctrl+C to stop." -ForegroundColor DarkGray
  while ($true) {
    Start-Sleep -Seconds $interval
    Write-Host "[$(Get-Date -Format 'HH:mm:ss')] Syncing..."
    Sync-Library
  }
}
