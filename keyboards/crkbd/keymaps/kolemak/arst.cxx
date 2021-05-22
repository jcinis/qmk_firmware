/* Conway's Game of Life using SIMD bitwise operations 
Dr. Orion Lawlor, lawlor@alaska.edu, 2011-10-18 (Public Domain)
*/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

/* This treats an integer as an array of bits: SIMD Within A Register */
typedef unsigned long swar; 

/* This stores an array of 3-bit counters *vertically* */
struct set_of_counters {
	swar L, M, H; // 3-bit saturating counters, one per *bit*
	set_of_counters() {L=M=H=0;}

	/* Add one bit from N to each of our counters */
	void add(long N) {
		// low bit half adder
		swar Lcarry=L&N;
		L=L^N;
		
		// middle bit half adder
		swar Mcarry=M&Lcarry;
		M=M^Lcarry;
		
		// last bit saturates
		H=H|Mcarry; 
	}
};
/*cf
 Run the rules for the game of life on these three rows.
 prev is the row above you, next is the row below you,
 cur is your current row.

 A 1 bit indicates a living cell.
*/
swar run_game(swar prev,swar cur,swar next) {
// Each counter stores the number of neighbors around this cell
	set_of_counters c;

// Add all eight neighbors to our counts
	c.add(prev>>1);	c.add(prev); c.add(prev<<1);
	c.add(cur>>1);               c.add(cur<<1);
	c.add(next>>1); c.add(next); c.add(next<<1);

// Run the rules on the resulting counts
	long are2=(~c.H)&c.M&(~c.L); // 2==010
	long are3=(~c.H)&c.M&c.L; // 3==011
	return (are2&cur)|are3; // if 2, unchanged.  If 3, alive.
}

// A 2D grid containing game of life cells.
struct grid {
	enum {ht=128};
	swar data[ht];
	
	// Create random initial conditions
	void randomize(int seed=1) {
		srand(seed);
		swar mask=1; mask=~mask; 
		mask=mask<<1; mask=mask>>1;// knock off high bit
		for (int y=0;y<ht;y++) data[y]=rand()&mask;
		data[0]=data[ht-1]=0; // clear top and bottom rows
	}
	
	// Run one game of life update, writing new cells into dest.
	void update(grid &dest) const {
		for (int y=1;y<ht-1;y++) {
			dest.data[y]=run_game(data[y-1],data[y],data[y+1]);
		}
	}

	// Print the grid onscreen
	void print(void) {
		for (int y=1;y<ht-1;y++) {
			for (unsigned int x=0;x<8*sizeof(swar);x++) {
				int bit=(data[y]>>x)&1;
				printf(bit ? "X" : " ");
			}
			printf("\n");
		}
	}
};	

void swap(grid *a, grid *b)
{
   grid t;
 
   t  = *b;
   *b = *a;
   *a = t;
}

int main(void) {
	grid a,b;
	a.randomize(1);

// Run time loop 
	for (int step=0;step<200;step++) {
		printf("\x1B[2J\x1B[H");
		printf("\n");
		a.print();

		a.update(b);
		swap(&a,&b);
		usleep(100000);
	}
	return 0;
}