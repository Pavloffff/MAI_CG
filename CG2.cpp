// g++ CG2.cpp -lGLEW -lGL -lglfw -lglut

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <GL/freeglut.h>
#include <iostream>

int a = 1;
double scale = 1.0; // Переменная масштабирования
double centerX = 0.5; // Центр по X
double centerY = 0.5; // Центр по Y

struct Vertex {
    float x, y, z;
};

struct Edge {
    int startVertexIndex, endVertexIndex;
};

struct Wedge {
    Vertex vertices[6]; 
    Edge edges[15]; 
};

// Определение клина
Wedge wedge = {
    // Вершины
    {
        {0, 0, 0},
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1},
        {1, 0, 1},
        {0, 1, 1}
    },
    // Рёбра
    {
        {0, 1},
        {1, 2},
        {2, 0},
        {0, 3},
        {1, 4},
        {2, 5},
        {3, 4},
        {4, 5},
        {5, 3}
    }
};

void drawWedge(const Wedge& wedge) {
    glBegin(GL_LINES);
    for (const Edge& edge : wedge.edges) {
        const Vertex& startVertex = wedge.vertices[edge.startVertexIndex];
        const Vertex& endVertex = wedge.vertices[edge.endVertexIndex];
        glVertex3f(startVertex.x, startVertex.y, startVertex.z);
        glVertex3f(endVertex.x, endVertex.y, endVertex.z);
    }
    glEnd();
}

void drawWedgeSurfaces(const Wedge& wedge) {
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
    glVertex3f(wedge.vertices[0].x, wedge.vertices[0].y, wedge.vertices[0].z);
    glVertex3f(wedge.vertices[2].x, wedge.vertices[2].y, wedge.vertices[2].z);
    glVertex3f(wedge.vertices[3].x, wedge.vertices[3].y, wedge.vertices[3].z);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex3f(wedge.vertices[2].x, wedge.vertices[2].y, wedge.vertices[2].z);
    glVertex3f(wedge.vertices[4].x, wedge.vertices[4].y, wedge.vertices[4].z);
    glVertex3f(wedge.vertices[3].x, wedge.vertices[3].y, wedge.vertices[3].z);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex3f(wedge.vertices[1].x, wedge.vertices[1].y, wedge.vertices[1].z);
    glVertex3f(wedge.vertices[2].x, wedge.vertices[2].y, wedge.vertices[2].z);
    glVertex3f(wedge.vertices[4].x, wedge.vertices[4].y, wedge.vertices[4].z);
    glEnd();
}

void windowResizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {

    std::cout << "Введите параметр масштабирования: ";
    std::cin >> a;

    int izoFlag = 0, deleteFlag = 0;

    
    std::cout << "Изометрическая / ортографическая проекция (1/0): ";
    std::cin >> izoFlag;
    std::cout << "\n";

    std::cout << "Удалять / не удалять невидимые линии (1/0): ";
    std::cin >> deleteFlag;
    std::cout << "\n";

    if (!glfwInit()) {
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(640, 480, "Каркасная визуализация выпуклого многогранника.", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glewInit();

    glfwSetWindowSizeCallback(window, windowResizeCallback);
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glLoadIdentity();

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        double aspect = (double)width / (double)height;

        // Устанавливаем масштабирование и центрирование
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        double halfWidth = 2.0 * a * scale; 
        double halfHeight = 2.0 * a * scale / aspect;
        glOrtho(centerX - halfWidth, centerX + halfWidth, centerY - halfHeight, centerY + halfHeight, -10.0, 10.0);
        glMatrixMode(GL_MODELVIEW);

        if (izoFlag) {
            glRotatef(45, 0.0, 1.0, 0.0);
            glRotatef(-35.264, 1.0, 0.0, 0.0);
        }

        if (deleteFlag) {
            glColor3f(1.0f, 1.0f, 1.0f); // Цвет фона
            drawWedgeSurfaces(wedge);
        }

        glColor3f(0.0, 0.0, 1.0); // Цвет рёбер

        glColor3f(0.0, 0.0, 1.0);
        glLineWidth(5.0);

        drawWedge(wedge);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
