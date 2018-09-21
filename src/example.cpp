#include "helpers/shader.hpp"
#include "objects/object.hpp"
#include <GLFW/glfw3.h>
#include <cmath>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
using namespace std;

const GLuint WIDTH = 800, HEIGHT = 600;

void frameBufferSizeCallback(GLFWwindow *window, int width, int height) {
  cout << "Width and height: " << width << ", " << height << "\n";
  glViewport(0, 0, width, height);
}

int main() {
  // GLFW initialization
  if (!glfwInit()) {
    cout << "GLFW initialization failed\n";
    glfwTerminate();
    return 1;
  }

  // GLFW setup
  glfwWindowHint(GLFW_SAMPLES, 4); // Anti aliasing
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

  GLFWwindow *window =
      glfwCreateWindow(WIDTH, HEIGHT, "Test window", nullptr, nullptr);
  if (!window) {
    cout << "GLFW windows creation failed failed\n";
    glfwTerminate();
    return 1;
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);

  // Glad initialization
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    cout << "Failed to initialize GLAD\n";
    return 1;
  }
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  // Build and compile shader programs.
  Shader shaders("shaders/example_v.glsl", "shaders/example_f.glsl");

  // Vertex data
  GLfloat vertices[] = {
      0.5f,  0.5f,  0.0f, // top right
      0.5f,  -0.5f, 0.0f, // bottom right
      -0.5f, -0.5f, 0.0f, // bottom left
      -0.5f, 0.5f,  0.0f  // top left
  };

  GLuint indices[] = {
      0, 1, 3, // first Triangle
      1, 2, 3  // second Triangle
  };

  Object triangle;
  triangle.setVerticesData(4, vertices, 6, indices);
  triangle.setShader(shaders.ID);
  triangle.setMode(GL_TRIANGLES);

  float blue[3] = {0.16f, 0.23f, 0.88f};

  // MVP
  // glm::mat4 projectionMat = glm::perspective(glm::radians(45.0f),
  // float(WIDTH) / HEIGHT, 0.1f, 100.0f);
  // For ortho camera:
  glm::mat4 projectionMat = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f,
                                       100.0f); // In world coordinates

  glm::mat4 viewMat = glm::lookAt(glm::vec3{4, 3, 3}, {0, 0, 0}, {0, 1, 0});

  glm::mat4 modelMat = glm::mat4(1.0f);

  glm::mat4 mvp = projectionMat * viewMat * modelMat;
  GLuint matrixId = glGetUniformLocation(shaders.ID, "mvp");
  GLint colorId = glGetUniformLocation(shaders.ID, "inColor");

  // Main loop
  while (!glfwWindowShouldClose(window)) {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    triangle.draw([&] {
      glUniform3fv(colorId, 1, blue);
      glUniformMatrix4fv(matrixId, 1, GL_FALSE, &mvp[0][0]);
    });

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  triangle.destroy();

  glfwTerminate();
}

/*
 * Use program
 * Bind VAO
 * Draw Array*/
// glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices2), vertices2);
