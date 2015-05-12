cmake_minimum_required(VERSION 2.8.9)

project(rts)

if(NOT WIN32)
	set(CMAKE_CXX_FLAGS "-std=c++0x")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/src/Mragpp/cmake/Modules")
set(CMAKE_BUILD_TYPE Debug)

add_definitions(-DDEBUG)

add_executable(rts
	src/Main.cpp
	src/StdH.h
	
	src/Game.cpp src/Game.h
	src/Menu.cpp src/Menu.h
	src/PlayerInfo.cpp src/PlayerInfo.h
	src/Sprite.cpp src/Sprite.h
	src/Tile.cpp src/Tile.h
	src/World.cpp src/World.h
	
	src/Sprites/Building.cpp src/Sprites/Building.h
	src/Sprites/Property.cpp src/Sprites/Property.h
	src/Sprites/Unit.cpp src/Sprites/Unit.h
	src/Sprites/Vehicle.cpp src/Sprites/Vehicle.h
	src/Sprites/WorldSelection.cpp src/Sprites/WorldSelection.h
	
	src/Sprites/Buildings/Airport.cpp src/Sprites/Buildings/Airport.h
	src/Sprites/Buildings/Base.cpp src/Sprites/Buildings/Base.h
	src/Sprites/Buildings/City.cpp src/Sprites/Buildings/City.h
	src/Sprites/Buildings/HQ.cpp src/Sprites/Buildings/HQ.h
	src/Sprites/Buildings/Port.cpp src/Sprites/Buildings/Port.h
	src/Sprites/Buildings/Silo.cpp src/Sprites/Buildings/Silo.h
	
	src/Sprites/Units/Infantry.cpp src/Sprites/Units/Infantry.h
	src/Sprites/Units/Mech.cpp src/Sprites/Units/Mech.h
	src/Sprites/Units/Recon.cpp src/Sprites/Units/Recon.h
	src/Sprites/Units/Tank.cpp src/Sprites/Units/Tank.h
)

add_subdirectory(src/Mragpp)

target_link_libraries(rts
	Mragpp
)

include_directories(
	src/Mragpp/
	src/Mragpp/Mragpp/Scratch/
)
