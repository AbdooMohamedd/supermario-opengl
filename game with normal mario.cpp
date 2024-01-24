#include <GL/glut.h>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <string>
#include <iostream>

// Global variables
int windowWidth = 1400;
int windowHeight = 800;

float characterX = 100.0f;
float characterY = 65.0f;
float characterSpeed = 5.0f;

float obstacleSpeed = 3.0f;

int score = 0;
int health = 100;

bool isJumping = false;
int jumpHeight = 0;
int maxJumpHeight = 300;
int jumpTimer = 0;
int fallTimer = 0;
bool reachedJumpHeight = false;

bool moveLeft = false;
bool moveRight = false;

bool gameover = false;
bool gameStarted = false;

float cloudX = 0.0;

const float initialCloudSpeed = 2.0f;
const float initialTreeSpeed = 2.0f;

float cloudSpeed = initialCloudSpeed;
float treeSpeed = initialTreeSpeed;
int gameLevel = 1;

// Define an cloud structure
struct Cloud {
    float x;
    float y;
    float size;
};
// Maintain a vector to store cloud information
std::vector<Cloud> clouds;

// Define an obstacle structure
struct Obstacle {
    float x;
    float y;
    int type;
};
// Vector to store 0: Man, 1: Bee, 2: Roadblock information
std::vector<Obstacle> obstacles;

// Define an tree structure
struct Tree {
    float x;
    float y;
    float size;
};
// Vector to store tree information
std::vector<Tree> trees;

// Constants for obstacle types
const int MAN_TYPE = 0;
const int COIN_TYPE = 1;
const int ROADBLOCK_TYPE = 10;
const int BEE_SCORE = 10;



// Function to draw a simple rectangle
void drawRectangle(float x, float y, float width, float height) {
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

// Function to draw the pipes
void drawPipes(float x, float y) {
    glColor3f(0.0f, 0.5f, 0.2f);

    // First rectangle
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + 60.0f, y);
    glVertex2f(x + 60.0f, y + 60.0f);
    glVertex2f(x, y + 60.0f);
    glVertex2f(x, y + 60.0f);
    glVertex2f(x + 60.0f, y + 60.0f);
    glVertex2f(x + 60.0f, y + 120.0f);
    glVertex2f(x, y + 120.0f);
    glVertex2f(x - 5.0f, y + 120.0f);
    glVertex2f(x + 65.0f, y + 120.0f);
    glVertex2f(x + 65.0f, y + 180.0f);
    glVertex2f(x - 5.0f, y + 180.0f);
    glEnd();
}

// Function to draw SuperMario
void drawMario(float x, float y) {
    // Color and size definitions
    float skinColorR = 0.98f, skinColorG = 0.81f, skinColorB = 0.69f; // Skin color
    float shirtColorR = 0.0f, shirtColorG = 0.0f, shirtColorB = 1.0f; // Shirt color (Green)
    float pantsColorR = 0.0f, pantsColorG = 0.0f, pantsColorB = 1.0f; // Pants color (Blue)
    float shoeColorR = 0.4f, shoeColorG = 0.2f, shoeColorB = 0.0f; // Brown shoe color
    float hatColorR = 1.0f, hatColorG = 0.0f, hatColorB = 0.0f; // Red hat color

    float bodyWidth = 25.0f;
    float bodyHeight = 60.0f;
    float headRadius = 15.0f;
    float armWidth = 5.0f, armHeight = bodyHeight / 1.5f;
    float legWidth = 10.0f, legHeight = bodyHeight / 2.0f;
    float shoeWidth = 10.0f, shoeHeight = 5.0f;
    float hatWidth = 20.0f, hatHeight = 10.0f;

    int numSegments = 150; // For circle drawing

    // Draw the body (rectangle)
    glColor3f(shirtColorR, shirtColorG, shirtColorB); // Set color for the shirt
    drawRectangle(x, y, bodyWidth, bodyHeight);

    // Draw the head (circle)
    glColor3f(skinColorR, skinColorG, skinColorB); // Skin color for the head
    glBegin(GL_POLYGON);
    for (int i = 0; i < numSegments; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / float(numSegments);
        float headX = x + bodyWidth / 2.0f + headRadius * cosf(theta);
        float headY = y + bodyHeight + headRadius * sinf(theta);
        glVertex2f(headX, headY);
    }
    glEnd();

    // Draw the eyes
    glColor3f(0.0f, 0.0f, 0.0f); // Eye color (black)
    float eyeRadius = 2.0f; // Adjust the eye size as needed
    float eyeOffsetX = 4.0f; // Offset from the center of the head
    float eyeOffsetY = 5.0f; // Offset from the top of the head

    glBegin(GL_POLYGON);
    for (int i = 0; i < numSegments; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / float(numSegments);
        float eyeX = x + bodyWidth / 2.0f + eyeOffsetX + eyeRadius * cosf(theta);
        float eyeY = y + bodyHeight + eyeOffsetY + eyeRadius * sinf(theta);
        glVertex2f(eyeX, eyeY);
    }
    glEnd();

    glBegin(GL_POLYGON);
    for (int i = 0; i < numSegments; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / float(numSegments);
        float eyeX = x + bodyWidth / 2.0f - eyeOffsetX + eyeRadius * cosf(theta);
        float eyeY = y + bodyHeight + eyeOffsetY + eyeRadius * sinf(theta);
        glVertex2f(eyeX, eyeY);
    }
    glEnd();

    // Draw the mustache
    glColor3f(0.5f, 0.25f, 0.0f); // Mustache color (brown)
    float mustacheWidth = 15.0f;
    float mustacheHeight = 3.0f;
    float mustacheOffsetY = -2.0f;

    glBegin(GL_QUADS);
    glVertex2f(x + bodyWidth / 2.0f - mustacheWidth / 2.0f, y + bodyHeight + mustacheOffsetY);
    glVertex2f(x + bodyWidth / 2.0f + mustacheWidth / 2.0f, y + bodyHeight + mustacheOffsetY);
    glVertex2f(x + bodyWidth / 2.0f + mustacheWidth / 2.0f, y + bodyHeight + mustacheOffsetY + mustacheHeight);
    glVertex2f(x + bodyWidth / 2.0f - mustacheWidth / 2.0f, y + bodyHeight + mustacheOffsetY + mustacheHeight);
    glEnd();

    // Draw the arms
    glColor3f(1.0f, 0.0f, 0.0f); // Shirt color for the arms
    drawRectangle(x - (armWidth / 2.0f), y-9 + bodyHeight / 3.0f, armWidth, armHeight); // Left arm
    drawRectangle(x + bodyWidth - (armWidth / 2.0f), y-9 + bodyHeight / 3.0f, armWidth, armHeight); // Right arm

    // Draw the legs
    glColor3f(pantsColorR, pantsColorG, pantsColorB); // Pants color for the legs
    drawRectangle(x-2 + (bodyWidth / 2.0f) - legWidth, y - legHeight, legWidth, legHeight); // Left leg
    drawRectangle(x+3 + (bodyWidth / 2.0f), y - legHeight, legWidth, legHeight); // Right leg

    // Draw the shoes
    glColor3f(shoeColorR, shoeColorG, shoeColorB); // Shoe color
    drawRectangle(x-2 + (bodyWidth / 2.0f) - legWidth, y - legHeight - shoeHeight, shoeWidth, shoeHeight); // Left shoe
    drawRectangle(x+3 + (bodyWidth / 2.0f), y - legHeight - shoeHeight, shoeWidth, shoeHeight); // Right shoe

    // Draw the hat (semi-circle)
    glColor3f(hatColorR, hatColorG, hatColorB); // Hat color
    float hatCenterX = x + 12;
    float hatCenterY = y + 5 + bodyHeight + hatHeight / 2.0f;
    float hatRadius = hatWidth / 2.0f;
    int numSegmentsHat = 100; // Adjust the number of segments for smoother curve

    glBegin(GL_POLYGON);
    for (int i = 0; i < numSegmentsHat; i++) {
        float theta = 3.1415926f * float(i) / float(numSegmentsHat);
        float hatX = hatCenterX + hatRadius * cosf(theta);
        float hatY = hatCenterY + hatRadius * sinf(theta);
        glVertex2f(hatX, hatY);
    }
    glEnd();
}
// Function to draw the main character
void drawCharacter() {
    drawMario(characterX, characterY + jumpHeight);
}

// Function to draw a filled rectangle
void drawFilledRectangle(float x, float y, float width, float height) {
    glBegin(GL_POLYGON);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

// Function to draw an obstacle
void drawCoins(float x, float y) {
    // Draw the body
    glColor3f(1.0f, 0.843f, 0.0f);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++) {
        float theta = 4.0f * 3.1415926f * float(i) / float(100);
        float circleX = x + 20.0f * cosf(theta);
        float circleY = y + 20.0f * sinf(theta);
        glVertex2f(circleX, circleY);
    }
    glEnd();

    // Draw the filled gold vertical rectangle with black edges inside the body
    glColor3f(1.0f, 0.843f, 0.0f);
    drawFilledRectangle(x - 5.0f, y - 15.0f, 10.0f, 30.0f);

    // Draw black edges for the rectangle
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x - 5.0f, y - 15.0f);
    glVertex2f(x + 5.0f, y - 15.0f);
    glVertex2f(x + 5.0f, y + 15.0f);
    glVertex2f(x - 5.0f, y + 15.0f);
    glEnd();
    glLineWidth(1.0f);
}

// Function to draw the score and health bars
void drawBars() {
    glColor3f(1.0f, 1.0f, 1.0f); // White color

    // Draw score bar
    glRasterPos2f(10, windowHeight - 20);
    std::string scoreStr = "Score: " + std::to_string(score);
    for (char c : scoreStr) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    // Draw health bar
    glRasterPos2f(windowWidth - 100, windowHeight - 20);
    std::string healthStr = "Health: " + std::to_string(health);
    for (char c : healthStr) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    // Draw level information
    glRasterPos2f(windowWidth - 200, windowHeight - 20);
    std::string levelStr = "Level: " + std::to_string(gameLevel);
    for (char c : levelStr) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}

// Function to draw a tree
void drawTree(float x, float y, float size) {
    // Draw the trunk
    glColor3f(0.5, 0.25, 0.0);
    glBegin(GL_POLYGON);
    glVertex2f(x + 25 - size / 6, y);
    glVertex2f(x + 25 + size / 6, y);
    glVertex2f(x + 25 + size / 6, y + size);
    glVertex2f(x + 25 - size / 6, y + size);
    glEnd();

    // Draw the tree top using three circles
    glColor3f(0.0, 1.0, 0.0);
    float resolution = 0.01;
    for (int i = 0; i < 3; i++) {
        glBegin(GL_POLYGON);
        for (float angle = 0.0; angle < 2 * M_PI; angle += resolution) {
            float tree_x = x + i * size / 2.0 + size / 2.0 * cos(angle);
            float tree_y = y + size + size / 2.0 * sin(angle);
            glVertex2f(tree_x, tree_y);
        }
        glEnd();
    }

}

// Function to draw the sky
void DrawSky() {
    glColor3f(0.53f, 0.81f, 0.98f);
    glRectf(0.0f, 0.0f, 1400.0f, 800.0f);
}

// Function to draw the Ground
void drawGround(float y, float width) {
    glColor3f(0.627, 0.475, 0.341);
    glBegin(GL_QUADS);
    glVertex2f(-width, -1.0);
    glVertex2f(width, -1.0);
    glVertex2f(width, y);
    glVertex2f(-width, y);
    glEnd();
}

// Function to draw clouds
void drawCloud(float x, float y, float size) {
    glColor3f(1.0, 1.0, 1.0);

    // Draw three circles together to make a cloud shape
    float resolution = 0.01;
    for (int i = 0; i < 3; i++) {
        glBegin(GL_POLYGON);
        for (float angle = 0.0; angle < 2 * M_PI; angle += resolution) {
            float cloud_x = x + i * size / 2.0 + size / 2.0 * cos(angle);
            float cloud_y = y + size / 2.0 * sin(angle);
            glVertex2f(cloud_x, cloud_y);
        }
        glEnd();
    }
}




// Function to draw the Starting screen
void drawStartScreen() {
    glColor3f(1.0f, 1.0f, 1.0f);

    glRasterPos2f(windowWidth / 2 - 80, windowHeight / 2);
    std::string startMessage = "Press Enter to Start";
    for (char c : startMessage) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}

// Function to draw the game over screen with score
void drawGameOver(int score) {
    glColor3f(1.0f, 1.0f, 1.0f);

    glRasterPos2f(windowWidth / 2 - 50, windowHeight / 2);
    std::string gameOverStr = "Game Over - Your Score = " + std::to_string(score);
    for (char c : gameOverStr) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}




// Function to handle keyboard input
void keyboardDownHandler(unsigned char key, int x, int y) {
    switch (key) {
    case ' ':
        if (!isJumping) {
            isJumping = true;
        }
        break;
    case 'a':
        moveLeft = true;
        break;
    case 'd':
        moveRight = true;
        break;
    case 13: // ASCII code for Enter key
        if (!gameStarted) {
            gameStarted = true;
            glutPostRedisplay();
        }
        break;
    case 27: // ASCII code for Escape key
        exit(0);
        break;
    }
}

// Function to handle releasing keys
void keyboardUpHandler(unsigned char key, int x, int y) {
    switch (key) {
    case 'a':
        moveLeft = false;
        break;
    case 'd':
        moveRight = false;
        break;
    }
}




// Function to update the game state
void updateGame(int value) {
    if (!gameover) {
        // Update character position based on keyboard input
        if (moveLeft && characterX > 0) {
            characterX -= characterSpeed;
        }
        if (moveRight && characterX < windowWidth - 20) {
            characterX += characterSpeed;
        }

        // Update jumping
        if (isJumping) {
            jumpTimer += 30;
            jumpHeight = static_cast<int>((jumpTimer / 1000.0) * maxJumpHeight);

            if (jumpTimer >= 1000) {
                isJumping = false;
                jumpTimer = 0;
                fallTimer = 0;
                reachedJumpHeight = false;
            }

            // Check if the player has reached the required jump height
            if (jumpHeight >= 150 && !reachedJumpHeight) {
                reachedJumpHeight = true;
            }
        } else if (reachedJumpHeight) {
            // Update falling
            fallTimer += 30;
            jumpHeight = maxJumpHeight - static_cast<int>((fallTimer / 1000.0) * maxJumpHeight);

            if (fallTimer >= 1000) {
                fallTimer = 0;
                characterY = 65;
                reachedJumpHeight = false;
            }
        } else {
            jumpHeight = 0;
        }

        // Update cloud position for continuous movement
        for (auto& cloud : clouds) {
            cloud.x -= cloudSpeed;
        }

        // Update tree position for continuous movement
        for (auto& tree : trees) {
            tree.x -= treeSpeed;
        }

        // Generate clouds with reduced frequency
        if (rand() % 300 == 0) {
            Cloud newCloud;
            newCloud.x = windowWidth;
            newCloud.y = rand() % 201 + 600;
            newCloud.size = rand() % 30 + 50;
            clouds.push_back(newCloud);
        }

        // Generate trees with reduced frequency
        if (rand() % 300 == 0) {
            Tree newTree;
            newTree.x = windowWidth;
            newTree.y = 30;
            newTree.size = 70;
            trees.push_back(newTree);
        }

        // Generate obstacles with reduced frequency for roadblocks and fixed y position
        if (rand() % 150 == 0) {
            Obstacle newObstacle;
            newObstacle.x = windowWidth;
            newObstacle.y = 30;
            newObstacle.type = rand() % 2;
            obstacles.push_back(newObstacle);
        }

        // Generate obstacles for bees with higher frequency
        else if (rand() % 150 == 0) {
            Obstacle newObstacle;
            newObstacle.x = windowWidth;

            // Choose one of the specified y-positions for coins: [90, 120, 150]
            int beeYPositions[] = {100, 150, 200};
            newObstacle.y = beeYPositions[rand() % 3];
            newObstacle.type = COIN_TYPE;
            obstacles.push_back(newObstacle);
        }

        // Update obstacle position
        for (auto it = obstacles.begin(); it != obstacles.end();) {
            it->x -= obstacleSpeed;
            if (it->x < 0) {
                it = obstacles.erase(it);
                score++;
            } else {
                // Check for collision with obstacles
                if (characterX < it->x + 30 &&
                    characterX + 20 > it->x &&
                    characterY + jumpHeight < it->y + 40 &&
                    characterY + jumpHeight + 40 > it->y) {

                    // Separate collision checks for different obstacle types
                    if (it->type == COIN_TYPE) {
                        // Handle collision with coins
                        score += BEE_SCORE;
                        it = obstacles.erase(it);
                    } else {
                        // Handle collision with other obstacles
                        health -= 10;
                        if (health <= 0) {
                            gameover = true;
                        }
                        it = obstacles.erase(it);
                    }
                } else {
                    ++it;
                }
            }
        }
        // Check if the player has reached a certain score to advance to the next level
        if (score >= 250 * gameLevel) {
            gameLevel++;

            // Increase game speed by 3
            obstacleSpeed += 3.0f;

            // Increase cloud and tree speeds by 2 each level
            cloudSpeed += 3;
            treeSpeed += 3;
        }



        glutPostRedisplay();
        glutTimerFunc(16, updateGame, 0);
    }
}



// Function to draw the scene
void Display() {
    glClear(GL_COLOR_BUFFER_BIT);

    if (gameStarted) {
        // Draw the sky
        DrawSky();
        drawGround(30.0, 1400.0);
        // Draw clouds
        for (const auto& cloud : clouds) {
            drawCloud(cloud.x, cloud.y, cloud.size);
        }
        // Draw trees
        for (const auto& tree : trees) {
            drawTree(tree.x, tree.y, tree.size);
        }
        // Draw obstacles
        for (const auto& obstacle : obstacles) {
            if (obstacle.type == 1) {
                drawCoins(obstacle.x, obstacle.y);
            } else {
                drawPipes(obstacle.x, obstacle.y);
            }
        }
        drawBars();

        // Draw the character
        drawCharacter();

        // Check for level progression
        if (score >= 100 * gameLevel) {
            gameLevel++;
            obstacleSpeed += 3.0f;
        }
        if (gameover) {
            drawGameOver(score);
        }
        glutSwapBuffers();
    } else {
        // Draw the start screen
        drawStartScreen();
        glutSwapBuffers();
    }
}



int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(50, 0);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Super Mario Game");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    gluOrtho2D(0, windowWidth, 0, windowHeight);

    glutDisplayFunc(Display);
    glutKeyboardFunc(keyboardDownHandler);
    glutKeyboardUpFunc(keyboardUpHandler);
    glutTimerFunc(25, updateGame, 0);

    glutMainLoop();

    return 0;
}
