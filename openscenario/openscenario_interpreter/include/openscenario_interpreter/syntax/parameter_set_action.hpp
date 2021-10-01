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

#ifndef OPENSCENARIO_INTERPRETER__SYNTAX__PARAMETER_SET_ACTION_HPP_
#define OPENSCENARIO_INTERPRETER__SYNTAX__PARAMETER_SET_ACTION_HPP_

#include <openscenario_interpreter/reader/attribute.hpp>
#include <openscenario_interpreter/scope.hpp>
#include <typeindex>
#include <unordered_map>
#include <utility>

namespace openscenario_interpreter
{
inline namespace syntax
{
/* ---- SetAction --------------------------------------------------------------
 *
 *  <xsd:complexType name="ParameterSetAction">
 *    <xsd:attribute name="value" type="String" use="required"/>
 *  </xsd:complexType>
 *
 * -------------------------------------------------------------------------- */
struct ParameterSetAction : private Scope
{
  const String parameter_ref;

  const String value;

  template <typename Node>
  explicit ParameterSetAction(const Node & node, Scope & scope, const String & parameter_ref)
  : Scope(scope),
    parameter_ref(parameter_ref),
    value(readAttribute<String>("value", node, localScope()))
  {
  }

  static auto accomplished() noexcept -> bool;

  /*  */ auto run() const -> void;

  static auto start() noexcept -> void;
};
}  // namespace syntax
}  // namespace openscenario_interpreter

#endif  // OPENSCENARIO_INTERPRETER__SYNTAX__PARAMETER_SET_ACTION_HPP_
