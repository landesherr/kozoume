#pragma once

#include "globaldefs.h"

#define ALPHA 255
#define GET_R(COLOR) ((COLOR >> 16) & 0xFF)
#define GET_G(COLOR) ((COLOR >> 8) & 0xFF)
#define GET_B(COLOR) (COLOR & 0xFF)

typedef unsigned color;

void render_init(void);
void render_teardown(void);
void render_screen(void);
