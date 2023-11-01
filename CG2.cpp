// g++ CG2.cpp -lGLEW -lGL -lglfw -lglut -lGLU

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <GL/freeglut.h>
#include <iostream>
#include <vector>
#include <GL/glu.h>

struct Vertex
{
    float x, y, z;
};

struct Edge
{
    int startVertexIndex, endVertexIndex;
};

struct Wedge
{
    Vertex vertices[6];
    Edge edges[15];
};

class Figure
{
private:
    Wedge wedge = {
        {{0, 0, 0},
         {1, 0, 0},
         {0, 1, 0},
         {0, 0, 1},
         {1, 0, 1},
         {0, 1, 1}},
        {{0, 1},
         {1, 2},
         {2, 0},
         {0, 3},
         {1, 4},
         {2, 5},
         {3, 4},
         {4, 5},
         {5, 3}}};

public:
    Figure() {}

    void deleteInvisibleLines()
    {
        glBegin(GL_TRIANGLES);
        glVertex3f(wedge.vertices[0].x, wedge.vertices[0].y, wedge.vertices[0].z);
        glVertex3f(wedge.vertices[1].x, wedge.vertices[1].y, wedge.vertices[1].z);
        glVertex3f(wedge.vertices[2].x, wedge.vertices[2].y, wedge.vertices[2].z);
        glEnd();

        glBegin(GL_TRIANGLES);
        glVertex3f(wedge.vertices[3].x, wedge.vertices[3].y, wedge.vertices[3].z);
        glVertex3f(wedge.vertices[4].x, wedge.vertices[4].y, wedge.vertices[4].z);
        glVertex3f(wedge.vertices[5].x, wedge.vertices[5].y, wedge.vertices[5].z);
        glEnd();

        glBegin(GL_TRIANGLES);
        glVertex3f(wedge.vertices[2].x, wedge.vertices[2].y, wedge.vertices[2].z);
        glVertex3f(wedge.vertices[4].x, wedge.vertices[4].y, wedge.vertices[4].z);
        glVertex3f(wedge.vertices[5].x, wedge.vertices[5].y, wedge.vertices[5].z);
        glEnd();

        glBegin(GL_TRIANGLES);
        glVertex3f(wedge.vertices[2].x, wedge.vertices[2].y, wedge.vertices[2].z);
        glVertex3f(wedge.vertices[3].x, wedge.vertices[3].y, wedge.vertices[3].z);
        glVertex3f(wedge.vertices[5].x, wedge.vertices[5].y, wedge.vertices[5].z);
        glEnd();

        glBegin(GL_TRIANGLES);
        glVertex3f(wedge.vertices[0].x, wedge.vertices[0].y, wedge.vertices[0].z);
        glVertex3f(wedge.vertices[3].x, wedge.vertices[3].y, wedge.vertices[3].z);
        glVertex3f(wedge.vertices[4].x, wedge.vertices[4].y, wedge.vertices[4].z);
        glEnd();

        glBegin(GL_TRIANGLES);
        glVertex3f(wedge.vertices[0].x, wedge.vertices[0].y, wedge.vertices[0].z);
        glVertex3f(wedge.vertices[1].x, wedge.vertices[1].y, wedge.vertices[1].z);
        glVertex3f(wedge.vertices[4].x, wedge.vertices[4].y, wedge.vertices[4].z);
        glEnd();

        glBegin(GL_TRIANGLES);
        glVertex3f(wedge.vertices[0].x, wedge.vertices[0].y, wedge.vertices[0].z);
        glVertex3f(wedge.vertices[2].x, wedge.vertices[2].y, wedge.vertices[2].z);
        glVertex3f(wedge.vertices[3].x, wedge.vertices[3].y, wedge.vertices[3].z);
        glEnd();

        glBegin(GL_TRIANGLES);
        glVertex3f(wedge.vertices[1].x, wedge.vertices[1].y, wedge.vertices[1].z);
        glVertex3f(wedge.vertices[2].x, wedge.vertices[2].y, wedge.vertices[2].z);
        glVertex3f(wedge.vertices[4].x, wedge.vertices[4].y, wedge.vertices[4].z);
        glEnd();


    }

    void Draw()
    {
        glBegin(GL_LINES);
        for (const Edge &edge : wedge.edges)
        {
            const Vertex &startVertex = wedge.vertices[edge.startVertexIndex];
            const Vertex &endVertex = wedge.vertices[edge.endVertexIndex];
            glVertex3f(startVertex.x, startVertex.y, startVertex.z);
            glVertex3f(endVertex.x, endVertex.y, endVertex.z);
        }
        glEnd();
    }
};

class Output
{
private:
    GLFWwindow *window;
    Figure figure;

    bool deleteFlag = false;
    float scaling = 5.0f;
    float xAngle = 0.0f;
    float yAngle = 0.0f;

public:
    Output(int size)
    {
        if (!glfwInit())
            exit(EXIT_FAILURE);

        window = glfwCreateWindow(size, size, "Каркасная визуализация выпуклого многогранника", NULL, NULL);

        if (!window)
        {
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        glfwMakeContextCurrent(window);

        glfwSetWindowUserPointer(window, this);

        glfwSetKeyCallback(window, keyCallback);
        glfwSetWindowSizeCallback(window, resizeCallback);

        figure = Figure();
    }

    void start()
    {
        while (!glfwWindowShouldClose(window))
        {
            drawFrame();
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        glfwDestroyWindow(window);
        glfwTerminate();
    }

    static void resizeCallback(GLFWwindow *window, int width, int height)
    {
        glViewport(0, 0, width, height);
        glMatrixMode(GL_PROJECTION);

        glLoadIdentity();

        float aspectRatio = (float)width / (float)height;
        gluPerspective(45.0f, aspectRatio, 1.0f, 100.0f);

        glMatrixMode(GL_MODELVIEW);
    }

    static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        Output *output = static_cast<Output *>(glfwGetWindowUserPointer(window));

        if (action == GLFW_PRESS || action == GLFW_REPEAT)
        {
            switch (key)
            {
            case GLFW_KEY_KP_ADD:
            case GLFW_KEY_EQUAL:
                output->scaling -= 0.5f;
                break;

            case GLFW_KEY_KP_SUBTRACT:
            case GLFW_KEY_MINUS:
                output->scaling += 0.5f;
                break;

            case GLFW_KEY_S:
                output->xAngle += 10.0f;
                if (output->xAngle > 360.0f)
                {
                    output->xAngle = 0.0f;
                }
                break;

            case GLFW_KEY_W:
                output->xAngle -= 10.0f;
                if (output->xAngle < 0.0f)
                {
                    output->xAngle = 360.0f;
                }
                break;

            case GLFW_KEY_D:
                output->yAngle += 10.0f;
                if (output->yAngle > 360.0f)
                {
                    output->yAngle = 0.0f;
                }
                break;

            case GLFW_KEY_A:
                output->yAngle -= 10.0f;
                if (output->yAngle < 0.0f)
                {
                    output->yAngle = 360.0f;
                }
                break;
            
            case GLFW_KEY_SPACE:
                output->deleteFlag = !output->deleteFlag;
                break;

            default:
                break;
            }
        }
    }

    void drawFrame()
    {
        glLoadIdentity();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glTranslatef(0.0, 0.0, -scaling);

        glRotatef(xAngle, 1.0, 0.0, 0.0);
        glRotatef(yAngle, 0.0, 1.0, 0.0);

        GLfloat matAmbientAndDiffuse[] = {0.8f, 0.0f, 0.2f, 1.0f};
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbientAndDiffuse);
        glColor3f(0.0, 0.0, 1.0);
        glLineWidth(3.0);
        figure.Draw();
        if (deleteFlag) {
            figure.deleteInvisibleLines();
        }
    }
};

int main()
{
    Output app(800);
    app.start();
    return 0;
}
