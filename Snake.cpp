#include <iostream>
#include <conio.h>
#include <windows.h>
#include <fstream>

using namespace std;

enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
Direction dir;
bool gameOver;
const int height = 20;
const int width = 20;
int headX, headY, fruitX, fruitY, score;
int tailx[100], taily[100];
int tail_len;

// Function declarations
void setup();
void draw();
void input();
void logic();
void setColor(int textColor, int backgroundColor);
void updateHighScore(int score);

int main() {
    char start;
    cout << "\t-------------------------------" << endl;
    cout << "\t\t :Snake King:" << endl;
    cout << "\t-------------------------------" << endl;
    cout << "\tPress 's' to start: ";
    cin >> start;

    if (start == 's') {
        setup();
        while (!gameOver) {
            draw();
            input();
            logic();
            
            // Dynamic speed adjustment
            int speed = 100 - (score / 2); // Decrease delay as score increases
            if (speed < 10) {
                speed = 10; // Minimum delay
            }
            Sleep(speed); 
            
            system("cls");
        }
        updateHighScore(score); // Update high score when game ends
    }
}

void setup() {
    gameOver = false;
    dir = STOP;
    headX = width / 2;
    headY = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
    tail_len = 0;
}

void draw() {
    system("cls");
    setColor(14, 0); // Set color for borders and text

    // Upper Border
    cout << "\t\t";
    for (int i = 0; i < width - 8; i++) {
        cout << "||";
    }
    cout << endl;

    // Snake, fruit, space and side borders
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            // Left border
            if (j == 0) {
                cout << "\t\t||";
            }
            // Snake head
            if (i == headY && j == headX) {
                setColor(10, 0); // Set color for snake head
                cout << "O";
            }
            // Fruit
            else if (i == fruitY && j == fruitX) {
                setColor(12, 0); // Set color for fruit
                cout << "*";
            }
            // Space, snake tail
            else {
                bool print = false;
                // Tail
                for (int k = 0; k < tail_len; k++) {
                    if (tailx[k] == j && taily[k] == i) {
                        setColor(11, 0); // Set color for snake tail
                        cout << "o";
                        print = true;
                    }
                }
                // Space
                if (!print) {
                    cout << " ";
                }
            }
            // Right border
            if (j == width - 1) {
                cout << "||";
            }
        }
        cout << endl;
    }

    // Lower Border
    cout << "\t\t";
    for (int i = 0; i < width - 8; i++) {
        cout << "||";
    }
    cout << endl;

    setColor(15, 0); // Reset color
    cout << "\t\t\tScore: " << score << endl;
}

void input() {
    if (_kbhit()) {
        switch (getch()) {
            case 'a':
                dir = LEFT;
                break;
            case 'd':
                dir = RIGHT;
                break;
            case 'w':
                dir = UP;
                break;
            case 's':
                dir = DOWN;
                break;
            default:
                break;
        }
    }
}

void logic() {
    // Tail logic
    int prevx = tailx[0];
    int prevy = taily[0];
    int prev2x, prev2y;
    tailx[0] = headX;
    taily[0] = headY;
    for (int i = 1; i < tail_len; i++) {
        prev2x = tailx[i];
        prev2y = taily[i];
        tailx[i] = prevx;
        taily[i] = prevy;
        prevx = prev2x;
        prevy = prev2y;
    }

    // Direction logic
    switch (dir) {
        case LEFT:
            headX--;
            break;
        case RIGHT:
            headX++;
            break;
        case UP:
            headY--;
            break;
        case DOWN:
            headY++;
            break;
        default:
            break;
    }

    // Touch walls
    if (headX >= width) {
        headX = 0;
    } else if (headX < 0) {
        headX = width - 1;
    }
    if (headY >= height) {
        headY = 0;
    } else if (headY < 0) {
        headY = height - 1;
    }

    // Snake bites itself
    for (int i = 0; i < tail_len; i++) {
        if (tailx[i] == headX && taily[i] == headY) {
            gameOver = true;
        }
    }

    // Snake eats fruit
    if (headX == fruitX && headY == fruitY) {
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        tail_len++;
    }
}

void setColor(int textColor, int backgroundColor) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (WORD)((backgroundColor << 4) | textColor));
}

void updateHighScore(int score) {
    ifstream infile("highscore.txt");
    int highScore;
    infile >> highScore;
    infile.close();

    if (score > highScore) {
        ofstream outfile("highscore.txt");
        outfile << score;
        outfile.close();
        cout << "New High Score: " << score << endl;
    }
}
