#pragma once
#include <vector>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <glm/gtx/string_cast.hpp>
/// Vertex
struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 tex_coo;
    glm::vec3 normal;
    glm::vec3 tangent;
};

/// Quad, 4 verteksa
struct Quad {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices = {0, 1, 2, 0, 2, 3};
    Quad(const glm::vec3& c, const float sz=1, const glm::vec2& max_tex_coo=glm::vec2(1, 1))
    {
        const auto u_max = max_tex_coo.x;
        const auto v_max = max_tex_coo.y;
        const float dl = sz/2.f;
        const glm::vec3 bottom_left(-dl, -dl, 0);
        Vertex v;
        // 0
        v.position = bottom_left;
        v.color = c;
        v.tex_coo = glm::vec2(0, 0);
        vertices.push_back(v);
        // 1
        //v.position.x = dl+sz;
        v.position += glm::vec3(sz, 0, 0);
        v.tex_coo = glm::vec2(u_max, 0);
        vertices.push_back(v);
        // 2
        //v.position.y = dl+sz;
        v.position += glm::vec3(0, sz, 0);
        v.tex_coo = glm::vec2(u_max, v_max);
        vertices.push_back(v);
        // 3
//        v.position.x = dl;
//        v.position.y = dl+sz;
        v.position -= glm::vec3(sz, 0, 0);
        v.tex_coo = glm::vec2(0, v_max);
        vertices.push_back(v);

        set_normals();

        const glm::vec3 tangent1 = glm::normalize(calc_tangent({0, 1, 2}));

        for (auto& v : vertices)
            v.tangent = tangent1;
    }

    glm::vec3 calc_normal() const
    {
        return glm::normalize(
            glm::cross(vertices[1].position - vertices[0].position,
                       vertices[2].position - vertices[0].position)
            );
    }
    void set_normals()
    {
        const auto normal = calc_normal();
        for(auto& v : vertices)
            v.normal = normal;
    }
    void invert_normals()
    {
        for(auto& v : vertices)
            v.normal *= -1.f;
    }

    glm::vec3 calc_tangent(const std::vector<unsigned int> &indices) const
    {
        const glm::vec3& pos1 = vertices[indices[0]].position;
        const glm::vec3& pos2 = vertices[indices[1]].position;
        const glm::vec3& pos3 = vertices[indices[2]].position;

        const glm::vec2& uv1 = vertices[indices[0]].tex_coo;
        const glm::vec2& uv2 = vertices[indices[1]].tex_coo;
        const glm::vec2& uv3 = vertices[indices[2]].tex_coo;

        glm::vec3 edge1 = pos2 - pos1;
        glm::vec3 edge2 = pos3 - pos1;

        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;

        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
        glm::vec3 tangent;
        tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
        return tangent;
    }
};

/// Box, 6 quada
struct Box {
    std::vector<Quad> sides;
    Box(const glm::vec3& c, const float sz = 1)
    {
        // front?
        Quad qf(c, sz);
        for(auto& v :qf.vertices)
        {
            v.position = glm::translate(glm::mat4(1),
                                        glm::vec3(0, 0, sz/2))
                         *glm::vec4(v.position, 1);
        }
        qf.set_normals();
        qf.invert_normals();
        sides.emplace_back(qf);
        // back?
        Quad qb(c, sz);
        std::reverse(qb.vertices.begin(), qb.vertices.end());
        for(auto& v : qb.vertices)
        {
            v.position = glm::translate(glm::mat4(1),
                                        glm::vec3(0, 0, -sz/2))
                         *glm::vec4(v.position, 1);
        }
        qb.set_normals();
//        qb.invert_normals();
        sides.emplace_back(qb);
        // top
        Quad qbtm(c, sz);
        for(auto& v :qbtm.vertices)
        {
            v.position = glm::rotate(glm::mat4(1),
                                     glm::radians(-90.f),
                                     glm::vec3(1., 0., 0.))
                         * glm::translate(glm::mat4(1), glm::vec3(0, 0, sz/2))
                         * glm::vec4(v.position, 1);
        }
        qbtm.set_normals();
        //qbtm.invert_normals();
        sides.emplace_back(qbtm);
        // bottom?
        Quad qtop(c, sz);
        for(auto& v :qtop.vertices)
        {
            v.position = glm::rotate(glm::mat4(1),
                                     glm::radians(90.f),
                                     glm::vec3(1., 0., 0.))
                         * glm::translate(glm::mat4(1), glm::vec3(0, 0, sz/2))
                         * glm::vec4(v.position, 1);
        }
        qtop.set_normals();
        //qtop.invert_normals();
        sides.emplace_back(qtop);
        // left?
        Quad ql(c, sz);
        for(auto& v :ql.vertices)
        {
            v.position = glm::rotate(glm::mat4(1),
                                     glm::radians(-90.f),
                                     glm::vec3(0., 1., 0.))
                         * glm::translate(glm::mat4(1), glm::vec3(0, 0, sz/2))
                         * glm::vec4(v.position, 1);
        }
        ql.set_normals();
        ql.invert_normals();
        sides.emplace_back(ql);
        // right?
        Quad qr(c, sz);
        for(auto& v :qr.vertices)
        {
            v.position = glm::rotate(glm::mat4(1),
                                     glm::radians(90.f),
                                     glm::vec3(0., 1., 0.))
                         * glm::translate(glm::mat4(1), glm::vec3(0, 0, sz/2))
                         * glm::vec4(v.position, 1);
        }
        qr.set_normals();
        //qr.invert_normals();
        sides.emplace_back(qr);
    }
};

