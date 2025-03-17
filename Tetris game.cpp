#include <iostream>
#include <vector>
#include <conio.h>
#include <Windows.h>
#include <random>
#include <chrono>
#include <string>

using namespace std;

// Key codes
const int KEY_ESC = 27;
const int KEY_LEFT = 75;
const int KEY_RIGHT = 77;
const int KEY_UP = 72;
const int KEY_DOWN = 80;
const int KEY_SPACE = 32;
const int KEY_ENTER = 13;

// Game constants
const int GRID_WIDTH = 10;
const int GRID_HEIGHT = 20;
const int INITIAL_SPEED = 500; // ms
const int SPEED_INCREMENT = 50; // ms
const int LINES_PER_LEVEL = 10;
const int SCORE_THRESHOLD = 300; // Score threshold for speed increase

// Console colors
enum ConsoleColor {
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    CYAN = 3,
    RED = 4,
    MAGENTA = 5,
    BROWN = 6,
    LIGHTGRAY = 7,
    DARKGRAY = 8,
    LIGHTBLUE = 9,
    LIGHTGREEN = 10,
    LIGHTCYAN = 11,
    LIGHTRED = 12,
    LIGHTMAGENTA = 13,
    YELLOW = 14,
    WHITE = 15
};

// Tetromino colors mapping
const ConsoleColor TETROMINO_COLORS[] = {
    BLUE,         // I - blue
    YELLOW,       // O - yellow
    LIGHTMAGENTA, // T - purple
    RED,          // S - red
    GREEN,        // Z - green
    LIGHTRED,     // J - pink (using LIGHTRED as pink)
    BROWN         // L - orange (using BROWN as orange)
};

// Tetromino shapes
const std::vector<std::vector<std::vector<int>>> TETROMINOES = {
    // I
    {
        {0, 0, 0, 0},
        {1, 1, 1, 1},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    // O
    {
        {1, 1},
        {1, 1}
    },
    // T
    {
        {0, 1, 0},
        {1, 1, 1},
        {0, 0, 0}
    },
    // S
    {
        {0, 1, 1},
        {1, 1, 0},
        {0, 0, 0}
    },
    // Z
    {
        {1, 1, 0},
        {0, 1, 1},
        {0, 0, 0}
    },
    // J
    {
        {1, 0, 0},
        {1, 1, 1},
        {0, 0, 0}
    },
    // L
    {
        {0, 0, 1},
        {1, 1, 1},
        {0, 0, 0}
    }
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

// Utility functions for console handling
void setCursorPosition(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void setConsoleColor(ConsoleColor textColor, ConsoleColor bgColor = BLACK) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (textColor) | (bgColor << 4));
}

void resetConsoleColor() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE | (BLACK << 4));
}

// Intro Animation Class
class IntroAnimation {
private:
    int screenWidth;
    int screenHeight;
    vector<pair<int, int>> fallingBlocks;
    random_device rd;
    mt19937 rng;
    
public:
    IntroAnimation() {
        // Get console window size
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        screenWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        screenHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        
        // Initialize RNG
        rng = mt19937(rd());
        
        // Initialize falling blocks
        generateFallingBlocks(30); // Start with 30 blocks
    }
    
    void generateFallingBlocks(int count) {
        uniform_int_distribution<int> distX(0, screenWidth - 2); // -2 to account for two characters per block
        uniform_int_distribution<int> distY(-20, 5); // Start above screen
        uniform_int_distribution<int> distColor(0, 6); // 7 tetromino colors
        
        fallingBlocks.clear();
        for (int i = 0; i < count; i++) {
            fallingBlocks.push_back(make_pair(distX(rng), distY(rng)));
        }
    }
    
    void drawLogo(int centerX, int centerY) {
        // Display the TETRIS logo
        int logoHeight = sizeof(TETRIS_LOGO) / sizeof(TETRIS_LOGO[0]);
        int logoWidth = TETRIS_LOGO[0].length();
        int startX = centerX - logoWidth / 2;
        int startY = centerY - logoHeight / 2;
        
        for (int i = 0; i < logoHeight; i++) {
            setCursorPosition(startX, startY + i);
            setConsoleColor(CYAN); // You can change this color if you prefer
            cout << TETRIS_LOGO[i];
        }
        
        // Add "WELCOME TO TETRIS" message
        string welcomeMsg = "W E L C O M E  T O  T E T R I S";
        setCursorPosition(centerX - welcomeMsg.length() / 2, centerY + 6); // Position it above the instructions
        
        // Display each letter in a different color for a rainbow effect
        ConsoleColor colors[] = {RED, YELLOW, GREEN, CYAN, BLUE, MAGENTA, LIGHTRED};
        for (size_t i = 0; i < welcomeMsg.length(); i++) {
            setConsoleColor(colors[i % 7]);
            cout << welcomeMsg[i];
        }
        
        resetConsoleColor();
    }
    
    void updateFallingBlocks() {
        // Move blocks down
        for (auto& block : fallingBlocks) {
            block.second++;
            
            // If block goes out of screen, reset it to top
            if (block.second > screenHeight) {
                uniform_int_distribution<int> distX(0, screenWidth - 2);
                block.first = distX(rng);
                block.second = -2;
            }
        }
    }
    
    void drawFallingBlocks() {
        // Get dimensions for the text areas to avoid
        int logoHeight = sizeof(TETRIS_LOGO) / sizeof(TETRIS_LOGO[0]);
        int logoWidth = TETRIS_LOGO[0].length();
        int logoStartX = screenWidth / 2 - logoWidth / 2;
        int logoStartY = screenHeight / 2 - 6 - logoHeight / 2;
        int logoEndX = logoStartX + logoWidth;
        int logoEndY = logoStartY + logoHeight;
        
        // Instructions area
        int instructionsStartY = logoEndY + 1; // Start just below the logo
        int instructionsEndY = instructionsStartY + 9; // Covers all instruction lines
        int instructionsStartX = screenWidth / 2 - 20; // Width buffer for instructions
        int instructionsEndX = screenWidth / 2 + 20;
    
        for (size_t i = 0; i < fallingBlocks.size(); i++) {
            // Skip if out of screen
            if (fallingBlocks[i].second < 0 || fallingBlocks[i].second >= screenHeight) {
                continue;
            }
            
            int blockX = fallingBlocks[i].first;
            int blockY = fallingBlocks[i].second;
            
            // Check if block is in the logo area
            bool inLogoArea = (blockX >= logoStartX && blockX <= logoEndX && 
                              blockY >= logoStartY && blockY <= logoEndY);
                              
            // Check if block is in the instructions area
            bool inInstructionsArea = (blockX >= instructionsStartX && blockX <= instructionsEndX &&
                                     blockY >= instructionsStartY && blockY <= instructionsEndY);
            
            // Only draw the block if it's not in any text area
            if (!inLogoArea && !inInstructionsArea) {
                setCursorPosition(blockX, blockY);
                // Use a random color for each block
                int colorIndex = i % 7;  // 7 colors available
                setConsoleColor(TETROMINO_COLORS[colorIndex]);
                cout << "##";
            }
        }
        resetConsoleColor();
    }
    
    void run() {
        system("cls");
        
        // Hide cursor
        CONSOLE_CURSOR_INFO cursorInfo;
        cursorInfo.dwSize = 100;
        cursorInfo.bVisible = FALSE;
        SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
        
        // Calculate center position for logo
        int centerX = screenWidth / 2;
        int centerY = screenHeight / 2 - 6; // Offset to leave space for instructions
        
        // Animation loop
        int frames = 0;
        while (true) {
            // Clear screen occasionally to avoid flickering
            if (frames % 10 == 0) {
                system("cls");
            }
            
            // Update and draw falling blocks
            updateFallingBlocks();
            drawFallingBlocks();
            
            // Draw logo
            drawLogo(centerX, centerY);
            
            // Draw instructions
            setCursorPosition(centerX - 14, centerY + 8);
            setConsoleColor(LIGHTCYAN);
            cout << "Press [ENTER] to start the game";
            
            setCursorPosition(centerX - 20, centerY + 10);
            setConsoleColor(LIGHTGREEN);
            cout << "Use Arrow Keys to Move and Rotate pieces";
            setCursorPosition(centerX - 13, centerY + 12);
            setConsoleColor(YELLOW);
            cout << "[SPACE] for hard drop";
            
            setCursorPosition(centerX - 13, centerY + 14);
            setConsoleColor(LIGHTRED);
            cout << "[P] to pause the game";
            
            resetConsoleColor();
            
            // Check for ENTER key press
            if (_kbhit()) {
                int key = _getch();
                if (key == KEY_ENTER) {
                    break;  // Exit the intro animation
                }
            }
            
            // Control animation speed
            Sleep(70);
            frames++;
        }
        
        // Clear screen before starting the game
        system("cls");
    }
};

// Tetromino class
class Tetromino {
private:
    int type;
    int rotation;
    int x, y;
    char symbol;
    ConsoleColor color;

public:
    Tetromino(int type) : type(type), rotation(0), x(GRID_WIDTH / 2 - 2), y(0) {
        symbol = '#';  // Using the same symbol for all tetrominoes
        color = TETROMINO_COLORS[type];  // Set color based on tetromino type
    }

    vector<vector<int>> getShape() const {
        vector<vector<int>> shape = TETROMINOES[type];
        // Apply rotation
        for (int r = 0; r < rotation; r++) {
            shape = rotateMatrix(shape);
        }
        return shape;
    }

    vector<vector<int>> rotateMatrix(const vector<vector<int>>& matrix) const {
        int n = matrix.size();
        vector<vector<int>> rotated(n, vector<int>(n, 0));
        
        // Rotate 90 degrees clockwise
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                rotated[j][n - 1 - i] = matrix[i][j];
            }
        }
        return rotated;
    }

    void rotate() {
        rotation = (rotation + 1) % 4;
    }

    void moveLeft() {
        x--;
    }

    void moveRight() {
        x++;
    }

    void moveDown() {
        y++;
    }

    int getX() const { return x; }
    int getY() const { return y; }
    char getSymbol() const { return symbol; }
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
    vector<vector<int>> colorGrid;  // Store color information
    Tetromino* currentPiece;
    Tetromino* nextPiece;
    bool gameOver;
    bool paused; // Track if the game is paused
    int score;
    int level;
    int linesCleared;
    int fallSpeed;
    std::chrono::time_point<std::chrono::system_clock> lastFall;

    std::mt19937 rng;

public:
    TetrisGame() : 
        grid(GRID_HEIGHT, vector<int>(GRID_WIDTH, 0)),
        colorGrid(GRID_HEIGHT, vector<int>(GRID_WIDTH, 0)),
        gameOver(false),
        paused(false), // Initialize paused to false
        score(0),
        level(1), // Initialize level to 1
        linesCleared(0),
        fallSpeed(INITIAL_SPEED) {
        
        // Initialize random number generator with time-based seed for true randomness
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        rng = mt19937(seed);
        
        // Create first piece
        currentPiece = new Tetromino(generateRandomPieceType());
        nextPiece = new Tetromino(generateRandomPieceType());
        
        // Record start time
        lastFall = chrono::system_clock::now();
    }

    ~TetrisGame() {
        delete currentPiece;
        delete nextPiece;
    }

    void reset() {
        // Reset game state
        grid.assign(GRID_HEIGHT, vector<int>(GRID_WIDTH, 0));
        colorGrid.assign(GRID_HEIGHT, vector<int>(GRID_WIDTH, 0));
        delete currentPiece;
        delete nextPiece;
        
        // Re-seed the random number generator for new random sequence
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        rng.seed(seed);
        
        currentPiece = new Tetromino(generateRandomPieceType());
        nextPiece = new Tetromino(generateRandomPieceType());
        gameOver = false;
        paused = false;
        score = 0;
        level = 1; // Reset level to 1
        linesCleared = 0;
        fallSpeed = INITIAL_SPEED;
        lastFall = chrono::system_clock::now();
    }

    int generateRandomPieceType() {
        std::uniform_int_distribution<int> dist(0, TETROMINOES.size() - 1);
        return dist(rng);
    }

    bool isCollision(const Tetromino& piece) const {
        vector<vector<int>> shape = piece.getShape();
        int pieceHeight = shape.size();
        int pieceWidth = shape[0].size();
        
        for (int i = 0; i < pieceHeight; i++) {
            for (int j = 0; j < pieceWidth; j++) {
                if (shape[i][j] == 0) continue;
                
                int gridX = piece.getX() + j;
                int gridY = piece.getY() + i;
                
                // Check if out of bounds
                if (gridX < 0 || gridX >= GRID_WIDTH || gridY >= GRID_HEIGHT) {
                    return true;
                }
                
                // Check if overlaps with placed blocks
                if (gridY >= 0 && grid[gridY][gridX] == 1) {
                    return true;
                }
            }
        }
        return false;
    }

    void placePiece() {
        vector<vector<int>> shape = currentPiece->getShape();
        int pieceHeight = shape.size();
        int pieceWidth = shape[0].size();
        int pieceType = currentPiece->getType();
        
        for (int i = 0; i < pieceHeight; i++) {
            for (int j = 0; j < pieceWidth; j++) {
                if (shape[i][j] == 0) continue;
                
                int gridX = currentPiece->getX() + j;
                int gridY = currentPiece->getY() + i;
                
                if (gridY < 0) {
                    gameOver = true;
                    return;
                }
                
                grid[gridY][gridX] = 1;
                colorGrid[gridY][gridX] = pieceType;  // Store the tetromino type for color reference
            }
        }
        
        // Check for lines to clear
        clearLines();
        
        // Check if the grid is full (top row)
        if (grid[0][0] == 1 || grid[0][1] == 1 || grid[0][2] == 1 || grid[0][3] == 1 || 
            grid[0][4] == 1 || grid[0][5] == 1 || grid[0][6] == 1 || grid[0][7] == 1 || 
            grid[0][8] == 1 || grid[0][9] == 1) {
            gameOver = true;
        }
        
        // Create new piece
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
                
                // Check the same line again
                y++;
            }
        }
        
        // Update score and level
        if (clearedLinesCount > 0) {
            // Increase score by 100 for each line cleared
            this->score += 100 * clearedLinesCount; // Score increases by 100 for each line cleared
            this->linesCleared += clearedLinesCount;
            
            // Update level calculation - level starts at 1 and increases for every 3 lines cleared
            // Use integer division to calculate the level (add 1 to start at level 1)
            this->level = (this->linesCleared / 3) + 1;
            
            // Update fall speed based on score
            if (score / SCORE_THRESHOLD > 0) {
                fallSpeed = max(INITIAL_SPEED - (score / SCORE_THRESHOLD) * SPEED_INCREMENT, 100);
            }
        }
    }

    void hardDrop() {
        while (!isCollision(*currentPiece)) {
            currentPiece->moveDown();
        }
        
        // Move back up one row (was last valid position)
        currentPiece->setY(currentPiece->getY() - 1);
        placePiece();
    }

    void update() {
        if (paused) return; // Skip update if paused

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
        // First time setup - clear screen once
        static bool firstDraw = true;
        if (firstDraw) {
            system("cls");
            firstDraw = false;
        }
        
        // Draw info section
        setCursorPosition(0, 0);
        resetConsoleColor();
        cout << "TETRIS" << endl;
        cout << "Score: " << score << " | Level: " << level << " | Lines: " << linesCleared << endl;
        cout << endl;
    
        // If the game is paused, display a paused message
        if (paused) {
            setCursorPosition(GRID_WIDTH / 2 - 5, GRID_HEIGHT / 2);
            cout << "PAUSED";
            return; // Skip the rest of the draw function
        }
    
        // Draw border and grid
        setCursorPosition(0, 3);
        cout << "+" << string(GRID_WIDTH * 2, '-') << "+" << endl;
    
        // Create a temporary grid with current piece
        vector<vector<int>> tempGrid = grid;
        vector<vector<int>> tempColorGrid = colorGrid;
    
        // Add current piece to temporary grid
        if (!gameOver) {
            vector<vector<int>> shape = currentPiece->getShape();
            int pieceHeight = shape.size();
            int pieceWidth = shape[0].size();
            int pieceType = currentPiece->getType();
    
            for (int i = 0; i < pieceHeight; i++) {
                for (int j = 0; j < pieceWidth; j++) {
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
            cout << "|";
    
            for (int x = 0; x < GRID_WIDTH; x++) {
                if (tempGrid[y][x] == 1) {
                    ConsoleColor blockColor = TETROMINO_COLORS[tempColorGrid[y][x]];
                    setConsoleColor(blockColor);
                    cout << "##";
                    resetConsoleColor();
                } else {
                    cout << "  ";
                }
            }
            cout << "|";
        }
    
        setCursorPosition(0, GRID_HEIGHT + 4);
        cout << "+" << string(GRID_WIDTH * 2, '-') << "+" << endl;
    
        // Clear the area for the next piece
        setCursorPosition(GRID_WIDTH * 2 + 5, 4);
        cout << "Next Piece:";
    
        // Display next piece
        vector<vector<int>> nextShape = nextPiece->getShape();
        ConsoleColor nextColor = TETROMINO_COLORS[nextPiece->getType()];
    
        for (int i = 0; i < nextShape.size(); i++) {
            setCursorPosition(GRID_WIDTH * 2 + 5, i + 5);
            for (int j = 0; j < nextShape[i].size(); j++) {
                if (nextShape[i][j] == 1) {
                    setConsoleColor(nextColor);
                    cout << "##";
                    resetConsoleColor();
                } else {
                    cout << "  ";
                }
            }
        }
    
        // Draw controls - using plain text instead of Unicode characters
        setCursorPosition(GRID_WIDTH * 2 + 5, 11);
        cout << "Controls:";
        setCursorPosition(GRID_WIDTH * 2 + 5, 12);
        cout << "Left/Right: Move Left/Right";
        setCursorPosition(GRID_WIDTH * 2 + 5, 13);
        cout << "Up: Rotate";
        setCursorPosition(GRID_WIDTH * 2 + 5, 14);
        cout << "Down: Soft Drop";
        setCursorPosition(GRID_WIDTH * 2 + 5, 15);
        cout << "Space: Hard Drop";
        setCursorPosition(GRID_WIDTH * 2 + 5, 16);
        cout << "P: Pause/Resume";
        setCursorPosition(GRID_WIDTH * 2 + 5, 17);
        cout << "1: End Game";
        setCursorPosition(GRID_WIDTH * 2 + 5, 18);
        cout << "2: Restart Game";
        setCursorPosition(GRID_WIDTH * 2 + 5, 19);
        cout << "ESC: Quit";
    
        // Game over message
        if (gameOver) {
            setCursorPosition(GRID_WIDTH - 7, GRID_HEIGHT / 2 + 4);
            cout << "G A M E  O V E R";
        }
    }

    void handleInput() {
        if (_kbhit()) {
            int ch = _getch();
            
            // For arrow keys, _getch() returns 224 followed by the actual key code
            if (ch == 224) {
                ch = _getch();
                
                if (!paused) { // Only handle movement if not paused
                    Tetromino tempPiece = *currentPiece;
                    
                    switch (ch) {
                        case KEY_LEFT:
                            currentPiece->moveLeft();
                            if (isCollision(*currentPiece)) {
                                *currentPiece = tempPiece;
                            }
                            break;
                            
                        case KEY_RIGHT:
                            currentPiece->moveRight();
                            if (isCollision(*currentPiece)) {
                                *currentPiece = tempPiece;
                            }
                            break;
                            
                        case KEY_UP:
                            currentPiece->rotate();
                            if (isCollision(*currentPiece)) {
                                *currentPiece = tempPiece;
                            }
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
            } else if (ch == 'p' || ch == 'P') { // Pause the game
                paused = !paused; // Toggle pause state
            } else if (ch == '1') { // End the game
                gameOver = true;
            } else if (ch == '2') { // Restart the game
                reset();
            }
        }
    }

    bool isGameOver() const {
        return gameOver;
    }
};

int main() {
    // Set up console
    system("cls");
    
    // Hide cursor
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 100;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    
    // Show the intro animation first
    IntroAnimation intro;
    intro.run();
    
    // Create game instance
    TetrisGame game;
    
    // Game loop
    while (true) {
        game.handleInput();
        game.update();
        game.draw();
        
        // Check for game over
        if (game.isGameOver()) {
            break;
        }
        
        // Control game speed
        Sleep(50);
    }
    
    // Show game over message and wait for user input
    setCursorPosition(0, GRID_HEIGHT + 6);
    cout << "Game Over! Press 1 to exit or 2 to replay...";
    
    while (true) {
        if (_kbhit()) {
            char ch = _getch();
            if (ch == '1') {
                break; // Exit the game
            } else if (ch == '2') {
                // If user wants to restart, show intro again
                IntroAnimation intro;
                intro.run();
                
                game.reset(); // Restart the game
                
                // New game loop
                while (true) {
                    game.handleInput();
                    game.update();
                    game.draw();
                    
                    if (game.isGameOver()) {
                        break;
                    }
                    
                    Sleep(50);
                }
                
                // Show game over message again
                setCursorPosition(0, GRID_HEIGHT + 6);
                cout << "Game Over! Press 1 to exit or 2 to replay...";
            }
        }
    }
    
    // Reset colors before exiting
    resetConsoleColor();
    
    return 0;
}
