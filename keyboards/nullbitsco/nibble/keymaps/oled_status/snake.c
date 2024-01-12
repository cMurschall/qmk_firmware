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

#define GAME_DISPLAY_WIDTH (OLED_DISPLAY_HEIGHT / 2)
#define GAME_DISPLAY_HEIGHT (OLED_DISPLAY_WIDTH / 2)

#define CLAMP(x, lower, upper) (MIN(upper, MAX(x, lower)))

// segment structure with head and tail coordinates
typedef struct {
    uint8_t x;
    uint8_t y;
} Segment;


// Snake direction enum
typedef enum { sUP = 0, sRIGHT, sDOWN, sLEFT } snakeDirections;

// Initialize the snake
uint8_t      snakeLength = SNAKE_INITIAL_LENGTH;
Segment snake[SNAKE_MAX_LENGTH]; // Maximum possible length

snakeDirections snake_dir        = sRIGHT;
bool            snakeInitialised = false;

uint16_t game_speed = GAME_INITIAL_SPEED;

Segment fruit; // Declare a global variable for the fruit

uint8_t  fuit_counter;
uint16_t fuit_render_timer;

uint16_t anim_timer = 0;

uint8_t lengthIncrement = 1; // Increase the length by 1

void write_pixel(uint8_t x, uint8_t y, bool on) {
    oled_write_pixel(y, x, on);
}

// Function to draw a 2x2 pixel block on the screen
void write_snake_pixel(uint8_t x, uint8_t y, bool on) {
    x *= 2;
    y *= 2;

    oled_write_pixel(y, x, on);
    oled_write_pixel(y, x + 1, on);

    oled_write_pixel(y + 1, x, on);
    oled_write_pixel(y + 1, x + 1, on);
}

// Function to spawn a fruit at a random location
void spawn_fruit(void) {
    fruit.x = CLAMP(rand() % GAME_DISPLAY_WIDTH, 4, GAME_DISPLAY_WIDTH - 4);   // Random x-coordinate within the game width
    fruit.y = CLAMP(rand() % GAME_DISPLAY_HEIGHT, 4, GAME_DISPLAY_HEIGHT - 4); // Random y-coordinate within the game height
}

// Function to initialize the snake
void initialize_snake(void) {
    srand(timer_read32());

    fuit_counter      = 0;
    fuit_render_timer = timer_read32();
    game_speed        = GAME_INITIAL_SPEED;

    snakeLength = SNAKE_INITIAL_LENGTH;
    snake_dir   = sRIGHT;

    snake[0].x = 15;
    snake[0].y = 15;

    for (int i = 1; i < SNAKE_INITIAL_LENGTH; i++) {
        snake[i].x = snake[i - 1].x + 1; // Set the x-coordinate for the current segment
        snake[i].y = snake[i - 1].y;     // Set the y-coordinate for the current segment
    }
}

// Function to append a new segment to the snake
void append_snake_segment(void) {
    // Check if the snake has reached its maximum length
    if (snakeLength < SNAKE_MAX_LENGTH) {
        // Update the tail coordinates to the current head coordinates
        snake[snakeLength].x = snake[snakeLength - 1].x;
        snake[snakeLength].y = snake[snakeLength - 1].y;

        // Increment the length of the snake
        snakeLength++;
    }
}

// Function to check if the snake collided with itself or the game boundaries
bool check_collision(void) {
    // Check collision with itself
    for (int i = 1; i < snakeLength; i++) {
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
            return true; // Collision with itself
        }
    }

    // Check collision with game boundaries (you might need to adjust this based on your game grid)
    if (snake[0].x < 0 || snake[0].x >= GAME_DISPLAY_WIDTH || snake[0].y < 0 || snake[0].y >= GAME_DISPLAY_HEIGHT) {
        return true; // Collision with boundaries
    }

    // Additional collision checks can be added here, e.g., collision with other game objects

    return false; // No collision
}

// Function to check if the snake has eaten the fruit
bool hasEatenFruit(void) {
    return (snake[0].x == fruit.x) && (snake[0].y == fruit.y);
}

// Function to update and draw the snake
void updateSnake(void) {
    // Move the snake
    for (uint8_t i = snakeLength - 1; i > 0; i--) {
        snake[i].x = snake[i - 1].x;
        snake[i].y = snake[i - 1].y;
    }

    // Move the head
    switch (snake_dir) {
        case sRIGHT: // right
            snake[0].y = CLAMP(snake[0].y + 1, 0, GAME_DISPLAY_HEIGHT);
            break;
        case sUP: // up
            snake[0].x = CLAMP(snake[0].x - 1, 0, GAME_DISPLAY_WIDTH);
            break;
        case sLEFT: // left
            snake[0].y = CLAMP(snake[0].y - 1, 0, GAME_DISPLAY_HEIGHT);
            break;
        case sDOWN: // down
            snake[0].x = CLAMP(snake[0].x + 1, 0, GAME_DISPLAY_WIDTH);
            break;
    }

    if (hasEatenFruit()) {
        append_snake_segment();
        spawn_fruit();
        game_speed = MAX(game_speed - 20, GAME_MAX_SPEED);

        fuit_counter++;
        fuit_render_timer = timer_read32();

#ifdef CONSOLE_ENABLE
        uprintf("snake eats fruit. new game speed %2u. fruits eaten: %2u. \n", game_speed, fuit_counter);
#endif
    }

    // uprintf("snake moved (x: %2u, y: %2u)\n", snake[0].headX, snake[0].headY)
}

void draw_snake(void) {
    // Draw the snake

    for (int i = 0; i < snakeLength - 1; i++) {
        write_snake_pixel(snake[i].x, snake[i].y, true);
    }
    // write_snake_pixel(0, 0, false);
}

void draw_fruit(void) {
    write_snake_pixel(fruit.x, fruit.y, true);
}

void draw_fuit_counter(void) {
    if (!(timer_elapsed(fuit_render_timer) > 2000)) {
#ifdef CONSOLE_ENABLE
        uprintf("render fruit score\n");
#endif
        oled_set_cursor(0, 0);
        if (fuit_counter) {
            static char fruit_msg_str[3];
            sprintf(fruit_msg_str, "%2u", fuit_counter);
            oled_write(fruit_msg_str, false);
        } else {
            oled_write_P(PSTR("snake"), false);
        }
    }
}

void update_snake_direction(snakeDirections new_direction) {
    // Prevent changing from up to down or from down to up, and from left to right or from right to left
    if ((snake_dir == sUP && new_direction == sDOWN) || (snake_dir == sDOWN && new_direction == sUP) || (snake_dir == sLEFT && new_direction == sRIGHT) || (snake_dir == sRIGHT && new_direction == sLEFT)) {
        // Do nothing
        return;
    }

    // Update the snake direction
    snake_dir = new_direction;
}

void process_record_snake(uint16_t keycode) {
    switch (keycode) {
        case KC_UP:
            update_snake_direction(sUP);
            break;
        case KC_DOWN:
            update_snake_direction(sDOWN);
            break;
        case KC_RGHT:
            update_snake_direction(sRIGHT);
            break;
        case KC_LEFT:
            update_snake_direction(sLEFT);
            break;
        default:
            break;
    }
}

void draw_border(void) {
    for (uint8_t x = 0; x < GAME_DISPLAY_WIDTH * 2; x++) {
        oled_write_pixel(0, x, true);
        oled_write_pixel(GAME_DISPLAY_HEIGHT * 2 - 1, x, true);
    }
    for (uint8_t y = 0; y < GAME_DISPLAY_HEIGHT * 2; y++) {
        oled_write_pixel(y, 0, true);
        oled_write_pixel(y, GAME_DISPLAY_WIDTH * 2 - 1, true);
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
    if (check_collision()) {
#ifdef CONSOLE_ENABLE
        uprintf("game over\n");
        snake_deinit();
#endif
    }

    if (timer_elapsed(anim_timer) > game_speed) {
        oled_clear();

        // Update and draw the snake
        draw_border();

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