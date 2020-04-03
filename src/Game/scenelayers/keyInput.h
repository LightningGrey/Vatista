#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include "GLM/glm.hpp"

namespace Vatista {
    class KeyInput {
    public:

        //void glfwSetKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

       

        int buttonSelected = 0;




    };

}



//
//
//static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
//{
//    //ESC to quit
//    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
//    {
//        glfwSetWindowShouldClose(window, GL_TRUE);
//        return;
//    }
//    if (key == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
//    {
//        double xpos, ypos;
//        //getting cursor position
//        glfwGetCursorPos(window, &xpos, &ypos);
//        cout << "Cursor Position at (" << xpos << " : " << ypos << endl;
//    }
//}
