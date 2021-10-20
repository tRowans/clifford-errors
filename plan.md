#Index Vectors

Under independent operation each lattice has index vectors for:

- Qubits (faces)
- Z stabs (edges)
- X stabs (cells) 
- Z logical representative
- X logical representative

as well as conversion vectors:

- faceToEdges
- edgeToFaces
- vertexToEdges
- edgeToVertices
- cellToFaces
- faceToCells
- vertexToFaces (rhombic only)
- faceToVertices (rhombic only)

with rhombic lattices also having faceToVertices and vertexToFaces conversion vectors.

Additionally, each lattice has vectors storing:

- Z error locations
- X error locations
- Z stabiliser syndrome
- X stabiliser syndrome
- defect locations

In total, this is 16 or 18 vectors per lattice and 52 vectors in total. Some functions need almost all of these so there needs to be a better way to manage them than just storing and passing them all individually. I think the best way is a lattice object for each lattice that just stores all the relevant vectors and then you can access them like

`rhombic1.edgeToFaces[i]`

The cubic code object will just have empty faceToVertices and vertexToFaces vectors. 

I'm changing cells to have the same index as the vertex at their centre. This means cell indexing doesn't increase as 0,1,2,3... but 0,2,4,... and also switches between odd and even numbers every time we change rows. This is a bit less intuitive but saves lots of annoying cell <-> vertex index conversion. I need to actually remember to do this in all the existing code I copy over though so putting this here to remind myself. 
