# sw-export.ps1  —  Auto-export SolidWorks files to STEP + STL
#
# Parts    → exported as .step and .stl beside the original file
# Assemblies → a subfolder named after the assembly, containing:
#               - assembly.step, assembly.stl
#               - each component as its own .stl
# Loose STLs → collected into a STL_Library folder
#
# Usage:
#   .\sw-export.ps1              # run once (SolidWorks must be open)
#   .\sw-export.ps1 -Watch       # keep running, re-exports every 5 min while SW is open
#   .\sw-export.ps1 -SetupTask   # install as a login startup task

param(
  [switch]$Watch,
  [switch]$SetupTask
)

# ── Configuration ─────────────────────────────────────────────────────────────
$SWFolders    = @(
  "C:\Users\jeffj\Desktop\SolidWorks",
  "C:\Users\jeffj\Desktop\Vex"
)
$STLLibrary   = "C:\Users\jeffj\Desktop\STL_Library"   # loose STL collection folder
$PollSeconds  = 300
# ─────────────────────────────────────────────────────────────────────────────

function Get-SW {
  try { return [System.Runtime.InteropServices.Marshal]::GetActiveObject("SldWorks.Application") }
  catch { return $null }
}

function Save-As($doc, $outPath) {
  $errors = 0
  $ok = $doc.Extension.SaveAs($outPath, 0, 1, $null, [ref]$errors, [ref]$null)
  return $ok -and $errors -eq 0
}

function Should-Export($srcPath, $outPath) {
  if (-not (Test-Path $outPath)) { return $true }
  return (Get-Item $srcPath).LastWriteTime -gt (Get-Item $outPath).LastWriteTime
}

function Ensure-Dir($path) {
  if (-not (Test-Path $path)) { New-Item -ItemType Directory -Force $path | Out-Null }
}

# ── Export a single part ──────────────────────────────────────────────────────
function Export-Part($sw, $partPath) {
  $dir      = Split-Path $partPath
  $base     = [IO.Path]::GetFileNameWithoutExtension($partPath)
  $stepPath = Join-Path $dir "$base.step"
  $stlPath  = Join-Path $dir "$base.stl"

  $needStep = Should-Export $partPath $stepPath
  $needSTL  = Should-Export $partPath $stlPath
  if (-not $needStep -and -not $needSTL) { return }

  $errors = 0; $warnings = 0
  $doc = $sw.OpenDoc6($partPath, 1, 1, "", [ref]$errors, [ref]$warnings)  # 1=part, read-only
  if (-not $doc) { Write-Host "  [SKIP] Could not open: $base" -ForegroundColor Yellow; return }

  if ($needStep) {
    if (Save-As $doc $stepPath) { Write-Host "  STEP  $base.step" -ForegroundColor Cyan }
    else                        { Write-Host "  [ERR] STEP failed: $base" -ForegroundColor Red }
  }
  if ($needSTL) {
    if (Save-As $doc $stlPath)  { Write-Host "  STL   $base.stl" -ForegroundColor Cyan }
    else                        { Write-Host "  [ERR] STL failed: $base" -ForegroundColor Red }
  }

  $sw.CloseDoc($partPath)
}

# ── Export an assembly (+ all top-level components) ───────────────────────────
function Export-Assembly($sw, $asmPath) {
  $base      = [IO.Path]::GetFileNameWithoutExtension($asmPath)
  $asmFolder = Join-Path (Split-Path $asmPath) $base
  Ensure-Dir $asmFolder

  $stepPath  = Join-Path $asmFolder "$base.step"
  $stlPath   = Join-Path $asmFolder "$base.stl"
  $needStep  = Should-Export $asmPath $stepPath
  $needSTL   = Should-Export $asmPath $stlPath

  $errors = 0; $warnings = 0
  $doc = $sw.OpenDoc6($asmPath, 2, 1, "", [ref]$errors, [ref]$warnings)  # 2=assembly
  if (-not $doc) { Write-Host "  [SKIP] Could not open assembly: $base" -ForegroundColor Yellow; return }

  Write-Host "  ASM   $base/" -ForegroundColor Magenta

  if ($needStep) {
    if (Save-As $doc $stepPath) { Write-Host "        → $base.step" -ForegroundColor Cyan }
  }
  if ($needSTL) {
    if (Save-As $doc $stlPath)  { Write-Host "        → $base.stl" -ForegroundColor Cyan }
  }

  # Export each top-level component as its own STL
  try {
    $comps = $doc.GetComponents($false)   # $false = top-level only
    if ($comps) {
      foreach ($comp in $comps) {
        $compDoc = $comp.GetModelDoc2()
        if (-not $compDoc) { continue }
        $compName = [IO.Path]::GetFileNameWithoutExtension($comp.GetPathName())
        if (-not $compName) { continue }
        $compSTL  = Join-Path $asmFolder "$compName.stl"
        if (-not (Test-Path $compSTL)) {
          if (Save-As $compDoc $compSTL) { Write-Host "        → $compName.stl" -ForegroundColor DarkCyan }
        }
      }
    }
  } catch {}

  $sw.CloseDoc($asmPath)
}

# ── Collect loose STL files into STL_Library ──────────────────────────────────
function Collect-LooseSTLs {
  Ensure-Dir $STLLibrary
  $copied = 0
  foreach ($folder in $SWFolders) {
    if (-not (Test-Path $folder)) { continue }
    Get-ChildItem $folder -Recurse -Filter "*.stl" | Where-Object {
      # Skip STLs that are already inside a folder we created (assembly export folders)
      $_.DirectoryName -notmatch [regex]::Escape($STLLibrary) -and
      $_.DirectoryName -notmatch '\\[^\\]+_export$'
    } | ForEach-Object {
      $dest = Join-Path $STLLibrary $_.Name
      if (-not (Test-Path $dest) -or $_.LastWriteTime -gt (Get-Item $dest).LastWriteTime) {
        Copy-Item $_.FullName $dest -Force
        $copied++
      }
    }
  }
  if ($copied -gt 0) { Write-Host "  STL_Library: copied $copied loose file(s)" -ForegroundColor DarkGray }
}

# ── Main export pass ──────────────────────────────────────────────────────────
function Run-Export {
  $sw = Get-SW
  if (-not $sw) {
    Write-Host "[$(Get-Date -Format 'HH:mm:ss')] SolidWorks not open — skipping" -ForegroundColor DarkGray
    Collect-LooseSTLs
    return
  }

  Write-Host "[$(Get-Date -Format 'HH:mm:ss')] SolidWorks found — exporting..." -ForegroundColor Green

  foreach ($folder in $SWFolders) {
    if (-not (Test-Path $folder)) {
      Write-Host "  [SKIP] Folder not found: $folder" -ForegroundColor Yellow
      continue
    }

    Write-Host "  Scanning: $folder" -ForegroundColor DarkGray

    # Parts first
    Get-ChildItem $folder -Recurse -Filter "*.sldprt" | ForEach-Object {
      Export-Part $sw $_.FullName
    }

    # Assemblies
    Get-ChildItem $folder -Recurse -Filter "*.sldasm" | ForEach-Object {
      Export-Assembly $sw $_.FullName
    }
  }

  Collect-LooseSTLs
  Write-Host "  Done." -ForegroundColor Green
}

# ── Startup task installer ────────────────────────────────────────────────────
function Install-Task {
  $scriptPath = $MyInvocation.ScriptName
  $regKey  = "HKCU:\SOFTWARE\Microsoft\Windows\CurrentVersion\Run"
  $regName = "JeffSWExport"
  $regVal  = "powershell.exe -NonInteractive -WindowStyle Hidden -File `"$scriptPath`" -Watch"
  Set-ItemProperty -Path $regKey -Name $regName -Value $regVal
  Write-Host "Installed — sw-export will run automatically on every login." -ForegroundColor Green
  Write-Host "To uninstall: Remove-ItemProperty -Path '$regKey' -Name '$regName'" -ForegroundColor DarkGray
}

# ── Entry point ───────────────────────────────────────────────────────────────
if ($SetupTask) { Install-Task; exit }

Write-Host "=== SW Export ===" -ForegroundColor Cyan
Run-Export

if ($Watch) {
  Write-Host "Watching — re-runs every $PollSeconds seconds. Ctrl+C to stop." -ForegroundColor DarkGray
  while ($true) {
    Start-Sleep -Seconds $PollSeconds
    Run-Export
  }
}
