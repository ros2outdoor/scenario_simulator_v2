// Copyright 2015-2020 Autoware Foundation. All rights reserved.
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

#ifndef OPEN_SCENARIO_INTERPRETER__SYNTAX__CONDITION_GROUP_HPP_
#define OPEN_SCENARIO_INTERPRETER__SYNTAX__CONDITION_GROUP_HPP_

#include <open_scenario_interpreter/syntax/condition.hpp>

#include <vector>

namespace open_scenario_interpreter
{
inline namespace syntax
{
/* ==== ConditionGroup =======================================================
 *
 * A condition group is an association of conditions that is assessed during
 * simulation time and signals true when all associated conditions are
 * evaluated to true.
 *
 * <xsd:complexType name="ConditionGroup">
 *   <xsd:sequence>
 *     <xsd:element name="Condition" type="Condition" maxOccurs="unbounded"/>
 *   </xsd:sequence>
 * </xsd:complexType>
 *
 * ======================================================================== */
struct ConditionGroup
  : public std::vector<Condition>
{
  template<typename Node, typename Scope>
  explicit ConditionGroup(const Node & node, Scope & scope)
  {
    callWithElements(node, "Condition", 1, unbounded, [&](auto && node)
      {
        emplace_back(node, scope);
      });
  }

  auto evaluate() const
  {
    return
      asBoolean(
      std::all_of(std::begin(*this), std::end(*this), [&](auto && each)
      {
        return each.evaluate().template as<Boolean>(__FILE__, __LINE__);
      }));
  }
};
}
}  // namespace open_scenario_interpreter

#endif  // OPEN_SCENARIO_INTERPRETER__SYNTAX__CONDITION_GROUP_HPP_