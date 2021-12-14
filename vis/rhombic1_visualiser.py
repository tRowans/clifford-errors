import csv
import bpy
import bmesh
import mathutils
import math
import numpy as np
from bpy.app import handlers

#------------------------------#
#----------!SET THESE!----------#
path = "/Users/tom/Documents/clifford-errors/vis/test_csv_r1/"
L = 6
#------------------------------#

#--------------------LIB--------------------

def read_index_file(filename):
    indices = []
    with open("{}".format(filename), newline='') as f:
        reader = csv.reader(f)
        for line in reader:
            indices.append([int(i) for i in line])
    return indices

def index_to_coord(i):
    coord = [i % L, (i//L) % L, (i//(L*L)) % L, i//(L*L*L)]
    #Sort out periodic boundaries
    for i in range(2):
        if coord[i] == L-1 and coord[3] == 1:
            coord[i] = -1
    coord = [coord[0]+coord[3]*0.5, 
             coord[1]+coord[3]*0.5,
             coord[2]+coord[3]*0.5]
    return coord

def face_to_base_vertex(face):
    base_vertex = face//3
    direction = face % 6
    coord = index_to_coord(base_vertex)
    if (sum(coord) % 2) == 1:
        if direction < 3:
            base_vertex += 1
        else:
            base_vertex -= 1
    return base_vertex

def face_to_vertex_coords(face):
    base_vertex = face_to_base_vertex(face)
    vertices = np.array([index_to_coord(base_vertex) for i in range(4)])
    print(vertices)
    direction = face % 6
    if direction == 0:
        vertices += np.array([[0,0,0],[0,1,1],[0.5,0.5,0.5],[-0.5,0.5,0.5]])
    elif direction == 1:
        vertices += np.array([[0,0,0],[1,0,1],[0.5,0.5,0.5],[0.5,-0.5,0.5]])
    elif direction == 2:
        vertices += np.array([[0,0,0],[1,1,0],[0.5,0.5,0.5],[0.5,0.5,-0.5]])
    elif direction == 3:
        vertices += np.array([[0,0,0],[0,1,-1],[0.5,0.5,-0.5],[-0.5,0.5,-0.5]])
    elif direction == 4:
        vertices += np.array([[0,0,0],[1,0,-1],[0.5,0.5,-0.5],[0.5,-0.5,-0.5]])
    else:
        vertices += np.array([[0,0,0],[1,-1,0],[0.5,-0.5,0.5],[0.5,-0.5,-0.5]])
    vertices = vertices.tolist()
    return vertices

def edge_to_vertex_coords(edge):
    direction = edge % 4
    vertices = np.array([index_to_coord(edge//4) for i in range(2)])
    if direction == 0:
        vertices[1] += [0.5,0.5,-0.5]
    elif direction == 1:
        vertices[1] += [0.5,-0.5,0.5]
    elif direction == 2:
        vertices[1] += [-0.5,0.5,0.5]
    else:
        vertices[1] += [0.5,0.5,0.5]
    return vertices

def setup_materials(mesh):
    for material in bpy.data.materials:
        material.user_clear()
        bpy.data.materials.remove(material)

    new_mat = bpy.data.materials.new("Blue")
    new_mat.diffuse_color = (0,0,1,1)
    mesh.materials.append(new_mat)
    new_mat = bpy.data.materials.new("Red")
    new_mat.diffuse_color = (1,0,0,1)
    mesh.materials.append(new_mat)
    new_mat = bpy.data.materials.new("Yellow")
    new_mat.diffuse_color = (1,1,0,1)
    mesh.materials.append(new_mat)
    new_mat = bpy.data.materials.new("White")
    new_mat.diffuse_color = (1,1,1,1)
    mesh.materials.append(new_mat)
    new_mat = bpy.data.materials.new("Black")
    new_mat.diffuse_color = (0,0,0,1)
    mesh.materials.append(new_mat)

def new_object(name):
    try:
        objs = bpy.data.objects
        objs.remove(objs[name], do_unlink=True)
    except:
        pass
    try:
        mesh = bpy.data.meshes[name+"_mesh"]
        mesh.user_clear()
        bpy.data.meshes.remove(mesh)
    except:
        pass
    new_mesh = bpy.data.meshes.new("{}_mesh".format(name))
    new_obj = bpy.data.objects.new(name, new_mesh)
    bpy.context.collection.objects.link(new_obj)
    return new_obj

def build_vertices(bm, vertices):
    for vertex in vertices:
        coord = index_to_coord(vertex)
        bm.verts.new((coord[0], coord[1], coord[2]))

def build_edges(bm, edges):
    for edge in edges:
        edge = [mathutils.Vector(index_to_coord(v)) for v in edge]
        verts = [v for v in bm.verts if v.co in edge]
        bmesh.ops.contextual_create(bm, geom=verts)

def build_Errors(bm, xErrors, zErrors, mat_dict):
    for error in xErrors:
        vertices = face_to_vertex_coords(error)
        face = [mathutils.Vector(coord) for coord in vertices]
        verts = [v for v in bm.verts if v.co in face]
        mat = "Blue"
        if (error in zErrors):
            mat = "Yellow"
            print("X error:")
            print(error)
        bmesh.ops.contextual_create(bm, geom=verts, mat_nr=mat_dict[mat])
    
    for error in zErrors:
        if (error in xErrors):
            print("Z error")
            print(error)
            continue
        vertices = face_to_vertex_coords(error)
        face = [mathutils.Vector(coord) for coord in vertices]
        verts = [v for v in bm.verts if v.co in face]
        bmesh.ops.contextual_create(bm, geom=verts, mat_nr=mat_dict["Red"])

def build_xSyndrome(bm, xSyndrome):
    for stab in xSyndrome:
        cell = index_to_coord(stab)
        translation = mathutils.Matrix.Translation(cell)
        bmesh.ops.create_uvsphere(bm, u_segments=32, v_segments=16, 
                                    diameter=0.1, matrix=translation)

def build_zSyndrome(bm, zSyndrome):
    for stab in zSyndrome:
        vertices = edge_to_vertex_coords(stab)
        vertex1 = mathutils.Vector(vertices[0])
        vertex2 = mathutils.Vector(vertices[1])
        direction = vertex2 - vertex1
        if direction[0] == direction[1] != direction[2]:
            rotation1 = mathutils.Matrix.Rotation(math.radians(55), 4, 'X')
            rotation2 = mathutils.Matrix.Rotation(math.radians(-45), 4, 'Z')
        elif direction[0] == direction[2] != direction[1]:
            rotation1 = mathutils.Matrix.Rotation(math.radians(55), 4, 'X')
            rotation2 = mathutils.Matrix.Rotation(math.radians(45), 4, 'Z')
        elif direction[1] == direction[2] != direction[0]:
            rotation1 = mathutils.Matrix.Rotation(math.radians(-55), 4, 'X')
            rotation2 = mathutils.Matrix.Rotation(math.radians(45), 4, 'Z')
        elif direction[0] == direction[1] == direction[2]:
            rotation1 = mathutils.Matrix.Rotation(math.radians(-55), 4, 'X')
            rotation2 = mathutils.Matrix.Rotation(math.radians(-45), 4, 'Z')
        rotation = rotation2 @ rotation1
        translation = mathutils.Matrix.Translation(vertex1 + direction/2)
        transform = translation @ rotation
        bmesh.ops.create_cone(bm, cap_ends=True, cap_tris=False, segments=12, 
                            diameter1=0.05, diameter2=0.05, depth=0.866, matrix=transform)
    
def build_defects(bm, defects):
    for defect in defects:
        translation = mathutils.Matrix.Translation(index_to_coord(defect))
        bmesh.ops.create_uvsphere(bm, u_segments=32, v_segments=16, 
                                    diameter=0.1, matrix=translation)

def build_model(step, vertices, edges, xErrors, zErrors, xSyndrome, zSyndrome, defects, L):
    
    model = new_object(step)
    
    mesh = model.data
    bm = bmesh.new()    
    setup_materials(mesh)
    mat_dict = {mat.name: i for i, mat in enumerate(mesh.materials)}
    bm.from_mesh(mesh)
    build_vertices(bm, vertices)
    build_edges(bm, edges)
    build_Errors(bm, xErrors, zErrors, mat_dict)
    bm.to_mesh(mesh)
    bm.free()

    xSynd = new_object("xSynd")
    mat = bpy.data.materials.get("Black")
    xSynd.data.materials.append(mat)
    bm = bmesh.new()
    bm.from_mesh(xSynd.data)
    build_xSyndrome(bm, xSyndrome)
    bm.to_mesh(xSynd.data)
    bm.free()
    
    zSynd = new_object("zSynd")
    mat = bpy.data.materials.get("White")
    zSynd.data.materials.append(mat)
    bm = bmesh.new()
    bm.from_mesh(zSynd.data)
    build_zSyndrome(bm, zSyndrome)
    build_defects(bm, defects)
    bm.to_mesh(zSynd.data)
    bm.free()
    
    bpy.data.objects[step].select_set(True)
    bpy.data.objects["xSynd"].select_set(True)
    bpy.data.objects["zSynd"].select_set(True)
    bpy.context.view_layer.objects.active = bpy.data.objects[step]
    bpy.ops.object.join()
    
    bpy.data.meshes.remove(bpy.data.meshes["xSynd_mesh"])
    bpy.data.meshes.remove(bpy.data.meshes["zSynd_mesh"])

#--------------------MAIN--------------------


try:
    objs = bpy.data.objects
    objs.remove(objs['Cube'], do_unlink=True)
except:
    pass

#steps = ["Empty",
#         "Init_X_true_synd",
#         "Init_X_synd_err",
#         "Init_X_meas_corr",
#         "Init_X_qubit_corr",
#         "CCZ",
#         "Depol_err",
#         "Single_qubit_meas_true",
#         "Single_qubit_meas_err",
#         "Single_qubit_synd_calc",
#         "Single_qubit_Z_corr",
#         "Jump_corr",
#         "Perfect_X_err_deco",
#         "Perfect_Z_err_deco"]

steps = ["Test"]

vertices = read_index_file("{}vertices.csv".format(path))
edges = read_index_file("{}edges.csv".format(path))
xErrors = read_index_file("{}xErrors.csv".format(path))
zErrors = read_index_file("{}zErrors.csv".format(path))
xSyndrome = read_index_file("{}xSyndrome.csv".format(path))
zSyndrome = read_index_file("{}zSyndrome.csv".format(path))
defects = read_index_file("{}defects.csv".format(path))

models = []
for i in range(len(steps)):
    build_model(steps[i], vertices[0], edges, xErrors[i], 
                 zErrors[i], xSyndrome[i], zSyndrome[i], defects[i], L)
    if (i != 0):
        model.hide_set(True)

