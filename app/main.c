#include <stdio.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <canvas.h>

#include "app.h"

int main()
{
    // Init glfw
    GLFWwindow *window = NULL;
    {
        if (glfwInit() != GLFW_TRUE)
        {
            const char *error;
            glfwGetError(&error);
            fprintf(stderr, "glfwInit() failed: %s\n", error);
        }

        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        window = glfwCreateWindow(700, 800, "Minestorm", NULL, NULL);
        if (window == NULL)
        {
            const char *error;
            glfwGetError(&error);
            fprintf(stderr, "glfwCreateWindow() failed: %s\n", error);
        }
        glfwMakeContextCurrent(window);
        glfwSwapInterval(1);
    }

    // Init canvas library
    CanvasConfig config = cvGetDefaultConfig();
    config.hideInternalWindow = true;
    config.backgroundColor = CV_COL32(0, 0, 0, 255);
    cvInit(window, config);

    // Init our app
    App app = {0};
    appInit(&app);

    // Main loop
    while (glfwWindowShouldClose(window) == GLFW_FALSE)
    {
        // Begin
        glfwPollEvents();
        cvNewFrame();

        appUpdate(&app);
        if (app.closeApp)
            glfwSetWindowShouldClose(window, 1);

        // End
        cvEndFrame();
        glfwSwapBuffers(window);
    }

    // Shutdown
    appShutdown(&app);
    cvShutdown();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
