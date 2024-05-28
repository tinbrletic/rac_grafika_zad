#pragma once
#include "box.h"
#include "indexbuffer.h"
#include "vertexbufferlayout.h"
#include "vertexbuffer.h"
#include "vertexarray.h"

#include <memory>

/// Render četverokuta
struct QuadRenderer
{
    IndexBuffer ib;
    VertexBuffer vb;
    VertexBufferLayout layout;
    VertexArray va;
    explicit QuadRenderer(const Quad& b) : ib(b.indices), vb(b.vertices)
    {
        layout.addFloat(3);
        layout.addFloat(3);
        layout.addFloat(2);
        layout.addFloat(3);
        layout.addFloat(3);

        va.addBuffer(vb, layout);
    }
};

/// Render kutije
struct BoxRenderer
{
    std::vector<std::unique_ptr<QuadRenderer>> renderers;
    explicit BoxRenderer(const Box& b)
    {
        for(const auto& q: b.sides/*quads*/)
        {
            renderers.emplace_back(std::make_unique<QuadRenderer>(q));
        }
    }
};
