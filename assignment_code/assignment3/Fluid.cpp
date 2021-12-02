#include "Fluid.hpp"
// #include "Solver.hpp"
#include "Parameters.hpp"
#include <vector>

namespace GLOO {

Fluid::Fluid(){
  for (int i=0; i<num_cells; i++){
    U0_y.push_back(0.f);
    U0_x.push_back(0.f);
    U1_y.push_back(0.f);
    U1_x.push_back(0.f);
    S0.push_back(0.f);
    S1.push_back(0.f);
  }
}

void Fluid::Render(){
  
}

void Fluid::swap_grids() {
    std::vector<float> temp;
    temp = U0_y; U0_y = U1_y; U1_y = temp;
    temp = U0_x; U0_x = U1_x; U1_x = temp;
    S1 = S0;
}

void Fluid::step() {
  v_step(U1_y, U1_x, U0_y, U0_x);
  s_step(S1, S0, U0_y, U0_x);
  swap_grids();
}

void Fluid::add_U_y_force_at(int y, int x, float force) {
    if (y > 0 && y < CELLS_Y - 1 && x > 0 && x < CELLS_X - 1) {
        U1_y[IndexOf(y, x)] += force;
    }
}

void Fluid::add_U_x_force_at(int y, int x, float force) {
    if (y > 0 && y < CELLS_Y - 1 && x > 0 && x < CELLS_X - 1) {
        U1_x[IndexOf(y, x)] += force;
    }
}

void Fluid::add_source_at(int y, int x, float source) {
    if (y > 0 && y < CELLS_Y - 1 && x > 0 && x < CELLS_X - 1) {
        S1[IndexOf(y, x)] += source;
    }
}

float Fluid::Uy_at(int y, int x) {
    return U1_y[IndexOf(y, x)];
}

float Fluid::Ux_at(int y, int x) {
    return U1_x[IndexOf(y, x)];
}

float Fluid::S_at(int y, int x) {
    return S1[IndexOf(y, x)];
}

void Fluid::v_step(std::vector<float> U1_y, std::vector<float> U1_x, std::vector<float> U0_y, std::vector<float> U0_x) {
  set_boundary_values(U1_y, 1);
  set_boundary_values(U1_x, 1);

  // diffuse
  if (VISCOSITY > 0.f) {
    std::swap(U0_y, U1_y);
    std::swap(U0_x, U1_x);
    diffuse(U1_y, U0_y, VISCOSITY, 1);
    diffuse(U1_x, U0_x, VISCOSITY, 2);
  }

  // pressure correction 1
  project(U0_y, U0_x, U1_y, U1_x);

  // advect
  transport(U1_y, U0_y, U0_y, U0_x, 1);
  transport(U1_x, U0_x, U0_y, U0_x, 2);

  // pressure ccorrection 2
  project(U0_y, U0_x, U1_y, U1_x);
}

void Fluid::s_step(std::vector<float> S1, std::vector<float> S0, std::vector<float> U_y, std::vector<float> U_x){
  // advect according to velocity field
  transport(S0, S1, U_y, U_x, 0);

  // diffuse
  if (DIFFUSION > 0.0f) {
      std::swap(S1, S0);
      diffuse(S0, S1, DIFFUSION, 0);
  }

  // dissipate
  std::swap(S1, S0);
  dissipate(S0, S1);
}



}
