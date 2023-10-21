// g++ CG3_4.cpp -lGLEW -lGL -lglfw -lglut -lGLU

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <GL/freeglut.h>
#include <iostream>
#include <vector>
#include <GL/glu.h>

class Figure
{
private:
    float _radius;
    int _precision;
    float _r;
    float _g;
    float _b;
    static const int MinPrecision = 3;

public:
    Figure() {}
    Figure(float radius, int precision)
        : _radius(radius), _precision(precision), _r(0.5f), _g(0.5f), _b(0.5f) {}

    int getPrecision() const
    {
        return _precision;
    }

    void setPrecision(int precision)
    {
        _precision = (precision < MinPrecision) ? MinPrecision : precision;
    }

    void Draw()
    {
        const float endPhi = static_cast<float>(M_PI) * 2.0f;
        const float endTheta = static_cast<float>(M_PI) * 0.5f;
        float dPhi = endPhi / _precision;
        float dTheta = endTheta / _precision;

        for (int pointPhi = 0; pointPhi < _precision; pointPhi++)
        {
            for (int pointTheta = 0; pointTheta < _precision; pointTheta++)
            {
                float phi = pointPhi * dPhi;
                float theta = pointTheta * dTheta;
                float phiT = (pointPhi + 1 == _precision) ? endPhi : (pointPhi + 1) * dPhi;
                float thetaT = (pointTheta + 1 == _precision) ? endTheta : (pointTheta + 1) * dTheta;

                std::vector<float> p0 = {_radius * (float)sin(theta) * (float)cos(phi),
                                         _radius * (float)sin(theta) * (float)sin(phi),
                                         _radius * (float)cos(theta)};

                std::vector<float> p1 = {_radius * (float)sin(thetaT) * (float)cos(phi),
                                         _radius * (float)sin(thetaT) * (float)sin(phi),
                                         _radius * (float)cos(thetaT)

                };

                std::vector<float> p2 = {
                    _radius * (float)sin(theta) * (float)cos(phiT),
                    _radius * (float)sin(theta) * (float)sin(phiT),
                    _radius * (float)cos(theta)};

                std::vector<float> p3 = {
                    _radius * (float)sin(thetaT) * (float)cos(phiT),
                    _radius * (float)sin(thetaT) * (float)sin(phiT),
                    _radius * (float)cos(thetaT)};

                glBegin(GL_TRIANGLES);

                glNormal3f(p0[0] / _radius, p0[1] / _radius, p0[2] / _radius);
                glVertex3f(p0[0], p0[1], p0[2]);

                glNormal3f(p2[0] / _radius, p2[1] / _radius, p2[2] / _radius);
                glVertex3f(p2[0], p2[1], p2[2]);

                glNormal3f(p1[0] / _radius, p1[1] / _radius, p1[2] / _radius);
                glVertex3f(p1[0], p1[1], p1[2]);

                glNormal3f(p3[0] / _radius, p3[1] / _radius, p3[2] / _radius);
                glVertex3f(p3[0], p3[1], p3[2]);

                glNormal3f(p1[0] / _radius, p1[1] / _radius, p1[2] / _radius);
                glVertex3f(p1[0], p1[1], p1[2]);

                glNormal3f(p2[0] / _radius, p2[1] / _radius, p2[2] / _radius);
                glVertex3f(p2[0], p2[1], p2[2]);

                glNormal3f(p0[0] / _radius, p0[1] / _radius, 0);
                glVertex3f(p0[0], p0[1], 0);

                glNormal3f(p2[0] / _radius, p2[1] / _radius, 0);
                glVertex3f(p2[0], p2[1], 0);

                glNormal3f(p1[0] / _radius, p1[1] / _radius, 0);
                glVertex3f(p1[0], p1[1], 0);

                glNormal3f(p3[0] / _radius, p3[1] / _radius, 0);
                glVertex3f(p3[0], p3[1], 0);

                glNormal3f(p1[0] / _radius, p1[1] / _radius, 0);
                glVertex3f(p1[0], p1[1], 0);

                glNormal3f(p2[0] / _radius, p2[1] / _radius, 0);
                glVertex3f(p2[0], p2[1], p2[2]);

                glEnd();
            }
        }
    }
};

class Output
{
private:
    GLFWwindow *window;
    Figure figure;

    float scaling = 10.0f;
    float xAngle = 0.0f;
    float yAngle = 0.0f;
    float lightPosition[4] = {20.0f, 0.0f, 0.0f, 1.0f};

public:
    Output(int size)
    {
        if (!glfwInit())
            exit(EXIT_FAILURE);

        window = glfwCreateWindow(size, size, "Основы построения фотореалистичных изображений", NULL, NULL);

        if (!window)
        {
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        glfwMakeContextCurrent(window);

        glfwSetWindowUserPointer(window, this);

        glfwSetKeyCallback(window, keyCallback);
        glfwSetWindowSizeCallback(window, resizeCallback);

        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);

        GLfloat lightAmbient[] = {0.5f, 0.5f, 0.5f, 1.0f};
        GLfloat lightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};

        glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

        figure = Figure(2, 20);
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
            int precision = output->figure.getPrecision();
            switch (key)
            {
            case GLFW_KEY_LEFT:
                output->lightPosition[0] -= 10.0f;
                if (output->lightPosition[0] < -360.0f)
                {
                    output->lightPosition[0] = 360.0f;
                }
                break;

            case GLFW_KEY_RIGHT:
                output->lightPosition[0] += 10.0f;
                if (output->lightPosition[0] > 360.0f)
                {
                    output->lightPosition[0] = -360.0f;
                }
                break;

            case GLFW_KEY_UP:
                
                output->figure.setPrecision(precision + 1);
                break;

            case GLFW_KEY_DOWN:
                output->figure.setPrecision(precision - 1);
                break;

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

            default:
                break;
            }
        }
    }

    void drawFrame()
    {
        glLoadIdentity();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

        glTranslatef(0.0, 0.0, -scaling);

        glRotatef(xAngle, 1.0, 0.0, 0.0);
        glRotatef(yAngle, 0.0, 1.0, 0.0);

        GLfloat matAmbientAndDiffuse[] = {0.8f, 0.0f, 0.2f, 1.0f};
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbientAndDiffuse);

        figure.Draw();
    }
};

int main()
{
    Output app(800);
    app.start();
    return 0;
}