# sw-export.ps1  —  Export SolidWorks files to STEP + STL
#
# Folder layout (created beside the source files; .sldprt/.sldasm untouched):
#
#   YourFolder/
#   ├── part.sldprt
#   ├── assembly.sldasm
#   ├── STL/                ← one .stl per part AND per assembly (merged mesh)
#   │   ├── part.stl
#   │   └── assembly.stl    ← whole assembly as a single STL, not split up
#   └── STEP/               ← one .step per part and per assembly
#       ├── part.step
#       └── assembly.step
#
# Runs ONLY when SolidWorks is CLOSED: it starts a hidden SolidWorks instance in
# the background, exports everything, then shuts it down — so it never disturbs
# your live CAD session. If SolidWorks is already open, it skips.
#
# Usage:
#   .\sw-export.ps1              — run once now (skips if SW is open)
#   .\sw-export.ps1 -Watch       — re-check every 5 min, export when SW is closed
#
# Normally you don't run this directly — auto.ps1 drives it automatically.

param([switch]$Watch, [switch]$SetupTask)

$SWFolders   = @(
  "C:\Users\jeffj\Desktop\SolidWorks",
  "C:\Users\jeffj\Desktop\Vex"
)
$PollSeconds = 300

# ── Helpers ───────────────────────────────────────────────────────────────────
$utf8 = New-Object System.Text.UTF8Encoding $false

$script:ownSW = $false
function Get-SW {
  # Only run when SolidWorks is CLOSED — never disturb an active session.
  if (Get-Process -Name 'SLDWORKS' -ErrorAction SilentlyContinue) {
    Write-Host "  SolidWorks is open — skipping (export only runs when SW is closed)." -ForegroundColor DarkGray
    return $null
  }
  try {
    $sw = New-Object -ComObject SldWorks.Application   # starts a fresh instance
    $sw.Visible = $false                               # keep it hidden in the background
    $script:ownSW = $true
    return $sw
  } catch {
    Write-Host "  Could not start SolidWorks headless: $_" -ForegroundColor Yellow
    return $null
  }
}

function Close-SW($sw) {
  if ($script:ownSW -and $sw) {
    try { $sw.ExitApp() } catch {}
    try { [Runtime.InteropServices.Marshal]::ReleaseComObject($sw) | Out-Null } catch {}
    $script:ownSW = $false
    Write-Host "  Closed the hidden SolidWorks instance." -ForegroundColor DarkGray
  }
}

function Ensure-Dir($p) { if (-not (Test-Path $p)) { New-Item -ItemType Directory -Force $p | Out-Null } }

function Is-Newer($src, $dest) {
  -not (Test-Path $dest) -or ((Get-Item $src).LastWriteTime -gt (Get-Item $dest).LastWriteTime)
}

function Save-Doc($doc, $outPath) {
  $err = 0
  try { $doc.Extension.SaveAs($outPath, 0, 1, $null, [ref]$err, [ref]$null) | Out-Null }
  catch { return $false }
  return (Test-Path $outPath)
}

function Open-Doc($sw, $path, $type) {
  # type: 1=part, 2=assembly
  $e = 0; $w = 0
  try { return $sw.OpenDoc6($path, $type, 1, "", [ref]$e, [ref]$w) }
  catch { return $null }
}

# ── Export a single part (.sldprt) ────────────────────────────────────────────
# $exported: hashtable of already-exported sldprt paths (dedup across assemblies)
function Export-Part($sw, $partPath, [hashtable]$exported) {
  if ($exported.ContainsKey($partPath)) { return }   # already done this run

  $dir  = Split-Path $partPath
  $base = [IO.Path]::GetFileNameWithoutExtension($partPath)
  $stlDir  = Join-Path $dir "STL";  Ensure-Dir $stlDir
  $stepDir = Join-Path $dir "STEP"; Ensure-Dir $stepDir
  $stlOut  = Join-Path $stlDir  "$base.stl"
  $stepOut = Join-Path $stepDir "$base.step"

  $needSTL  = Is-Newer $partPath $stlOut
  $needSTEP = Is-Newer $partPath $stepOut
  if (-not $needSTL -and -not $needSTEP) { $exported[$partPath] = $true; return }

  $doc = Open-Doc $sw $partPath 1
  if (-not $doc) { Write-Host "  [SKIP] $base (can't open)" -ForegroundColor Yellow; return }

  if ($needSTL  -and (Save-Doc $doc $stlOut))  { Write-Host "  STL  $base.stl"  -ForegroundColor Cyan }
  if ($needSTEP -and (Save-Doc $doc $stepOut)) { Write-Host "  STEP $base.step" -ForegroundColor Cyan }

  $sw.CloseDoc($partPath)
  $exported[$partPath] = $true
}

# ── Export an assembly (.sldasm) ──────────────────────────────────────────────
# The whole assembly is exported as ONE merged STL + ONE STEP (no per-component
# files) — same flat layout as parts: STL/asm.stl and STEP/asm.step.
function Export-Assembly($sw, $asmPath, [hashtable]$exported) {
  $dir     = Split-Path $asmPath
  $base    = [IO.Path]::GetFileNameWithoutExtension($asmPath)
  $stlDir  = Join-Path $dir "STL";  Ensure-Dir $stlDir
  $stepDir = Join-Path $dir "STEP"; Ensure-Dir $stepDir

  $asmStl  = Join-Path $stlDir  "$base.stl"
  $asmStep = Join-Path $stepDir "$base.step"
  $needSTL  = Is-Newer $asmPath $asmStl
  $needSTEP = Is-Newer $asmPath $asmStep
  if (-not $needSTL -and -not $needSTEP) { $exported[$asmPath] = $true; return }

  Write-Host "  ASM  $base" -ForegroundColor Magenta

  $doc = Open-Doc $sw $asmPath 2
  if (-not $doc) { Write-Host "  [SKIP] $base (can't open)" -ForegroundColor Yellow; return }

  # SolidWorks exports an assembly to STL as a single merged mesh by default.
  if ($needSTL  -and (Save-Doc $doc $asmStl))  { Write-Host "       → $base.stl (single mesh)" -ForegroundColor Cyan }
  if ($needSTEP -and (Save-Doc $doc $asmStep)) { Write-Host "       → $base.step" -ForegroundColor Cyan }

  $sw.CloseDoc($asmPath)
  $exported[$asmPath] = $true
}

# ── Main scan ─────────────────────────────────────────────────────────────────
function Run-Export {
  $sw = Get-SW
  if (-not $sw) {
    Write-Host "[$(Get-Date -Format 'HH:mm:ss')] SolidWorks not open — skipping export" -ForegroundColor DarkGray
    return
  }

  Write-Host "[$(Get-Date -Format 'HH:mm:ss')] SolidWorks open — exporting..." -ForegroundColor Green

  # Force assemblies to export as ONE merged STL, not separate per-component files.
  # swSTLComponentsIntoOneFile = 218 (swUserPreferenceToggle_e)
  try { $sw.SetUserPreferenceToggle(218, $true) } catch {}

  $exported = @{}

  foreach ($folder in $SWFolders) {
    if (-not (Test-Path $folder)) { continue }
    Write-Host "  [$folder]" -ForegroundColor DarkGray

    # Parts first — so assemblies can copy from STL/ instead of re-exporting
    Get-ChildItem $folder -Recurse -Filter "*.sldprt" |
      Where-Object { $_.FullName -notmatch '\\(STL|STEP)\\' } |
      ForEach-Object { Export-Part $sw $_.FullName $exported }

    # Assemblies
    Get-ChildItem $folder -Recurse -Filter "*.sldasm" |
      Where-Object { $_.FullName -notmatch '\\(STL|STEP)\\' } |
      ForEach-Object { Export-Assembly $sw $_.FullName $exported }
  }

  Close-SW $sw
  Write-Host "  Export complete." -ForegroundColor Green
}

# ── Startup task ──────────────────────────────────────────────────────────────
function Install-Task {
  $me  = $PSCommandPath
  $key = "HKCU:\SOFTWARE\Microsoft\Windows\CurrentVersion\Run"
  Set-ItemProperty -Path $key -Name "JeffSWExport" `
    -Value "powershell.exe -NonInteractive -WindowStyle Hidden -File `"$me`" -Watch"
  Write-Host "Installed — sw-export runs automatically on every login." -ForegroundColor Green
  Write-Host "Uninstall: Remove-ItemProperty -Path '$key' -Name 'JeffSWExport'" -ForegroundColor DarkGray
}

# ── Entry ─────────────────────────────────────────────────────────────────────
if ($SetupTask) { Install-Task; exit }

Write-Host "=== SW Export ===" -ForegroundColor Cyan
Run-Export

if ($Watch) {
  Write-Host "Watching — checks every $PollSeconds s. Ctrl+C to stop." -ForegroundColor DarkGray
  while ($true) { Start-Sleep $PollSeconds; Run-Export }
}
