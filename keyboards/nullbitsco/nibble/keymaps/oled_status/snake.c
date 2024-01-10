#include "snake.h"
#include <stdio.h>
#include <stdbool.h>

#include "quantum.h"
#include "oled_display.h"

#ifndef SNAKE_STARTING_LENGTH
#    define SNAKE_STARTING_LENGTH 2
#endif

#ifndef OLED_DISPLAY_WIDTH
#    define OLED_DISPLAY_WIDTH 128
#endif

#ifndef OLED_DISPLAY_HEIGHT
#    define OLED_DISPLAY_HEIGHT 32
#endif

// Update game values in ms
#ifndef GAME__INITIALSPEED
#    define GAME_INITIAL_SPEED 300
#    define GAME_MAX_SPEED 40

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

// Fruit structure with coordinates
typedef struct {
    uint8_t x;
    uint8_t y;
} Fruit;

// Snake direction enum
typedef enum { sUP = 0, sRIGHT, sDOWN, sLEFT } snakeDirections;

// Initialize the snake
uint8_t      snakeLength = SNAKE_INITIAL_LENGTH;
SnakeSegment snake[SNAKE_MAX_LENGTH]; // Maximum possible length

snakeDirections snake_dir        = sRIGHT;
bool            snakeInitialised = false;

uint16_t game_speed = GAME_INITIAL_SPEED;

Fruit fruit; // Declare a global variable for the fruit

uint8_t fuit_counter;
uint8_t fuit_render_counter;

uint16_t anim_timer = 0;

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

// Function to spawn a fruit at a random location
void spawn_fruit(void) {
    fruit.x = CLAMP(rand() % GAME_DISPLAY_WIDTH, 4, GAME_DISPLAY_WIDTH - 4);   // Random x-coordinate within the game width
    fruit.y = CLAMP(rand() % GAME_DISPLAY_HEIGHT, 4, GAME_DISPLAY_HEIGHT - 4); // Random y-coordinate within the game height
}

// Function to initialize the snake
void initialize_snake(void) {
    srand(timer_read32());

    fuit_counter        = 0;
    fuit_render_counter = 0;
    game_speed          = GAME_INITIAL_SPEED;

    snakeLength = SNAKE_INITIAL_LENGTH;
    snake_dir   = sRIGHT;

    snake[0].headX = 15;
    snake[0].headY = 15;

    for (int i = 1; i < SNAKE_INITIAL_LENGTH; i++) {
        snake[i].headX = snake[i - 1].tailX; // Set head based on tail of previous segment
        snake[i].headY = snake[i - 1].tailY;
        snake[i].tailX = snake[i].headX - 2; // Set initial tail coordinates
        snake[i].tailY = snake[i].headY;
    }
}

// Function to append a new segment to the snake
void append_snake_segment(void) {
    // Check if the snake has reached its maximum length
    if (snakeLength < SNAKE_MAX_LENGTH) {
        // Update the tail coordinates to the current head coordinates
        snake[snakeLength].tailX = snake[0].headX;
        snake[snakeLength].tailY = snake[0].headY;

        // Increment the length of the snake
        snakeLength++;
    }
}

// Function to check if the snake has eaten the fruit
bool hasEatenFruit(void) {
    // Assuming each snake segment occupies a 2x2 pixel area
    for (int i = 0; i < snakeLength; ++i) {
        // Check if any part of the snake (head or tail) is on the fruit
        if ((snake[i].headX <= fruit.x && fruit.x < snake[i].headX + 2) && (snake[i].headY <= fruit.y && fruit.y < snake[i].headY + 2)) {
            return true;
        }

        if ((snake[i].tailX <= fruit.x && fruit.x < snake[i].tailX + 2) && (snake[i].tailY <= fruit.y && fruit.y < snake[i].tailY + 2)) {
            return true;
        }
    }

    return false;
}

// Function to update and draw the snake
void updateSnake(void) {
    // Move the snake
    for (uint8_t i = snakeLength - 1; i > 0; i--) {
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

    if (hasEatenFruit()) {
        append_snake_segment();
        spawn_fruit();
        game_speed = MAX(game_speed - 20, GAME_MAX_SPEED);

        fuit_counter++;
        fuit_render_counter = 3;

#ifdef CONSOLE_ENABLE
        uprintf("snake eats fruit. new game speed %2u. fruits eaten: %2u. \n", game_speed, fuit_counter);
#endif
    }

    // uprintf("snake moved (x: %2u, y: %2u)\n", snake[0].headX, snake[0].headY)
}

void draw_snake(void) {
    // Draw the snake

    for (int i = 0; i < snakeLength - 1; i++) {
        // uprintf("draw segment  %u long. draws at (x: %2u, y: %2u)\n", i, snake[i].headX, snake[i].headY);
        write_pixel2(snake[i].headX, snake[i].headY, true);
        write_pixel2(snake[i].tailX, snake[i].tailY, true);
    }
    write_pixel2(0,0,false);
}

void draw_fruit(void) {
    write_pixel2(fruit.x, fruit.y, true);
}

void draw_fuit_counter(void) {
    if (fuit_render_counter > 0) {
#ifdef CONSOLE_ENABLE
        uprintf("render fruit score\n");
#endif
        static char msg_str[3];
        sprintf(msg_str, "%2u", fuit_counter);
        oled_set_cursor(0, 0);
        oled_write(msg_str, false);
        fuit_render_counter--;
    }
}

void process_record_snake(uint16_t keycode) {
    switch (keycode) {
        case KC_UP:
#ifdef CONSOLE_ENABLE
            uprintf("snake goes up \n");
#endif
            snake_dir = sUP;
            break;
        case KC_DOWN:
            snake_dir = sDOWN;
#ifdef CONSOLE_ENABLE
            uprintf("snake goes down \n");
#endif
            break;
        case KC_RGHT:
            snake_dir = sRIGHT;
#ifdef CONSOLE_ENABLE
            uprintf("snake goes right \n");
#endif
            break;
        case KC_LEFT:
            snake_dir = sLEFT;
#ifdef CONSOLE_ENABLE
            uprintf("snake goes left \n");
#endif
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
    initialize_snake();

    anim_timer = timer_read32();

    oled_clear();
    oled_render();

    snakeInitialised = true;
}

void render_snake_game(void) {
    if (!snakeInitialised) {
        snake_init();
        spawn_fruit();
    }

    if (timer_elapsed(anim_timer) > game_speed) {
        oled_clear();

        // Update and draw the snake
        // draw_border();

        updateSnake();
        draw_snake();

        draw_fruit();
        draw_fuit_counter();

        oled_render();

        anim_timer = timer_read32();
    }
}

void encoder_update_snake(uint8_t index, bool clockwise) {
    if (clockwise) {
        game_speed = MAX(game_speed - 20, GAME_MAX_SPEED);
    } else {
        game_speed = MAX(game_speed + 20, GAME_MAX_SPEED);
    }
}