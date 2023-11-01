// g++ CG7.cpp -lGLEW -lGL -lglfw -lglut -lGLU

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include <iostream>
#include <vector>

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

std::vector<std::pair<GLfloat, GLfloat>> controlPoints;

void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        int width, height;
        glfwGetWindowSize(window, &width, &height);

        // Преобразуем координаты курсора в координаты OpenGL
        xpos = xpos / width;
        ypos = 1.0 - ypos / height;

        if (controlPoints.size() < 4)
        {
            controlPoints.push_back({(GLfloat)xpos, (GLfloat)ypos});
        }
    }
}

void display()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Рисуем точки управления
    glColor3f(0.0f, 0.0f, 0.0f);
    glPointSize(5.0f);
    glBegin(GL_POINTS);
    for (const auto &point : controlPoints)
    {
        glVertex2f(point.first, point.second);
    }
    glEnd();

    if (controlPoints.size() == 4)
    {
        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_LINE_STRIP);
        for (double t = 0; t <= 1; t += 0.001)
        {
            double x = controlPoints[0].first * (1 - t) * (1 - t) * (1 - t) +
                       controlPoints[1].first * 3 * (1 - t) * (1 - t) * t +
                       controlPoints[2].first * 3 * (1 - t) * t * t + 
                       controlPoints[3].first * t * t * t;
            double y = controlPoints[0].second * (1 - t) * (1 - t) * (1 - t) + 
                       controlPoints[1].second * 3 * (1 - t) * (1 - t) * t + 
                       controlPoints[2].second * 3 * (1 - t) * t * t + 
                       controlPoints[3].second * t * t * t;
            glVertex2d(x, y);
        }
        glEnd();
    }

    glFlush();
}

int main(int argc, char **argv)
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    GLFWwindow *window = glfwCreateWindow(800, 600, "Построение плоских полиномиальных кривых", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);

    while (!glfwWindowShouldClose(window))
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        framebufferSizeCallback(window, width, height);
        display();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}