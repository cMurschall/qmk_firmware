#include "snake.h"
#include <stdio.h>
#include <stdbool.h>

#include "quantum.h"
#include "oled_display.h"

#ifndef SNAKE_STARTING_LENGTH
#    define SNAKE_STARTING_LENGTH 2
#endif

// 40 fps
#define FRAME_TIMEOUT (1000 / 30)

#ifndef FOOD_FLASH_RATE
#    define FOOD_FLASH_RATE 250
#endif

#ifndef OLED_DISPLAY_WIDTH
#    define OLED_DISPLAY_WIDTH 128
#endif

#ifndef OLED_DISPLAY_HEIGHT
#    define OLED_DISPLAY_HEIGHT 32
#endif

// Update game values in ms
#ifndef GAME_SPEED
#    define GAME_SPEED 500
#endif

#define GAME_DISPLAY_WIDTH OLED_DISPLAY_HEIGHT
#define GAME_DISPLAY_HEIGHT OLED_DISPLAY_WIDTH

#define CLAMP(x, lower, upper) (MIN(upper, MAX(x, lower)))

// Snake structure with head and tail coordinates
typedef struct {
    uint8_t headX;
    uint8_t headY;
    uint8_t tailX;
    uint8_t tailY;
} SnakeSegment;

// Snake direction enum
typedef enum { sUP = 0, sRIGHT, sDOWN, sLEFT } snakeDirections;

// Initialize the snake
uint8_t      snakeLength = SNAKE_INITIAL_LENGTH;
SnakeSegment snake[SNAKE_MAX_LENGTH]; // Maximum possible length

snakeDirections snake_dir        = sRIGHT;
bool            snakeInitialised = false;

// Initialize the snake

uint16_t snake_move_timer = 0;
uint16_t anim_timer       = 0;

uint8_t lengthIncrement = 1; // Increase the length by 1

void write_pixel(uint8_t x, uint8_t y, bool on) {
    oled_write_pixel(y, x, on);
}

// Function to draw a 2x2 pixel block on the screen
void write_pixel2(uint8_t x, uint8_t y, bool on) {
    for (int i = x; i < x + 2; i++) {
        for (int j = y; j < y + 2; j++) {
            // Adjust the coordinates based on the size of the pixel block
            oled_write_pixel(j, i, on);
        }
    }
}

// Function to initialize the snake
void initializeSnake(void) {
    snake[0].headX = 15;
    snake[0].headY = 15;

    for (int i = 1; i < SNAKE_INITIAL_LENGTH; i++) {
        snake[i].headX = snake[i - 1].tailX; // Set head based on tail of previous segment
        snake[i].headY = snake[i - 1].tailY;
        snake[i].tailX = snake[i].headX - 2; // Set initial tail coordinates
        snake[i].tailY = snake[i].headY;
    }
}

// Function to check for collision
bool checkCollision(void) {
    // Check boundary collision
    if (snake[0].headX <= 0 || snake[0].headX >= GAME_DISPLAY_WIDTH || snake[0].headY <= 0 || snake[0].headY >= GAME_DISPLAY_HEIGHT) {
        return true; // Boundary collision
    }

    // Check self-collision
    for (int i = 1; i < snakeLength; i++) {
        if (snake[0].headX == snake[i].headX && snake[0].headY == snake[i].headY) {
            return true; // Self-collision
        }
    }

    return false; // No collision
}

// Function to update and draw the snake
void updateSnake(void) {
    if (timer_elapsed(snake_move_timer) >= GAME_SPEED) {
        // Move the snake
        for (uint8_t i = snakeLength - 1; i > 0; i--) {
            // snake[i].headX = snake[i - 1].headX;
            // snake[i].headY = snake[i - 1].headY;
            // snake[i].tailX = snake[i].headX - 2;
            // snake[i].tailY = snake[i].headY;

            snake[i].headX = snake[i - 1].headX;
            snake[i].headY = snake[i - 1].headY;
            snake[i].tailX = snake[i - 1].tailX; // Adjust the tail coordinates
            snake[i].tailY = snake[i - 1].tailY;
        }

        // Move the head
        switch (snake_dir) {
            case sRIGHT: // right
                snake[0].headY = CLAMP(snake[0].headY + 2, 0, GAME_DISPLAY_HEIGHT);
                break;
            case sUP: // up
                snake[0].headX = CLAMP(snake[0].headX - 2, 0, GAME_DISPLAY_WIDTH);
                break;
            case sLEFT: // left
                snake[0].headY = CLAMP(snake[0].headY - 2, 0, GAME_DISPLAY_HEIGHT);
                break;
            case sDOWN: // down
                snake[0].headX = CLAMP(snake[0].headX + 2, 0, GAME_DISPLAY_WIDTH);
                break;
        }

        snake_move_timer = timer_read32();

        // uprintf("snake moved (x: %2u, y: %2u)\n", snake[0].headX, snake[0].headY);
    }
    // Draw the snake

    for (int i = 0; i < snakeLength; i++) {
        // uprintf("draw segment  %u long. draws at (x: %2u, y: %2u)\n", i, snake[i].headX, snake[i].headY);
        write_pixel2(snake[i].headX, snake[i].headY, true);
        write_pixel2(snake[i].tailX, snake[i].tailY, true);
    }
}

void process_record_snake(uint16_t keycode) {
    switch (keycode) {
        case KC_UP:
            uprintf("snake goes up \n");
            snake_dir = sUP;
            break;
        case KC_DOWN:
            snake_dir = sDOWN;
            uprintf("snake goes down \n");
            break;
        case KC_RGHT:
            snake_dir = sRIGHT;
            uprintf("snake goes right \n");
            break;
        case KC_LEFT:
            snake_dir = sLEFT;
            uprintf("snake goes left \n");
            break;
        default:
            break;
    }
}

void draw_border(void) {
    for (uint8_t x = 0; x < GAME_DISPLAY_WIDTH; x++) {
        oled_write_pixel(0, x, true);
        oled_write_pixel(GAME_DISPLAY_HEIGHT - 1, x, true);
    }
    for (uint8_t y = 0; y < GAME_DISPLAY_HEIGHT; y++) {
        oled_write_pixel(y, 0, true);
        oled_write_pixel(y, GAME_DISPLAY_WIDTH - 1, true);
    }
}

void snake_deinit(void) {
        snakeInitialised = false;
}

void snake_init(void) {
    initializeSnake();

    snake_move_timer = timer_read32();
    anim_timer       = timer_read32();

    oled_clear();
    oled_render();

    snakeInitialised = true;
}

void render_snake_game(void) {
    if (!snakeInitialised) {
        snake_init();
    }
 
    if (timer_elapsed(anim_timer) > FRAME_TIMEOUT) {
        oled_clear();

        draw_border();
        // Update and draw the snake
        updateSnake();
        oled_render();

        anim_timer = timer_read32();
    }
}
