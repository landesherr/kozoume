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
bool noise_enable = false;

word square1_shadow = 0;
byte square1_sweep_time = 0;
byte square1_shift = 0;

byte square1_length = 0;
byte square1_length_remain = 0;
byte square2_length = 0;
byte square2_length_remain = 0;
byte wave_length = 0;
byte wave_length_remain = 0;
byte noise_length = 0;
byte noise_length_remain = 0;

byte square1_volume = 0xF;
byte square1_volume_time = 0;
byte square2_volume = 0xF;
byte square2_volume_time = 0;
byte noise_volume = 0;
byte noise_volume_time = 0;

byte volume_phase1 = 0;
byte sweep_phase = 0;
byte volume_phase2 = 0;
byte volume_phase_noise = 0;

word noise_lfsr = 0;
bool noise_mode = 0;

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
			unsigned nf;
			if(!NOISE_DIV_RATIO) nf = (Z80_CYCLES / 4) / (2 << NOISE_SHIFT_CLOCK);
			else nf = ((Z80_CYCLES / 8) / NOISE_DIV_RATIO) / (2 << NOISE_SHIFT_CLOCK);
			audio_sample sample = square1_enable ? audio_gen_square(SQUARE1_DUTY, s1f) : 0;
			sample = sample * (square1_volume / 16.0);
			audio_sample sample2 = square2_enable ? audio_gen_square(SQUARE2_DUTY, s2f) : 0;
			sample2 = sample2 * (square2_volume / 16.0);
			audio_sample wave_sample = wave_enable ? audio_gen_wave(wf) : 0;
			audio_sample noise_sample = noise_enable ? audio_gen_noise(nf) : 0;
			noise_sample = noise_sample * (noise_volume / 16.0);
			int mixed = sample + sample2 + wave_sample + noise_sample;
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

static word quasirandom()
{
	static byte s = 0x4B, t = 0x7A;
	word qr = 0;
	s = (5 * s) + 1;
	t = (((t >> 4) & 1) == ((t >> 7) * 1)) ? (t << 1) + 1 : t << 1;
	qr = (s ^ t);
	s = (5 * s) + 1;
	t = (((t >> 4) & 1) == ((t >> 7) * 1)) ? (t << 1) + 1 : t << 1;
	qr |= ((s ^ t) << 8);
	return qr;
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
		//wave_enable = false;
		wave_index = 0;
	}
	sample = memory_get8((0xFF30 + (wave_index & 0xFE)));
	if(wave_index & 1) sample &= 0x0F;
	else sample >>= 4;
	sample = (sample * 0x11) - 0x80;
	return *((char*)&sample);
}

audio_sample audio_gen_noise(unsigned freq)
{
	if(freq < 20) return 0;
	unsigned cycles_per_period = (Z80_CYCLES / freq);
	audio_sample output = (noise_lfsr & 1) ? AS_LOW : AS_HIGH;
	if(cycles % cycles_per_period < cycles_prev % cycles_per_period)
	{
		bool xor_lower_two = ((noise_lfsr >> 1) ^ noise_lfsr) & 1;
		noise_lfsr >>= 1;
		noise_lfsr = (noise_lfsr & 0x3FFF) | (xor_lower_two << 14);
		if(noise_mode == 1) noise_lfsr = (noise_lfsr & 0x3F) | (xor_lower_two << 6);
	}
	//return (noise_lfsr & 1) ? AS_LOW : AS_HIGH;
	return output;
}

void audio_memory_write(word address, byte value)
{
	if(address == NR_10)
	{
		square1_shift = value & 0x7;;
		square1_sweep_time = (value >> 4) & 0x7;
	}
	if(address == NR_14)
	{
		if(value >> 7)
		{
			square1_enable = true;
			square1_length = 64 - SQUARE1_LENGTH_LOAD;
			square1_length_remain = square1_length;
			square1_volume = SQUARE1_VOLUME;
			square1_volume_time = SQUARE1_PERIOD;
			volume_phase1 = 0;
			square1_sweep_time = SQUARE1_SWEEP_PERIOD;
			square1_shift = SQUARE1_SHIFT;
			square1_shadow = SQUARE1_FREQ;
			sweep_phase = 0;
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
			volume_phase2 = 0;
		}
		else square2_enable = false;
	}
	else if(address == NR_34)
	{
		if(value >> 7)
		{
			wave_enable = true;
			wave_length = 255 - WAVE_LENGTH;
		}
		else wave_enable = false;
	}
	else if(address == NR_44)
	{
		if(value >> 7)
		{
			noise_enable = true;
			noise_mode = NOISE_STEP_TYPE;
			if(!noise_lfsr) noise_lfsr = quasirandom() & 0x7FFF;
			noise_length = 64 - NOISE_LENGTH_LOAD;
			noise_length_remain = noise_length;
			noise_volume = NOISE_VOLUME;
			noise_volume_time = NOISE_PERIOD;
			volume_phase_noise = 0;
		}
		else noise_enable = false;
	}
	else if(address == NR_52) audio_enable = (value >> 7);
}

static inline void update_length_counter()
{
	if(square1_length_remain) square1_length_remain--;
	else if(SQUARE1_LENGTH_ENABLE) square1_enable = false;
	if(square2_length_remain) square2_length_remain--;
	else if(SQUARE2_LENGTH_ENABLE) square2_enable = false;
	if(wave_length_remain) wave_length_remain--;
	else if(WAVE_LENGTH_ENABLE) wave_enable = false;
	if(noise_length_remain) noise_length_remain--;
	else if(NOISE_LENGTH_ENABLE) noise_enable = false;
}

static inline void update_sweep()
{
	if(!square1_enable || !square1_sweep_time) return;
	if(++sweep_phase != square1_sweep_time) return;
	word freq = square1_shadow >> square1_shift;
	bool negate = SQUARE1_NEGATE;
	freq = negate ? (square1_shadow - freq) : (square1_shadow + freq);
	if(freq > 2047 || !square1_shift) square1_enable = false;
	else
	{
		sweep_phase = 0;
		SQUARE1_SET_FREQ(freq);
		square1_shadow = freq;
		freq >>= square1_shift;
		freq = negate ? (square1_shadow - freq) : (square1_shadow + freq);
		if(freq > 2047) square1_enable = false;
	}
}

static inline void update_volume_envelope()
{
	volume_phase1++;
	volume_phase2++;
	volume_phase_noise++;
	if(square1_volume_time == volume_phase1)
	{
		volume_phase1 = 0;
		if(SQUARE1_VOLUME_MODE && square1_volume < 0xF) square1_volume++;
		else if(!SQUARE1_VOLUME_MODE && square1_volume) square1_volume--;
	}
	if(square2_volume_time == volume_phase2)
	{
		volume_phase2 = 0;
		if(SQUARE2_VOLUME_MODE && square2_volume < 0xF) square2_volume++;
		else if(!SQUARE2_VOLUME_MODE && square2_volume) square2_volume--;
	}
	if(noise_volume_time == volume_phase_noise)
	{
		volume_phase_noise = 0;
		if(NOISE_VOLUME_MODE && noise_volume < 0xF) noise_volume++;
		else if(!NOISE_VOLUME_MODE && noise_volume) noise_volume--;
	}
}
