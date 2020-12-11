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

from ament_index_python.packages import get_package_share_directory
from argparse import ArgumentParser
from copy import deepcopy
from itertools import product
from pathlib import Path
from re import sub
from sys import exit, stderr

import numpy
import xmlschema
import yaml


def iota(start: float, step: float, stop: float) -> numpy.ndarray:

    if step < 1:
        return numpy.linspace(start, stop, 1, endpoint=True)

    else:
        return numpy.linspace(
            start, stop, int((stop - start) // step) + 1)


class MacroExpander:

    def __init__(self, rules, schema):

        self.rules = rules

        self.schema = schema

        self.specs = []

        if rules is not None:
            for each in rules['ScenarioModifier']:
                name = each['name']
                if 'list' in each:
                    self.specs.append(list(map(
                        lambda x: (name, x), each['list'])))
                else:
                    self.specs.append(list(map(
                        lambda x: (name, x),
                        iota(each['start'], each['step'], each['stop']))))

    def __call__(self, xosc: str, output: Path, basename: str):

        paths = []

        if self.specs is not None:
            for bindings in product(*self.specs):

                target = deepcopy(xosc)

                target_name = deepcopy(basename)

                for binding in bindings:
                    target_name += '__' + str(binding[0]) + '_' + str(binding[1])
                    target = sub(str(binding[0]), str(binding[1]), target)

                paths.append(
                    output.joinpath(target_name + ".xosc"))

                with paths[-1].open(mode='w') as file:
                    file.write(target)

                    try:
                        self.schema.validate(target)

                    except xmlschema.XMLSchemaValidationError as exception:
                        print("File: " + str(paths[-1]), file=stderr)
                        print("", file=stderr)
                        print("Error: " + str(exception), file=stderr)
                        exit()

        else:
            paths.append(
                output.joinpath(target_name + ".xosc"))

            with paths[-1].open(mode='w') as f:
                f.write(xosc)

        return paths


def load_yaml(path):

    if path.exists():
        with path.open('r') as file:
            return yaml.safe_load(file)
    else:
        print("No such file or directory: " + path, file=stderr)
        exit()


def from_yaml(keyword, node):

    if isinstance(node, dict):
        #
        # ???: { ... }
        #
        result = {}

        for tag, value in node.items():

            if isinstance(value, list) and len(value) == 0:
                #
                # Tag: []
                #
                # => REMOVE
                #
                continue

            if str.islower(tag[0]):
                #
                # tag: { ... }
                #
                # => @tag: { ... }
                #
                result["@" + tag] = str(value)
            else:
                #
                # Tag: { ... }
                #
                # => NO CHANGES
                #
                result[tag] = from_yaml(tag, value)

        return result

    elif isinstance(node, list):
        #
        # ???: [ ... ]
        #
        result = []

        for index, item in enumerate(node):
            result.append(from_yaml(keyword, item))

        return result

    elif isinstance(node, str):
        return node

    else:
        return None


def convert(input: Path, output: Path):

    if output.exists():
        for each in output.iterdir():
            each.resolve().unlink()
    else:
        output.mkdir(parents=True, exist_ok=True)

    path = Path(
        get_package_share_directory('scenario_test_utility')
        ).joinpath('../ament_index/resource_index/packages/OpenSCENARIO.xsd')

    schema = xmlschema.XMLSchema(str(path))

    yaml = load_yaml(input)

    macroexpand = MacroExpander(
        yaml.pop('ScenarioModifiers', None), schema)

    xosc, errors = schema.encode(
        from_yaml('OpenSCENARIO', yaml),
        indent=2,
        preserve_root=True,
        unordered=True,  # Reorder elements
        validation='lax',  # The "strict" mode is too strict than we would like.
        )

    if not schema.is_valid(xosc) and len(errors) != 0:
        print("Error: " + errors[0], file=stderr)  # Error other than the first is not important.
        exit()

    else:
        paths = macroexpand(
            xmlschema.XMLResource(xosc).tostring(
                ).replace("True", "true").replace("False", "false"),
            output,
            input.stem)

        for each in paths:
            print(each)


def main():
    parser = ArgumentParser(description='Convert OpenSCENARIO.yaml into .xosc')

    parser.add_argument('--input', type=str, required=True)
    parser.add_argument('--output', type=str, default=Path('/tmp').joinpath('xosc'))

    args = parser.parse_args()

    convert(Path(args.input).resolve(), Path(args.output).resolve())


if __name__ == "__main__":
    main()
