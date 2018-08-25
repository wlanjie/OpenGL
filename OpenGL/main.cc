#include <iostream>
#include <glfw3.h>
#include "shader_program.h"
#include "image.h"
#include "blur.h"
#include "render_screen.h"
#include "mirror.h"

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

int main() {
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(560, 940, "OpenGL", nullptr, nullptr);
    if(!window) {
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyCallback);

    Mirror image("/Users/wlanjie/project/OpenGL/resources/textures/wang.jpg");
    RenderScreen renderScreen;
    
    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        int textureId = image.processImage(false);
        renderScreen.draw(textureId);
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}
