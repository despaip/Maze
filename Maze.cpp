#include "Maze.hpp"

Maze::Maze(int height, int width)
    : height(height),
      width(width),
      grid(nullptr),
      rng(static_cast<unsigned>(std::chrono::steady_clock::now().time_since_epoch().count())) {
    grid = new Cell*[height];
    for (int i = 0; i < height; ++i) {
        grid[i] = new Cell[width];
    }
}

Maze::~Maze() {
    for (int i = 0; i < height; ++i) {
        delete[] grid[i];
    }
    delete[] grid;
}

bool Maze::isValidCoordinates(int x, int y) const {
    return x >= 0 && y >= 0 && x < width && y < height;
}

int Maze::findUnvisitedNeighbours(int x, int y, Position* unvisitedNeighbours) {
    int cnt = 0;

    if (isValidCoordinates(x - 1, y) && !grid[y][x - 1].visited) {
        unvisitedNeighbours[cnt++] = {x - 1, y};
    }
    if (isValidCoordinates(x, y - 1) && !grid[y - 1][x].visited) {
        unvisitedNeighbours[cnt++] = {x, y - 1};
    }
    if (isValidCoordinates(x + 1, y) && !grid[y][x + 1].visited) {
        unvisitedNeighbours[cnt++] = {x + 1, y};
    }
    if (isValidCoordinates(x, y + 1) && !grid[y + 1][x].visited) {
        unvisitedNeighbours[cnt++] = {x, y + 1};
    }

    std::shuffle(unvisitedNeighbours, unvisitedNeighbours + cnt, rng);
    return cnt;
}

void Maze::removeWall(Position a, Position b) {
    if (a.x == b.x) {
        if (a.y > b.y) {
            grid[a.y][a.x].isShowTopWall = false;
            grid[b.y][b.x].isShowBottomWall = false;
        } else {
            grid[a.y][a.x].isShowBottomWall = false;
            grid[b.y][b.x].isShowTopWall = false;
        }
    } else if (a.y == b.y) {
        if (a.x > b.x) {
            grid[a.y][a.x].isShowLeftWall = false;
            grid[b.y][b.x].isShowRightWall = false;
        } else {
            grid[a.y][a.x].isShowRightWall = false;
            grid[b.y][b.x].isShowLeftWall = false;
        }
    }
}

void Maze::generateMaze(int startX, int startY) {
    std::stack<Position> stack;

    stack.push({startX, startY});
    grid[startY][startX].visited = true;

    while (!stack.empty()) {
        Position current = stack.top();
        Position unvisitedNeighbours[4];

        int numberOfNeighbours = findUnvisitedNeighbours(current.x, current.y, unvisitedNeighbours);
        if (numberOfNeighbours != 0) {
            Position next = unvisitedNeighbours[0];
            removeWall(current, next);
            stack.push(next);
            grid[next.y][next.x].visited = true;
        } else {
            stack.pop();
        }
    }
}

void Maze::drawWall(sf::RenderWindow& window, float x, float y, float width, float height) const {
    sf::RectangleShape wall({width, height});
    wall.setPosition(x, y);
    wall.setFillColor(sf::Color(45, 45, 45));
    window.draw(wall);
}

void Maze::show(sf::RenderWindow& window, Position player) const {
    sf::RectangleShape start({kCellSize, kCellSize});
    start.setPosition(kWallSize, kWallSize);
    start.setFillColor(sf::Color(140, 210, 140));
    window.draw(start);

    sf::RectangleShape finish({kCellSize, kCellSize});
    finish.setPosition(
        static_cast<float>((width - 1) * kCellSize + kWallSize),
        static_cast<float>((height - 1) * kCellSize + kWallSize)
    );
    finish.setFillColor(sf::Color(230, 120, 120));
    window.draw(finish);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const float left = static_cast<float>(x * kCellSize);
            const float top = static_cast<float>(y * kCellSize);

            if (grid[y][x].isShowTopWall) {
                drawWall(window, left, top, kCellSize + kWallSize, kWallSize);
            }
            if (grid[y][x].isShowLeftWall) {
                drawWall(window, left, top, kWallSize, kCellSize + kWallSize);
            }
            if (grid[y][x].isShowRightWall) {
                drawWall(window, left + kCellSize, top, kWallSize, kCellSize + kWallSize);
            }
            if (grid[y][x].isShowBottomWall) {
                drawWall(window, left, top + kCellSize, kCellSize + kWallSize, kWallSize);
            }
        }
    }

    sf::CircleShape playerShape(kCellSize * 0.32f);
    playerShape.setFillColor(sf::Color(70, 120, 230));
    playerShape.setPosition(
        static_cast<float>(player.x * kCellSize + kCellSize * 0.18f),
        static_cast<float>(player.y * kCellSize + kCellSize * 0.18f)
    );
    window.draw(playerShape);
}

bool Maze::canMove(Position pos, char direction) const {
    switch (direction) {
        case 'w': return !grid[pos.y][pos.x].isShowTopWall;
        case 'd': return !grid[pos.y][pos.x].isShowRightWall;
        case 's': return !grid[pos.y][pos.x].isShowBottomWall;
        case 'a': return !grid[pos.y][pos.x].isShowLeftWall;
        default: return false;
    }
}

Position Maze::move(Position pos, char direction) const {
    if (!canMove(pos, direction)) {
        return pos;
    }

    switch (direction) {
        case 'w': --pos.y; break;
        case 'd': ++pos.x; break;
        case 's': ++pos.y; break;
        case 'a': --pos.x; break;
        default: break;
    }
    return pos;
}

bool Maze::isExit(Position pos) const {
    return pos.x == width - 1 && pos.y == height - 1;
}

int Maze::getHeight() const {
    return height;
}

int Maze::getWidth() const {
    return width;
}
