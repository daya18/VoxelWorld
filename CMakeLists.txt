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

set_source_files_properties ( external/glad/src/glad.c PROPERTIES LANGUAGE CXX)

target_compile_definitions ( VoxelWorld PRIVATE
	IMGUI_DEFINE_MATH_OPERATORS
)

target_sources ( VoxelWorld PRIVATE 
	external/glad/src/glad.c
	
	external/imgui/imgui_demo.cpp
	external/imgui/imgui_draw.cpp
	external/imgui/imgui_widgets.cpp
	external/imgui/imgui_tables.cpp
	external/imgui/imgui.cpp
	external/imgui/backends/imgui_impl_glfw.cpp
	external/imgui/backends/imgui_impl_opengl3.cpp

	"source/Main.cpp"		
	"source/Window.cpp"
	"source/Application.cpp" 
	
	"source/RenderContext.cpp" 
	"source/VoxelWorld.cpp" 
	"source/SignalEmitter.cpp"
	"source/Camera.cpp"  
	
	"source/Voxel.cpp"
	"source/World.cpp"  
	 
	
 "source/VoxelWorldRaycaster.cpp"    
 "source/impex.cpp" 
 "source/VoxelModel.cpp" 
 "source/Core.cpp" 
 "source/Player.cpp"   
 "source/opengl/VectorBuffer.cpp" 
 "source/SimpleVoxelWorldRenderer.cpp" "source/Mesh.cpp" "source/opengl/Buffer.cpp" "source/opengl/VertexArray.cpp" "source/opengl/Type.cpp")

target_include_directories ( VoxelWorld PRIVATE 
	external/glad/include
	external
	external/OBJ-Loader/Source
	external/Epsilon-Intersection/include
	external/imgui
)

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
