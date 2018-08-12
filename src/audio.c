/*
	audio.c

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

#include "audio.h"
#include "globaldefs.h"
#include "z80.h"
#include "memory.h"
#include "interpreter.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <string.h>
#include <stdio.h>

#define DEFAULT_VOLUME .05

static inline void update_sweep(void);
static inline void update_length_counter(void);
static inline void update_volume_envelope(void);

static SDL_AudioSpec settings = {};
static audio_sample audio_buffer[SAMPLES_PER_FRAME*2];
static unsigned audio_index = 0;
static bool use_audio = true;

bool audio_enable = false;
bool square1_enable = false;
bool square2_enable = false;
bool wave_enable = false;

word square1_shadow = 0;
byte square1_sweep_time = 0;
byte square1_shift = 0;

byte square1_length = 0;
byte square1_length_remain = 0;
byte square2_length = 0;
byte square2_length_remain = 0;

byte square1_volume = 0xF;
byte square1_volume_time = 0;
byte square2_volume = 0xF;
byte square2_volume_time = 0;

void audio_init()
{
	SDL_InitSubSystem(SDL_INIT_AUDIO);
	settings.freq = AUDIO_FREQ;
	settings.format = AUDIO_S8;
	settings.channels = 2;
	settings.samples = SAMPLES_PER_FRAME;
	settings.callback = NULL;
	settings.userdata = NULL;
	if(SDL_OpenAudio(&settings, NULL) < 0)
	{
		dbgwrite("Could not open audio device. Disabling audio...\n");
		use_audio = false;
	}
	memset(audio_buffer, 0, SAMPLES_PER_FRAME);
	SDL_PauseAudio(0);
}

void audio_tick()
{
	if(!audio_enable) return;
	if(cycles % FRAME_SEQUENCER_TICKS < cycles_prev % FRAME_SEQUENCER_TICKS) audio_tick_frame_sequencer();
	if(cycles % CYCLES_PER_SAMPLE < cycles_prev % CYCLES_PER_SAMPLE)
	{
		if(audio_index != SAMPLES_PER_FRAME*2)
		{
			unsigned s1f = 131072 / (2048 - (SQUARE1_FREQ));
			unsigned s2f = 131072 / (2048 - (SQUARE2_FREQ));
			unsigned wf = 65536 / (2048 - (WAVE_FREQ));
			char sample = square1_enable ? audio_gen_square(SQUARE1_DUTY, s1f) : 0;
			sample = sample * (square1_volume / 16.0);
			char sample2 = square2_enable ? audio_gen_square(SQUARE2_DUTY, s2f) : 0;
			sample2 = sample2 * (square2_volume / 16.0);
			char wave_sample = wave_enable ? audio_gen_wave(wf) : 0;
			int mixed = sample + sample2 + wave_sample;
			if(mixed > 127) mixed = 127;
			if(mixed < -128) mixed = -128;
			mixed *= DEFAULT_VOLUME;
			audio_buffer[audio_index++] = (char) mixed;
			audio_buffer[audio_index++] = (char) mixed;
		}
		else
		{
			//TODO consider dropping frames if queue is too full
			if(SDL_GetQueuedAudioSize(1) <= SAMPLES_PER_FRAME*48) SDL_QueueAudio(1, audio_buffer, SAMPLES_PER_FRAME*2);
			audio_index = 0;
		}
	}
}

void audio_tick_frame_sequencer()
{
	static byte step = 0;
	switch(step)
	{
		case 2:
		case 6:
			update_sweep();
		case 0:
		case 4:
			update_length_counter();
			break;
		case 7:
			update_volume_envelope();
			break;
		default:
			break;
	}
	step = (step + 1) & 7;
}

audio_sample audio_gen_square(duty_cycle dc, unsigned freq)
{
	//unsigned frequency_cycles = cycles & 7;
	if(freq < 20) return 0;
	unsigned cycles_per_period = (Z80_CYCLES / freq);
	switch(dc)
	{
		case DUTY_CYCLE_12_5:
			if(cycles % cycles_per_period <= (cycles_per_period / 8)) return AS_HIGH;
			else return AS_LOW;
		case DUTY_CYCLE_25:
			if(cycles % cycles_per_period <= (cycles_per_period / 4)) return AS_HIGH;
			else return AS_LOW;
		case DUTY_CYCLE_50:
			if(cycles % cycles_per_period <= (cycles_per_period / 2)) return AS_HIGH;
			else return AS_LOW;
		case DUTY_CYCLE_75:
			if(cycles % cycles_per_period <= (cycles_per_period * 3) / 4) return AS_HIGH;
			else return AS_LOW;
	}
	return 0;
}

audio_sample audio_gen_wave(unsigned freq)
{
	static unsigned wave_index = 0;
	if(freq < 20) return 0;
	byte sample;
	unsigned cycles_per_sample = (Z80_CYCLES / freq);
	if(cycles % cycles_per_sample < cycles_prev % cycles_per_sample) wave_index++;
	if(wave_index == 64)
	{
		wave_enable = false;
		wave_index = 0;
	}
	else
	{
		sample = memory_get8((0xFF30 + (wave_index & 0xFE)));
		if(wave_index & 1) sample &= 0x0F;
		else sample >>= 4;
	}
	sample = (sample * 0x11) - 0x80;
	return *((char*)&sample);
}

void audio_memory_write(word address, byte value)
{
	if(address == NR_14)
	{
		if(value >> 7)
		{
			square1_enable = true;
			square1_length = 64 - SQUARE1_LENGTH_LOAD;
			square1_length_remain = square1_length;
			square1_volume = SQUARE1_VOLUME;
			square1_volume_time = SQUARE1_PERIOD;
			square1_sweep_time = SQUARE1_SWEEP_PERIOD;
			square1_shift = SQUARE1_SHIFT;
			square1_shadow = SQUARE1_FREQ;
		}
		else square1_enable = false;
	}
	else if(address == NR_24)
	{
		if(value >> 7)
		{
			square2_enable = true;
			square2_length = 64 - SQUARE2_LENGTH_LOAD;
			square2_length_remain = square2_length;
			square2_volume = SQUARE2_VOLUME;
			square2_volume_time =SQUARE2_PERIOD;
		}
		else square2_enable = false;
	}
	else if(address == NR_52) audio_enable = (value >> 7);
}

static inline void update_length_counter()
{
	//byte s1 = SQUARE1_LENGTH_LOAD;
	//byte s2 = SQUARE2_LENGTH_LOAD;
	if(!square1_length_remain)
	{
		//square1_length = 64 - SQUARE1_LENGTH_LOAD;
		//square1_length_remain = square1_length;
	}
	if(!square2_length_remain)
	{
		//square2_length = 64 - SQUARE2_LENGTH_LOAD;
		//square2_length_remain = square2_length;
	}
	if(square1_length_remain) square1_length_remain--;
	else if(SQUARE1_LENGTH_ENABLE) square1_enable = false;
	if(square2_length_remain) square2_length_remain--;
	else if(SQUARE2_LENGTH_ENABLE) square2_enable = false;
}

static inline void update_sweep()
{
	if(!square1_enable || !square1_sweep_time) return;
	bool negate = SQUARE1_NEGATE;
	word freq = square1_shadow;
	freq >>= square1_shift;
	freq = negate ? (square1_shadow - freq) : (square1_shadow + freq);
	if(freq > 2047) square1_enable = false;
	else if(square1_shift)
	{
		SQUARE1_SET_FREQ(freq);
		square1_shadow = freq;
		square1_sweep_time--;
		freq >>= square1_shift;
		freq = negate ? (square1_shadow - freq) : (square1_shadow + freq);
		if(freq > 2047) square1_enable = false;
	}
}

static inline void update_volume_envelope()
{
	static byte phase1 = 0, phase2 = 0;
	phase1++;
	phase2++;
	if(square1_volume_time == phase1)
	{
		phase1 = 0;
		//square1_volume_time--;
		if(SQUARE1_VOLUME_MODE && square1_volume < 0xF) square1_volume++;
		else if(!SQUARE1_VOLUME_MODE && square1_volume) square1_volume--;
	}
	//else square1_volume_time = SQUARE1_VOLUME_TIME;
	if(square2_volume_time == phase2)
	{
		phase2 = 0;
		//square2_volume_time--;
		if(SQUARE2_VOLUME_MODE && square2_volume < 0xF) square2_volume++;
		else if(!SQUARE2_VOLUME_MODE && square2_volume) square2_volume--;
	}
	//else square2_volume_time = SQUARE2_VOLUME_TIME;
}
