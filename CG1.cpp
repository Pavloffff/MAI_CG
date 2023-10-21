// g++ CG1.cpp -lGLEW -lGL -lglfw -lglut
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <GL/freeglut.h>
#include <string>

using namespace std;

double a = 1; // Константа a

double scale = 1.0; // Переменная масштабирования
double centerX = 0.0; // Центр по X
double centerY = 0.0; // Центр по Y

void textCallback(GLFWwindow* window, unsigned int codepoint) {
    static string input;
    if (codepoint == '\b') {
        if (!input.empty()) {
            input.pop_back();
        }
    }
    else if (codepoint == '\r' || codepoint == '\n') {
        try {
            a = stod(input);
        }
        catch (const std::invalid_argument& e) {
            cout << "Неверный формат числа" << endl;
        }
        input.clear();
    }
    else {
        input += static_cast<char>(codepoint);
    }
}

void drawPolarAxes(int numCircles) {
    const int numRays = 12; // количество лучей (по умолчанию 12 для каждых 30 градусов)

    glColor3f(0.5, 0.5, 0.5); // цвет осей

    // Рисуем круги
    for (int i = 1; i <= numCircles; ++i) {
        float radius = i * 0.2; // допустим, каждый следующий круг на 0.2 единицы больше предыдущего
        glBegin(GL_LINE_LOOP);
        for (int j = 0; j < 360; ++j) {
            double angle = j * M_PI / 180.0; // переводим градусы в радианы
            glVertex2d(radius * cos(angle), radius * sin(angle));
        }
        glEnd();
    }

    // Рисуем лучи (линии, идущие от центра наружу на каждые 30 градусов, например)
    for (int i = 0; i < numRays; ++i) {
        double angle = i * 2 * M_PI / numRays; // радианы
        glBegin(GL_LINES);
        glVertex2d(0.0, 0.0); // начало в центре
        glVertex2d(numCircles * 0.2 * cos(angle), numCircles * 0.2 * sin(angle));
        glEnd();
    }
}

void drawPolarFunction(float a) {
    const int numPoints = 1000; // количество точек для рисования графика

    glColor3f(0.0, 0.0, 1.0);
    glLineWidth(3.0);

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i <= numPoints; ++i) {
        double phi = i * 2 * M_PI / numPoints; // перебор углов от 0 до 2π
        double rhoSquared = a * a * cos(2 * phi); // вычисление ρ^2

        if (rhoSquared < 0) {
            // Для отрицательных значений ρ^2, мы меняем направление угла на 180 градусов
            // и берем корень из абсолютного значения
            phi += M_PI;
            rhoSquared = fabs(rhoSquared);
        }

        double rho = sqrt(rhoSquared); // вычисление ρ

        // Конвертация полярных координат в декартовы и отрисовка точки
        glVertex2d(rho * cos(phi), rho * sin(phi));
    }
    glEnd();
}

int main(int argc, char** argv) {
    float param = 1;
    std::cout << "Введите параметр масштабирования: ";
    std::cin >> a;
    std::cout << "\nВведите параметр кривой: ";
    std::cin >> param;

    // Инициализация GLUT
    glutInit(&argc, argv);
    // Инициализация GLFW
    if (!glfwInit()) {
        std::cerr << "Ошибка инициализации GLFW" << std::endl;
        return -1;
    }

    // Создаем окно GLFW
    GLFWwindow* window;
    window = glfwCreateWindow(800, 800, "Построение изображений 2D- кривых", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Инициализируем GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Ошибка инициализации GLEW" << std::endl;
        return -1;
    }

    // Устанавливаем вьюпорт и настройки OpenGL
    glViewport(0, 0, 800, 800);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-2.0, 2.0, -2.0, 2.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);

    glfwSetCharCallback(window, textCallback);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        double aspect = (double)width / (double)height;

        // Устанавливаем масштабирование и центрирование
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        double halfWidth = 2.0 * a * scale; 
        double halfHeight = 2.0 * a * scale / aspect;
        glOrtho(centerX - halfWidth, centerX + halfWidth, centerY - halfHeight, centerY + halfHeight, -1.0, 1.0);
        glMatrixMode(GL_MODELVIEW);

        glColor3f(0.0, 0.0, 0.0); // Устанавливаем цвет

        drawPolarAxes(param * 10);
        drawPolarFunction(param);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
