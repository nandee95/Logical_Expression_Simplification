cmake_minimum_required (VERSION 3.1)

#set C++ standard
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

project(LogicalExpressionSimlification CXX)

add_executable(LogicalExpressionSimlification
	main.cpp
	application.cpp
	application.hpp
	expression.cpp
	expression.hpp
	column.cpp
	column.hpp
	group.hpp
	group.cpp
	node.cpp
	node.hpp
	string.cpp
	string.hpp
)
