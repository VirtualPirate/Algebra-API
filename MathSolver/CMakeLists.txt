cmake_minimum_required(VERSION 3.16.3)

set(CMAKE_C++_COMPILER "/usr/bin/clang/")
set(CMAKE_CXX_STANDARD 17)
# set(INCLUDE_DIR "D:/workshop/cpp/math_project/MathSolver/")

project(AutomateAlgebra)

include_directories(${INCLUDE_DIR})
add_executable(program
	Operand.cpp
	Constant.cpp
	Variable.cpp
	OperandList.cpp
	Term.cpp
	Expression.cpp
	Operation_Functions.cpp
	Substitutor.cpp
	Parser.cpp
	Equation.cpp
	main.cpp
)