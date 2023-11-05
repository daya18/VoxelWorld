cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("VoxelWorld")

include ("FindOpenGL")

# Link all external dependencies statically
set ( BUILD_SHARED_LIBS OFF )

# Add GLFW
set ( GLFW_BUILD_DOCS OFF )
set ( GLFW_BUILD_EXAMPLES OFF )
set ( GLFW_BUILD_TESTS OFF )
add_subdirectory ( external/glfw )

# Add GLM
add_subdirectory ( external/glm )

add_executable ( VoxelWorld )

target_sources ( VoxelWorld PRIVATE 
	"source/Main.cpp"		
	"source/Window.cpp"
 "source/Application.cpp")

target_precompile_headers ( VoxelWorld PRIVATE source/vwpch.hpp )

target_link_libraries ( VoxelWorld PRIVATE 
	glfw
	glm
	OpenGL::GL
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET VoxelWorld PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
