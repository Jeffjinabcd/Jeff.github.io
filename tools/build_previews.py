# build_previews.py  —  Bake fast GLB previews for every CAD file.
#
# Scans library/cad for .stl/.step/.stp, and for each one that has no up-to-date
# GLB, tessellates + decimates it (FreeCAD) and packs it to a small GLB (trimesh)
# under library/glb/, mirroring the folder structure.
#
# Run via:  python tools/build_previews.py    (from the repo root)

import os, sys, json, subprocess, time

REPO    = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
CAD     = os.path.join(REPO, "library", "cad")
GLB     = os.path.join(REPO, "library", "glb")
STAGE   = os.path.join(os.environ.get("TEMP", REPO), "glb_stage")
FREECAD = r"C:\Program Files\FreeCAD 1.1\bin\freecadcmd.exe"
TOOLS   = os.path.join(REPO, "tools")

def main():
    jobs = []
    for dp, _, fns in os.walk(CAD):
        for fn in fns:
            if not fn.lower().endswith(('.stl', '.step', '.stp')):
                continue
            src = os.path.join(dp, fn)
            rel = os.path.relpath(src, CAD)
            base = os.path.splitext(rel)[0]
            glb = os.path.join(GLB, base + ".glb")
            if os.path.exists(glb) and os.path.getmtime(glb) >= os.path.getmtime(src):
                continue                     # already up to date
            jobs.append({"src": src,
                         "stl_tmp": os.path.join(STAGE, base + ".stl"),
                         "glb": glb})

    print(f"[build_previews] {len(jobs)} file(s) need a preview")
    if not jobs:
        print("[build_previews] everything is up to date.")
        return

    jobs_path = os.path.join(STAGE, "jobs.json")
    os.makedirs(STAGE, exist_ok=True)
    json.dump(jobs, open(jobs_path, "w", encoding="utf-8"))

    t0 = time.time()
    print("[build_previews] stage 1/2 — FreeCAD tessellate + decimate ...")
    subprocess.run([FREECAD, os.path.join(TOOLS, "freecad_batch.py"), jobs_path])
    print("[build_previews] stage 2/2 — pack to GLB ...")
    subprocess.run([sys.executable, os.path.join(TOOLS, "glb_batch.py"), jobs_path])

    # clean staging STLs to save disk
    try:
        import shutil
        for j in jobs:
            if os.path.exists(j["stl_tmp"]):
                os.remove(j["stl_tmp"])
    except Exception:
        pass

    made = sum(1 for j in jobs if os.path.exists(j["glb"]))
    print(f"[build_previews] done — {made}/{len(jobs)} previews in {time.time()-t0:.0f}s")

if __name__ == "__main__":
    main()
