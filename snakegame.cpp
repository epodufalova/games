#include <GLFW/glfw3.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <iostream>
#include <cmath>

const int WIDTH = 800;
const int HEIGHT = 800;
const int GRID_SIZE = 20;
const int CELL_SIZE = WIDTH / GRID_SIZE;

struct Point {
    int x, y;
};

std::vector<Point> snake = { {10, 10} };
std::vector<Point> coins; 
const int MAX_COINS = 3;  
Point direction = { 1, 0 };  
bool gameOver = false;
int score = 0;

void drawGradientSquare(float x, float y, float size, float r1, float g1, float b1, float r2, float g2, float b2) {
    glBegin(GL_QUADS);
    glColor3f(r1, g1, b1);
    glVertex2f(x, y);
    glColor3f(r2, g2, b2);
    glVertex2f(x + size, y);
    glVertex2f(x + size, y + size);
    glColor3f(r1, g1, b1);
    glVertex2f(x, y + size);
    glEnd();
}

void drawCircle(float cx, float cy, float r, float rColor, float gColor, float bColor) {
    glColor3f(rColor, gColor, bColor);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= 100; i++) {
        float angle = 2.0f * 3.14159f * i / 100;
        glVertex2f(cx + cos(angle) * r, cy + sin(angle) * r);
    }
    glEnd();
}

void drawSnake() {
    float snakeSegmentSize = CELL_SIZE * 0.8f; 
    for (size_t i = 0; i < snake.size(); ++i) {
        float gradient = static_cast<float>(i) / snake.size();
        drawGradientSquare(static_cast<float>(snake[i].x) * CELL_SIZE, static_cast<float>(snake[i].y) * CELL_SIZE, snakeSegmentSize,
            1.0f, 0.4f, 0.6f - gradient * 0.4f, 
            0.0f, 0.5f, 1.0f - gradient * 0.5f); 
    }
}

void drawCoins() {
    for (const auto& coin : coins) {
        drawCircle(static_cast<float>(coin.x) * CELL_SIZE + CELL_SIZE / 2, static_cast<float>(coin.y) * CELL_SIZE + CELL_SIZE / 2, static_cast<float>(CELL_SIZE) / 2,
            1.0f, 0.8f, 0.0f); 
    }
}

void generateCoins() {
    coins.clear();
    int coinCount = 1 + rand() % MAX_COINS; 
    for (int i = 0; i < coinCount; ++i) {
        coins.push_back({ rand() % GRID_SIZE, rand() % GRID_SIZE });
    }
}

void moveSnake() {
    Point newHead = { snake[0].x + direction.x, snake[0].y + direction.y };
    if (newHead.x < 0 || newHead.x >= GRID_SIZE || newHead.y < 0 || newHead.y >= GRID_SIZE) {
        gameOver = true;
        return;
    }
    for (const auto& segment : snake) {
        if (newHead.x == segment.x && newHead.y == segment.y) {
            gameOver = true;
            return;
        }
    }
    snake.insert(snake.begin(), newHead);
    bool ateCoin = false;
    for (size_t i = 0; i < coins.size(); ++i) {
        if (newHead.x == coins[i].x && newHead.y == coins[i].y) {
            score += 10;
            ateCoin = true;
            break;
        }
    }
    if (ateCoin) {
        generateCoins(); 
    }
    else {
        snake.pop_back(); 
    }
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch (key) {
        case GLFW_KEY_UP:
            if (direction.y != 1) direction = { 0, -1 };
            break;
        case GLFW_KEY_DOWN:
            if (direction.y != -1) direction = { 0, 1 };
            break;
        case GLFW_KEY_LEFT:
            if (direction.x != 1) direction = { -1, 0 };
            break;
        case GLFW_KEY_RIGHT:
            if (direction.x != -1) direction = { 1, 0 };
            break;
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, true);
            break;
        }
    }
}

int main() {
    srand(static_cast<unsigned>(time(0)));
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW!" << std::endl;
        return -1;
    }
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Snake Game", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyCallback);
    glOrtho(0.0, WIDTH, HEIGHT, 0.0, -1.0, 1.0);
    generateCoins();
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        if (!gameOver) {
            moveSnake();
            drawSnake();
            drawCoins();
        }
        else {
            std::cout << "Game Over! Score: " << score << std::endl;
            break;
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
        glfwWaitEventsTimeout(0.1);
    }
    glfwTerminate();
    return 0;
}

