#include "game_of_life.h"

// #include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/* This stores an array of 3-bit counters *vertically* */
struct set_of_counters {
	swar L, M, H; // 3-bit saturating counters, one per *bit*
};

swar data_tmp[GameOfLifeHeight];

/* Add one bit from N to each of our counters */
void add(struct set_of_counters * s, long N) {
	// low bit half adder
	swar Lcarry=s->L&N;
	s->L=s->L^N;
	
	// middle bit half adder
	swar Mcarry=s->M&Lcarry;
	s->M=s->M^Lcarry;
	
	// last bit saturates
	s->H=s->H|Mcarry; 
}

/*cf
 Run the rules for the game of life on these three rows.
 prev is the row above you, next is the row below you,
 cur is your current row.

 A 1 bit indicates a living cell.
*/
swar run_game(swar prev,swar cur,swar next) {
// Each counter stores the number of neighbors around this cell
	struct set_of_counters c = {0,0,0};

// Add all eigDisplayWidth neighbors to our counts
	add(&c, prev>>1);	add(&c, prev); add(&c, prev<<1);
	add(&c, cur>>1);               add(&c, cur<<1);
	add(&c, next>>1); add(&c, next); add(&c, next<<1);

// Run the rules on the resulting counts
	long are2=(~c.H)&c.M&(~c.L); // 2==010
	long are3=(~c.H)&c.M&c.L; // 3==011
	return (are2&cur)|are3; // if 2, unchanged.  If 3, alive.
}

// Create random initial conditions
void gol_init(struct gol_grid * g) {
	srand(g->seed);
	swar mask=1; mask=~mask; 
	mask=mask<<1; mask=mask>>1;// knock off high bit
	for (int y=0;y<GameOfLifeHeight;++y) g->data[y]=rand()&mask;
	g->data[0]=g->data[GameOfLifeHeight-1]=0; // clear top and bottom rows
	g->iter = GameOfLifeIterMax;
}

// Run one game of life update, writing new cells into dest.
void gol_update(struct gol_grid * g, const char seed) {
	if(seed)
		g->seed = seed;
	if(--g->iter == 0)
		gol_init(g);

	// if(g->iter % 10l)
	for (int y=1;y<GameOfLifeHeight-1;++y) {
		data_tmp[y]=run_game(g->data[y-1],g->data[y],g->data[y+1]);
	}

	// swap(g, dest);
	memcpy(g->data, data_tmp, sizeof(g->data));
}
