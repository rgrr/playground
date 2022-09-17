###############################################################################
#
# add_unittest()
#
# This Function adds an executable to the test run. In a typical project,
# we have one test executable per unit. Use "ctest" to run the tests.
#
# Usage: add_unittest  (TEST_NAME     <string>
#           RUNNER      <list>
#                       INCLUDE_DIRS  <list>
#                       SRCS          <list>
#                       DEPENDENCY    <list>
#                       )
#
# Input:        MODULE          : unique name of the module/library
#               RUNNER      : runner where the main is located
#               INCLUDE_DIRS    : headers provided by this library
#               SRCS            : source files that compile this module
#               DEPENDS         : libraries and modules this module links to
#
# Output:
#               Adds test to the test run
function(add_unittest)
    cmake_parse_arguments(
        PARSED                                # prefix
        ""                                    # Booleans
        "TEST_NAME"                           # this is the only string, one value
        "RUNNER;INCLUDE_DIRS;SRCS;DEPENDENCY" # variables for a list
        ${ARGN}                               # these are the arguments to parse
    )

    # put every test case into its own binary
    add_executable(${PARSED_TEST_NAME} ${PARSED_RUNNER} ${PARSED_SRCS})
    target_link_libraries(${PARSED_TEST_NAME} PRIVATE ${PARSED_DEPENDENCY})
    target_include_directories(${PARSED_TEST_NAME} PUBLIC ${PARSED_INCLUDE_DIRS})
    add_test(NAME ${PARSED_TEST_NAME} COMMAND ${PARSED_TEST_NAME})
    #set_target_properties(${PARSED_TEST_NAME} PROPERTIES COMPILE_FLAGS "-Wall")
    #set_target_properties(${PARSED_TEST_NAME} PROPERTIES LINK_FLAGS "-fprofile-arcs")

endfunction()
