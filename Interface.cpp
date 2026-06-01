#include "Interface.hpp"

namespace {
int levelSize(int baseSize, int level) {
    return baseSize + (level - 1) * kLevelSizeStep;
}

int mazePixelWidthFor(int level) {
    return levelSize(kBaseMazeWidth, level) * kCellSize + static_cast<int>(kWallSize);
}

int mazePixelHeightFor(int level) {
    return levelSize(kBaseMazeHeight, level) * kCellSize + static_cast<int>(kWallSize);
}

sf::Vector2u fixedWindowSize() {
    return {
        static_cast<unsigned int>(mazePixelWidthFor(kMaxLevel)),
        static_cast<unsigned int>(mazePixelHeightFor(kMaxLevel))
    };
}

std::string makeWindowTitle(int level) {
    return "MazeGame | Level " + std::to_string(level) + "/" + std::to_string(kMaxLevel) +
           " | W A S D - move, R - restart, Enter - next, F11 - fullscreen, Esc - exit";
}

void createGameWindow(sf::RenderWindow& window, int level, bool fullscreen) {
    if (fullscreen) {
        window.create(sf::VideoMode::getDesktopMode(), makeWindowTitle(level), sf::Style::Fullscreen);
    } else {
        const sf::Vector2u size = fixedWindowSize();
        window.create(sf::VideoMode(size.x, size.y), makeWindowTitle(level), sf::Style::Titlebar | sf::Style::Close);
    }

    window.setFramerateLimit(60);
}

void updateMazeView(sf::RenderWindow& window, const Maze& maze) {
    const float mazeWidth = static_cast<float>(maze.getWidth() * kCellSize + static_cast<int>(kWallSize));
    const float mazeHeight = static_cast<float>(maze.getHeight() * kCellSize + static_cast<int>(kWallSize));

    sf::View mazeView({mazeWidth / 2.0f, mazeHeight / 2.0f}, {mazeWidth, mazeHeight});

    const sf::Vector2u windowSize = window.getSize();
    const float windowRatio = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);
    const float mazeRatio = mazeWidth / mazeHeight;

    if (windowRatio > mazeRatio) {
        const float viewportWidth = mazeRatio / windowRatio;
        mazeView.setViewport({(1.0f - viewportWidth) / 2.0f, 0.0f, viewportWidth, 1.0f});
    } else {
        const float viewportHeight = windowRatio / mazeRatio;
        mazeView.setViewport({0.0f, (1.0f - viewportHeight) / 2.0f, 1.0f, viewportHeight});
    }

    window.setView(mazeView);
}

void updateUiView(sf::RenderWindow& window) {
    const sf::Vector2u windowSize = window.getSize();
    window.setView(sf::View(
        sf::FloatRect(0.0f, 0.0f, static_cast<float>(windowSize.x), static_cast<float>(windowSize.y))
    ));
}

void resetLevel(std::unique_ptr<Maze>& maze, Position& player, int level) {
    maze = std::make_unique<Maze>(
        levelSize(kBaseMazeHeight, level),
        levelSize(kBaseMazeWidth, level)
    );
    maze->generateMaze();

    player = {0, 0};
}

bool loadCompletionFont(sf::Font& font) {
#ifndef COMPLETION_FONT_FILENAME
#define COMPLETION_FONT_FILENAME "completion.ttf"
#endif

    return font.loadFromFile(COMPLETION_FONT_FILENAME) ||
           font.loadFromFile("fonts/completion.ttf") ||
           font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf") ||
           font.loadFromFile("/usr/share/fonts/truetype/liberation2/LiberationSans-Regular.ttf") ||
           font.loadFromFile("C:/Windows/Fonts/segoeui.ttf") ||
           font.loadFromFile("C:/Windows/Fonts/arial.ttf");
}

void drawCenteredText(sf::RenderWindow& window,
                      const sf::Font& font,
                      const std::string& text,
                      unsigned int characterSize,
                      const sf::Vector2f& center) {
    sf::Text label;
    label.setFont(font);
    label.setString(sf::String::fromUtf8(text.begin(), text.end()));
    label.setCharacterSize(characterSize);
    label.setFillColor(sf::Color(45, 45, 45));

    const sf::FloatRect bounds = label.getLocalBounds();
    label.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
    label.setPosition(center);
    window.draw(label);
}

void drawCompletionPanel(sf::RenderWindow& window, const sf::Font& font, bool isFontLoaded, int level) {
    updateUiView(window);

    const sf::Vector2u windowSize = window.getSize();
    const sf::Vector2f panelSize(
        std::min(520.0f, static_cast<float>(windowSize.x) - 40.0f),
        120.0f
    );
    const sf::Vector2f panelPosition(
        (static_cast<float>(windowSize.x) - panelSize.x) / 2.0f,
        (static_cast<float>(windowSize.y) - panelSize.y) / 2.0f
    );

    sf::RectangleShape completionPanel(panelSize);
    completionPanel.setPosition(panelPosition);
    completionPanel.setFillColor(sf::Color(255, 244, 190));
    completionPanel.setOutlineColor(sf::Color(45, 45, 45));
    completionPanel.setOutlineThickness(3.0f);
    window.draw(completionPanel);

    if (isFontLoaded) {
        const bool lastLevel = level == kMaxLevel;
        const std::string title = lastLevel
            ? "Congratulations! You passed all levels!"
            : "Level " + std::to_string(level) + " complete!";
        const std::string hint = lastLevel
            ? "Press R to restart this level or Esc to exit"
            : "Press Enter or Space to start level " + std::to_string(level + 1);

        drawCenteredText(
            window,
            font,
            title,
            26,
            {panelPosition.x + panelSize.x / 2.0f, panelPosition.y + 42.0f}
        );
        drawCenteredText(
            window,
            font,
            hint,
            18,
            {panelPosition.x + panelSize.x / 2.0f, panelPosition.y + 83.0f}
        );
    }
}
}

void startGame() {
    int level = 1;
    bool completed = false;
    bool fullscreen = false;
    Position player{0, 0};

    std::unique_ptr<Maze> maze;
    resetLevel(maze, player, level);

    sf::RenderWindow window;
    createGameWindow(window, level, fullscreen);

    static sf::Font completionFont;
    static const bool isCompletionFontLoaded = loadCompletionFont(completionFont);

    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::Resized) {
                updateUiView(window);
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
                        completed = false;
                        resetLevel(maze, player, level);
                        createGameWindow(window, level, fullscreen);
                        break;
                    case sf::Keyboard::Enter:
                    case sf::Keyboard::Space:
                        if (completed && level < kMaxLevel) {
                            ++level;
                            completed = false;
                            resetLevel(maze, player, level);
                            createGameWindow(window, level, fullscreen);
                        }
                        break;
                    case sf::Keyboard::F11:
                        fullscreen = !fullscreen;
                        createGameWindow(window, level, fullscreen);
                        break;
                    case sf::Keyboard::Escape:
                        window.close();
                        break;
                    default:
                        break;
                }

                if (!completed && direction != '\0') {
                    player = maze->move(player, direction);
                    if (maze->isExit(player)) {
                        completed = true;
                    }
                }
            }
        }

        window.clear(sf::Color(245, 245, 245));

        updateMazeView(window, *maze);
        maze->show(window, player);

        if (completed) {
            drawCompletionPanel(window, completionFont, isCompletionFontLoaded, level);
        }

        window.display();
    }
}
