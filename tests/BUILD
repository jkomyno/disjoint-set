load("//:variables.bzl", "COPTS")

cc_test(
    name = "tests",
    srcs = glob(["**/*.cpp"]),
    deps = [
        "//disjoint_set:disjoint_set",
        "@googletest//:gtest_main",
    ],
    copts = COPTS,
)