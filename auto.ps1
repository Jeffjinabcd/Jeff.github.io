# auto.ps1  —  Unified automation watcher for the portfolio site
#
#   • WEB SYNC (sync.ps1) runs whenever the Claude desktop WINDOW is open
#     (an actual window on screen — not just the background/tray process).
#
#   • SOLIDWORKS EXPORT (sw-export.ps1) runs only when SolidWorks is CLOSED and
#     some part/assembly is missing or older than its STL export. It spins up a
#     hidden SolidWorks instance, exports, and shuts it down — never disturbing
#     your CAD work.
#
# Usage:
#   .\auto.ps1            run the watch loop in this window
#   .\auto.ps1 -Install   install to run hidden on every login (recommended)
#   .\auto.ps1 -Once      run a single check and exit
#   .\auto.ps1 -Uninstall remove the login entry

param([switch]$Install, [switch]$Uninstall, [switch]$Once)

$ErrorActionPreference = 'Continue'
$repo        = $PSScriptRoot
$PollSeconds = 120
$SWFolders   = @("$env:USERPROFILE\Desktop\Vex")   # must match sw-export.ps1's scope
$RunKey      = "HKCU:\SOFTWARE\Microsoft\Windows\CurrentVersion\Run"

# throttle SolidWorks export attempts so a stubborn part can't relaunch SW every tick
$script:lastSWAttempt = [datetime]::MinValue

function Claude-WindowOpen {
  $p = Get-Process -Name 'claude' -ErrorAction SilentlyContinue |
       Where-Object { $_.MainWindowHandle -ne 0 -and $_.MainWindowTitle }
  return [bool]$p
}

function SW-Running {
  return [bool](Get-Process -Name 'SLDWORKS' -ErrorAction SilentlyContinue)
}

# Any .sldprt/.sldasm without an up-to-date STL export? (filesystem check only)
function SW-WorkPending {
  foreach ($root in $SWFolders) {
    if (-not (Test-Path $root)) { continue }
    $parts = Get-ChildItem $root -Recurse -File -Include *.sldprt,*.sldasm -ErrorAction SilentlyContinue |
             Where-Object { $_.Name -notlike '~$*' -and $_.FullName -notmatch '\\(STL|STEP)\\' }
    foreach ($f in $parts) {
      $base = [IO.Path]::GetFileNameWithoutExtension($f.Name)
      $stl  = Join-Path (Split-Path $f.FullName) "STL\$base.stl"
      if (-not (Test-Path $stl) -or (Get-Item $f.FullName).LastWriteTime -gt (Get-Item $stl).LastWriteTime) {
        return $true
      }
    }
  }
  return $false
}

function Tick {
  # 1) Publish to the web — only while the Claude window is open
  if (Claude-WindowOpen) {
    Write-Host "[$(Get-Date -Format 'HH:mm:ss')] Claude window open → syncing site" -ForegroundColor Cyan
    try { & "$repo\sync.ps1" } catch { Write-Host "  sync error: $_" -ForegroundColor Red }
  }

  # 2) Export SolidWorks files — only when SW is closed, work is pending, and not too soon
  if (-not (SW-Running) -and (SW-WorkPending) -and
      ((Get-Date) - $script:lastSWAttempt).TotalMinutes -ge 30) {
    Write-Host "[$(Get-Date -Format 'HH:mm:ss')] SolidWorks closed + parts pending → exporting (hidden)" -ForegroundColor Magenta
    $script:lastSWAttempt = Get-Date
    try { & "$repo\sw-export.ps1" } catch { Write-Host "  export error: $_" -ForegroundColor Red }
  }
}

if ($Uninstall) {
  try { Remove-ItemProperty -Path $RunKey -Name 'JeffAuto' -ErrorAction Stop; Write-Host "Removed JeffAuto from startup." -ForegroundColor Green }
  catch { Write-Host "JeffAuto was not installed." -ForegroundColor DarkGray }
  exit
}

if ($Install) {
  # Remove the older standalone startup entries so nothing double-runs
  foreach ($old in 'JeffGitHubSync','JeffSWExport') {
    try { Remove-ItemProperty -Path $RunKey -Name $old -ErrorAction Stop; Write-Host "Removed old task: $old" -ForegroundColor DarkGray } catch {}
  }
  Set-ItemProperty -Path $RunKey -Name 'JeffAuto' `
    -Value "powershell.exe -NonInteractive -WindowStyle Hidden -File `"$repo\auto.ps1`""
  Write-Host "Installed — auto.ps1 runs hidden on every login." -ForegroundColor Green
  Write-Host "  - Site syncs while the Claude window is open." -ForegroundColor Gray
  Write-Host "  - SolidWorks export runs when SW is closed and parts changed." -ForegroundColor Gray
  Write-Host "Uninstall later with: .\auto.ps1 -Uninstall" -ForegroundColor DarkGray
  exit
}

if ($Once) { Tick; exit }

Write-Host "=== Auto watcher running (every ${PollSeconds}s). Ctrl+C to stop. ===" -ForegroundColor Cyan
while ($true) { Tick; Start-Sleep -Seconds $PollSeconds }
