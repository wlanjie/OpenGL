#include <glfw3.h>
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#include "stb_image.h"
#include "image.h"
#include "mirror.h"
#include "render_screen.h"
#include "scale.h"
#include "color.h"
#include "soul_out.h"
#include "shake.h"
#include "flash_white.h"
#include "glitch_filter.h"

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
    
    int nrChannels;
    stbi_set_flip_vertically_on_load(true);
    int width;
    int height;
    GLuint textureId;
    
    unsigned char *data = stbi_load("/Users/wlanjie/project/OpenGL/resources/textures/keyboard.jpg", &width, &height, &nrChannels, 0);
    printf("width = %d height = %d\n", width, height);
    
    glGenTextures(1, &textureId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
    
    GlitchFilter image(width, height);
    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        int frameBufferTextureId = image.processImage(textureId);
        renderScreen.draw(frameBufferTextureId);
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}
