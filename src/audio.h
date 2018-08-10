/*
	audio.h

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
#include "memory.h"
#include <stdbool.h>

//Getter macros
#define GET_DUTY_CYCLE(ADDRESS) (memory_get8(ADDRESS) >> 6)
#define SQUARE1_DUTY GET_DUTY_CYCLE(NR_11)
#define SQUARE2_DUTY GET_DUTY_CYCLE(NR_21)
#define GET_SQUARE_FREQ(ADRLOW, ADRHIGH) (memory_get8(ADRLOW) | ((memory_get8(ADRHIGH) & 7) << 8))
#define SQUARE1_FREQ GET_SQUARE_FREQ(NR_13, NR_14)
#define SQUARE2_FREQ GET_SQUARE_FREQ(NR_23, NR_24)
#define WAVE_FREQ GET_SQUARE_FREQ(NR_33, NR_34)
#define GET_LENGTH_LOAD(ADDRESS) (memory_get8(ADDRESS) & 0x3F)
#define SQUARE1_LENGTH_LOAD GET_LENGTH_LOAD(NR_11)
#define SQUARE2_LENGTH_LOAD GET_LENGTH_LOAD(NR_21)
#define GET_VOLUME(ADDRESS) (memory_get8(ADDRESS) >> 4)
#define SQUARE1_VOLUME GET_VOLUME(NR_12)
#define SQUARE2_VOLUME GET_VOLUME(NR_22)
#define GET_TRIGGER(ADDRESS) (memory_get8(ADDRESS) >> 7)
#define SQUARE1_TRIGGER GET_TRIGGER(NR_14)
#define SQUARE2_TRIGGER GET_TRIGGER(NR_24)
#define GET_LENGTH_ENABLE(ADDRESS) ((memory_get8(ADDRESS) >> 6) & 1)
#define SQUARE1_LENGTH_ENABLE GET_LENGTH_ENABLE(NR_14)
#define SQUARE2_LENGTH_ENABLE GET_LENGTH_ENABLE(NR_24)
#define GET_PERIOD(ADDRESS) (memory_get8(ADDRESS) & 7)
#define SQUARE1_PERIOD GET_PERIOD(NR_12)
#define SQUARE2_PERIOD GET_PERIOD(NR_22)
#define GET_ADD_MODE(ADDRESS) ((memory_get8(ADDRESS) >> 3) & 1)
#define SQUARE1_ADD_MODE GET_ADD_MODE(NR_12)
#define SQUARE2_ADD_MODE GET_ADD_MODE(NR_22)

#define SQUARE1_SWEEP_PERIOD ((memory_get8(NR_10) & 0x70) >> 4)
#define SQUARE1_NEGATE ((memory_get8(NR_10) >> 3) & 1)
#define SQUARE1_SHIFT (memory_get8(NR_10) & 7)

#define WAVE_ON (memory_get8(NR_30) & 0x80)
//Setter macros
#define SET_LENGTH_COUNTER(ADDRESS, VALUE) memory_set8(ADDRESS, (memory_get8(ADDRESS) & 0xC0) | (VALUE & 0x3F))
#define SQUARE1_SET_LENGTH(VALUE) SET_LENGTH_COUNTER(NR_11, VALUE)
#define SQUARE2_SET_LENGTH(VALUE) SET_LENGTH_COUNTER(NR_12, VALUE)
#define SET_DUTY_CYCLE(ADDRESS, VALUE) memory_set8(ADDRESS, (memory_get8(ADDRESS) & 0x3F) | (VALUE << 6))
#define SQUARE1_SET_DUTY(VALUE) SET_DUTY_CYCLE(NR_11)
#define SQUARE2_SET_DUTY(VALUE) SET_DUTY_CYCLE(NR_12)
#define SET_FREQ(ADRLOW, ADRHIGH, VALUE) memory_set8(ADRLOW, VALUE & 0xFF); memory_set8(ADRHIGH, (memory_get8(ADRHIGH) & 0xF8) | (VALUE >> 8))
#define SQUARE1_SET_FREQ(VALUE) SET_FREQ(NR_13, NR_14, VALUE & 0x7FF)
#define SQUARE1_SET_SWEEP_PERIOD(VALUE) memory_set8(NR_10, memory_get8(NR_10) & 0x8F | ((VALUE & 7) << 8))

#define FRAME_SEQUENCER_TICKS (Z80_CYCLES / 512)
typedef enum duty_cycle
{
	DUTY_CYCLE_12_5,
	DUTY_CYCLE_25,
	DUTY_CYCLE_50,
	DUTY_CYCLE_75
} duty_cycle;

//Sample contants
#define AS_HIGH 127
#define AS_LOW -128
#define AUDIO_FREQ 48000
#define SAMPLES_PER_FRAME 256
#define CYCLES_PER_SAMPLE (Z80_CYCLES / AUDIO_FREQ)
//Signed 8-bit samples
typedef char audio_sample;

extern bool square1_enable, square2_enable;
extern bool audio_enable;

void audio_init(void);
void audio_tick(void);
void audio_tick_frame_sequencer(void);
audio_sample audio_gen_square(duty_cycle, unsigned);
audio_sample audio_gen_wave(unsigned);
