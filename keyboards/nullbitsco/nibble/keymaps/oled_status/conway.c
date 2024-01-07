#include QMK_KEYBOARD_H
#include "conway.h"

// 40 fps
#define CONWAY_FRAME_TIMEOUT 1000

#define CONWAY_WIDTH OLED_DISPLAY_HEIGHT / 2
#define CONWAY_HEIGHT OLED_DISPLAY_WIDTH / 2
#define CELLS_IN_INT (sizeof(int) * 8)

int grid[(CONWAY_WIDTH * CONWAY_HEIGHT + CELLS_IN_INT - 1) / CELLS_IN_INT] = {0};

uint16_t anim_timer;

bool is_initialized = false;

void drawPixel(uint8_t x, uint8_t y, bool on) {
    x *=2;
    y *=2;

    oled_write_pixel(y, x, on);
    oled_write_pixel(y + 1, x, on);
    oled_write_pixel(y, x + 1, on);
    oled_write_pixel(y + 1, x + 1, on);
}

// Function to get the state of a cell
bool getCellState(int x, int y) {
    int index     = y * CONWAY_WIDTH + x;
    int bitOffset = index % CELLS_IN_INT;
    return (grid[index / CELLS_IN_INT] & (1 << bitOffset)) != 0;
}

// Function to set the state of a cell
void setCellState(int x, int y, bool state) {
    int index     = y * CONWAY_WIDTH + x;
    int bitOffset = index % CELLS_IN_INT;
    if (state) {
        grid[index / CELLS_IN_INT] |= (1 << bitOffset);
    } else {
        grid[index / CELLS_IN_INT] &= ~(1 << bitOffset);
    }
}

// Function to initialize the grid with a glider pattern
void initializeGrid(void) {
    // int glider[] = {2, 3, WIDTH + 1, WIDTH + 2, WIDTH + 3};
    // for (int i = 0; i < sizeof(glider) / sizeof(glider[0]); ++i) {
    //     int index = glider[i];
    //     setCellState(index % WIDTH, index / WIDTH, true);
    // }
    for (int y = 0; y < CONWAY_HEIGHT; ++y) {
        for (int x = 0; x < CONWAY_WIDTH; ++x) {
            setCellState(x, y, rand() & 1);
        }
    }
}

// Function to count live neighbors of a cell
int countNeighbors(int x, int y) {
    int count = 0;
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i == 0 && j == 0) {
                continue; // Skip the current cell
            }
            int newX = x + i;
            int newY = y + j;
            if (newX >= 0 && newX < CONWAY_WIDTH && newY >= 0 && newY < CONWAY_HEIGHT) {
                count += getCellState(newX, newY);
            }
        }
    }
    return count;
}

// Function to display the current grid
void displayGrid(void) {
    for (int y = 0; y < CONWAY_HEIGHT; ++y) {
        for (int x = 0; x < CONWAY_WIDTH; ++x) {
            drawPixel(x, y, getCellState(x, y));
        }
    }
    // Add a delay if needed
    // usleep(500000); // 0.5 seconds
}

void updateGrid(void) {
    int newGrid[(CONWAY_WIDTH * CONWAY_HEIGHT + CELLS_IN_INT - 1) / CELLS_IN_INT] = {0};

    for (int y = 0; y < CONWAY_HEIGHT; ++y) {
        for (int x = 0; x < CONWAY_WIDTH; ++x) {
            int  index        = y * CONWAY_WIDTH + x;
            int  bitOffset    = index % CELLS_IN_INT;
            int  neighbors    = countNeighbors(x, y);
            bool currentState = getCellState(x, y);
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
}

void render_conway_grid(void) {
    if (!is_initialized) {
        initializeGrid();
    }

    if (timer_elapsed(anim_timer) > CONWAY_FRAME_TIMEOUT) {
        uprintf("render_conway_grid\n");

        oled_clear();
        displayGrid();
        updateGrid();

        oled_render();
        anim_timer = timer_read32();
    }
}

void process_record_conway(uint16_t keycode) {
    // switch (keycode) {
    //     case START:
    //         // uprintf("start_state_machine \n");
    //         start_state_machine();
    //         break;
    //     case RESET:
    //         // uprintf("reset_state_machine \n");
    //         reset_state_machine();
    //         break;
    //     default:
    //         if(seeding) {
    //             // uprintf("add_seed \n");
    //             add_seed(keycode);
    //         } else {
    //             // uprintf("update_state \n");
    //             update_state();
    //         }
    // }
    // render_grid();
}
