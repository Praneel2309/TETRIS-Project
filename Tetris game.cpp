#include<bits/stdc++.h>
#include <conio.h>
#include <Windows.h>

using namespace std;

// Key codes (Specific libraries)
const int KEY_ESC = 27, KEY_LEFT = 75, KEY_RIGHT = 77, KEY_UP = 72, KEY_DOWN = 80, KEY_SPACE = 32, KEY_ENTER = 13;

// Game constants
const int GRID_WIDTH = 10, GRID_HEIGHT = 20, INITIAL_SPEED = 500, SPEED_INCREMENT = 50, LINES_PER_LEVEL = 10, SCORE_THRESHOLD = 300;

// Console colors(giving colors by numbers)
enum ConsoleColor {
    BLACK = 0, BLUE = 1, GREEN = 2, CYAN = 3, RED = 4, MAGENTA = 5, BROWN = 6, LIGHTGRAY = 7, 
    DARKGRAY = 8, LIGHTBLUE = 9, LIGHTGREEN = 10, LIGHTCYAN = 11, LIGHTRED = 12, LIGHTMAGENTA = 13, YELLOW = 14, WHITE = 15
};

// Tetromino colors mapping
const ConsoleColor TETROMINO_COLORS[] = {BLUE, YELLOW, LIGHTMAGENTA, RED, GREEN, LIGHTRED, BROWN};

// Tetromino shapes{0=blank,1=color}
const vector<vector<vector<int>>> TETROMINOES = {
    {{0,0,0,0}, {1,1,1,1}, {0,0,0,0}, {0,0,0,0}}, // I
    {{1,1}, {1,1}}, // O
    {{0,1,0}, {1,1,1}, {0,0,0}}, // T
    {{0,1,1}, {1,1,0}, {0,0,0}}, // S
    {{1,1,0}, {0,1,1}, {0,0,0}}, // Z
    {{1,0,0}, {1,1,1}, {0,0,0}}, // J
    {{0,0,1}, {1,1,1}, {0,0,0}}  // L
};

// ASCII Art for the intro screen with gaps between letters
const string TETRIS_LOGO[] = {
    "  _______  ______  _______  _____   _____   _____  ",
    " |__   __||  ____||__   __||  __ \\ |_   _| / ____| ",
    "    | |   | |__      | |   | |__) |  | |  | (___   ",
    "    | |   |  __|     | |   |  _  /   | |   \\___ \\  ",
    "    | |   | |____    | |   | | \\ \\  _| |_  ____) | ",
    "    |_|   |______|   |_|   |_|  \\_\\|_____||_____/  "
};


// Utility functions for console handling{prevent flickering by pasting over itself}
void setCursorPosition(int x, int y) {
    COORD coord = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

//For background & text color 
void setConsoleColor(ConsoleColor textColor, ConsoleColor bgColor = BLACK) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (textColor) | (bgColor << 4));
}

//prevents color overlapping 
void resetConsoleColor() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE | (BLACK << 4));
}

// Intro Animation Class
class IntroAnimation {
private:
    int screenWidth, screenHeight;
    vector<pair<int, int>> fallingBlocks;
    mt19937 rng; //Mersenne Twister - random block sequence generator
    
public:
    IntroAnimation() {
        // Get console window size
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        screenWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        screenHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        
        // Initialize RNG and falling blocks
        rng = mt19937(random_device{}());
        generateFallingBlocks(30);
    }
    
    //falling blocks for intro animation
    void generateFallingBlocks(int count) {
        uniform_int_distribution<int> distX(0, screenWidth - 2);
        uniform_int_distribution<int> distY(-20, 5); //falling downward
        
        fallingBlocks.clear();
        for (int i = 0; i < count; i++) {
            fallingBlocks.push_back(make_pair(distX(rng), distY(rng)));
        }
    }
    
    //Displaying TETRIS logo and "Welcome" message
    void drawLogo(int centerX, int centerY) {
        int logoHeight = sizeof(TETRIS_LOGO) / sizeof(TETRIS_LOGO[0]);
        int logoWidth = TETRIS_LOGO[0].length();
        int startX = centerX - logoWidth / 2;
        int startY = centerY - logoHeight / 2;
        
        ConsoleColor mixedColors[] = {BLUE, RED, YELLOW, GREEN, CYAN, LIGHTMAGENTA};
        
        for (int i = 0; i < logoHeight; i++) {
            setCursorPosition(startX, startY + i);
            setConsoleColor(mixedColors[i % 6]);
            cout << TETRIS_LOGO[i];
        }
        
        string welcomeMsg = "W E L C O M E  T O  T E T R I S";
        setCursorPosition(centerX - welcomeMsg.length() / 2, centerY + 6);
        
        for (size_t i = 0; i < welcomeMsg.length(); i++) {
            setConsoleColor(mixedColors[i % 6]);
            cout << welcomeMsg[i];
        }
        
        resetConsoleColor();
    }
    
    //update position and re-draw in each fall
    void updateAndDrawBlocks() {
        int logoHeight = sizeof(TETRIS_LOGO) / sizeof(TETRIS_LOGO[0]);
        int logoWidth = TETRIS_LOGO[0].length();
        int logoStartX = screenWidth / 2 - logoWidth / 2;
        int logoStartY = screenHeight / 2 - 6 - logoHeight / 2;
        int logoEndX = logoStartX + logoWidth;
        int logoEndY = logoStartY + logoHeight;
        
        int instructionsStartY = logoEndY + 1;
        int instructionsEndY = instructionsStartY + 9;
        int instructionsStartX = screenWidth / 2 - 20;
        int instructionsEndX = screenWidth / 2 + 20;
        
        for (auto& block : fallingBlocks) {
            block.second++;
            if (block.second > screenHeight) {
                uniform_int_distribution<int> distX(0, screenWidth - 2);
                block.first = distX(rng);
                block.second = -2;
            }
        }
    
        for (size_t i = 0; i < fallingBlocks.size(); i++) {
            if (fallingBlocks[i].second < 0 || fallingBlocks[i].second >= screenHeight) continue;
            
            int blockX = fallingBlocks[i].first;
            int blockY = fallingBlocks[i].second;
            
            // Skip if block is in logo or instructions area
            if ((blockX >= logoStartX && blockX <= logoEndX && blockY >= logoStartY && blockY <= logoEndY) ||
                (blockX >= instructionsStartX && blockX <= instructionsEndX && blockY >= instructionsStartY && blockY <= instructionsEndY))
                continue;
            
            setCursorPosition(blockX, blockY);
            setConsoleColor(TETROMINO_COLORS[i % 7]);
            cout << "##";
        }
        resetConsoleColor();
    }
    
    //To display IntroAnimation and game
    void run() {
        system("cls");
        
        // Hide cursor
        CONSOLE_CURSOR_INFO cursorInfo = {100, FALSE};
        SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
        
        int centerX = screenWidth / 2;
        int centerY = screenHeight / 2 - 6;
        
        int frames = 0;
        while (true) {
            if (frames % 10 == 0) system("cls");
            
            updateAndDrawBlocks();
            drawLogo(centerX, centerY);
            
            // Draw instructions
            const vector<pair<int, ConsoleColor>> instructions = {
                {8, LIGHTCYAN}, {10, LIGHTGREEN}, {12, YELLOW}, {14, LIGHTRED}
            };
            
            const vector<string> instructionText = {
                "Press [ENTER] to start the game",
                "Use Arrow Keys to Move and Rotate pieces",
                "[SPACE] for hard drop",
                "[P] to pause the game"
            };
            
            for (size_t i = 0; i < instructions.size(); i++) {
                setCursorPosition(centerX - (instructionText[i].length() / 2), centerY + instructions[i].first);
                setConsoleColor(instructions[i].second);
                cout << instructionText[i];
            }
            
            resetConsoleColor();
            
            if (_kbhit() && _getch() == KEY_ENTER) break;
            
            Sleep(70);
            frames++;
        }
        
        system("cls");
    }
};

// Tetromino class
class Tetromino {
private:
    int type, rotation, x, y;
    ConsoleColor color;

public:
    Tetromino(int type) : type(type), rotation(0), x(GRID_WIDTH / 2 - 2), y(0), color(TETROMINO_COLORS[type]) {}

    vector<vector<int>> getShape() const {
        vector<vector<int>> shape = TETROMINOES[type];
        for (int r = 0; r < rotation; r++) {
            shape = rotateMatrix(shape);
        }
        return shape;
    }

    vector<vector<int>> rotateMatrix(const vector<vector<int>>& matrix) const {
        int n = matrix.size();
        vector<vector<int>> rotated(n, vector<int>(n, 0));
        
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                rotated[j][n - 1 - i] = matrix[i][j];
                
        return rotated;
    }

    void rotate() { rotation = (rotation + 1) % 4; }
    void moveLeft() { x--; }
    void moveRight() { x++; }
    void moveDown() { y++; }

    int getX() const { return x; } //To bring the terominoes to next position
    int getY() const { return y; }
    ConsoleColor getColor() const { return color; }
    int getType() const { return type; }
    
    void setX(int newX) { x = newX; }
    void setY(int newY) { y = newY; }
    void setRotation(int newRotation) { rotation = newRotation; }
};

// Game class
class TetrisGame {
private:
    vector<vector<int>> grid;
    vector<vector<int>> colorGrid;
    Tetromino* currentPiece;
    Tetromino* nextPiece;
    bool gameOver, paused;
    int score, level, linesCleared, fallSpeed;
    std::chrono::time_point<std::chrono::system_clock> lastFall;
    std::mt19937 rng;

public:
    TetrisGame() : 
        grid(GRID_HEIGHT, vector<int>(GRID_WIDTH, 0)),
        colorGrid(GRID_HEIGHT, vector<int>(GRID_WIDTH, 0)),
        gameOver(false), paused(false), score(0), level(1), linesCleared(0), fallSpeed(INITIAL_SPEED) {
        
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        rng = mt19937(seed);
        
        currentPiece = new Tetromino(generateRandomPieceType());
        nextPiece = new Tetromino(generateRandomPieceType());
        
        lastFall = chrono::system_clock::now();
    }

    ~TetrisGame() {
        delete currentPiece;
        delete nextPiece;
    }

    //resets for running
    void reset() {
        grid.assign(GRID_HEIGHT, vector<int>(GRID_WIDTH, 0));
        colorGrid.assign(GRID_HEIGHT, vector<int>(GRID_WIDTH, 0));
        delete currentPiece;
        delete nextPiece;
        
        rng.seed(chrono::system_clock::now().time_since_epoch().count());
        
        currentPiece = new Tetromino(generateRandomPieceType());
        nextPiece = new Tetromino(generateRandomPieceType());
        gameOver = false;
        paused = false;
        score = 0;
        level = 1;
        linesCleared = 0;
        fallSpeed = INITIAL_SPEED;
        lastFall = chrono::system_clock::now();
    }

    int generateRandomPieceType() {
        std::uniform_int_distribution<int> dist(0, TETROMINOES.size() - 1);
        return dist(rng);
    }

    //To ensure block overlap
    bool isCollision(const Tetromino& piece) const { 
        vector<vector<int>> shape = piece.getShape();
        
        for (size_t i = 0; i < shape.size(); i++) {
            for (size_t j = 0; j < shape[i].size(); j++) {
                if (shape[i][j] == 0) continue;
                
                int gridX = piece.getX() + j;
                int gridY = piece.getY() + i;
                
                if (gridX < 0 || gridX >= GRID_WIDTH || gridY >= GRID_HEIGHT || 
                    (gridY >= 0 && grid[gridY][gridX] == 1)) {
                    return true;
                }
            }
        }
        return false;
    }

    void placePiece() {
        vector<vector<int>> shape = currentPiece->getShape();
        int pieceType = currentPiece->getType();
        
        for (size_t i = 0; i < shape.size(); i++) {
            for (size_t j = 0; j < shape[i].size(); j++) {
                if (shape[i][j] == 0) continue;
                
                int gridX = currentPiece->getX() + j;
                int gridY = currentPiece->getY() + i;
                
                if (gridY < 0) {
                    gameOver = true; //ensure that game ends
                    return;
                }
                
                grid[gridY][gridX] = 1;
                colorGrid[gridY][gridX] = pieceType;
            }
        }
        
        clearLines();
        
        // Check if top row has any blocks
        for (int x = 0; x < GRID_WIDTH; x++) {
            if (grid[0][x] == 1) {
                gameOver = true;
                break;
            }
        }
        
        delete currentPiece;
        currentPiece = nextPiece;
        nextPiece = new Tetromino(generateRandomPieceType());
    }

    void clearLines() {
        int clearedLinesCount = 0;
        
        for (int y = GRID_HEIGHT - 1; y >= 0; y--) {
            bool lineFull = true;
            
            for (int x = 0; x < GRID_WIDTH; x++) {
                if (grid[y][x] == 0) {
                    lineFull = false;
                    break;
                }
            }
            
            if (lineFull) {
                clearedLinesCount++;
                
                // Move all lines above down
                for (int y2 = y; y2 > 0; y2--) {
                    for (int x = 0; x < GRID_WIDTH; x++) {
                        grid[y2][x] = grid[y2 - 1][x];
                        colorGrid[y2][x] = colorGrid[y2 - 1][x];
                    }
                }
                
                // Clear top line
                for (int x = 0; x < GRID_WIDTH; x++) {
                    grid[0][x] = 0;
                    colorGrid[0][x] = 0;
                }
                
                // Check the same line again(ensuring line is cleared)
                y++;
            }
        }
        
        if (clearedLinesCount > 0) {
            score += 100 * clearedLinesCount;
            linesCleared += clearedLinesCount;
            level = (linesCleared / 3) + 1;
            
            if (score / SCORE_THRESHOLD > 0) {
                fallSpeed = max(INITIAL_SPEED - (score / SCORE_THRESHOLD) * SPEED_INCREMENT, 100);
            }
        }
    }

    void hardDrop() {
        while (!isCollision(*currentPiece)) {
            currentPiece->moveDown();
        }
        
        currentPiece->setY(currentPiece->getY() - 1);
        placePiece();
    }

    //Measuring time since last drop 
    void update() {
        if (paused) return;

        auto now = chrono::system_clock::now();
        auto elapsed = chrono::duration_cast<chrono::milliseconds>(now - lastFall).count();
        
        if (elapsed > fallSpeed) {
            currentPiece->moveDown();
            
            if (isCollision(*currentPiece)) {
                currentPiece->setY(currentPiece->getY() - 1);
                placePiece();
            }
            
            lastFall = now;
        }
    }

    void draw() {
        static bool firstDraw = true;
        if (firstDraw) {
            system("cls");
            firstDraw = false;
        }
        
        // Draw game info
        setCursorPosition(0, 0);
        const vector<pair<string, ConsoleColor>> gameInfo = {
            {"TETRIS ", LIGHTRED},
            {"Score: " + to_string(score) + " | ", YELLOW},
            {"Level: " + to_string(level) + " | ", LIGHTGREEN},
            {"Lines: " + to_string(linesCleared), LIGHTCYAN}
        };
        
        for (const auto& info : gameInfo) {
            setConsoleColor(info.second);
            cout << info.first;
        }
        cout << endl << endl;
        
        // If paused, show message and return
        if (paused) {
            setCursorPosition(GRID_WIDTH / 2 - 5, GRID_HEIGHT / 2);
            setConsoleColor(LIGHTMAGENTA);
            cout << "PAUSED";
            resetConsoleColor();
            return;
        }
    
        // Draw border and grid
        setCursorPosition(0, 3);
        setConsoleColor(WHITE);
        cout << "+" << string(GRID_WIDTH * 2, '-') << "+" << endl;
    
        // Create temporary grid with current piece
        vector<vector<int>> tempGrid = grid;
        vector<vector<int>> tempColorGrid = colorGrid;
    
        if (!gameOver) {
            vector<vector<int>> shape = currentPiece->getShape();
            int pieceType = currentPiece->getType();
    
            for (size_t i = 0; i < shape.size(); i++) {
                for (size_t j = 0; j < shape[i].size(); j++) {
                    if (shape[i][j] == 0) continue;
    
                    int gridX = currentPiece->getX() + j;
                    int gridY = currentPiece->getY() + i;
    
                    if (gridX >= 0 && gridX < GRID_WIDTH && gridY >= 0 && gridY < GRID_HEIGHT) {
                        tempGrid[gridY][gridX] = 1;
                        tempColorGrid[gridY][gridX] = pieceType;
                    }
                }
            }
        }
    
        // Draw grid with colors
        for (int y = 0; y < GRID_HEIGHT; y++) {
            setCursorPosition(0, y + 4);
            setConsoleColor(WHITE);
            cout << "|";
    
            for (int x = 0; x < GRID_WIDTH; x++) {
                if (tempGrid[y][x] == 1) {
                    setConsoleColor(TETROMINO_COLORS[tempColorGrid[y][x]]);
                    cout << "##";
                } else {
                    cout << "  ";
                }
            }
            
            setConsoleColor(WHITE);
            cout << "|";
        }
    
        setCursorPosition(0, GRID_HEIGHT + 4);
        setConsoleColor(WHITE);
        cout << "+" << string(GRID_WIDTH * 2, '-') << "+" << endl;
    
        // Display next piece info
        setCursorPosition(GRID_WIDTH * 2 + 5, 4);
        setConsoleColor(LIGHTBLUE);
        cout << "Next Piece:";
        resetConsoleColor();
    
        vector<vector<int>> nextShape = nextPiece->getShape();
        ConsoleColor nextColor = TETROMINO_COLORS[nextPiece->getType()];
    
        //Drawing tetrominoes
        for (size_t i = 0; i < nextShape.size(); i++) {
            setCursorPosition(GRID_WIDTH * 2 + 5, i + 5);
            for (size_t j = 0; j < nextShape[i].size(); j++) {
                if (nextShape[i][j] == 1) {
                    setConsoleColor(nextColor);
                    cout << "##";
                } else {
                    cout << "  ";
                }
            }
        }
    
        // Draw controls with different colors
        const vector<pair<string, ConsoleColor>> controls = {
            {"Controls:", LIGHTMAGENTA},
            {"Left/Right: Move Left/Right", LIGHTCYAN},
            {"Up: Rotate", LIGHTGREEN},
            {"Down: Soft Drop", YELLOW},
            {"Space: Hard Drop", RED},
            {"P: Pause/Resume", LIGHTMAGENTA},
            {"1: End Game", LIGHTRED},
            {"2: Restart Game", LIGHTGREEN},
            {"ESC: Quit", LIGHTRED}
        };
        
        for (size_t i = 0; i < controls.size(); i++) {
            setCursorPosition(GRID_WIDTH * 2 + 5, 11 + i);
            setConsoleColor(controls[i].second);
            cout << controls[i].first;
        }
        resetConsoleColor();
    
        // Game over message
        if (gameOver) {
            setCursorPosition(GRID_WIDTH - 4, GRID_HEIGHT / 2 + 4);
            setConsoleColor(RED);
            cout << "GAME OVER";
            resetConsoleColor();
        }
    }

    void handleInput() {
        if (_kbhit()) {
            int ch = _getch();
            
            if (ch == 224) {
                ch = _getch();
                
                if (!paused) {
                    Tetromino tempPiece = *currentPiece;
                    
                    switch (ch) {
                        case KEY_LEFT:
                            currentPiece->moveLeft();
                            if (isCollision(*currentPiece)) *currentPiece = tempPiece;
                            break;
                        case KEY_RIGHT:
                            currentPiece->moveRight();
                            if (isCollision(*currentPiece)) *currentPiece = tempPiece;
                            break;
                        case KEY_UP:
                            currentPiece->rotate();
                            if (isCollision(*currentPiece)) *currentPiece = tempPiece;
                            break;
                        case KEY_DOWN:
                            currentPiece->moveDown();
                            if (isCollision(*currentPiece)) {
                                currentPiece->setY(currentPiece->getY() - 1);
                                placePiece();
                            }
                            lastFall = chrono::system_clock::now();
                            break;
                    }
                }
            } else if (ch == KEY_SPACE) {
                hardDrop();
            } else if (ch == KEY_ESC) {
                gameOver = true;
            } else if (ch == 'p' || ch == 'P') {
                paused = !paused;
            } else if (ch == '1') {
                gameOver = true;
            } else if (ch == '2') {
                reset();
            }
        }
    }

    bool isGameOver() const {
        return gameOver;
    }
};

int main() {
    system("cls");
    
    // Hide cursor - prevents cursor flickering
    CONSOLE_CURSOR_INFO cursorInfo = {100, FALSE};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    
    // Show intro animation
    IntroAnimation intro;
    intro.run();
    
    // Create and run game
    TetrisGame game;
    
    auto runGameLoop = [&]() {
        while (!game.isGameOver()) {
            game.handleInput();
            game.update();
            game.draw();
            Sleep(50);
        }
    };
    
    runGameLoop();
    
    // Game over screen
    setCursorPosition(0, GRID_HEIGHT + 6);
    cout << "Game Over! Press 1 to exit or 2 to replay...";
    
    while (true) {
        if (_kbhit()) {
            char ch = _getch();
            if (ch == '1') {
                break;
            } else if (ch == '2') {
                IntroAnimation().run();
                game.reset();
                runGameLoop();
                setCursorPosition(0, GRID_HEIGHT + 6);
                cout << "Game Over! Press 1 to exit or 2 to replay...";
            }
        }
    }
    
    resetConsoleColor();
    return 0;
}
