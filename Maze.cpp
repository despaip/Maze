#include "Maze.hpp"

#include <queue>
#include <vector>

Maze::Maze(int height, int width)
    : height(height),
      width(width),
      grid(nullptr),
      rng(static_cast<unsigned>(std::chrono::steady_clock::now().time_since_epoch().count())),
      exit{width - 1, height - 1} {
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

void Maze::updateExitPosition(Position start) {
    std::vector<std::vector<int>> distance(height, std::vector<int>(width, -1));
    std::queue<Position> queue;

    distance[start.y][start.x] = 0;
    queue.push(start);
    exit = start;

    const char directions[4] = {'w', 'd', 's', 'a'};

    while (!queue.empty()) {
        Position current = queue.front();
        queue.pop();

        if (distance[current.y][current.x] > distance[exit.y][exit.x]) {
            exit = current;
        }

        for (char direction : directions) {
            Position next = move(current, direction);
            if (next.x == current.x && next.y == current.y) {
                continue;
            }
            if (distance[next.y][next.x] != -1) {
                continue;
            }

            distance[next.y][next.x] = distance[current.y][current.x] + 1;
            queue.push(next);
        }
    }
}

void Maze::generateMaze(int startX, int startY) {
    struct Edge {
        Position from;
        Position to;
    };

    std::vector<Edge> frontier;

    auto addFrontier = [&](Position cell) {
        const Position neighbours[4] = {
            {cell.x - 1, cell.y},
            {cell.x, cell.y - 1},
            {cell.x + 1, cell.y},
            {cell.x, cell.y + 1}
        };

        for (Position next : neighbours) {
            if (isValidCoordinates(next.x, next.y) &&
                !grid[next.y][next.x].visited) {
                frontier.push_back({cell, next});
            }
        }
    };

    Position start{startX, startY};

    grid[start.y][start.x].visited = true;
    addFrontier(start);

    while (!frontier.empty()) {
        std::uniform_int_distribution<int> dist(
            0,
            static_cast<int>(frontier.size()) - 1
        );

        int index = dist(rng);

        Edge edge = frontier[index];
        frontier.erase(frontier.begin() + index);

        if (grid[edge.to.y][edge.to.x].visited) {
            continue;
        }

        removeWall(edge.from, edge.to);

        grid[edge.to.y][edge.to.x].visited = true;

        addFrontier(edge.to);
    }

    const int extraPassages = std::max(1, width * height / 30);
    std::uniform_int_distribution<int> xDist(0, width - 1);
    std::uniform_int_distribution<int> yDist(0, height - 1);
    std::uniform_int_distribution<int> directionDist(0, 3);

    int createdPassages = 0;
    int attempts = 0;
    const int maxAttempts = extraPassages * 20;

    while (createdPassages < extraPassages && attempts < maxAttempts) {
        ++attempts;

        Position current{xDist(rng), yDist(rng)};
        Position next = current;

        switch (directionDist(rng)) {
            case 0: --next.x; break;
            case 1: --next.y; break;
            case 2: ++next.x; break;
            case 3: ++next.y; break;
            default: break;
        }

        if (!isValidCoordinates(next.x, next.y)) {
            continue;
        }

        const bool wallExists =
            (current.x == next.x && current.y > next.y && grid[current.y][current.x].isShowTopWall) ||
            (current.x == next.x && current.y < next.y && grid[current.y][current.x].isShowBottomWall) ||
            (current.y == next.y && current.x > next.x && grid[current.y][current.x].isShowLeftWall) ||
            (current.y == next.y && current.x < next.x && grid[current.y][current.x].isShowRightWall);

        if (!wallExists) {
            continue;
        }

        removeWall(current, next);
        ++createdPassages;
    }

    updateExitPosition({startX, startY});
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
        static_cast<float>(exit.x * kCellSize + kWallSize),
        static_cast<float>(exit.y * kCellSize + kWallSize)
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
    return pos.x == exit.x && pos.y == exit.y;
}

int Maze::getHeight() const {
    return height;
}

int Maze::getWidth() const {
    return width;
}
