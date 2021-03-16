# Copyright 2021 Google Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
################################################################################
#

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "http_template_lib",
    srcs = [
        "src/http_template.cc",
    ],
    hdrs = [
        "include/http_pattern_matcher/http_template.h",
    ],
)

cc_library(
    name = "path_matcher_lib",
    srcs = [
        "src/path_matcher.cc",
        "src/path_matcher_node.cc",
    ],
    hdrs = [
        "include/http_pattern_matcher/path_matcher.h",
        "include/http_pattern_matcher/path_matcher_node.h",
    ],
    deps = [
        ":http_template_lib",
        "@com_google_absl//absl/strings",
    ],
)

cc_library(
    name = "path_matcher_utility_lib",
    hdrs = [
        "include/http_pattern_matcher/path_matcher_utility.h",
    ],
    deps = [
        ":path_matcher_lib",
        "@com_google_googleapis//google/api:http_cc_proto",
    ],
)

cc_test(
    name = "http_template_test",
    size = "small",
    srcs = [
        "test/http_template_test.cc",
    ],
    deps = [
        ":http_template_lib",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "path_matcher_test",
    size = "small",
    srcs = [
        "test/path_matcher_test.cc",
    ],
    deps = [
        ":path_matcher_lib",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "path_matcher_utility_test",
    size = "small",
    srcs = [
        "test/path_matcher_utility_test.cc",
    ],
    deps = [
        "path_matcher_utility_lib",
        "@com_google_googletest//:gtest_main",
    ],
)
