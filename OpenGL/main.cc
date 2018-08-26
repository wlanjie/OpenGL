#include <glfw3.h>
#include "image.h"
#include "render_screen.h"

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

int main() {
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL", nullptr, nullptr);
    if(!window) {
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyCallback);
    RenderScreen renderScreen;

    Image image("/Users/wlanjie/Documents/OpenGL/resources/textures/test.jpg");
    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        int textureId = image.processImage(false);
        renderScreen.draw(textureId);
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}
