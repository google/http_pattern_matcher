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
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

ABSEIL_COMMIT = "99477fa9f1e89a7d8253c8aeee331864710d080c"
ABSEIL_SHA256 = "495e8e1c481018126b2a84bfe36e273907ce282b135e7d161e138e463d295f3d"

def absl_repositories():
    http_archive(
        name = "com_google_absl",
        strip_prefix = "abseil-cpp-" + ABSEIL_COMMIT,
        url = "https://github.com/abseil/abseil-cpp/archive/" + ABSEIL_COMMIT + ".tar.gz",
        sha256 = ABSEIL_SHA256,
    )


PROTOBUF_COMMIT = "3.15.5"  # Mar 04, 2021
PROTOBUF_SHA256 = "bc3dbf1f09dba1b2eb3f2f70352ee97b9049066c9040ce0c9b67fb3294e91e4b"

RULES_PROTO_SHA = "97d8af4dc474595af3900dd85cb3a29ad28cc313"  # Oct 31, 2019
RULES_PROTO_SHA256 = "602e7161d9195e50246177e7c55b2f39950a9cf7366f74ed5f22fd45750cd208"

def protobuf_repositories():
    http_archive(
        name = "com_google_protobuf",
        strip_prefix = "protobuf-" + PROTOBUF_COMMIT,
        url = "https://github.com/google/protobuf/archive/v" + PROTOBUF_COMMIT + ".tar.gz",
        sha256 = PROTOBUF_SHA256,
    )

    http_archive(
        name = "rules_proto",
        sha256 = RULES_PROTO_SHA256,
        strip_prefix = "rules_proto-" + RULES_PROTO_SHA,
        urls = [
            "https://mirror.bazel.build/github.com/bazelbuild/rules_proto/archive/" + RULES_PROTO_SHA + ".tar.gz",
            "https://github.com/bazelbuild/rules_proto/archive/" + RULES_PROTO_SHA + ".tar.gz",
        ],
    )

GOOGLETEST_COMMIT = "703bd9caab50b139428cea1aaff9974ebee5742e"  # v1.10.0: Oct 2, 2019
GOOGLETEST_SHA256 = "d17b1b83a57b3933565a6d0616fe261107326d47de20288d0949ed038e1c342d"

def googletest_repositories():
    http_archive(
        name = "com_google_googletest",
        strip_prefix = "googletest-" + GOOGLETEST_COMMIT,
        url = "https://github.com/google/googletest/archive/" + GOOGLETEST_COMMIT + ".tar.gz",
        sha256 = GOOGLETEST_SHA256,
    )

GOOGLEAPIS_COMMIT = "1d5522ad1056f16a6d593b8f3038d831e64daeea"  # Sept 03, 2020
GOOGLEAPIS_SHA256 = "cd13e547cffaad217c942084fd5ae0985a293d0cce3e788c20796e5e2ea54758"

def googleapis_repositories():
    http_archive(
        name = "com_google_googleapis",
        strip_prefix = "googleapis-" + GOOGLEAPIS_COMMIT,
        url = "https://github.com/googleapis/googleapis/archive/" + GOOGLEAPIS_COMMIT + ".tar.gz",
        sha256 = GOOGLEAPIS_SHA256,
    )
