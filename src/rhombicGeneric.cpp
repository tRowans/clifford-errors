int findFace(vint &vertices, vvint &vertexToFaces, vvint &faceToVertices)
{
    std::sort(vertices.begin(), vertices.end());
    auto v0Faces = vertexToFaces[vertices[0]];
    for (const auto &face : v0Faces)
    {
        //just the two w=0 vertices is enough to specify a face
        if (faceToVertices[face][1] == vertices[1])
        {
            return face;
        }
    }
    return -1; //no face contains both of these vertices
}

int scalarProduct(std::vector<float> vec, int dir)
{
    //scalar product of a vector with one of the edge directions
    //but these directions can be negative
    int sign = 1;
    if (dir < 0) 
    {
        dir = -1*dir;
        sign = -1;
    }
    if (dir == 1) return sign*vec[0] + sign*vec[1] - sign*vec[2];   //{1,1,-1}
    else if (dir == 2) return sign*vec[0] - sign*vec[1] + sign*vec[2];  //{1,-1,1}
    else if (dir == 3) return -1*sign*vec[0] + sign*vec[1] + sign*vec[2];   //{-1,1,1}
    else if (dir == 4) return sign*vec[0] + sign*vec[1] + sign*vec[2];  //{1,1,1}
    else throw std::invalid_argument("Invalid direction");
}
