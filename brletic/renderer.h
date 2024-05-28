#pragma once
#include "vertexarray.h"
#include "indexbuffer.h"
#include "shader.h"
#include "texture.h"
#include <vector>

class Renderer
{
public:
    static void clear();
    static void drawTriangles(const VertexArray &va, const IndexBuffer &ib, const Shader &shader);

    static void drawPoints(const VertexArray &va, const IndexBuffer &ib, const Shader &shader);

    static void unBind(/*const VertexArray &va, const IndexBuffer &ib, const Shader &shader*/);
};
