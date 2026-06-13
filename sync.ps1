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

function Export-SWToSTL($mapping) {
  $src = $mapping.source
  if (-not (Test-Path $src)) { return }
  $parts = Get-ChildItem $src -Recurse -Include "*.sldprt","*.sldasm" -ErrorAction SilentlyContinue
  if (-not $parts -or $parts.Count -eq 0) { return }

  $sw = $null
  try { $sw = [System.Runtime.InteropServices.Marshal]::GetActiveObject("SldWorks.Application") } catch {}
  if (-not $sw) { return }  # only run if SolidWorks is already open

  $destDir = Join-Path $repoRoot "library\cad"
  foreach ($part in $parts) {
    $rel     = $part.FullName.Substring($src.Length).TrimStart('\/')
    $stlRel  = $rel -replace '\.(sldprt|sldasm)$','.stl'
    $stlPath = Join-Path $destDir $stlRel
    if (Test-Path $stlPath) { continue }
    $stlDir  = Split-Path $stlPath
    if (-not (Test-Path $stlDir)) { New-Item -ItemType Directory -Force $stlDir | Out-Null }
    try {
      $errors = 0; $warnings = 0
      $doc = $sw.OpenDoc6($part.FullName, 1, 1, "", [ref]$errors, [ref]$warnings)
      if ($doc) {
        $null = $doc.Extension.SaveAs($stlPath, 0, 1, $null, [ref]$errors, [ref]$null)
        $sw.CloseDoc($part.FullName)
        Write-Host "  SW export: $($part.Name) → STL" -ForegroundColor DarkCyan
      }
    } catch {}
  }
}

function Sync-Library {
  $cfg = Get-Content "$repoRoot\config.json" -Raw | ConvertFrom-Json
  $maxBytes = $cfg.maxFileSizeMB * 1MB
  $files = [System.Collections.Generic.List[object]]::new()
  $copied = 0
  $skipped = 0

  # Auto-export SolidWorks parts to STL if SW is already open
  $cfg.mappings | Where-Object { $_.category -eq 'cad' } | ForEach-Object { Export-SWToSTL $_ }

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

    $excludeDirs = @(
      'node_modules','.git','.history','__pycache__','.vs',
      'bin','obj','build','dist','.cache',
      'FurMark_win64','gpushark',       # GPU benchmark tool
      'vendor','packages','.dart_tool', # dependency managers
      'Thumbs.db','.DS_Store'
    )
    Get-ChildItem $src -Recurse -File | Where-Object {
      $f = $_
      $skip = $false
      foreach ($d in $excludeDirs) {
        if ($f.FullName -like "*\$d\*" -or $f.FullName -like "*\$d") { $skip = $true; break }
      }
      -not $skip -and $exts -contains $f.Extension.ToLower()
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

  $ts     = (Get-Date).ToUniversalTime().ToString("o")
  $counts = @{}
  $utf8   = New-Object System.Text.UTF8Encoding $false   # no BOM — required for browser JSON.parse

  foreach ($cat in @('cad','code','images')) {
    $subset = $files | Where-Object { $_.category -eq $cat } |
              Sort-Object { [datetime]$_.modified } -Descending |
              Select-Object -First 500
    $counts[$cat] = $subset.Count
    $json = [PSCustomObject]@{ generated = $ts; files = @($subset) } | ConvertTo-Json -Depth 5
    [System.IO.File]::WriteAllText("$libDir\manifest-$cat.json", $json, $utf8)
  }

  $idx = [PSCustomObject]@{ generated = $ts; counts = $counts } | ConvertTo-Json
  [System.IO.File]::WriteAllText("$libDir\manifest.json", $idx, $utf8)

  Write-Host "  Synced $copied file(s), skipped $skipped" -ForegroundColor Green

  # Git commit & push
  Push-Location $repoRoot
  try {
    $null = git add -A
    $ts = Get-Date -Format "yyyy-MM-dd HH:mm"
    $result = git commit -m "auto: sync library $ts" 2>&1
    if ($LASTEXITCODE -eq 0) {
      $null = git push 2>&1
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
