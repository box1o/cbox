import bpy, os
from mathutils import Vector, Matrix
from pathlib import Path
import random, math

# Config ---------------------------------------------------------------
OUTPUT_DIR = "/home/toor/Code/cbox/dataset"
IMG_W, IMG_H = 1920, 1080
MARKER_RADIUS = 0.010
BASE_SIZE = 1.0
NUM_RANDOM_MARKERS = 6
RANDOM_SPREAD = 2.5
SEED = 42

USE_DEFAULT_POSITIONS = True
MAX_CAMERAS = 7
INCLUDE_TOP = True

# Default deterministic camera positions (name, (x,y,z))
# 3 on +X side, 3 around other sides, 1 modest elevated (not very high)
DEFAULT_CAMERA_POSITIONS = [
    ("c0", (8.0, -1.4, 3.1)),
    ("c1", (8.0,  0.0, 3.0)),
    ("c2", (8.0,  1.4, 3.05)),
    ("c3", (-8.0, 0.0, 3.1)),
    ("c4", (0.0,  8.2, 3.0)),
    ("c5", (0.0, -8.2, 3.0)),
    ("c6", (2.0,  1.5, 8.5)),  # modest top, slight offset
]

# If not using defaults, fallback simple param set
RADIUS_RANGE = (7.5, 9.0)
HEIGHT_RANGE = (2.8, 3.4)
TOP_HEIGHT = 8.5
TOP_OFFSET = (2.0, 1.5)  # x,y offset if generated

# ---------------------------------------------------------------------

random.seed(SEED)
scene = bpy.context.scene
scene.render.engine = 'BLENDER_EEVEE_NEXT'
scene.render.resolution_x = IMG_W
scene.render.resolution_y = IMG_H
scene.render.image_settings.file_format = 'PNG'

world = bpy.data.worlds.get("World") or bpy.data.worlds.new("World")
scene.world = world
world.use_nodes = True
bg = world.node_tree.nodes.get("Background")
if bg:
    bg.inputs[0].default_value = (0,0,0,1)
    bg.inputs[1].default_value = 0

bpy.ops.object.select_all(action='SELECT')
bpy.ops.object.delete(use_global=False)

TARGET_POINT = Vector((0.0, 0.0, 0.4))

def look_at(obj, target, tilt_deg=0.0):
    d = (target - obj.location).normalized()
    if tilt_deg != 0.0:
        axis = Vector((0,1,0))
        ang = math.radians(tilt_deg)
        c = math.cos(ang); s = math.sin(ang); v = 1 - c
        kx, ky, kz = axis.x, axis.y, axis.z
        Rtilt = Matrix((
            (kx*kx*v + c,     kx*ky*v - kz*s, kx*kz*v + ky*s, 0),
            (ky*kx*v + kz*s,  ky*ky*v + c,    ky*kz*v - kx*s, 0),
            (kz*kx*v - ky*s,  kz*ky*v + kx*s, kz*kz*v + c,    0),
            (0,0,0,1)
        ))
        d = (Rtilt.to_3x3() @ d).normalized()
    z_axis = -d
    up = Vector((0,0,1))
    if abs(z_axis.dot(up)) > 0.999: up = Vector((0,1,0))
    x_axis = up.cross(z_axis).normalized()
    y_axis = z_axis.cross(x_axis).normalized()
    rot = Matrix((
        (x_axis.x,y_axis.x,z_axis.x,0),
        (x_axis.y,y_axis.y,z_axis.y,0),
        (x_axis.z,y_axis.z,z_axis.z,0),
        (0,0,0,1)
    ))
    obj.matrix_world = Matrix.Translation(obj.location) @ rot

def create_marker(loc, name, color):
    bpy.ops.mesh.primitive_uv_sphere_add(radius=MARKER_RADIUS, location=loc, segments=16, ring_count=12)
    o = bpy.context.active_object
    o.name = name
    mat = bpy.data.materials.new(name=f"Mat_{name}")
    mat.use_nodes = True
    nd = mat.node_tree.nodes
    nd.clear()
    em = nd.new(type='ShaderNodeEmission')
    em.inputs[0].default_value = (*color,1)
    em.inputs[1].default_value = 50.0
    out = nd.new(type='ShaderNodeOutputMaterial')
    mat.node_tree.links.new(em.outputs[0], out.inputs[0])
    o.data.materials.append(mat)
    return o

def world_to_cam(cam, coord):
    from bpy_extras.object_utils import world_to_camera_view
    co = world_to_camera_view(scene, cam, coord)
    px = co.x * IMG_W
    py = (1.0 - co.y) * IMG_H
    vis = 0.0 <= co.x <= 1.0 and 0.0 <= co.y <= 1.0 and co.z > 0
    return round(px,2), round(py,2), vis

def intrinsics(cam):
    focal_mm = cam.data.lens
    sensor_w = cam.data.sensor_width
    fx = (focal_mm / sensor_w) * IMG_W
    fy = fx
    cx = IMG_W * 0.5
    cy = IMG_H * 0.5
    return fx, fy, cx, cy

def add_camera(name, loc, tilt=False):
    bpy.ops.object.camera_add(location=loc)
    cam = bpy.context.active_object
    cam.name = name
    cam.data.lens = 35
    cam.data.sensor_width = 36
    look_at(cam, TARGET_POINT, tilt_deg=5.0 if tilt else 0.0)  # slight tilt only if top
    return cam

# Markers ---------------------------------------------------------------
MARKER_COLORS = [
    (1.0,0.0,0.0),(1.0,0.0,1.0),(0.0,0.3,1.0),(0.0,1.0,1.0),
    (0.0,1.0,0.0),(1.0,1.0,0.0),(1.0,0.5,0.0),(0.6,0.0,1.0),
    (1.0,0.4,0.6),(0.3,1.0,0.3),(0.8,0.2,0.2),(0.2,0.8,0.8),
    (0.9,0.9,0.1),(0.5,0.0,0.5),(0.0,0.5,0.5),
]
markers = []
mid = BASE_SIZE * 0.5
base = [(-mid,-mid,0),(mid,-mid,0),(mid,mid,0),(-mid,mid,0)]
for i,pos in enumerate(base):
    markers.append(create_marker(pos, f"m{i}", MARKER_COLORS[i % len(MARKER_COLORS)]))
idx = len(base)
for _ in range(NUM_RANDOM_MARKERS):
    x = random.uniform(-RANDOM_SPREAD, RANDOM_SPREAD)
    y = random.uniform(-RANDOM_SPREAD, RANDOM_SPREAD)
    z = random.uniform(0.2, RANDOM_SPREAD)
    markers.append(create_marker((x,y,z), f"m{idx}", MARKER_COLORS[idx % len(MARKER_COLORS)]))
    idx += 1

# Cameras ---------------------------------------------------------------
cameras = []
if USE_DEFAULT_POSITIONS:
    for name, loc in DEFAULT_CAMERA_POSITIONS:
        if name == "c6" and not INCLUDE_TOP:
            continue
        if len(cameras) >= MAX_CAMERAS:
            break
        tilt = (name == "c6")
        cameras.append(add_camera(name, Vector(loc), tilt=tilt))
else:
    # Fallback generation if needed
    # 3 line cameras on +X
    line_x = 8.0
    offsets = [-1.4, 0.0, 1.4]
    for k,off in enumerate(offsets):
        if len(cameras) >= MAX_CAMERAS: break
        cameras.append(add_camera(f"c{k}", Vector((line_x, off, 3.05))))
    # side random until max-1 if top included
    while len(cameras) < (MAX_CAMERAS - (1 if INCLUDE_TOP else 0)):
        r = random.uniform(*RADIUS_RANGE)
        ang = random.uniform(0, 2*math.pi)
        x = r * math.cos(ang)
        y = r * math.sin(ang)
        z = random.uniform(*HEIGHT_RANGE)
        name = f"c{len(cameras)}"
        cameras.append(add_camera(name, Vector((x,y,z))))
    if INCLUDE_TOP and len(cameras) < MAX_CAMERAS:
        cameras.append(add_camera(f"c{len(cameras)}", Vector((TOP_OFFSET[0], TOP_OFFSET[1], TOP_HEIGHT)), tilt=True))

# Export ---------------------------------------------------------------
Path(OUTPUT_DIR).mkdir(parents=True, exist_ok=True)
lines = []
for cam in cameras:
    scene.camera = cam
    fx, fy, cx, cy = intrinsics(cam)
    lines.append(f"camera {cam.name}")
    lines.append(f"fx {fx:.6f}")
    lines.append(f"fy {fy:.6f}")
    lines.append(f"cx {cx:.6f}")
    lines.append(f"cy {cy:.6f}")
    for m in markers:
        px, py, vis = world_to_cam(cam, m.location)
        if vis:
            lines.append(f"uv {m.name} {px:.6f} {py:.6f}")
        else:
            lines.append(f"uv {m.name} -1 -1")
    lines.append("")
    scene.render.filepath = os.path.join(OUTPUT_DIR, f"{cam.name}.png")
    scene.render.image_settings.color_mode = 'RGB'
    bpy.ops.render.render(write_still=True)

with open(os.path.join(OUTPUT_DIR, "scene.scn"), "w") as f:
    f.write("\n".join(lines))

print(f"Saved scene.scn with {len(cameras)} cameras and {len(markers)} markers in {OUTPUT_DIR}")
