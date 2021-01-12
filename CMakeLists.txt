cmake_minimum_required(VERSION 3.1)
project(lumpyspace)


# Find SDL2 
#set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules")
find_package(SDL2 REQUIRED)

    find_library(_SDL2TTF_LIB SDL2_ttf)
    list(APPEND SDL2_LIBRARIES ${_SDL2TTF_LIB})
    find_library(_SDL2IMAGE_LIB SDL2_image)
    list(APPEND SDL2_LIBRARIES ${_SDL2IMAGE_LIB})
    find_library(_SDL2MIXER_LIB SDL2_mixer)
    list(APPEND SDL2_LIBRARIES ${_SDL2MIXER_LIB})

include_directories(${SDL2_INCLUDE_DIRS})
find_package(Boost REQUIRED COMPONENTS filesystem)


# Define executable target
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2main_INCLUDE_DIRS} ${CMAKE_BINARY_DIR})
add_executable(lumpyspace main.cpp 
CAlien.cpp
CBullet.cpp
CParaMove.cpp
CTexture.cpp
CAnimate.cpp
CEgg.cpp
CPlayer.cpp
CTimer.cpp
CAnimation.cpp
CEngine.cpp
CProgressBar.cpp
CUfoAttack.cpp
CBomb.cpp
CLevel.cpp
CRectangle.cpp
CUfo.cpp
CBounceBox.cpp
CMissile.cpp
CRules.cpp
CXMove.cpp
CBoxBounce.cpp
CMotion.cpp
CText.cpp)
target_link_libraries(lumpyspace ${SDL2_LIBRARIES} Boost::filesystem)

install(TARGETS lumpyspace DESTINATION /home/pi/bmos/lumpyspace)
install(DIRECTORY fonts DESTINATION /home/pi/bmos/lumpyspace/)
install(DIRECTORY graphics DESTINATION /home/pi/bmos/lumpyspace/)
install(DIRECTORY sounds DESTINATION /home/pi/bmos/lumpyspace/)



