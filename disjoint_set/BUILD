package(default_visibility = ["//visibility:public"])

load("@rules_cc//cc:defs.bzl", "cc_library") #, "cc_binary")
load("//:variables.bzl", "COPTS")

cc_library(
    name = "disjoint_set",
    hdrs = glob(["*.h"]),
    copts = COPTS,
)
