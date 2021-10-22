cmake_minimum_required(VERSION 3.19)
project(projeto_2 C)

set(CMAKE_C_STANDARD 11)

add_executable(projeto_2 main.c auxiliar.c estrutura_dados.c estrutura_dados.h)
add_compile_options(-Wall -Wextra)