project (Freesync2_${GFX_API})

set(sources
        Freesync2Sample.cpp
        Freesync2Sample.h
        SampleRenderer.cpp
        SampleRenderer.h
        Freesync2Debug.cpp
        Freesync2Debug.h
        FlipBookAnimation.cpp
        FlipBookAnimation.h
        stdafx.cpp
        stdafx.h)

set(Freesync2_Shaders_src
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders/FS2DebugPS.hlsl
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders/FlipBookAnimationVS.hlsl
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders/FlipBookAnimationPS.hlsl)

source_group("Sources"                  FILES ${sources})
source_group("Freesync2Shaders"         FILES ${Freesync2_Shaders_src})

add_executable(${PROJECT_NAME} WIN32 ${sources} ${Freesync2_Shaders_src})
target_link_libraries (${PROJECT_NAME} LINK_PUBLIC Framework_DX12 ImGUI amd_ags DXC)
set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_HOME_DIRECTORY}/bin")

set_source_files_properties(${Freesync2_Shaders_src} PROPERTIES VS_TOOL_OVERRIDE "Text")

function(copyCommand list dest)
	foreach(fullFileName ${list})
		get_filename_component(file ${fullFileName} NAME)
		message("Generating custom command for ${fullFileName}")
		add_custom_command(
			OUTPUT   ${dest}/${file}
			PRE_BUILD
			COMMAND cmake -E make_directory ${dest}
			COMMAND cmake -E copy ${fullFileName}  ${dest}
			MAIN_DEPENDENCY  ${fullFileName}
			COMMENT "Updating ${file} into ${dest}"
		)
	endforeach()
endfunction()

copyCommand("${Freesync2_Shaders_src}" ${CMAKE_HOME_DIRECTORY}/bin/ShaderLibDX)