add_unittest(
      TEST_NAME "02-bigint"
      RUNNER "test-bigint.cpp"
      INCLUDE_DIRS "${ROOT_DIR}/modules"
      SRCS "${ROOT_DIR}/modules/bigint.cpp"
      DEPENDENCY gtest gtest_main
)
