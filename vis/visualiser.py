import csv
import bpy
import bmesh
import mathutils
import math
import numpy as np
from bpy.app import handlers

#------------------------------#
#----------!SET THESE!----------#
path = "path/to/csv"
L = 6
vis_c = 1
vis_r1 = 1
vis_r2 = 1
#------------------------------#

#------------------------------LIB------------------------------

#----------I/O----------

def read_index_file(filename):
    indices = []
    with open("{}".format(filename), newline='') as f:
        reader = csv.reader(f)
        for line in reader:
            indices.append([int(i) for i in line])
    return indices

def read_all_files(name):
    data_dict = {}
    data_dict["vertices"] = read_index_file("{}vertices_{}.csv".format(path,name))
    data_dict["edges"] = read_index_file("{}edges_{}.csv".format(path,name))
    data_dict["xErrors"] = read_index_file("{}xErrors_{}.csv".format(path,name))
    data_dict["zErrors"] = read_index_file("{}zErrors_{}.csv".format(path,name))
    data_dict["xSyndrome"] = read_index_file("{}xSyndrome_{}.csv".format(path,name))
    data_dict["zSyndrome"] = read_index_file("{}zSyndrome_{}.csv".format(path,name))
    data_dict["defects"] = read_index_file("{}defects_{}.csv".format(path,name))
    return data_dict

#----------LATTICE STUFF----------

def index_to_coord(i,lattice):
    if lattice == "c":
        coord = [i % L, (i//L) % L, (i//(L*L)) % L]
    else:
        coord = [i % L, (i//L) % L, (i//(L*L)) % L, i//(L*L*L)]
        #Sort out periodic boundaries
        for i in range(2):
            if coord[i] == L-1 and coord[3] == 1:
                coord[i] = -1
        coord = [coord[0]+coord[3]*0.5, 
                 coord[1]+coord[3]*0.5,
                 coord[2]+coord[3]*0.5]
    return coord

def face_to_base_vertex(face,lattice):
    base_vertex = face//3
    direction = face % 6
    coord = index_to_coord(base_vertex,lattice)
    if ((sum(coord) % 2) == 1 and lattice == "r1"
            or (sum(coord) % 2) == 0 and lattice == "r2"):
        if direction < 3:
            base_vertex += 1
        else:
            base_vertex -= 1
    return base_vertex

def face_to_vertex_coords(face,lattice):
    if lattice == "c":
        direction = face % 3
        vertices = np.array([index_to_coord(face//3,lattice) for i in range(4)])
        if (direction == 0):
            vertices += np.array([[0,0,0],[1,0,0],[0,1,0],[1,1,0]])
        elif (direction == 1):
            vertices += np.array([[0,0,0],[1,0,0],[0,0,1],[1,0,1]])
        else:
            vertices += np.array([[0,0,0],[0,1,0],[0,0,1],[0,1,1]])
        vertices = vertices.tolist()
    else:
        base_vertex = face_to_base_vertex(face,lattice)
        vertices = np.array([index_to_coord(base_vertex,lattice) for i in range(4)])
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

def edge_to_vertex_coords(edge,lattice):
    if lattice == "c":
        direction = edge % 3
        vertices = [index_to_coord(edge//3, lattice) for i in range(2)]
        if (direction == 0):
            vertices[1][0] += 1
        elif (direction == 1):
            vertices[1][1] += 1
        else:
            vertices[1][2] += 1
    else:
        direction = edge % 4
        vertices = np.array([index_to_coord(edge//4, lattice) for i in range(2)])
        if direction == 0:
            vertices[1] += [0.5,0.5,-0.5]
        elif direction == 1:
            vertices[1] += [0.5,-0.5,0.5]
        elif direction == 2:
            vertices[1] += [-0.5,0.5,0.5]
        else:
            vertices[1] += [0.5,0.5,0.5]
    return vertices

#----------MODEL STUFF----------

def add_materials(mesh):
    for colour in ["Blue","Red","Yellow","White","Black"]:
        mat = bpy.data.materials.get(colour)
        mesh.materials.append(mat)
        
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

def build_vertices(bm, vertices, lattice):
    for vertex in vertices:
        coord = index_to_coord(vertex,lattice)
        bm.verts.new((coord[0], coord[1], coord[2]))

def build_edges(bm, edges, lattice):
    for edge in edges:
        edge = [mathutils.Vector(index_to_coord(v,lattice)) for v in edge]
        verts = [v for v in bm.verts if v.co in edge]
        bmesh.ops.contextual_create(bm, geom=verts)

def build_errors(bm, xErrors, zErrors, mat_dict, lattice):
    for error in xErrors:
        vertices = face_to_vertex_coords(error,lattice)
        face = [mathutils.Vector(coord) for coord in vertices]
        verts = [v for v in bm.verts if v.co in face]
        mat = "Blue"
        if (error in zErrors):
            mat = "Yellow"
        bmesh.ops.contextual_create(bm, geom=verts, mat_nr=mat_dict[mat])
        
    for error in zErrors:
        if (error in xErrors):
            continue
        vertices = face_to_vertex_coords(error,lattice)
        face = [mathutils.Vector(coord) for coord in vertices]
        verts = [v for v in bm.verts if v.co in face]
        bmesh.ops.contextual_create(bm, geom=verts, mat_nr=mat_dict["Red"]) 

def build_xSyndrome(bm, xSyndrome, lattice):
    for stab in xSyndrome:
        if lattice == "c":
            corner = index_to_coord(stab,lattice)
            cell = [i + 0.5 for i in corner]
        else:
            cell = index_to_coord(stab,lattice)
        translation = mathutils.Matrix.Translation(cell)
        bmesh.ops.create_uvsphere(bm, u_segments=32, v_segments=16, 
                                    radius=0.1, matrix=translation)

def build_zSyndrome(bm, zSyndrome, lattice):
    for stab in zSyndrome:
        vertices = edge_to_vertex_coords(stab,lattice)
        vertex1 = mathutils.Vector(vertices[0])
        vertex2 = mathutils.Vector(vertices[1])
        direction = vertex2 - vertex1
        if lattice == "c":
            if direction[0] != 0:
                rotation = mathutils.Matrix.Rotation(math.radians(90), 4, 'Y')
            elif direction[1] != 0:
                rotation = mathutils.Matrix.Rotation(math.radians(90), 4, 'X')
            else:
                rotation = mathutils.Matrix.Rotation(0, 4, "Z")
        else:
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
                                    radius1=0.05, radius2=0.05, depth=0.866, matrix=transform)

def build_defects(bm, defects, lattice):
    for defect in defects:
        translation = mathutils.Matrix.Translation(index_to_coord(defect,lattice))
        bmesh.ops.create_uvsphere(bm, u_segments=32, v_segments=16, 
                                    radius=0.1, matrix=translation)

def build_model(step, lattice_data, lattice, i):
   
    model_name = "{}_{}".format(step,lattice)
    model = new_object(model_name)
    
    mesh = model.data
    bm = bmesh.new()    
    add_materials(mesh)
    mat_dict = {mat.name: i for i, mat in enumerate(mesh.materials)}
    bm.from_mesh(mesh)
    build_vertices(bm, lattice_data["vertices"][0], lattice)
    build_edges(bm, lattice_data["edges"], lattice)
    build_errors(bm, lattice_data["xErrors"][i], 
                    lattice_data["zErrors"][i], mat_dict, lattice)
    bm.to_mesh(mesh)
    bm.free()

    xSynd = new_object("xSynd_{}".format(model_name))
    mat = bpy.data.materials.get("Black")
    xSynd.data.materials.append(mat)
    bm = bmesh.new()
    bm.from_mesh(xSynd.data)
    build_xSyndrome(bm, lattice_data["xSyndrome"][i], lattice)
    bm.to_mesh(xSynd.data)
    bm.free()
    
    zSynd = new_object("zSynd_{}".format(model_name))
    mat = bpy.data.materials.get("White")
    zSynd.data.materials.append(mat)
    bm = bmesh.new()
    bm.from_mesh(zSynd.data)
    build_zSyndrome(bm, lattice_data["zSyndrome"][i], lattice)
    build_defects(bm, lattice_data["defects"][i], lattice)
    bm.to_mesh(zSynd.data)
    bm.free()
    
    bpy.data.objects[model_name].select_set(True)
    bpy.data.objects["xSynd_{}".format(model_name)].select_set(True)
    bpy.data.objects["zSynd_{}".format(model_name)].select_set(True)
    bpy.context.view_layer.objects.active = bpy.data.objects[model_name]
    bpy.ops.object.join()
    
    if model_name != "1.Empty_c":
        model.hide_set(True)

    bpy.data.objects[model_name].select_set(False)

#--------------------MAIN--------------------

try:
    objs = bpy.data.objects
    objs.remove(objs['Cube'], do_unlink=True)
except:
    pass

try:
    for material in bpy.data.materials:
            bpy.data.materials.remove(material)
except:
    pass

new_mat = bpy.data.materials.new("Blue")
new_mat.diffuse_color = (0,0,1,1)
new_mat = bpy.data.materials.new("Red")
new_mat.diffuse_color = (1,0,0,1)
new_mat = bpy.data.materials.new("Yellow")
new_mat.diffuse_color = (1,1,0,1)
new_mat = bpy.data.materials.new("White")
new_mat.diffuse_color = (1,1,1,1)
new_mat = bpy.data.materials.new("Black")
new_mat.diffuse_color = (0,0,0,1)

steps = ["1.Empty",
         "2.Init_X_true_synd",
         "3.Init_X_synd_err",
         "4.Init_X_meas_corr",
         "5.Init_X_qubit_corr",
         "6.CCZ",
         "7.Depol_err",
         "8.Single_qubit_meas_true",
         "9.Single_qubit_meas_err",
         "10.Single_qubit_synd_calc",
         "11.Single_qubit_Z_corr",
         "12.Jump_corr",
         "13.Perfect_X_err_deco",
         "14.Perfect_Z_err_deco"]

if vis_c:
    data_c = read_all_files("C")
if vis_r1:
    data_r1 = read_all_files("R1")
if vis_r2:
    data_r2 = read_all_files("R2")


for i in range(len(steps)):
    if vis_c:
        build_model(steps[i], data_c, "c", i)
    if vis_r1:
        build_model(steps[i], data_r1, "r1", i)
    if vis_r2:
        build_model(steps[i], data_r2, "r2", i)
