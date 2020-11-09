#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""Launch description for scenario runner moc."""

# Copyright (c) 2020 Autoware Foundation. All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource


def generate_launch_description():
    """Launch description for scenario runner moc."""
    lanlet_path = os.path.join(
            get_package_share_directory('kashiwanoha_map'), 'map', 'lanelet2_map.osm')
    rviz_config_dir = os.path.join(
            get_package_share_directory('simulation_api'),
            'config',
            'moc_test.rviz')
    lexus_package_path = get_package_share_directory('lexus_rx_450h_description')
    description_launch_dir = os.path.join(lexus_package_path, 'launch')
    return LaunchDescription([
        Node(
            package='simulation_api',
            node_executable='scenario_runner_moc_node',
            node_name='scenario_runner_node',
            output='screen',
            parameters=[{
                'map_path': lanlet_path,
                'origin_latitude': 35.903555800615614,
                'origin_longitude': 139.93339979022568,
                'port': 8080}],
            arguments=[('__log_level:=info')]),
        Node(
            package='scenario_simulator',
            node_executable='scenario_simulator_node',
            node_name='scenario_simulator_node',
            output='log',
            parameters=[{
                'port': 8080
            }],
            arguments=[('__log_level:=warn')],
            ),
        Node(
            package='rviz2',
            node_executable='rviz2',
            node_name='rviz2',
            arguments=['-d', rviz_config_dir],
            output='screen'),
        Node(
            package='openscenario_visualization',
            node_executable='openscenario_visualization_node',
            node_name='openscenario_visualization_node',
            output='screen'
            ),
        IncludeLaunchDescription(
            PythonLaunchDescriptionSource(
                [description_launch_dir, '/lexus_rx_450h_visualisation.launch.py']),
        )
    ])