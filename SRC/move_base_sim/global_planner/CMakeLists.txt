cmake_minimum_required(VERSION 2.8.3)
project(global_planner)

find_package(catkin REQUIRED
  COMPONENTS
    angles
    dynamic_reconfigure
    geometry_msgs
    costmap_2d
    nav_core
    nav_msgs
    pluginlib
    roscpp
    tf
)

generate_dynamic_reconfigure_options(
  cfg/GlobalPlanner.cfg
)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES ${PROJECT_NAME}
  CATKIN_DEPENDS
    dynamic_reconfigure
    geometry_msgs
    navfn
    nav_msgs
    pluginlib
    roscpp
    tf
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

add_library(${PROJECT_NAME}
  src/quadratic_calculator.cpp
  src/dijkstra.cpp
  src/astar.cpp
  src/grid_path.cpp
  src/gradient_path.cpp
  src/orientation_filter.cpp
  src/planner_core.cpp
)
add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(${PROJECT_NAME} ${catkin_LIBRARIES})
