VIA_ENABLE = yes
DIGITIZER_ENABLE = yes
PLOOPY_TOUCHPAD_POC = yes
OPT_DEFS += -DPLOOPY_TOUCHPAD_POC
SRC += pixel_scroll_bridge.c touchpad_poc.c
