# This is the unittest CMakeLists.txt
#
# Here the sub-CMakeLists.txt containing the actual unittests are included.
# Additionally gtest/gmock are compiled into a library

#get_filename_component(TESTCORE_DIR          "_googletest"        ABSOLUTE)

#add_library(gtestX
#                    "${TESTCORE_DIR}/googlemock/src/gmock-all.cc"
#                    "${TESTCORE_DIR}/googletest/src/gtest-all.cc"
#)

#target_include_directories(gtestX PRIVATE
#                    "${TESTCORE_DIR}/googletest"
#                    "${TESTCORE_DIR}/googletest/include"
#                    "${TESTCORE_DIR}/googlemock"
#                    "${TESTCORE_DIR}/googlemock/include"
#)

add_subdirectory("_googletest" googletest)


add_subdirectory(01-first-test)
add_subdirectory(02-bigint)
add_subdirectory(bag)
