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

#ifndef AWAPI_AWAUTO_ADAPTER__AWAPI_VEHICLE_STATUS_PUBLISHER_HPP_
#define AWAPI_AWAUTO_ADAPTER__AWAPI_VEHICLE_STATUS_PUBLISHER_HPP_
#include <autoware_api_msgs/msg/awapi_vehicle_status.hpp>
#include <awapi_awauto_adapter/utility/visibility.h>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/bool.hpp>
#include <std_msgs/msg/float32.hpp>
#include <std_msgs/msg/string.hpp>

#include <chrono>

namespace autoware_api
{
class AutowareVehicleStatusPublisher : public rclcpp::Node
{
  /** ---- VehicleStatus -------------------------------------------------------
  *  Topic: /awapi/vehicle/get/status
  * ------------------------------------------------------------------------ */
  using VehicleStatus = autoware_api_msgs::msg::AwapiVehicleStatus;
  rclcpp::Publisher<VehicleStatus>::SharedPtr pub_vehicle_status_;
  VehicleStatus vehicle_status_;
  rclcpp::TimerBase::SharedPtr timer_vehicle_status_;

public:
  AWAPI_AWAUTO_ADAPTER_PUBLIC
  explicit AutowareVehicleStatusPublisher(const rclcpp::NodeOptions &);
  void publish_vehicle_status();
};
}  // namespace autoware_api
#endif  // AWAPI_AWAUTO_ADAPTER__AWAPI_VEHICLE_STATUS_PUBLISHER_HPP_
