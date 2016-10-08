PROJECT(demo)
FILE (GLOB_RECURSE demo_SRCS *.cpp *.cxx *.cc *.C *.c)

SET (demo_LIBS sshfile Threads::Threads)
SET (demo_BIN "demo")

ADD_EXECUTABLE(${demo_BIN} ${demo_SRCS})

ExternalProject_Get_Property(libedit install_dir)
target_include_directories(${demo_BIN} PUBLIC ${install_dir}/include)

TARGET_LINK_LIBRARIES(${demo_BIN} ${demo_LIBS} ${install_dir}/lib/libedit.so)

SET_TARGET_PROPERTIES(${demo_BIN} PROPERTIES VERSION "${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}" OUTPUT_NAME ${demo_BIN} CLEAN_DIRECT_OUTPUT 1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -parallel -xHOST -O3 -ipo -no-prec-div -fp-model fast=2 -std=c++14 -static-intel -fPIC")

# TARGET_COMPILE_FEATURES(${demo_BIN} PRIVATE cxx_constexpr)
# TARGET_COMPILE_FEATURES(${demo_BIN} PRIVATE cxx_decltype_auto)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_WARNINGS}")

INSTALL(TARGETS ${demo_BIN} DESTINATION bin)
