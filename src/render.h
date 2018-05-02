/*
	render.h

	Copyright 2018 Nolan Varani

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

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
