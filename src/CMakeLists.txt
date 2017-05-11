include_directories("../include/")
include_directories("/usr/local/include/")
include_directories("/usr/include/")

set(CMAKE_CXX_FLAGS_DEBUG "-o -g")

link_directories("/usr/local/lib")
link_directories("/usr/lib")

add_executable(robot_arm Main.cpp RobotArm.cpp)
target_link_libraries(robot_arm glut GLEW GL m GLU)
