# Run via freecadcmd. Reads jobs.json [{src, stl_tmp}], meshes+decimates each to a light STL.
import sys, os, json, time
import FreeCAD as App
import Part, MeshPart, Mesh as MeshMod
# Variable face budget: every part keeps its own natural detail up to this ceiling.
# Simple parts stay tiny; complex/large parts keep up to CAP faces (~8 MB GLB, ~a few
# seconds to load) instead of being crushed to a chunky low-poly blob.
CAP = 550000
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
            dim = max(bb.XLength, bb.YLength, bb.ZLength)
            # fine tessellation: tight angular so curves stay smooth at any size
            m = MeshPart.meshFromShape(Shape=sh, LinearDeflection=max(dim * 0.0006, 0.05),
                                       AngularDeflection=0.2, Relative=False)
        else:
            m = MeshMod.Mesh(src)
            bb = m.BoundBox
            dim = max(bb.XLength, bb.YLength, bb.ZLength)
        if m.CountFacets > CAP:
            try: m.decimate(dim * 0.0004, 1.0 - CAP / float(m.CountFacets))
            except Exception: pass
        m.write(out)
        print("OK", m.CountFacets, os.path.basename(src))
    except Exception as e:
        print("ERR", os.path.basename(src), str(e)[:80])
