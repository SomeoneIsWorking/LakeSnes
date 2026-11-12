cmake_minimum_required(VERSION 3.24)

project(lakesnes VERSION 0.1.0 LANGUAGES C)

add_library(lakesnes_core STATIC
	snes/apu.c
	snes/cart.c
	snes/cpu.c
	snes/cx4.c
	snes/dma.c
	snes/dsp.c
	snes/input.c
	snes/ppu.c
	snes/snes.c
	snes/snes_other.c
	snes/spc.c
	snes/statehandler.c
)
add_library(lakesnes::core ALIAS lakesnes_core)
set_target_properties(lakesnes_core PROPERTIES OUTPUT_NAME lakesnes EXPORT_NAME core C_STANDARD 11 C_STANDARD_REQUIRED ON POSITION_INDEPENDENT_CODE ON)
target_include_directories(lakesnes_core PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/snes>
	$<INSTALL_INTERFACE:include/lakesnes>
)
if(NOT MSVC)
	target_link_libraries(lakesnes_core PRIVATE m)
endif()

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
install(TARGETS lakesnes_core EXPORT lakesnesTargets ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY snes/ DESTINATION include/lakesnes FILES_MATCHING PATTERN "*.h")
install(EXPORT lakesnesTargets NAMESPACE lakesnes:: DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/lakesnes)
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/lakesnesConfig.cmake "include(\${CMAKE_CURRENT_LIST_DIR}/lakesnesTargets.cmake)\n")
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/lakesnesConfigVersion.cmake COMPATIBILITY ExactVersion)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/lakesnesConfig.cmake ${CMAKE_CURRENT_BINARY_DIR}/lakesnesConfigVersion.cmake
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/lakesnes)
