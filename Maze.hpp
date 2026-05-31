#include "Cell.hpp"
#include "Stack.hpp"
#include <chrono>
#include <random>
#include <algorithm>
#include <SFML/Graphics.hpp>

const int kMazeWidth = 40;
const int kMazeHeight = 25;
const int kCellSize = 32;
const float kWallSize = 4.0f;

class Maze {
private:
    int height;
    int width;
    Cell** grid;
    std::mt19937 rng;

    bool isValidCoordinates(int x, int y);
    int findUnvisitedNeighbours(int x, int y, Position* unvisitedNeighbours);
    void removeWall(Position a, Position b);
    bool canMove(Position pos, char direction) const;
    void drawWall(sf::RenderWindow& window, float x, float y, float width, float height) const;
public:
    Maze(int height = kMazeHeight, int width = kMazeWidth);
    ~Maze();

    void generateMaze(int startX = 0, int startY = 0);

    void show(sf::RenderWindow& window, Position player) const;

    Position move(Position pos, char direction) const;

    bool isExit(Position pos) const;

    int getHeight() const;
    int getWidth() const;
};
