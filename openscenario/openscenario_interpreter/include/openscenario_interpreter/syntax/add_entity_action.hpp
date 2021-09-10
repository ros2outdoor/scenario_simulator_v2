// Copyright 2015-2020 Tier IV, Inc. All rights reserved.
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

#ifndef OPENSCENARIO_INTERPRETER__SYNTAX__ADD_ENTITY_ACTION_HPP_
#define OPENSCENARIO_INTERPRETER__SYNTAX__ADD_ENTITY_ACTION_HPP_

#include <openscenario_interpreter/reader/element.hpp>
#include <openscenario_interpreter/scope.hpp>
#include <openscenario_interpreter/syntax/position.hpp>
#include <openscenario_interpreter/syntax/scenario_object.hpp>

namespace openscenario_interpreter
{
inline namespace syntax
{
/* ---- AddEntityAction --------------------------------------------------------
 *
 *  <xsd:complexType name="AddEntityAction">
 *    <xsd:all>
 *      <xsd:element name="Position" type="Position"/>
 *    </xsd:all>
 *  </xsd:complexType>
 *
 * -------------------------------------------------------------------------- */
struct AddEntityAction : private Scope
{
  const Position position;

  explicit AddEntityAction(const Scope & scope, const Position & position)
  : Scope(scope), position(position)
  {
  }

  template <typename Tree>
  explicit AddEntityAction(const Tree & tree, Scope & scope)
  : Scope(scope), position(readElement<Position>("Position", tree, scope))
  {
  }

  static auto accomplished() noexcept -> bool { return endsImmediately(); }

  static auto endsImmediately() noexcept -> bool { return true; }

  inline auto operator()(const std::string & entity_ref) const -> void
  try {
    const auto entity = global().entities.at(entity_ref);  // TODO: catch

    auto add_entity_action = overload(

      [&](const Vehicle & vehicle) {
        if (not applyAddEntityAction(
              entity.as<ScenarioObject>().object_controller.isEgo(),  //
              entity_ref,                                             //
              static_cast<openscenario_msgs::msg::VehicleParameters>(vehicle))) {
          return false;
        } else {
          applyAssignControllerAction(entity_ref, entity.as<ScenarioObject>().object_controller);
          entity.as<ScenarioObject>().activateSensors();
          entity.as<ScenarioObject>().activateOutOfRangeMetric(vehicle);
          return true;
        }
      },

      [&](const Pedestrian & pedestrian) {
        return applyAddEntityAction(
          false, entity_ref, static_cast<openscenario_msgs::msg::PedestrianParameters>(pedestrian));
      },

      [&](const MiscObject & misc_object) {
        return applyAddEntityAction(
          false, entity_ref,
          static_cast<openscenario_msgs::msg::MiscObjectParameters>(misc_object));
      });

    if (not std::exchange(entity.as<ScenarioObject>().is_added, true)) {
      // TODO bool => void
      apply<bool>(add_entity_action, entity.as<EntityObject>());
    }
  } catch (const std::out_of_range &) {
    throw SemanticError("No such name of entity ", std::quoted(entity_ref));
  }
};
}  // namespace syntax
}  // namespace openscenario_interpreter

#endif  // OPENSCENARIO_INTERPRETER__SYNTAX__ADD_ENTITY_ACTION_HPP_
