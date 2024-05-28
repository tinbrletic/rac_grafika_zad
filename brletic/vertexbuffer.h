#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "GL/glew.h"

struct VertexBuffer {
private:
    unsigned int m_bufferID=0;

public:
//    explicit VertexBuffer(const std::vector<float> &arr);
//    explicit VertexBuffer(const std::vector<glm::vec3> &arr);
    template<typename T>
    explicit VertexBuffer(const std::vector<T>& arr) {
        glGenBuffers(1, &m_bufferID);
        glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
        glBufferData(GL_ARRAY_BUFFER,
                     arr.size() * sizeof(T),
                     &arr.front(),
                     GL_STATIC_DRAW);
    }
    ~VertexBuffer();

    void updateBufferData(const std::vector<float> &arr);

    void bind() const;
    static void unBind();
};
