# sw-export.ps1  —  Export SolidWorks files to STEP + STL
#
# Folder layout (created beside the source files):
#
#   YourFolder/
#   ├── part.sldprt
#   ├── assembly.sldasm
#   ├── STL/                ← all part STLs here
#   │   ├── part.stl
#   │   └── assembly/       ← each assembly gets its own subfolder
#   │       ├── assembly.stl
#   │       ├── component_a.stl
#   │       └── component_b.stl
#   └── STEP/               ← all STEP files here
#       ├── part.step
#       └── assembly.step
#
# Deduplication: parts that appear in multiple assemblies are only
# exported once to STL/ — assembly subfolders link back to that file.
#
# Usage:
#   .\sw-export.ps1              — run once (SolidWorks must be open)
#   .\sw-export.ps1 -Watch       — re-run every 5 min while SW is open
#   .\sw-export.ps1 -SetupTask   — install as login startup task

param([switch]$Watch, [switch]$SetupTask)

$SWFolders   = @(
  "C:\Users\jeffj\Desktop\SolidWorks",
  "C:\Users\jeffj\Desktop\Vex"
)
$PollSeconds = 300

# ── Helpers ───────────────────────────────────────────────────────────────────
$utf8 = New-Object System.Text.UTF8Encoding $false

function Get-SW {
  try { return [System.Runtime.InteropServices.Marshal]::GetActiveObject("SldWorks.Application") }
  catch { return $null }
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
function Export-Assembly($sw, $asmPath, [hashtable]$exported) {
  $dir     = Split-Path $asmPath
  $base    = [IO.Path]::GetFileNameWithoutExtension($asmPath)
  $stlDir  = Join-Path $dir "STL"
  $stepDir = Join-Path $dir "STEP"
  $asmStlFolder = Join-Path $stlDir $base   # e.g. STL/Robot_v2/
  Ensure-Dir $asmStlFolder
  Ensure-Dir $stepDir

  $asmStl  = Join-Path $asmStlFolder "$base.stl"
  $asmStep = Join-Path $stepDir "$base.step"
  $needSTL  = Is-Newer $asmPath $asmStl
  $needSTEP = Is-Newer $asmPath $asmStep

  Write-Host "  ASM  $base/" -ForegroundColor Magenta

  $doc = Open-Doc $sw $asmPath 2
  if (-not $doc) { Write-Host "  [SKIP] $base (can't open)" -ForegroundColor Yellow; return }

  if ($needSTL  -and (Save-Doc $doc $asmStl))  { Write-Host "       → $base.stl"  -ForegroundColor Cyan }
  if ($needSTEP -and (Save-Doc $doc $asmStep)) { Write-Host "       → $base.step" -ForegroundColor Cyan }

  # Export top-level components as individual STLs into the assembly subfolder
  # Skip components already exported to the STL/ folder (avoids VEX duplicate clutter)
  try {
    $comps = $doc.GetComponents($false)
    if ($comps) {
      # collect unique component paths (VEX assemblies reuse the same parts heavily)
      $seen = @{}
      foreach ($comp in $comps) {
        $compPath = try { $comp.GetPathName() } catch { $null }
        if (-not $compPath -or $seen.ContainsKey($compPath)) { continue }
        $seen[$compPath] = $true

        $compBase   = [IO.Path]::GetFileNameWithoutExtension($compPath)
        $compStl    = Join-Path $asmStlFolder "$compBase.stl"
        $siblingStl = Join-Path $stlDir "$compBase.stl"   # already exported as standalone?

        if (Test-Path $siblingStl) {
          # Already exists in STL/ — just copy it, no need to re-export from SW
          if (-not (Test-Path $compStl)) {
            Copy-Item $siblingStl $compStl
            Write-Host "       → $compBase.stl (copied)" -ForegroundColor DarkCyan
          }
        } elseif (-not (Test-Path $compStl)) {
          $compDoc = try { $comp.GetModelDoc2() } catch { $null }
          if ($compDoc -and (Save-Doc $compDoc $compStl)) {
            Write-Host "       → $compBase.stl" -ForegroundColor DarkCyan
          }
        }
      }
    }
  } catch {}

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
