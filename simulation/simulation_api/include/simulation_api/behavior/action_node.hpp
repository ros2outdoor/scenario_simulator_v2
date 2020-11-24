// Copyright 2015-2020 TierIV.inc. All rights reserved.
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

#ifndef SIMULATION_API__BEHAVIOR__ACTION_NODE_HPP_
#define SIMULATION_API__BEHAVIOR__ACTION_NODE_HPP_

#include <behaviortree_cpp_v3/action_node.h>
#include <simulation_api/hdmap_utils/hdmap_utils.hpp>
#include <simulation_api/entity/entity_base.hpp>

#include <boost/algorithm/clamp.hpp>

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>

namespace entity_behavior
{
class BehaviorTreeRuntimeError : public std::runtime_error
{
public:
  explicit BehaviorTreeRuntimeError(const char * message)
  : runtime_error(message) {}
};

class ActionNode : public BT::ActionNodeBase
{
public:
  ActionNode(const std::string & name, const BT::NodeConfiguration & config);
  ~ActionNode() override = default;
  bool foundConflictingEntity(const std::vector<std::int64_t> & following_lanelets) const;
  boost::optional<simulation_api::entity::EntityStatus> getConflictingEntityStatus(
    const std::vector<std::int64_t> & following_lanelets) const;
  boost::optional<double> getDistanceToConflictingEntity(
    const std::vector<std::int64_t> & following_lanelets) const;
  boost::optional<simulation_api::entity::EntityStatus> getFrontEntityStatus();
  double calculateStopDistance() const;
  boost::optional<double> getDistanceToFrontEntity();
  boost::optional<double> getDistanceToStopLine(
    const std::vector<std::int64_t> & following_lanelets);
  std::vector<simulation_api::entity::EntityStatus> getRightOfWayEntities();
  std::vector<simulation_api::entity::EntityStatus> getRightOfWayEntities(
    const std::vector<std::int64_t> & following_lanelets);
  boost::optional<double> getYieldStopDistance(
    const std::vector<std::int64_t> & following_lanelets);

  /// throws if the derived class return RUNNING.
  BT::NodeStatus executeTick() override;

  /// You don't need to override this
  void halt() override
  {
    setStatus(BT::NodeStatus::IDLE);
  }

  static BT::PortsList providedPorts()
  {
    return
      {
        BT::InputPort<std::string>("request"),
        BT::InputPort<std::shared_ptr<hdmap_utils::HdMapUtils>>("hdmap_utils"),
        BT::InputPort<simulation_api::entity::EntityStatus>("entity_status"),
        BT::InputPort<double>("current_time"),
        BT::InputPort<double>("step_time"),
        BT::InputPort<boost::optional<double>>("target_speed"),
        BT::OutputPort<simulation_api::entity::EntityStatus>("updated_status"),
        BT::OutputPort<std::string>("request"),
        BT::InputPort<std::unordered_map<std::string, simulation_api::entity::EntityStatus>>(
          "other_entity_status"),
        BT::InputPort<std::unordered_map<std::string, simulation_api::entity::EntityType>>(
          "entity_type_list")
      };
  }
  void getBlackBoardValues();
  std::string request;
  std::shared_ptr<hdmap_utils::HdMapUtils> hdmap_utils;
  simulation_api::entity::EntityStatus entity_status;
  double current_time;
  double step_time;
  boost::optional<double> target_speed;
  simulation_api::entity::EntityStatus updated_status;
  std::unordered_map<std::string, simulation_api::entity::EntityStatus> other_entity_status;
  std::unordered_map<std::string, simulation_api::entity::EntityType> entity_type_list;
};
}  // namespace entity_behavior

#endif  // SIMULATION_API__BEHAVIOR__ACTION_NODE_HPP_
