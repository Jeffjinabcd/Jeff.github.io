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
      $exts -contains $_.Extension.ToLower() -and
      $_.FullName -notmatch '\\node_modules\\' -and
      $_.FullName -notmatch '\\.git\\'
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

  # Write split manifests per category (keeps browser fetch sizes small)
  $libDir = Join-Path $repoRoot "library"
  if (-not (Test-Path $libDir)) { New-Item -ItemType Directory -Force $libDir | Out-Null }

  $ts = (Get-Date).ToUniversalTime().ToString("o")
  $counts = @{}

  foreach ($cat in @('cad','code','images')) {
    $subset = $files | Where-Object { $_.category -eq $cat } |
              Sort-Object { [datetime]$_.modified } -Descending |
              Select-Object -First 500
    $counts[$cat] = $subset.Count
    [PSCustomObject]@{ generated = $ts; files = @($subset) } |
      ConvertTo-Json -Depth 5 |
      Set-Content "$libDir\manifest-$cat.json" -Encoding utf8
  }

  # Tiny index manifest (just counts — loaded on page open)
  [PSCustomObject]@{ generated = $ts; counts = $counts } |
    ConvertTo-Json | Set-Content "$libDir\manifest.json" -Encoding utf8

  Write-Host "  Synced $copied file(s), skipped $skipped" -ForegroundColor Green

  # Git commit & push
  Push-Location $repoRoot
  try {
    $null = git add -A
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
  $regKey     = "HKCU:\SOFTWARE\Microsoft\Windows\CurrentVersion\Run"
  $regName    = "JeffGitHubSync"
  $regValue   = "powershell.exe -NonInteractive -WindowStyle Hidden -File `"$scriptPath`" -Watch"

  Set-ItemProperty -Path $regKey -Name $regName -Value $regValue
  Write-Host "Installed — sync will start automatically on every login." -ForegroundColor Green
  Write-Host "Running first sync now..." -ForegroundColor DarkGray
  Sync-Library
  Write-Host ""
  Write-Host "To uninstall: Remove-ItemProperty -Path '$regKey' -Name '$regName'" -ForegroundColor DarkGray
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
