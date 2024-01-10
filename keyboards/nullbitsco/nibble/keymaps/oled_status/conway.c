#include QMK_KEYBOARD_H
#include "conway.h"

// 40 fps
#define CONWAY_FRAME_TIMEOUT 400

#define CONWAY_WIDTH OLED_DISPLAY_HEIGHT / 2
#define CONWAY_HEIGHT OLED_DISPLAY_WIDTH / 2

// #define CONWAY_WIDTH 20
// #define CONWAY_HEIGHT 20

#define CELLS_IN_INT (sizeof(int) * 8)

#define CLAMP(x, lower, upper) (MIN(upper, MAX(x, lower)))

int grid[(CONWAY_WIDTH * CONWAY_HEIGHT + CELLS_IN_INT - 1) / CELLS_IN_INT] = {0};

// Macro to get the state of a cell
// #define GET_CELL_STATE(x, y) ((grid[((y) / 2) * (WIDTH / 2) + ((x) / 2)] & (1 << (((y) % 2) * 2 + ((x) % 2)))) != 0)

uint16_t anim_timer;

bool is_initialized = false;

void write_conway_pixel(uint8_t x, uint8_t y, bool on) {
    x *= 2;
    y *= 2;

    oled_write_pixel(y, x, on);
    oled_write_pixel(y, x + 1, on);

    oled_write_pixel(y + 1, x, on);
    oled_write_pixel(y + 1, x + 1, on);
}

// Function to get the state of a cell
bool getCellState(uint8_t x, uint8_t y) {
    int index     = y * CONWAY_WIDTH + x;
    int bitOffset = index % CELLS_IN_INT;
    return (grid[index / CELLS_IN_INT] & (1 << bitOffset)) != 0;
}

// Function to set the state of a cell
void setCellState(uint8_t x, uint8_t y, bool state) {
    int index     = y * CONWAY_WIDTH + x;
    int bitOffset = index % CELLS_IN_INT;
    if (state) {
        grid[index / CELLS_IN_INT] |= (1 << bitOffset);
    } else {
        grid[index / CELLS_IN_INT] &= ~(1 << bitOffset);
    }
}

// Function to initialize the grid with a glider pattern
void initialize_grid_glider(void) {
#ifdef CONSOLE_ENABLE
    uprintf("conway init \n");
#endif
    // int newGrid[(CONWAY_WIDTH * CONWAY_HEIGHT + CELLS_IN_INT - 1) / CELLS_IN_INT] = {0};

    // // Copy the new grid back to the original grid
    // for (uint8_t i = 0; i < sizeof(grid) / sizeof(grid[0]); ++i) {
    //     grid[i] = newGrid[i];
    // }

    int glider[] = {2, 3, WIDTH + 1, WIDTH + 2, WIDTH + 3};
    for (uint8_t i = 0; i < sizeof(glider) / sizeof(glider[0]); ++i) {
        uint8_t index = glider[i];
        setCellState(index % WIDTH, index / WIDTH, true);
    }

    is_initialized = true;
}

void initialize_grid_random(void) {
#ifdef CONSOLE_ENABLE
    uprintf("conway init random \n");
#endif
    // int newGrid[(CONWAY_WIDTH * CONWAY_HEIGHT + CELLS_IN_INT - 1) / CELLS_IN_INT] = {0};

    // // Copy the new grid back to the original grid
    // for (uint8_t i = 0; i < sizeof(grid) / sizeof(grid[0]); ++i) {
    //     grid[i] = newGrid[i];
    // }

    for (int y = 0; y < CONWAY_HEIGHT; ++y) {
        for (int x = 0; x < CONWAY_WIDTH; ++x) {
            setCellState(x, y, rand() & 1);
        }
    }

    is_initialized = true;
}

// Function to count live neighbors of a cell
uint8_t countNeighbors(uint8_t x, uint8_t y) {
    uint8_t count = 0;
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i == 0 && j == 0) {
                continue; // Skip the current cell
            }
            int newX = CLAMP(x + i, 0, CONWAY_WIDTH);
            int newY = CLAMP(y + j, 0, CONWAY_HEIGHT);
            if (getCellState(newX, newY)) {
                count += 1;
            }
        }
    }
    return count;
}

// Function to display the current grid
void displayGrid(void) {
    for (int y = 0; y < CONWAY_HEIGHT; ++y) {
        for (int x = 0; x < CONWAY_WIDTH; ++x) {
            write_conway_pixel(x, y, getCellState(x, y));
        }
    }
    // Add a delay if needed
    // usleep(500000); // 0.5 seconds
}

void updateGrid(void) {
    int newGrid[(CONWAY_WIDTH * CONWAY_HEIGHT + CELLS_IN_INT - 1) / CELLS_IN_INT] = {0};

    for (int y = 0; y < CONWAY_HEIGHT; ++y) {
        for (int x = 0; x < CONWAY_WIDTH; ++x) {
            int index        = y * CONWAY_WIDTH + x;
            int bitOffset    = index % CELLS_IN_INT;
            int neighbors    = countNeighbors(x, y);
            bool    currentState = getCellState(x, y);
            if (currentState) {
                newGrid[index / CELLS_IN_INT] |= ((neighbors == 2 || neighbors == 3) << bitOffset);
            } else {
                newGrid[index / CELLS_IN_INT] |= ((neighbors == 3) << bitOffset);
            }
        }
    }

    // Copy the new grid back to the original grid
    for (int i = 0; i < sizeof(grid) / sizeof(grid[0]); ++i) {
        grid[i] = newGrid[i];
    }


    #ifdef CONSOLE_ENABLE
    uprintf("conway grid updated \n");
#endif
}

void render_conway_grid(void) {
    if (!is_initialized) {
        initialize_grid_random();
    }

    if (timer_elapsed(anim_timer) > CONWAY_FRAME_TIMEOUT) {
        // uprintf("render_conway_grid\n");

        oled_clear();
        displayGrid();
        updateGrid();

        oled_render();
        anim_timer = timer_read32();
    }
}

void conway_deinit(void) {
    is_initialized = false;
}

void process_record_conway(uint16_t keycode) {
    switch (keycode) {
        case KC_BSPC:
            initialize_grid_glider();
            break;
        case KC_ENT:
            initialize_grid_random();
            break;
        default:
            break;
    }
}
