# 🎮 Tetris Game - C++ Project 🎮🚀

## 📌 Overview
This is a classic Tetris Game with modern features 🔹🎮, implemented in C++. It features colorful blocks, level progression, and an engaging intro animation ⭐, making the gameplay progressively challenging as the player advances.

## 📜 Function Overview
This document provides an overview of the functions used in the Tetris game, explaining their purpose and role in the game.

### 🎮 Function Table

| 🔹 Function Name | ✨ Description |
|------------------|---------------|
| 🎲 `setCursorPosition()` | 🏗️ Positions the cursor at specific coordinates on the console window |
| 🎨 `setConsoleColor()` | 🖥️ Changes the color of text and background for console output |
| 🎭 `IntroAnimation::run()` | 🌱 Displays an engaging intro screen with falling blocks and logo |
| 🏃 `Tetromino::rotate()` | ➡️ Rotates the current tetromino piece by 90 degrees clockwise |
| ⚠️ `TetrisGame::isCollision()` | 💥 Checks if the current tetromino collides with walls or placed blocks |
| 🍽️ `TetrisGame::placePiece()` | 🏆 Places the current tetromino on the grid when it can't move further |
| ⏫ `TetrisGame::clearLines()` | 🚀 Clears completed lines and updates score and level |
| ☠️ `TetrisGame::hardDrop()` | 🎭 Instantly drops the tetromino to the lowest possible position |
| 🎮 `TetrisGame::handleInput()` | ⌨️ Captures user input to move and rotate pieces |
| 🔄 `TetrisGame::update()` | 🔄 Updates game state including piece position and falling speed |
| 🖥️ `TetrisGame::draw()` | 🎨 Renders the game grid, current piece, next piece, and UI elements |

## 🌟 Features

### 🎯 Grid-Based Gameplay
✅ The game operates on a structured grid where tetrominoes fall and stack  
✅ Random tetromino generation with next piece preview  
✅ Seven different tetromino shapes with distinct colors

### 🧩 Tetromino Mechanics
✅ Rotation and movement in four directions  
✅ Hard drop for quick placement  
✅ Collision detection with walls and other blocks  
✅ Clear animation when lines are completed

### 🔥 Levels & Difficulty Progression
✅ Speed increases as score grows  
✅ Level progression based on lines cleared  
✅ Game becomes more challenging at higher levels

### ⭐ Scoring System
✅ Points awarded for each line cleared  
✅ Level multiplier increases score potential  
✅ Score tracking displayed on screen

### 🎬 Visual Elements
✅ Colorful tetrominoes using console colors  
✅ Engaging intro animation with falling blocks  
✅ Clean UI showing score, level, and next piece

## 🎮 Game Controls

Take full control of your falling tetrominoes using the following keys:

| 🕹️ Key | 🎯 Action |
|--------|----------|
| ◀️ Left Arrow | Move Left ⬅️ |
| ▶️ Right Arrow | Move Right ➡️ |
| 🔼 Up Arrow | Rotate Piece 🔄 |
| 🔽 Down Arrow | Soft Drop (move down faster) ⬇️ |
| ⬜ Space | Hard Drop (instant drop) ⏬ |
| ⏸️ P | Pause/Resume Game ⏯️ |
| 🔄 1 | End Current Game ❌ |
| 🔄 2 | Restart Game 🔁 |
| 🚪 ESC | Quit Game 🚶 |

Master these controls and aim for the highest score! 🚀🎮

## 🏆 Score & Level
- Score: Increases by 100 points for each line cleared  
- Level: Increases for every 3 lines cleared  
- Game speed increases with higher scores and levels

## 🖥️ Optimized for Console Execution
✅ Designed for Windows console  
✅ Uses Windows API for colorful graphics and cursor positioning  
✅ Smooth animations using console manipulation techniques

## ⚙️ Technologies Used
🛠️ C++ (Standard Library)  
📦 Windows API (for console manipulation)  
🎨 Console colors for visual elements  
🎲 Randomization (for tetromino generation)

## 🫠 Code Design & Working

### 🧩 How Tetrominos Move and Rotate
- Tetrominoes are represented as 2D matrices, with rotation transforming the matrix  
- Movement is controlled by updating X,Y coordinates  
- Gravity is simulated with automatic downward movement at intervals  
- The game loop continuously checks for collisions during movement

### 🎲 How Random Pieces Are Generated
- A random number generator selects from seven different tetromino types  
- Each piece has a unique color and shape  
- The "next piece" is displayed on the side panel to let players plan ahead

### 💥 How Collision Detection Works
- Wall Collision → Prevents pieces from moving outside the grid boundaries  
- Block Collision → Detects when a piece would overlap with already placed blocks  
- Floor Collision → Triggers piece placement when it can't move down further

### 🧹 How Line Clearing Works
- The game checks each row to see if it's completely filled  
- When a line is complete, it's removed and all lines above shift down  
- Multiple lines can be cleared at once for higher scores

### 🎯 How the Game Ends
- If a new tetromino can't be placed at the top of the grid, the game ends  
- Final score is shown and the player can restart or exit

## 👥 Team - Bug Busters 🏆
This project was developed by Bug Busters, a passionate and dedicated team from Dhirubhai Ambani University (DAU). From conceptualization to execution, we poured our efforts into designing an immersive and challenging Tetris Game that goes beyond the classic version by introducing colorful blocks, level progression, and an engaging intro animation.

💡 Our journey was filled with brainstorming sessions, debugging marathons, and countless refinements to ensure smooth gameplay. Implementing the grid system, optimizing tetromino movement, and developing advanced mechanics like difficulty progression and color-coded pieces required both creativity and technical expertise.

Throughout this project, we encountered numerous challenges—from handling complex logic in C++ to fine-tuning collision detection and randomization. However, every hurdle strengthened our understanding of game development, data structures, and algorithmic problem-solving.

This game is more than just a project; it's a testament to our teamwork, perseverance, and passion for coding. We hope you enjoy playing it as much as we enjoyed building it! 🚀🎮

👨‍💻 Team Members:
* 📌 Nihar Patel - 202401148
* 📌 Raj Patel - 202401153
* 📌 Praneel Sharma - 202401166
* 📌 Pranshu Patel - 202401167

## 🚀 How to Run
### 📌 Compile the code using a C++ compiler:

g++ snake_game.cpp -o snake_game

## 🔮 Future Enhancements
✅ Adding high score tracking and persistence 🏆  
✅ Implementing ghost piece preview to show where the piece will land 👻  
✅ Additional game modes like Sprint (clear 40 lines as fast as possible) 🏁  
✅ Enhanced visual effects and animations 🎨  
✅ Sound effects and background music 🎵  

## ✨ Unique Features
- Intro Animation 🌟: Engaging startup screen with falling blocks and animated logo  
- Color Coding 🎨: Each tetromino type has its own distinct color  
- Next Piece Preview 🔮: Shows the upcoming piece for strategic planning  
- Dynamic Speed 🚀: Gameplay speed increases as you score more points  
- Pause Functionality ⏸️: Allows players to take a break and resume later  

## ⭐ Contributions & Feedback
If you have suggestions or want to contribute to the project, feel free to create a pull request or open an issue on GitHub! 🚀

📌 Star the repository if you like our work! ⭐
