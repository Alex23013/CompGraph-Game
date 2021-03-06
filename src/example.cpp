#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include "helpers/shader.hpp"
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
using namespace std;

const GLuint WIDTH = 800, HEIGHT = 600;

void frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
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
  //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

  GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Test window", nullptr, nullptr);
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

  // Build and compile shader programs.
  Shader shaders("shaders/example_v.glsl", "shaders/example_f.glsl");

  // Vertex data 
  GLfloat vertices[] = {
    0.5f, -0.5f, 0.0f, // left  
    -0.5f, -0.5f, 0.0f, // right 
    0.0f,  0.5f, 0.0f,  // top  
  };

 GLfloat colors[] = {
    1.0f, 0.0f, 0.0f, // red
    0.0f, 1.0f, 0.0f, // green 
    0.0f, 0.0f, 1.0f,  // blue  
  };

  GLuint vbo, vao, vboColors;
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &vboColors);

  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *) 0);
  glEnableVertexAttribArray(0);

  // Setting the colors using other VBO
  glBindBuffer(GL_ARRAY_BUFFER, vboColors);
  glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *) 0);
  glEnableVertexAttribArray(1);

  // Unbinding the buffer because it is already registered in the VAO.
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  // This is optional, unbinding VAO.
  //glBindVertexArray(0);

  // MVP
  //glm::mat4 projectionMat = glm::perspective(glm::radians(45.0f), float(WIDTH) / HEIGHT, 0.1f, 100.0f);
  // For ortho camera:
  glm::mat4 projectionMat = glm::ortho(-1.0f,1.0f,-1.0f,1.0f,0.0f,100.0f); // In world coordinates

  glm::mat4 viewMat = glm::lookAt(glm::vec3{4,3,3}, {0,0,0}, {0,1,0});

  glm::mat4 modelMat = glm::mat4(1.0f);

  glm::mat4 mvp = projectionMat * viewMat * modelMat;
  GLuint matrixId = glGetUniformLocation(shaders.ID, "mvp");

  // Main loop
  while (!glfwWindowShouldClose(window)) {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shaders.use();
    //glBindVertexArray(vao);
    glUniformMatrix4fv(matrixId, 1, GL_FALSE, &mvp[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);

  glfwTerminate();
}

/*
 * Use program
 * Bind VAO
 * Draw Array*/
// glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices2), vertices2);
