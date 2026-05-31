#include "Interface.hpp"

void startGame() {
    Maze* maze = new Maze(kMazeHeight, kMazeWidth);
    maze->generateMaze();

    Position player{0, 0};

    const int windowWidth = maze->getWidth() * kCellSize + static_cast<int>(kWallSize);
    const int windowHeight = maze->getHeight() * kCellSize + static_cast<int>(kWallSize);

     sf::RenderWindow window(
        sf::VideoMode(windowWidth, windowHeight),
        "MazeGame | W A S D - move, R - restart, Esc - exit"
    );


    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed) {
                char direction = '\0';
                switch (event.key.code) {
                    case sf::Keyboard::W:
                    case sf::Keyboard::Up:
                        direction = 'w';
                        break;
                    case sf::Keyboard::D:
                    case sf::Keyboard::Right:
                        direction = 'd';
                        break;
                    case sf::Keyboard::S:
                    case sf::Keyboard::Down:
                        direction = 's';
                        break;
                    case sf::Keyboard::A:
                    case sf::Keyboard::Left:
                        direction = 'a';
                        break;
                    case sf::Keyboard::R:
                        delete maze;
                        maze = new Maze(kMazeHeight, kMazeWidth);
                        maze->generateMaze();
                        window.setTitle("MazeGame | W A S D - move, R - restart, Esc - exit");
                        player = {0, 0};
                        break;
                    case sf::Keyboard::Escape:
                        window.close();
                        break;
                    default:
                        break;
                }

                if (direction != '\0') {
                    player = maze->move(player, direction);
                    if (maze->isExit(player)) {
                        window.setTitle("MazeGame | You have passed the maze! Press R to restart");
                    }
                }
            }
        }

        maze->show(window, player);
    }

    delete maze;
}
