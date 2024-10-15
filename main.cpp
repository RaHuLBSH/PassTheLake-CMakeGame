#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <sstream>

// Size of the grid
const int GRID_SIZE = 10;
const int TILE_SIZE = 50;
const int WINDOW_SIZE = GRID_SIZE * TILE_SIZE;
const int PADDING = 50;
const int WINDOW_SIZE_WITH_PADDING = WINDOW_SIZE + PADDING * 2; 

// Possible tile colors
enum TileColor { GREEN, BROWN, BLUE, TURQUOISE };

// Game states
enum GameState { MENU, PLAYING };

class Game {
private:
    std::vector<std::vector<TileColor>> grid;
    sf::Vector2i piratePosition;
    sf::RenderWindow window;
    GameState gameState;

    sf::Font font;
    sf::Text title;
    sf::Text easyButton, mediumButton, hardButton;
    sf::Texture pirateTexture;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::Text statusMessage;
    sf::Clock messageClock;
    sf::Clock gameClock;  
    sf::Clock titleBlinkClock; 
    sf::Text highScoreText; 
    sf::Text timerText;        // Text to display timer
    std::string formatFloat(float value) {
        std::stringstream stream;
        stream << std::fixed << std::setprecision(2) << value;
        return stream.str();
    }

    int selectedMode;
    float highScores[3] = {-1.0f, -1.0f, -1.0f}; // Store high scores for easy, medium, hard 
    float elapsedTime = 0.0f; 

    bool upPressed = false;    
    bool downPressed = false;  
    bool leftPressed = false;
    bool rightPressed = false;
    bool qPressed = false;
    bool pPressed = false;
    bool zPressed = false;
    bool mPressed = false;
    bool showMessage = false;

public:
    Game() : window(sf::VideoMode(WINDOW_SIZE_WITH_PADDING, WINDOW_SIZE_WITH_PADDING), "Pass the Lake"), gameState(MENU), selectedMode(1) {
        // Load font for text
        if (!font.loadFromFile("resources/arial.ttf")) {
            std::cerr << "Failed to load font\n";
            return;
        }
        if (!pirateTexture.loadFromFile("resources/pirate.png")) {
            std::cerr << "Error loading pirate texture!" << std::endl;
        }
        if (!backgroundTexture.loadFromFile("resources/background.jpeg")) {
            std::cerr << "Error loading background image!" << std::endl;
        }


        backgroundSprite.setTexture(backgroundTexture);
        backgroundSprite.setPosition(0, 0); // Set the position of the background if necessary
        float scaleX = (float)window.getSize().x / backgroundTexture.getSize().x;
        float scaleY = (float)window.getSize().y / backgroundTexture.getSize().y;
        backgroundSprite.setScale(scaleX, scaleY);

        // Set up menu text
        title.setFont(font);
        title.setString("PASS THE LAKE");
        title.setCharacterSize(40);
        title.setFillColor(sf::Color::Yellow);
        sf::FloatRect textRect = title.getLocalBounds();
        title.setOrigin(textRect.width / 2.0f, textRect.height / 2.0f);
        title.setPosition(window.getSize().x / 2.0f, 50); // Center horizontally, keep Y position the same


        easyButton.setFont(font);
        easyButton.setString("Easy");
        easyButton.setCharacterSize(30);
        easyButton.setFillColor(sf::Color::Green);
        sf::FloatRect easyRect = easyButton.getLocalBounds();
        easyButton.setOrigin(easyRect.width / 2.0f, easyRect.height / 2.0f);
        easyButton.setPosition(window.getSize().x / 2.0f, 150);

        mediumButton.setFont(font);
        mediumButton.setString("Medium");
        mediumButton.setCharacterSize(30);
        mediumButton.setFillColor(sf::Color::Red);
        sf::FloatRect mediumRect = mediumButton.getLocalBounds();
        mediumButton.setOrigin(mediumRect.width / 2.0f, mediumRect.height / 2.0f);
        mediumButton.setPosition(window.getSize().x / 2.0f, 200);

        hardButton.setFont(font);
        hardButton.setString("Hard");
        hardButton.setCharacterSize(30);
        hardButton.setFillColor(sf::Color::Red);
        sf::FloatRect hardRect = hardButton.getLocalBounds();
        hardButton.setOrigin(hardRect.width / 2.0f, hardRect.height / 2.0f);
        hardButton.setPosition(window.getSize().x / 2.0f, 250);

        statusMessage.setFont(font);
        statusMessage.setCharacterSize(30);
        statusMessage.setFillColor(sf::Color::Black); // Message color
        statusMessage.setPosition(10, WINDOW_SIZE - 40); // Position for the status message

        highScoreText.setFont(font);
        highScoreText.setCharacterSize(30);
        highScoreText.setFillColor(sf::Color::White);
        sf::FloatRect highRect = highScoreText.getLocalBounds();
        highScoreText.setOrigin(highRect.width / 2.0f, highRect.height / 2.0f);
        highScoreText.setPosition(window.getSize().x / 3.0f, 350);

        timerText.setFont(font);
        timerText.setCharacterSize(30);
        timerText.setFillColor(sf::Color::White);
        timerText.setPosition(PADDING, PADDING - 40); // Place the timer just above the grid

        // Initialize the grid with BROWN tiles
        grid.resize(GRID_SIZE, std::vector<TileColor>(GRID_SIZE, BROWN));
        grid[0][0] = GREEN; // Start position
        grid[GRID_SIZE - 1][GRID_SIZE - 1] = GREEN; // End position

        piratePosition = sf::Vector2i(0, 0);
        srand(static_cast<unsigned int>(time(0)));

        updateModeButtons(); // Update the mode buttons to show " < Play"
    }

    void generateGrid(int mode) {
        for (int i = 0; i < GRID_SIZE; i++) {
            for (int j = 0; j < GRID_SIZE; j++) {
                if (i == 0 && j == 0) continue;
                if (i == GRID_SIZE - 1 && j == GRID_SIZE - 1) continue;

                int randNum = rand() % 100;
                if (mode == 1) { // Easy
                    grid[i][j] = (randNum < 70) ? BROWN : BLUE;
                } else if (mode == 2) { // Medium
                    grid[i][j] = (randNum < 50) ? BROWN : (randNum < 80 ? BLUE : TURQUOISE);
                } else { // Hard
                    grid[i][j] = (randNum < 30) ? BROWN : (randNum < 70 ? BLUE : TURQUOISE);
                }
            }
        }
    }

    void drawGrid() {
        window.clear();
        window.draw(backgroundSprite);

        elapsedTime = gameClock.getElapsedTime().asSeconds();
        timerText.setString("Time: " + formatFloat(elapsedTime) + "s");

        for (int i = 0; i < GRID_SIZE; i++) {
            for (int j = 0; j < GRID_SIZE; j++) {
                sf::RectangleShape tile(sf::Vector2f(TILE_SIZE, TILE_SIZE));
                tile.setPosition(j * TILE_SIZE + PADDING, i * TILE_SIZE + PADDING);

                switch (grid[i][j]) {
                    case GREEN:
                        tile.setFillColor(sf::Color::Green);
                        break;
                    case BROWN:
                        tile.setFillColor(sf::Color(139, 69, 19)); // Brown
                        break;
                    case BLUE:
                        tile.setFillColor(sf::Color::Blue);
                        break;
                    case TURQUOISE:
                        tile.setFillColor(sf::Color(64, 224, 208)); // Turquoise
                        break;
                }

                window.draw(tile);
            }
        }

        // Draw the pirate with the texture
        sf::RectangleShape pirate(sf::Vector2f(TILE_SIZE, TILE_SIZE));
        pirate.setPosition(piratePosition.x * TILE_SIZE + PADDING, piratePosition.y * TILE_SIZE + PADDING);
        pirate.setTexture(&pirateTexture);
        window.draw(pirate);

        // Draw the status message
        window.draw(statusMessage);
        window.draw(timerText);

        window.display();
    }

    void updateTitleBlink() {
        float time = titleBlinkClock.getElapsedTime().asSeconds();
        if (static_cast<int>(time * 2) % 2 == 0) { // Toggles every half second
            title.setFillColor(sf::Color::Red); // Set the color you want
        } else {
            title.setFillColor(sf::Color::White); // Or any other color
        }
    }

    void drawMenu() {
        window.clear();
        window.draw(backgroundSprite);
        updateTitleBlink();
        window.draw(title);
        window.draw(easyButton);
        window.draw(mediumButton);
        window.draw(hardButton);

        // Display high scores conditionally
        std::string highScoreString = "High Scores\n";
        
        highScoreString += "Easy: " + (highScores[0] != -1.0f ? formatFloat(highScores[0]) + "s" : "N/A") + "\n";
        highScoreString += "Medium: " + (highScores[1] != -1.0f ? formatFloat(highScores[1]) + "s" : "N/A") + "\n";
        highScoreString += "Hard: " + (highScores[2] != -1.0f ? formatFloat(highScores[2]) + "s" : "N/A");
        
        highScoreText.setString(highScoreString);
        window.draw(highScoreText);

        window.display();
    }


    void movePirate(int dx, int dy) {
        int newX = piratePosition.x + dx;
        int newY = piratePosition.y + dy;

        if (newX >= 0 && newX < GRID_SIZE && newY >= 0 && newY < GRID_SIZE) {
            piratePosition.x = newX;
            piratePosition.y = newY;

            if (newX == GRID_SIZE - 1 && newY == GRID_SIZE - 1) {
                if (highScores[selectedMode - 1] == -1.0f || elapsedTime < highScores[selectedMode - 1]) {
                    highScores[selectedMode - 1] = elapsedTime;
                }
                statusMessage.setString("Challenge Completed!");
                statusMessage.setPosition(WINDOW_SIZE_WITH_PADDING / 2 - statusMessage.getGlobalBounds().width / 2, WINDOW_SIZE_WITH_PADDING / 2);
                statusMessage.setFillColor(sf::Color::Green);
                showMessage = true; // Set the flag to show the message
                messageClock.restart(); // Start the clock
            } else if (grid[newY][newX] == BLUE || (grid[newY][newX] == TURQUOISE && rand() % 2 == 0)) {
                statusMessage.setString("You Drowned");
                statusMessage.setPosition(WINDOW_SIZE_WITH_PADDING / 2 - statusMessage.getGlobalBounds().width / 2, WINDOW_SIZE_WITH_PADDING / 2);
                statusMessage.setFillColor(sf::Color::Red);
                showMessage = true; // Set the flag to show the message
                messageClock.restart(); // Start the clock
            } else {
                statusMessage.setString("");
                generateGrid(selectedMode);
                grid[newY][newX] = GREEN;
            }
        }
    }

    void run() {
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
            }

            if (gameState == MENU) {
                handleMenuInput();
                drawMenu();
            } 
            else if (gameState == PLAYING) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace)) {
                    gameState = MENU;
                    statusMessage.setString("");
                } else if (!showMessage) {
                    handlePirateMovement();
                }

                // If a message is shown, check how much time has passed
                if (showMessage) {
                    if (messageClock.getElapsedTime().asSeconds() >= 2.0f) {
                        showMessage = false;
                        gameState = MENU; // Redirect to menu after 2 seconds
                        statusMessage.setString(""); // Clear message
                        piratePosition = sf::Vector2i(0, 0); // Reset pirate position
                    }
                }

                drawGrid();
            }
        }
    }


    void updateModeButtons() {
        easyButton.setFillColor(selectedMode == 1 ? sf::Color::Green : sf::Color::Red);
        mediumButton.setFillColor(selectedMode == 2 ? sf::Color::Green : sf::Color::Red);
        hardButton.setFillColor(selectedMode == 3 ? sf::Color::Green : sf::Color::Red);

        // Update the play indication
        easyButton.setString("Easy" + std::string(selectedMode == 1 ? " < Play" : ""));
        mediumButton.setString("Medium" + std::string(selectedMode == 2 ? " < Play" : ""));
        hardButton.setString("Hard" + std::string(selectedMode == 3 ? " < Play" : ""));
    }

    void handleMenuInput() {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !upPressed) {
            upPressed = true;
            selectedMode = (selectedMode == 1) ? 3 : selectedMode - 1;
            updateModeButtons();
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !downPressed) {
            downPressed = true;
            selectedMode = (selectedMode == 3) ? 1 : selectedMode + 1;
            updateModeButtons();
        } else {
            if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                upPressed = false;
            }
            if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                downPressed = false;
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
            gameState = PLAYING;
            gameClock.restart();
            generateGrid(selectedMode);
        }
    }

    void handlePirateMovement() {
        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad2)) && !upPressed) {
            upPressed = true;
            movePirate(0, -1);
        } else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad2)) {
            upPressed = false; // Reset if key is not pressed
        }

        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad8)) && !downPressed) {
            downPressed = true;
            movePirate(0, 1);
        } else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad8)) {
            downPressed = false;
        }

        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad4)) && !leftPressed) {
            leftPressed = true;
            movePirate(-1, 0);
        } else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad4)) {
            leftPressed = false;
        }

        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad6)) && !rightPressed) {
            rightPressed = true;
            movePirate(1, 0);
        } else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad6)) {
            rightPressed = false;
        }

        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Q) || sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad1)) && !qPressed) {
            qPressed = true;
            movePirate(-1, -1); // Move north-west
        } else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Q) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad1)) {
            qPressed = false;
        }

        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::P) || sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad3)) && !pPressed) {
            pPressed = true;
            movePirate(1, -1); // Move north-east
        } else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::P) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad3)) {
            pPressed = false;
        }

        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Z) || sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad7)) && !zPressed) {
            zPressed = true;
            movePirate(-1, 1); // Move south-west
        } else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad7)) {
            zPressed = false;
        }

        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::M) || sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad9)) && !mPressed) {
            mPressed = true;
            movePirate(1, 1); // Move south-east
        } else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::M) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad9)) {
            mPressed = false;
        }
    }

};

int main() {
    Game game;
    game.run();
    return 0;
}
