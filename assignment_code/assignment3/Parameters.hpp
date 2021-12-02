#ifndef PARAMETERS_H
#define PARAMETERS_H

// Grid parameters
#define CELLS_Y    120
#define CELLS_X    120
#define num_cells (CELLS_Y * CELLS_X)

// Fluid parameters
#define VISCOSITY      0
#define DIFFUSION      0
#define DISSIPATION 0.02

// Simulation parameters
#define NUM_ITER              5
#define DT                  0.1
#define CLEANUP           false

// indexing function
inline int IndexOf(int y, int x) { return y * CELLS_X + x; }

#endif
