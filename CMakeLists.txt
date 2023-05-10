cmake_minimum_required(VERSION 3.18.2)

set(PROJECT_NAME Xlib)

# Set C++ standard to 14
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
# Define project
project(${PROJECT_NAME}) 

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

#--------------------------- PLATFORM SPECIFIC -------------------------------------------------------------------------

if(APPLE)
  # 生成XCode工程
  # 设置C编译器路径
  set(CMAKE_XCODE_GENERATE_SCHEME ON)
  set(CMAKE_XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH NO)
  set(CMAKE_XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT "dwarf-with-dsym")
  set(CMAKE_XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT[variant=Debug] "dwarf-with-dsym")
  set(CMAKE_XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT[variant=Release] "dwarf-with-dsym")
  # Fix linking on 10.14+. See https://stackoverflow.com/questions/54068035
  include_directories(/usr/local/include)
  link_directories(/usr/local/lib)
endif()

if(NOT WIN32)
  string(ASCII 27 Esc)
  set(ColorReset   "${Esc}[m")
  set(ColorBold    "${Esc}[1m")
  set(ColorRed     "${Esc}[31m")
  set(ColorBoldRed "${ColorRed}${ColorBold}")
endif()

# 设置源文件为当前目录和src路径下的所有子目录
file(GLOB_RECURSE SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*/*.cpp 
    EXCLUDE
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

# Add executable and link libraries
# add_executable(${PROJECT_NAME} ${SOURCES})
# target_link_libraries(${PROJECT_NAME} ${LIBRARIES})

# 生成静态链接库
add_library(Xlib_static STATIC ${SOURCES})
set_target_properties(Xlib_static PROPERTIES OUTPUT_NAME "Xlib_static")

# 生成执行main.cpp的测试执行文件
enable_testing()

add_executable(test_main ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
target_link_libraries(test_main Xlib_static)

add_test(name Test_main COMMAND test_main)

# add_executable(test1 test1.cpp)
# add_test(NAME Test1 COMMAND test1)

# add_executable(test2 test2.cpp)
# add_test(NAME Test2 COMMAND test2)

# add_test(NAME MyTest COMMAND ${CMAKE_CTEST_COMMAND}
#          --output-on-failure
#          -C $<CONFIGURATION>
#          -R "^Test[12]$")
