# Run via system python (trimesh). Reads jobs.json [{stl_tmp, glb}], packs each to GLB.
import sys, os, json, trimesh
jobs = json.load(open(sys.argv[1], encoding='utf-8'))
tot = 0
for j in jobs:
    stl, glb = j['stl_tmp'], j['glb']
    try:
        if not os.path.exists(stl): continue
        if os.path.exists(glb) and os.path.getmtime(glb) >= os.path.getmtime(stl): 
            tot += os.path.getsize(glb); continue
        os.makedirs(os.path.dirname(glb), exist_ok=True)
        m = trimesh.load(stl, force='mesh'); m.export(glb)
        tot += os.path.getsize(glb)
        print("GLB %.2fMB %s" % (os.path.getsize(glb)/1e6, os.path.basename(glb)))
    except Exception as e:
        print("GLBERR", os.path.basename(glb), str(e)[:80])
print("TOTAL_GLB_MB %.1f" % (tot/1e6))
