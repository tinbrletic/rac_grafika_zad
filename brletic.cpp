#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <vector>

#include "pathconfig.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>


#include <fstream>
#include <sstream>

#include "indexbuffer.h"
#include "renderer.h"
#include "shader.h"
#include "vertexarray.h"
#include "vertexbuffer.h"
#include "vertexbufferlayout.h"
#include <glm/gtc/type_ptr.hpp>

#include "box.h"
#include "box_renderer.h"
#include "texture.h"

const unsigned int SCR_WIDTH = 1320;
const unsigned int SCR_HEIGHT = 768;

constexpr const glm::vec3 dimgrey{105. / 255, 105. / 255, 105. / 255};
constexpr const glm::vec3 coral{255. / 255, 127. / 255, 80. / 255};
constexpr const glm::vec3 darkOrange{255. / 255, 140. / 255, 0. / 255};


#include <algorithm>
#include <numeric>


void framebuffer_size_callback(GLFWwindow *window, int width, int height);

int main()
{
    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_SAMPLES, 4);// ovo nam za sada ne treba
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);// Za MacOS, tako kazu
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Zadatak 1.", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(2);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glewExperimental = true;// ovo nam treba za CORE Profile

    unsigned int err = glewInit();
    if (err != GLEW_OK)
    {
        std::cout << "error\n";
        return -1;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);

    //std::cout << "textures folder: " << textures_folder << std::endl;
    std::cout << "shaders folder: " << shaders_folder << std::endl;

    // Izvor svjetla:
    const std::string light_vs = shaders_folder + "point_v.glsl";
    const std::string light_fs = shaders_folder + "point_f.glsl";

    glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
    Shader light_shader({{GL_VERTEX_SHADER, light_vs}, {GL_FRAGMENT_SHADER, light_fs}});
    light_shader.bind();
    light_shader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

    std::vector<glm::vec3> light_arr = {lightPos};
    VertexBuffer light_vb(light_arr);

    VertexBufferLayout light_layout;
    light_layout.addFloat(3);

    VertexArray light_va;
    light_va.addBuffer(light_vb, light_layout);
    IndexBuffer light_ibo({0});

    // Sphere
    const std::string sphere_vs = shaders_folder + "sphere_vertex.glsl";
    const std::string sphere_fs = shaders_folder + "sphere_fragment.glsl";

    Shader sphere_shader({{GL_VERTEX_SHADER, sphere_vs}, {GL_FRAGMENT_SHADER, sphere_fs}});

    // Generate vertices and indices
    std::vector<glm::vec3> vertices;
    std::vector<unsigned int> indices;

    unsigned int numStacks = 50;
    unsigned int numSlices = 50;

    for (unsigned int i = 0; i <= numStacks; ++i)
    {
        float V = i / (float) numStacks;
        float phi = V * glm::pi<float>();

        // Loop through slices
        for (unsigned int j = 0; j <= numSlices; ++j)
        {
            float U = j / (float) numSlices;
            float theta = U * (glm::pi<float>() * 2);

            // Use spherical coordinates to calculate the positions.
            float x = cos(theta) * sin(phi);
            float y = cos(phi);
            float z = sin(theta) * sin(phi);

            vertices.push_back(glm::vec3(x, y, z));
        }
    }

    for (unsigned int i = 0; i < numSlices * numStacks + numSlices; ++i)
    {
        indices.push_back(i);
        indices.push_back(i + numSlices + 1);
        indices.push_back(i + numSlices);

        indices.push_back(i + numSlices + 1);
        indices.push_back(i);
        indices.push_back(i + 1);
    }

    // Create vertex buffer and index buffer...
    VertexBuffer sphere_vb(vertices);
    IndexBuffer sphere_ibo(indices);


    VertexArray sphere_va;
    VertexBufferLayout sphere_layout;

    sphere_layout.addFloat(3);
    sphere_va.addBuffer(sphere_vb, sphere_layout);

    float scaleFactor = 0.3f;// Adjust this value to change the size of the sphere
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(scaleFactor));
    glm::vec3 sphere_position(0.0f, 0.5f, 0.0f);
    glm::mat4 sphere_model = glm::translate(glm::mat4(1.0f), sphere_position);

    // Box
    const std::string t_vs = shaders_folder + "vert.glsl";
    const std::string t_fs = shaders_folder + "frag.glsl";

    Shader quad_shader({{GL_VERTEX_SHADER, t_vs}, {GL_FRAGMENT_SHADER, t_fs}});


    Quad quad(darkOrange, 5);
    QuadRenderer quad_renderer(quad);

    VertexBuffer quad_vb(quad.vertices);
    VertexBufferLayout quad_layout;
    quad_layout.addFloat(3);
    quad_layout.addFloat(3);
    quad_layout.addFloat(2);
    quad_layout.addFloat(3);

    IndexBuffer quad_ib(quad.indices);
    VertexArray quad_va;
    quad_va.addBuffer(quad_vb, quad_layout);

    glm::vec3 point_light_position = glm::vec3(-5.0f, 2.0f, 1.0f);


    auto m = glm::rotate(glm::mat4(1), glm::radians(-90.f), glm::vec3(1., 0., 0.));

    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::vec3 rotatedLightPos = glm::vec3(rotation * glm::vec4(point_light_position, 1.0f));
    


    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Renderer::clear();

        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);

        const float radius = 6.0f;
        float camX = std::sin(glfwGetTime()) * radius;
        float camZ = std::cos(glfwGetTime()) * radius;

        glm::vec3 view_position = glm::vec3(5, 3.0, 2.0);

        view = glm::lookAt(view_position, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.));
        projection = glm::perspective(glm::radians(45.0f), (float) SCR_WIDTH / (float) SCR_HEIGHT,
                                      0.1f, 100.0f);

        quad_shader.bind();
        quad_shader.setMat4("view", view);
        quad_shader.setMat4("projection", projection);

        // point light 1
        quad_shader.setVec3("pointLight.position", rotatedLightPos);
        quad_shader.setVec3("pointLight.ambient", 1.0f, 1.0f, 1.0f);
        quad_shader.setVec3("pointLight.diffuse", 0.5f, 0.5f, 0.5f);
        quad_shader.setVec3("pointLight.specular", 0.7f, 0.7f, 0.7f);
        quad_shader.setFloat("pointLight.constant", 1.0f);
        quad_shader.setFloat("pointLight.linear", 0.09);
        quad_shader.setFloat("pointLight.quadratic", 0.032);

        quad_shader.setFloat("material.shininess", 50.f);
        quad_shader.setVec3("viewPos", view_position);

        quad_shader.setMat4("transform", m);

        quad_shader.setInt("material.diffuse", 0);
        quad_shader.setInt("material.normal_map", 1);
        Renderer::drawTriangles(quad_va, quad_ib, quad_shader);
        Renderer::unBind();

        // Render Sphere
        sphere_shader.bind();
        sphere_shader.setMat4("view", view);
        sphere_shader.setMat4("projection", projection);
        sphere_shader.setMat4("model", sphere_model);
        sphere_shader.setMat4("scale", scale);
        sphere_shader.setVec3("lightPos", lightPos);
        sphere_shader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
        Renderer::drawTriangles(sphere_va, sphere_ibo, sphere_shader);
        Renderer::unBind();


        /// crtanje izvora svjetla
        light_shader.bind();
        light_shader.setMat4("view", view);
        light_shader.setMat4("projection", projection);

        const glm::mat4 mlightPos = glm::translate(glm::mat4(1.0f), rotatedLightPos);
        //const glm::mat4 mlightPos = glm::translate(glm::mat4(1.0f), point_light_position + glm::vec3(camX,0,0));
        light_shader.setMat4("transform", mlightPos);
        light_shader.setFloat("ptSize", 15);
        Renderer::drawPoints(light_va, light_ibo, light_shader);


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
