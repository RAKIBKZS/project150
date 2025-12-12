
#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>
#include <cstdlib>
using namespace std;

char board[3][3];
bool playerTurn = true;
bool gameOver = false;
char winner = ' ';

void resetBoard() {
    char c = '1';
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++) board[i][j] = c++;
}

bool checkWin(char m) {
    for (int i = 0; i < 3; i++) if (board[i][0] == m && board[i][1] == m && board[i][2] == m) return true;
    for (int j = 0; j < 3; j++) if (board[0][j] == m && board[1][j] == m && board[2][j] == m) return true;
    if (board[0][0] == m && board[1][1] == m && board[2][2] == m) return true;
    if (board[0][2] == m && board[1][1] == m && board[2][0] == m) return true;
    return false;
}

bool isDraw() {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] != 'X' && board[i][j] != 'O') return false;
    return true;
}

void aiMove() {
    vector<int> freeCells;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] != 'X' && board[i][j] != 'O') freeCells.push_back(i * 3 + j);
    if (freeCells.empty()) return;
    int r = freeCells[rand() % freeCells.size()];
    board[r / 3][r % 3] = 'O';
}

int main() {
    srand(time(0));
    resetBoard();

    sf::RenderWindow window(sf::VideoMode(600, 600), "Tic Tac Toe (SFML)");
    sf::Font font;
    if (!font.loadFromFile("ARIAL.TTF")) {
        return -1; // Make sure you have the font in your directory
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();

            // Player click
            if (event.type == sf::Event::MouseButtonPressed) {
                int x = event.mouseButton.x / 200;
                int y = event.mouseButton.y / 200;

                if (!gameOver) {
                    if (playerTurn && board[y][x] != 'X' && board[y][x] != 'O') {
                        board[y][x] = 'X';
                        if (checkWin('X')) { gameOver = true; winner = 'X'; }
                        else if (isDraw()) { gameOver = true; winner = 'D'; }
                        else playerTurn = false;
                    }
                }
                else {
                    // Restart game on click after game over
                    resetBoard();
                    gameOver = false;
                    winner = ' ';
                    playerTurn = true;
                }
            }
        }

        // AI Move
        if (!playerTurn && !gameOver) {
            aiMove();
            if (checkWin('O')) { gameOver = true; winner = 'O'; }
            else if (isDraw()) { gameOver = true; winner = 'D'; }
            playerTurn = true;
        }

        window.clear(sf::Color(30, 30, 30));

        // Draw grid
        sf::RectangleShape line(sf::Vector2f(600, 5));
        line.setFillColor(sf::Color::White);
        for (int i = 1; i < 3; i++) {
            line.setPosition(0, i * 200);
            window.draw(line);
        }
        line.setSize(sf::Vector2f(5, 600));
        for (int i = 1; i < 3; i++) {
            line.setPosition(i * 200, 0);
            window.draw(line);
        }

        // Draw X & O
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                sf::Text t;
                t.setFont(font);
                t.setCharacterSize(120);
                t.setPosition(j * 200 + 60, i * 200 + 20);
                if (board[i][j] == 'X') { t.setString("X"); t.setFillColor(sf::Color::Red); }
                else if (board[i][j] == 'O') { t.setString("O"); t.setFillColor(sf::Color::Green); }
                else continue;
                window.draw(t);
            }
        }

        // Display winner or draw
        if (gameOver) {
            sf::Text msg;
            msg.setFont(font);
            msg.setCharacterSize(70);
            msg.setFillColor(sf::Color::Yellow);
            msg.setPosition(100, 250);

            if (winner == 'X') msg.setString("Player Wins!");
            else if (winner == 'O') msg.setString("AI Wins!");
            else msg.setString("Draw!");

            window.draw(msg);
        }

        window.display();
    }

    return 0;
}
