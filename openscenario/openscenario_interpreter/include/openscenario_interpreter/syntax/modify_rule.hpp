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

#ifndef OPENSCENARIO_INTERPRETER__SYNTAX__MODIFY_RULE_HPP_
#define OPENSCENARIO_INTERPRETER__SYNTAX__MODIFY_RULE_HPP_

#include <openscenario_interpreter/reader/element.hpp>
#include <openscenario_interpreter/syntax/parameter_add_value_rule.hpp>
#include <openscenario_interpreter/syntax/parameter_multiply_by_value_rule.hpp>
#include <utility>

namespace openscenario_interpreter
{
inline namespace syntax
{
/* ---- ModifyRule -------------------------------------------------------------
 *
 * <xsd:complexType name="ModifyRule">
 *   <xsd:choice>
 *     <xsd:element name="AddValue" type="ParameterAddValueRule"/>
 *     <xsd:element name="MultiplyByValue" type="ParameterMultiplyByValueRule"/>
 *   </xsd:choice>
 * </xsd:complexType>
 *
 * -------------------------------------------------------------------------- */
#define ELEMENT(TYPE)                                                            \
  std::make_pair(#TYPE, [&](auto && node) {                                      \
    return make<Parameter##TYPE##Rule>(node, std::forward<decltype(xs)>(xs)...); \
  })

struct ModifyRule : public Element
{
  template <typename Node, typename... Ts>
  explicit ModifyRule(const Node & node, Ts &&... xs)
  : Element(choice(node, ELEMENT(AddValue), ELEMENT(MultiplyByValue)))
  {
  }
};

#undef ELEMENT
}  // namespace syntax
}  // namespace openscenario_interpreter

#endif  // OPENSCENARIO_INTERPRETER__SYNTAX__MODIFY_RULE_HPP_