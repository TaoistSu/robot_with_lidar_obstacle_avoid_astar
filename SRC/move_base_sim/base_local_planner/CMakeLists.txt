cmake_minimum_required(VERSION 2.8.3)
project(base_local_planner)

find_package(catkin REQUIRED
        COMPONENTS
            angles
            cmake_modules
            costmap_2d
            dynamic_reconfigure
            geometry_msgs
            message_generation
            nav_core
            nav_msgs
            pcl_conversions
            pcl_ros
            pluginlib
            roscpp
            rospy
            std_msgs
            tf
            voxel_grid
        )


include_directories(
    include
    ${catkin_INCLUDE_DIRS}
)

catkin_python_setup()

# messages
add_message_files(
    DIRECTORY msg
    FILES
    Position2DInt.msg
)

generate_messages(
    DEPENDENCIES
        std_msgs
)

# dynamic reconfigure
generate_dynamic_reconfigure_options(
    cfg/BaseLocalPlanner.cfg
)

catkin_package(
    INCLUDE_DIRS include
    LIBRARIES
        base_local_planner
    CATKIN_DEPENDS
        angles
        costmap_2d
        dynamic_reconfigure
        geometry_msgs
        message_runtime
        nav_core
        nav_msgs
        pluginlib
        roscpp
        std_msgs
        tf
)


add_library(base_local_planner
	src/footprint_helper.cpp
	src/goal_functions.cpp
	src/map_cell.cpp
	src/map_grid.cpp
	src/map_grid_visualizer.cpp
	src/map_grid_cost_function.cpp
	src/latched_stop_rotate_controller.cpp
	src/local_planner_util.cpp
	src/odometry_helper_ros.cpp
	src/obstacle_cost_function.cpp
	src/oscillation_cost_function.cpp
	src/prefer_forward_cost_function.cpp
        src/rotate_first_cost_function.cpp
	src/point_grid.cpp
	src/costmap_model.cpp
	src/simple_scored_sampling_planner.cpp
	src/simple_trajectory_generator.cpp
	src/trajectory.cpp
	src/twirling_cost_function.cpp
	src/voxel_grid_model.cpp)
add_dependencies(base_local_planner nav_msgs_generate_messages_cpp base_local_planner_generate_messages_cpp base_local_planner_gencfg)
target_link_libraries(base_local_planner
    ${catkin_LIBRARIES}
)
