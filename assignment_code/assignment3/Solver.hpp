// #ifndef SOLVER_H_
// #define SOLVER_H_
//
// #include "Parameters.hpp"
// #include <iostream>
//
// namespace GLOO {
// class Solver {
// public:
//   Solver();
//   void v_step(float U1_y, float U1_x, float U0_y, float U0_x);
//   void s_step(float S1, float S0, float U1);
//
//   void negate_field(std::vector<float> field){
//     for (int i=0; i<num_cells; i++) {field[i] = -field[i];}
//   }
//
//   void set_boundary_values(std::vector<float> field, int key){
//     switch (key) {
//       case 1:
//         // vertical velocity
//         for (int y = 1; y < CELLS_Y - 1; y++) {
//             field[IndexOf(y, 0)] = field[IndexOf(y, 1)];
//             field[IndexOf(y, CELLS_X - 1)] = field[IndexOf(y, CELLS_X - 2)];
//         }
//         for (int x = 1; x < CELLS_X - 1; x++) {
//             field[IndexOf(0, x)] = -field[IndexOf(1, x)];
//             field[IndexOf(CELLS_Y - 1, x)] = -field[IndexOf(CELLS_Y - 2, x)];
//         }
//         break;
//       case 2:
//         // horizontal velocity
//         for (int y = 1; y < CELLS_Y - 1; ++y) {
//             field[IndexOf(y, 0)] = -field[IndexOf(y, 1)];
//             field[IndexOf(y, CELLS_X - 1)] = -field[IndexOf(y, CELLS_X - 2)];
//         }
//         for (int x = 1; x < CELLS_X - 1; ++x) {
//             field[IndexOf(0, x)] = field[IndexOf(1, x)];
//             field[IndexOf(CELLS_Y - 1, x)] = field[IndexOf(CELLS_Y - 2, x)];
//         }
//         break;
//       case 3:
//         // scalar
//         for (int y=1; y<CELLS_Y-1; y++){
//           field[IndexOf(y,0)] = field[IndexOf(y,1)];
//           field[IndexOf(y, CELLS_X - 1)] = field[IndexOf(y, CELLS_X - 2)];
//         }
//         for (int x = 1; x < CELLS_X - 1; x++) {
//           field[IndexOf(0, x)] = field[IndexOf(1, x)];
//           field[IndexOf(CELLS_Y - 1, x)] = field[IndexOf(CELLS_Y - 2, x)];
//         }
//         break;
//     }
//
//     // corner values
//     field[IndexOf(0, 0)] = (field[IndexOf(0, 1)] + field[IndexOf(1, 0)]) / 2.0f;
//     field[IndexOf(0, CELLS_X - 1)] = (field[IndexOf(0, CELLS_X - 2)] + field[IndexOf(1, CELLS_X - 1)]) / 2.0f;
//     field[IndexOf(CELLS_Y - 1, 0)] = (field[IndexOf(CELLS_Y - 1, 1)] + field[IndexOf(CELLS_Y - 2, 0)]) / 2.0f;
//     field[IndexOf(CELLS_Y - 1, CELLS_X - 1)] = (field[IndexOf(CELLS_Y - 1, CELLS_X - 2)] + field[IndexOf(CELLS_Y - 2, CELLS_X - 1)]) / 2.0f;
//   }
//
//   void add_force(std::vector<float> field, std::vector<float> force, int key){
//     for (int y = 1; y < CELLS_Y - 1; y++) {
//         for (int x = 1; x < CELLS_X - 1; x++) {
//             field[IndexOf(y, x)] += force[IndexOf(y, x)];
//         }
//     }
//     set_boundary_values(field, key);
//   }
//
//   float lin_interp(float y, float x, std::vector<float> field){
//     int yfloor = floor(y);
//     int xfloor = floor(x);
//
//     float ydiff = (y - 0.5f) - (float) yfloor;
//     float xdiff = (x - 0.5f) - (float) xfloor;
//
//     float tl = field[IndexOf(yfloor, xfloor)];
//     float bl = field[IndexOf(yfloor + 1, xfloor)];
//     float tr = field[IndexOf(yfloor, xfloor + 1)];
//     float br = field[IndexOf(yfloor + 1, xfloor + 1)];
//
//     float vl = (1.0f - ydiff) * tl + ydiff * bl;
//     float vr = (1.0f - ydiff) * tr + ydiff * br;
//
//     return (1.0f - xdiff) * vl + xdiff * vr;
//   }
//
//   void transport(std::vector<float> S1, std::vector<float> S0, std::vector<float> U_y, std::vector<float> U_x, int key){
//     for (int y = 1; y < CELLS_Y - 1; y++) {
//         for (int x = 1; x < CELLS_X - 1; x++) {
//             // trace particle
//             float y0 = ((float) y + 0.5f) - DT * U_y[IndexOf(y, x)];
//             float x0 = ((float) x + 0.5f) - DT * U_x[IndexOf(y, x)];
//
//             y0 = fmax(1.0f, fmin(((float) CELLS_Y) - 2.0f, y0));
//             x0 = fmax(1.0f, fmin(((float) CELLS_X) - 2.0f, x0));
//
//             S1[IndexOf(y, x)] = lin_interp(y0, x0, S0);
//         }
//     }
//     set_boundary_values(S1, key);
//   }
//
//   void lin_solve(std::vector<float> S1, std::vector<float> S0, float a, float b, int key) {
//     for (int i = 0; i < NUM_ITER; i++) {
//         for (int y = 1; y < CELLS_Y - 1; y++) {
//             for (int x = 1; x < CELLS_X - 1; x++) {
//                 S1[IndexOf(y, x)] = (S0[IndexOf(y, x)]
//                         + a * (S1[IndexOf(y + 1, x)] + S1[IndexOf(y - 1, x)]
//                              + S1[IndexOf(y, x + 1)] + S1[IndexOf(y, x - 1)])) / b;
//             }
//         }
//         set_boundary_values(S1, key);
//     }
//   }
//
//   void diffuse(std::vector<float> S1, std::vector<float> S0, float diff, int key) {
//     float a = DT * diff * num_cells;
//     lin_solve(S1, S0, a, 1.0f + 4.0f * a, key);
//   }
//
//   void project(std::vector<float> U1_y, std::vector<float> U1_x, std::vector<float> U0_y, std::vector<float> U0_x) {
//       // construct initial guess for the solution
//       std::vector<float> S(num_cells, 0.f);
//
//       // compute the divergence of the velocity field
//       std::vector<float> divergence(num_cells, 0.f);
//       for (int y = 1; y < CELLS_Y - 1; y++) {
//           for (int x = 1; x < CELLS_X - 1; x++) {
//               divergence[IndexOf(y, x)] = U0_y[IndexOf(y + 1, x)] - U0_y[IndexOf(y - 1, x)]
//                                       + U0_x[IndexOf(y, x + 1)] - U0_x[IndexOf(y, x - 1)];
//           }
//       }
//       set_boundary_values(divergence, 0);
//
//       // solve the Poisson equation
//       negate_field(divergence);
//       lin_solve(S, divergence, 1.0f, 4.0f, 0);
//
//       // subtract the gradient from the previous solution
//       for (int y = 1; y < CELLS_Y - 1; y++) {
//           for (int x = 1; x < CELLS_X - 1; x++) {
//               U1_y[IndexOf(y, x)] = U0_y[IndexOf(y, x)] - (S[IndexOf(y + 1, x)] - S[IndexOf(y - 1, x)]) / 2.0f;
//               U1_x[IndexOf(y, x)] = U0_x[IndexOf(y, x)] - (S[IndexOf(y, x + 1)] - S[IndexOf(y, x - 1)]) / 2.0f;
//           }
//       }
//       set_boundary_values(U1_y, 1);
//       set_boundary_values(U1_x, 2);
//   }
//
//   void dissipate(std::vector<float> S1, std::vector<float> S0) {
//       for (int i = 0; i < num_cells; i++) {
//           S1[i] = S0[i] / (1.0f + DT * DISSIPATION);
//       }
//   }
//
//   float curl(int y, int x, std::vector<float> U_y, std::vector<float> U_x) {
//     return (U_y[IndexOf(y, x + 1)] - U_y[IndexOf(y, x - 1)]
//             - U_x[IndexOf(y + 1, x)] + U_x[IndexOf(y - 1, x)]) / 2.0f;
//   }
//
// };
// }  // namespace GLOO
//
// #endif
