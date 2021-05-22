LTO_ENABLE = yes

BOOTMAGIC_ENABLE           = no  # Virtual DIP switch configuration(+1000)
MOUSEKEY_ENABLE            = yes  # Mouse keys(+4700)
EXTRAKEY_ENABLE            = yes # Audio control and System control(+450)
CONSOLE_ENABLE             = no  # Console for debug(+400)
COMMAND_ENABLE             = no  # Commands for debug and configuration
NKRO_ENABLE                = yes # Nkey Rollover - if this doesn't work, see here: https://github.com/tmk/tmk_keyboard/wiki/FAQ#nkro-doesnt-work
RGB_MATRIX_ENABLE 		   = WS2812
SSD1306OLED                = yes


BOOTLOADER = qmk-dfu

# If you want to change the display of OLED, you need to change here
SRC += ./lib/layer_state_reader.c #\

#  ./lib/glcdfont.c \
#         ./lib/rgb_state_reader.c \
#         ./lib/layer_state_reader.c \
#         ./lib/logo_reader.c \
#         ./lib/mode_icon_reader.c \
#         ./lib/host_led_state_reader.c \
#         ./lib/timelogger.c \


# To enable flashiness, uncomment:
SRC += game_of_life.c \
		./lib/glcdfont.c \
		./lib/keylogger.c

# util/docker_build.sh crkbd:kolemak:dfu-split-right
# util/docker_build.sh crkbd:kolemak:dfu-split-right
