# Run via freecadcmd. Reads jobs.json [{src, stl_tmp}], meshes+decimates each to a light STL.
import sys, os, json, time
import FreeCAD as App
import Part, MeshPart, Mesh as MeshMod
TARGET = 120000
jobs = json.load(open(sys.argv[2], encoding='utf-8'))
for j in jobs:
    src, out = j['src'], j['stl_tmp']
    try:
        if os.path.exists(out) and os.path.getmtime(out) >= os.path.getmtime(src):
            print("SKIP", os.path.basename(src)); continue
        os.makedirs(os.path.dirname(out), exist_ok=True)
        ext = os.path.splitext(src)[1].lower()
        if ext in ('.step', '.stp'):
            sh = Part.Shape(); sh.read(src)
            bb = sh.BoundBox
            d = max(bb.XLength, bb.YLength, bb.ZLength) * 0.003
            m = MeshPart.meshFromShape(Shape=sh, LinearDeflection=d, AngularDeflection=0.6, Relative=False)
        else:
            m = MeshMod.Mesh(src)
        if m.CountFacets > TARGET:
            try: m.decimate(0.05, 1.0 - TARGET/float(m.CountFacets))
            except Exception: pass
        m.write(out)
        print("OK", m.CountFacets, os.path.basename(src))
    except Exception as e:
        print("ERR", os.path.basename(src), str(e)[:80])
