OLED_ENABLE = yes
OLED_DRIVER = SSD1306
WPM_ENABLE = no
VIA_ENABLE = no
SNAKE_ENABLE = no
CONWAY_ENABLE = no

ifeq ($(strip $(OLED_ENABLE)), yes)
	SRC += oled_display.c
endif


ifeq ($(strip $(SNAKE_ENABLE)), yes)
    OPT_DEFS += -DSNAKE_ENABLE
	SRC += snake.c
endif

ifeq ($(strip $(CONWAY_ENABLE)), yes)
    OPT_DEFS += -DCONWAY_ENABLE
	SRC += conway.c
endif

