# Name the project you can choose any name you want here
PROJECT(Reco) 

# Check requirements
CMAKE_MINIMUM_REQUIRED(VERSION 2.4.0)

# set the compiler flags
SET(CMAKE_BUILD_TYPE debug)
SET(CMAKE_CXX_FLAGS "-Wall -std=c++11")

# Add path to additional packages (makes it easier to include common libraries)
set(CMAKE_MODULE_PATH ${Reco_SOURCE_DIR}/CMakeModules/)

# Define where your executables should be put
set(EXECUTABLE_OUTPUT_PATH ${Reco_BINARY_DIR})

# Add required packages (e.g. libraries)
# e.g. FIND_PACKAGE(OpenGL 3.3 REQUIRED)


# Set include directories containing used header files
INCLUDE_DIRECTORIES(
  ${Reco_SOURCE_DIR}/include/
  ${Reco_SOURCE_DIR}/libs/glm-0.9.2.3
)

# Set link directories for addiontional libraries
#LINK_DIRECTORIES(
#  ${Reco_SOURCE_DIR}/libs/
#)


SET(Reco_SRC
  src/Image.cpp
  src/main.cpp
)

ADD_EXECUTABLE(Reco ${Reco_SRC})

TARGET_LINK_LIBRARIES(
  Reco
  # add libraries here...
)