// Copyright 2015 TIER IV, Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <quaternion_operation/quaternion_operation.h>

#include <ament_index_cpp/get_package_share_directory.hpp>
#include <cpp_mock_scenarios/catalogs.hpp>
#include <cpp_mock_scenarios/cpp_scenario_node.hpp>
#include <rclcpp/rclcpp.hpp>
#include <traffic_simulator/api/api.hpp>
#include <traffic_simulator_msgs/msg/behavior_parameter.hpp>

// headers in STL
#include <memory>
#include <string>
#include <vector>

namespace cpp_mock_scenarios
{
class SynchronizedAction : public cpp_mock_scenarios::CppScenarioNode
{
public:
  explicit SynchronizedAction(const rclcpp::NodeOptions & option)
  : cpp_mock_scenarios::CppScenarioNode(
      "synchronized_action",
      ament_index_cpp::get_package_share_directory("kashiwanoha_map") + "/map", "lanelet2_map.osm",
      __FILE__, false, option)
  {
    start();
  }

private:
  bool requested = false;
  const traffic_simulator::CanonicalizedLaneletPose ego_target =
    getSampleLaneletPose(traffic_simulator::helper::constructLaneletPose(34510, 0, 0, 0, 0, 0));
  const traffic_simulator::CanonicalizedLaneletPose npc_target =
    getSampleLaneletPose(traffic_simulator::helper::constructLaneletPose(34495, 15, 0, 0, 0, 0));

  void onUpdate() override
  {
    // SUCCESS
    // check if npc is in the target lanelet when ego is in the target lanelet and npc is stopped
    if (
      api_.requestSynchronize("npc", ego_target, npc_target, 0.2) &&
      // api_.getIfArrivedToTargetLaneletPose("ego", ego_target, 0.2) &&
      // api_.getCurrentTwist("npc").linear.x == 0.0) {
      true) {
      stop(cpp_mock_scenarios::Result::SUCCESS);
    }

    // FAILURES
    if (api_.getCurrentTime() >= 20.0) {
      stop(cpp_mock_scenarios::Result::FAILURE);
    }
    if (api_.checkCollision("ego", "npc")) {
      stop(cpp_mock_scenarios::Result::FAILURE);
    }

  }
  void onInitialize() override
  {
    api_.spawn(
      "ego",
      api_.canonicalize(traffic_simulator::helper::constructLaneletPose(34513, 15, 0, 0, 0, 0)),
      getVehicleParameters());
    api_.setLinearVelocity("ego", 5);
    // api_.requestSpeedChange("ego", 5, true);
    // api_.requestLaneChange("ego", 34513);

    api_.spawn(
      "npc",
      api_.canonicalize(traffic_simulator::helper::constructLaneletPose(34564, 5, 0, 0, 0, 0)),
      getVehicleParameters());
    api_.setLinearVelocity("npc", 5);
  }

  auto getSampleLaneletPose(const traffic_simulator::LaneletPose & lanelet_pose)
    -> traffic_simulator::CanonicalizedLaneletPose
  {
    return api_.canonicalize(lanelet_pose);
  }
};
}  // namespace cpp_mock_scenarios

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::NodeOptions options;
  auto component = std::make_shared<cpp_mock_scenarios::SynchronizedAction>(options);
  rclcpp::spin(component);
  rclcpp::shutdown();
  return 0;
}
