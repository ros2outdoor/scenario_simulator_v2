// Copyright 2015-2021 Tier IV, Inc. All rights reserved.
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

#include <openscenario_interpreter/syntax/relative_world_position.hpp>

namespace openscenario_interpreter
{
inline namespace syntax
{
RelativeWorldPosition::operator geometry_msgs::msg::Point() const
{
  geometry_msgs::msg::Point result;
  {
    result.x = dx;
    result.y = dy;
    result.z = dz;
  }

  return result;
}

RelativeWorldPosition::operator geometry_msgs::msg::Pose() const
{
  throw UNSUPPORTED_CONVERSION_DETECTED(RelativeWorldPosition, geometry_msgs::msg::Pose);
}

RelativeWorldPosition::operator openscenario_msgs::msg::LaneletPose() const
{
  throw UNSUPPORTED_CONVERSION_DETECTED(RelativeWorldPosition, openscenario_msgs::msg::LaneletPose);
}
}  // namespace syntax
}  // namespace openscenario_interpreter
