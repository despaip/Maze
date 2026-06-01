#ifndef MAZE_HPP
#define MAZE_HPP

#include <stack>
#include <chrono>
#include <random>
#include <SFML/Graphics.hpp>

const int kBaseMazeWidth = 20;
const int kBaseMazeHeight = 7;
const int kLevelSizeStep = 5;
const int kMaxLevel = 5;
const int kCellSize = 32;
const float kWallSize = 4.0f;

struct Position {
    int x;
    int y;
};

class Maze {
private:
    struct Cell {
        bool isShowBottomWall = true;
        bool isShowTopWall = true;
        bool isShowRightWall = true;
        bool isShowLeftWall = true;
        bool visited = false;
    };

    int height;
    int width;
    Cell** grid;
    std::mt19937 rng;
    Position exit;

    bool isValidCoordinates(int x, int y) const;
    void removeWall(Position a, Position b);
    bool canMove(Position pos, char direction) const;
    void updateExitPosition(Position start);
    void drawWall(sf::RenderWindow& window, float x, float y, float width, float height) const;

public:
    Maze(int height = kBaseMazeHeight, int width = kBaseMazeWidth);
    ~Maze();

    Maze(const Maze&) = delete;
    Maze& operator=(const Maze&) = delete;

    void generateMaze(int startX = 0, int startY = 0);
    void show(sf::RenderWindow& window, Position player) const;

    Position move(Position pos, char direction) const;
    bool isExit(Position pos) const;

    int getHeight() const;
    int getWidth() const;
};

#endif
