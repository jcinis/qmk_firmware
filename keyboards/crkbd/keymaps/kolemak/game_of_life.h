#pragma once

#ifndef GameOfLifeHeight
//#ifdef RGB_MATRIX_SPLIT_RIGHT
#define GameOfLifeHeight 126
//#else
//#define GameOfLifeHeight 3
// #endif

#define GameOfLifeIterMax 200
#define GameOfLifeUpdateSteps 10
#endif

/* This treats an integer as an array of bits: SIMD Within A Register */
typedef unsigned long swar; 

// A 2D grid containing game of life cells.
struct gol_grid {
	swar data[GameOfLifeHeight];
	unsigned int iter;
	char seed;
};

struct gol_grid game_of_life_grid;

void gol_init(struct gol_grid * g);
void gol_update(struct gol_grid * g, const char seed);