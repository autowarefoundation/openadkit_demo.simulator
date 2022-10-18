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

#ifndef OPENSCENARIO_INTERPRETER__SYNTAX__SPEED_PROFILE_ACTION_HPP_
#define OPENSCENARIO_INTERPRETER__SYNTAX__SPEED_PROFILE_ACTION_HPP_

#include <openscenario_interpreter/scope.hpp>
#include <openscenario_interpreter/simulator_core.hpp>
#include <openscenario_interpreter/syntax/dynamic_constraints.hpp>
#include <openscenario_interpreter/syntax/entity_ref.hpp>
#include <openscenario_interpreter/syntax/following_mode.hpp>
#include <openscenario_interpreter/syntax/speed_profile_entry.hpp>
#include <pugixml.hpp>

namespace openscenario_interpreter
{
inline namespace syntax
{
/* ---- SpeedProfileAction 1.2 -------------------------------------------------
 *
 *  <xsd:complexType name="SpeedProfileAction">
 *    <xsd:sequence>
 *      <xsd:element name="DynamicConstraints" type="DynamicConstraints" minOccurs="0"/>
 *      <xsd:element name="SpeedProfileEntry" type="SpeedProfileEntry" maxOccurs="unbounded"/>
 *    </xsd:sequence>
 *    <xsd:attribute name="entityRef" type="String"/>
 *    <xsd:attribute name="followingMode" type="FollowingMode" use="required"/>
 *  </xsd:complexType>
 *
 * -------------------------------------------------------------------------- */
struct SpeedProfileAction : private Scope,  // NOTE: Required for access to actors
                            private SimulatorCore::ActionApplication,
                            private SimulatorCore::ConditionEvaluation
{
  const EntityRef entity_ref;

  const FollowingMode following_mode;

  const DynamicConstraints dynamic_constraints;

  const std::list<SpeedProfileEntry> speed_profile_entry;

  std::unordered_map<String, std::list<SpeedProfileEntry>::const_iterator> accomplishments;

  explicit SpeedProfileAction(const pugi::xml_node &, Scope &);

  auto accomplished() -> bool;

  auto endsImmediately() const -> bool;

  auto run() -> void;

  auto start() -> void;
};
}  // namespace syntax
}  // namespace openscenario_interpreter

#endif  // OPENSCENARIO_INTERPRETER__SYNTAX__SPEED_PROFILE_ACTION_HPP_
