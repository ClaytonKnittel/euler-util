cc_library(
    name = "prime_sieve",
    srcs = ["prime_sieve.cc"],
    hdrs = ["prime_sieve.h"],
    visibility = ["//visibility:public"],
    deps = [
        "@googletest//:gtest",
    ],
)

cc_test(
    name = "prime_sieve_test",
    size = "small",
    srcs = ["prime_sieve_test.cc"],
    deps = [
        ":prime_sieve",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)
