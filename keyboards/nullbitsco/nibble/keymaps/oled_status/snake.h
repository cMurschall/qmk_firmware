#pragma once




#if defined (SNAKE_ENABLE) && defined (OLED_ENABLE)
#include QMK_KEYBOARD_H

#define SNAKE_MAX_LENGTH 32

#define SNAKE_INITIAL_LENGTH 5


/**
 * 
 **/


void snake_deinit(void);
void render_snake_game(void);


void process_record_snake(uint16_t keycode);
void encoder_update_snake(uint8_t index, bool clockwise);




#endif