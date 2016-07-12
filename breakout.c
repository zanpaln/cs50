//
// breakout.c
//
// Computer Science 50
// Problem Set 3
//

// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stanford Portable Library
#include "gevents.h"
#include "gobjects.h"
#include "gwindow.h"

// height and width of game's window in pixels
#define HEIGHT 600
#define WIDTH 400

// height and wigth of brik
#define BRICK_HEIGHT 10
#define BRICK_WIDTH 36
#define BETWEEN_BRICK 4

// height and width of paddle
#define PADDLE_HEIGHT 5
#define PADDLE_WIDTH 40

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 10

// radius of ball in pixels
#define RADIUS 10

// lives
#define LIVES 3

// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
GObject detectCollision(GWindow window, GOval ball);
void reverse(double *velocity);

int main(void)
{
    // seed pseudorandom number generator
    srand48(time(NULL));

    // instantiate window
    GWindow window = newGWindow(WIDTH, HEIGHT);

    // instantiate bricks
    initBricks(window);

    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);

    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);

    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel label = initScoreboard(window);

    // number of bricks initially
    int bricks = COLS * ROWS;

    // number of lives initially
    int lives = LIVES;

    // number of points initially
    int points = 0;

    // set velocity
    double randMagnitude = (drand48() + 1) / 40;
    int direction = (rand() % 2) * 2 - 1;
    double velocity_x = direction * randMagnitude;
    double velocity_y = 1;
    
    // keep playing until game over
    while (lives > 0 && bricks > 0)
    {
        GEvent event = getNextEvent(MOUSE_EVENT);
        
        if(event != NULL)
        {
            if(getEventType(event) == MOUSE_MOVED)
            {
                double paddle_x = getX(event) - getWidth(paddle);
                double paddle_y = getY(paddle);
                
                setLocation(paddle, paddle_x, paddle_y);
            }
        }
        
        move(ball, velocity_x, velocity_y);
        
        if(getX(ball) + getWidth(ball) >= getWidth(window))
            reverse(&velocity_x);
        else if(getX(ball) <= 0)
            reverse(&velocity_x);
        else if(getY(ball) <= 0)
            reverse(&velocity_y);
        else if(getY(ball) + getHeight(ball) >= getHeight(window))
        {
            lives--;
            setLocation(ball, (WIDTH - RADIUS) /2, (HEIGHT - 100) / 2);
            setLocation(paddle, (WIDTH - PADDLE_WIDTH) /2, HEIGHT - 50);
            waitForClick();
        }
        
        GObject collision = detectCollision(window, ball);
        
        if(collision != NULL && strcmp(getType(collision), "GRect") == 0)
        {
            reverse(&velocity_y);
            
            if(collision != paddle)
            {
                removeGWindow(window, collision);
                bricks--;
                points++;
                updateScoreboard(window, label, points);
            }
        }
    }

    // wait for click before exiting
    waitForClick();

    // game over
    closeGWindow(window);
    return 0;
}

/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{
    char* colors[ROWS] = {"BLUE", "YELLOW", "CYAN", "ORANGE", "GREEN"};
    int top = 25;
    
    for(int i = 0; i < ROWS; i++)
    {
        for(int j = 0; j < COLS; j++)
        {
            GRect rect = newGRect((BETWEEN_BRICK + j * (BETWEEN_BRICK + BRICK_WIDTH)), (BETWEEN_BRICK + i * (BETWEEN_BRICK + BRICK_HEIGHT) + top), BRICK_WIDTH, BRICK_HEIGHT);
            setColor(rect, colors[i]);
            setFilled(rect, true);
            add(window, rect);
        }
    }
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    GOval ball = newGOval(WIDTH/2 - RADIUS, HEIGHT/2 - RADIUS, RADIUS * 2, RADIUS * 2);
    setColor(ball, "LIGHT_GRAY");
    setFilled(ball, true);
    add(window, ball);
    
    return ball;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
    GRect paddle = newGRect(WIDTH/2 - PADDLE_WIDTH, HEIGHT - 50, PADDLE_WIDTH, PADDLE_HEIGHT);
    setColor(paddle, "DARK_GRAY");
    setFilled(paddle, true);
    add(window, paddle);
    
    return paddle;
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
    GLabel label = newGLabel("0");
    setFont(label, "SansSerif-18");
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) /2;
    setLocation(label, x, y);
    setColor(label, "CYAN");
    setFilled(label, true);
    add(window, label);
    
    return label;
}

/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    sprintf(s, "%i", points);
    setLabel(label, s);

    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
}

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);

    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // no collision
    return NULL;
}

void reverse(double *velocity)
{
    *velocity *= -1;
    
    return;
}
