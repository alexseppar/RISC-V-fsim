cmake_minimum_required(VERSION 3.1)

project(RISC-V-fsim)

find_library(ELF_LIB elf)
find_library(M_LIB m)

include_directories(asmjit/src)

set(SIM_DIR sim)
set(SIM_SRC
    ${SIM_DIR}/decoder.cpp
    ${SIM_DIR}/elf_reader.cpp
    ${SIM_DIR}/exec_inst.cpp
    ${SIM_DIR}/ir.cpp
    ${SIM_DIR}/isa_desc.cpp
    ${SIM_DIR}/jit.cpp
    ${SIM_DIR}/main.cpp
    ${SIM_DIR}/options.cpp
    ${SIM_DIR}/sim.cpp
    ${SIM_DIR}/syscall.cpp)

set_source_files_properties(${SIM_SRC} PROPERTIES COMPILE_FLAGS
    "-std=c++17 -Wall -Wextra -Wno-packed-bitfield-compat")

add_subdirectory(asmjit)
add_executable(riscvsim "${SIM_SRC}")

target_link_libraries(riscvsim asmjit ${ELF_LIB} ${M_LIB})
