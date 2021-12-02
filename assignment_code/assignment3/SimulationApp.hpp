#ifndef SIMULATION_APP_H_
#define SIMULATION_APP_H_

#include "gloo/Application.hpp"


namespace GLOO {
class SimulationApp : public Application {
 public:
  SimulationApp(const std::string& app_name,
                glm::ivec2 window_size);
  void SetupScene() override;

 private:
  float num_particles_ = 300;

  int size_x_ = 30;
  int size_y_ = 30;

};
}  // namespace GLOO

#endif
