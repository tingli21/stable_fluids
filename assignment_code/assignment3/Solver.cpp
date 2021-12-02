// #include "Solver.hpp"
// #include <fstream>
//
// #include "gloo/utils.hpp"
// #include "gloo/components/RenderingComponent.hpp"
// #include "gloo/debug/PrimitiveFactory.hpp"
// #include "glm/gtx/string_cast.hpp"
//
// namespace GLOO {
// void Solver::v_step(float U1_y, float U1_x, float U0_y, float U0_x) {
//   set_boundary_values(U1_y, 1);
//   set_boundary_values(U1_x, 1);
//
//   // diffuse
//   if (VISCOSITY > 0.f) {
//     std::swap(U0_y, U1_y);
//     std::swap(U0_x, U1_x);
//     diffuse(U1_y, U0_y, VISCOSITY, 1);
//     diffuse(U1_x, U0_x, VISCOSITY, 2);
//   }
//
//   // pressure correction 1
//   project(U0_y, U0_x, U1_y, U1_x);
//
//   // advect
//   transport(U1_y, U0_y, U0_y, U0_x, 1);
//   transport(U1_x, U0_x, U0_y, U0_x, 2);
// 
//   // pressure ccorrection 2
//   project(U0_y, U0_x, U1_y, U1_x);
// }
//
// void Solver::s_step(float S1, float S0, float U1){
//   // advect according to velocity field
//   transport(S0, S1, U_y, U_x, 0);
//
//   // diffuse
//   if (DIFFUSION > 0.0f) {
//       std::swap(S1, S0);
//       diffuse(S0, S1, DIFFUSION, 0);
//   }
//
//   // dissipate
//   std::swap(S1, S0);
//   dissipate(S0, S1);
// }
//
//
// }  // namespace GLOO
