#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# Copyright 2020 Tier IV, Inc. All rights reserved.
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

import argparse
import time

from scenario_common.logger import Logger
from scenario_common.manager import Manager
from scenario_launcher.converter_handler import ConverterHandler
from scenario_launcher.database_handler import DatabaseHandler
from scenario_launcher.lifecycle_controller import LifecycleController


class Launcher:

    SERVER_URI = 'http://0.0.0.0:10000'
    SLEEP_RATE = 1
    PKILLER = "kill -9 `lsof -w -n -i tcp:10000| awk '{print $2}'|awk 'END{print}'`;"

    def __init__(self, timeout):
        self.timeout = timeout
        self.database_path = None
        self.lifecycle_controller = None
        self.launcher_path = ""
        self.log_path = ""
        self.yaml_scenarios = []
        self.xosc_scenarios = []
        self.map_dict = dict()

    def main(self):
        self.launcher_path, self.log_path, self.yaml_scenarios, self.map_dict \
            = DatabaseHandler.read_database()
        self.xosc_scenarios = ConverterHandler.convert_all_scenarios(
            self.yaml_scenarios, self.launcher_path)
        self.lifecycle_controller = LifecycleController()
        self.run_all_scenarios()

    def monitor_state(self):
        start = time.time()
        while (time.time() - start) < self.timeout:
            Logger.print_info("    Monitoring in Launcher")
            current_state = self.lifecycle_controller.get_lifecycle_state()
            Logger.print_info("    scenario runner state is " + current_state)
            if(current_state == "inactive"):
                Logger.print_process("    end of running")
                return
            time.sleep(self.SLEEP_RATE)
        Logger.print_warning("Reached to maximum simulation time")
        self.lifecycle_controller.deactivate_node()

    def run_scenario(self):
        Logger.print_process(
            "Set maximum simulation time: " + str(self.timeout))
        time.sleep(self.SLEEP_RATE)
        self.lifecycle_controller.activate_node()
        self.monitor_state()
        print("")

    def run_all_scenarios(self):
        Manager.mkdir(self.log_path)
        for index, scenario in enumerate(self.xosc_scenarios):
            print(str(index+1), scenario)
        for index, scenario in enumerate(self.xosc_scenarios):
            Logger.print_separator(
                    "Test case " + str(index+1) + " of " + str(len(self.xosc_scenarios)))
            self.lifecycle_controller.configure_node(scenario)
            if (self.lifecycle_controller.get_lifecycle_state() == "unconfigured"):
                Logger.print_warning(
                    "Skip this scenario because of activation failure")
                continue
            self.run_scenario()
            self.lifecycle_controller.cleanup_node()
        self.lifecycle_controller.shutdown()

    def __del__(self):
        pass


def main():
    parser = argparse.ArgumentParser(description='launch simulator')

    parser.add_argument('--timeout',
                        type=int,
                        default=20,
                        help='Specify simulation time limit in seconds. \
                  The default is 180 seconds.')

    parser.add_argument('--log',
                        default='screen',
                        help='Specify the type of log output.')

    parser.add_argument('--scenario',
                        help='Specify the scenario you want to execute.')

    args = parser.parse_args()
    launcher = Launcher(args.timeout)
    launcher.main()


if __name__ == '__main__':
    main()
