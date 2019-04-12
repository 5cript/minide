if debugging == nil then
    debugging = true
end

function getParameters()
	return {
        project_name = "MY_PROJECT",
        cmake_required = "3.11",
        cxx_standard = "17",
        default_build_type = "Release",
        is_library = false,
        library_type = "STATIC",
        binary_name = "",
        debug_options = "-fexecptions -g -Wall -pedantic-errors -pedantic",
        release_options = "-fexceptions -O3 -Wall -pedantic"	
	}
end

function getDescriptions()
	return {
        project_name = "The name of the project",
        cmake_required = "The minimum required CMake version",
        cxx_standard = "The default C++ standard for this project, 0 = dont include default cxx standard",
        default_build_type = "Adds a section to set a default build type, empty = omit section",
        is_library = "Generate Library? (or executable?)",
        library_type = "Leave empty if no lib. STATIC/SHARED",
        binary_name = "Specify name for binary output, leave empty if binary_name=project_name",
        debug_options = "Build options for debug build, empty = dont set",
        release_options = "Build options for release build, empty = dont set"
    }
end

function runWizard(parameters)
	if parameters == nil then
		if not debugging then
			return 1
		end
	end

	-- Libraries
	local Pretty = require "pl.pretty"
	local Template = require ("pl.text").Template
	local dedent = require("pl.text").dedent

	if debugging then
		Pretty.dump(parameters)
	end

	function substitute(params, str)
		return dedent(Template(str):safe_substitute(params))
	end
	function newlineIf(nonempty)
		if nonempty == "" then
			return ""
		else
			return "\n"
		end
	end

	local versionRequired = substitute(parameters, [[
		# Version Check
		cmake_minimum_required(VERSION ${cmake_required})
	]])

	local cxxStandard = substitute(parameters, [[
		# Default CXX Standard
		if (NOT "${${CMAKE_CXX_STANDARD}}")
			set(CMAKE_CXX_STANDARD ${cxx_standard})
		endif()
	]])
	if parameters.cxx_standard == 0 then
		cxxStandard = ""
	end

	local defaultBuildType = substitute(parameters, [[
		# Default Build Type
		if (NOT EXISTS ${${CMAKE_BINARY_DIR}}/CMakeCache.txt)
			if (NOT CMAKE_BUILD_TYPE)
				set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE STRING "" FORCE)
			endif()
		endif()
	]])
	if parameters.default_build_type == "" then
		defaultBuildType = ""
	end

	local project = substitute(parameters, [[
		# Project
		project(${project_name})
	]])

	local files = substitute(parameters, [[
		# Add Files
		file(GLOB sources "*.cpp")
	]])

	local addLibrary = substitute(parameters, [[
		# Add Library
		add_library(${project_name} ${library_type} ${${sources}})
	]])

	local addExecutable = substitute(parameters, [[
		# Add Executable
		add_executable(${project_name} ${${sources}})
	]])

	local addBinary = ""
	if parameters.is_library then
		addBinary = addLibrary
	else
		addBinary = addExecutable
	end

	local binaryName = substitute(parameters, [[
		# Rename Binary
		set_target_properties(${project_name} PROPERTIES OUTPUT_NAME "${binary_name}")
	]])
	if parameters.binary_name == "" then
		binaryName = ""
	end

	local compilerOptionsDebug = substitute(parameters, [[
		set(DEBUG_OPTIONS ${debug_options})
		target_compile_options(${project_name} PUBLIC "$<$<CONFIG:DEBUG>:${${DEBUG_OPTIONS}}>")
	]])
	if parameters.debug_options == "" then
		compilerOptionsDebug = ""
	end

	local compilerOptionsRelease = substitute(parameters, [[
		set(RELEASE_OPTIONS ${release_options})
		target_compile_options(${project_name} PUBLIC "$<$<CONFIG:RELEASE>:${${RELEASE_OPTIONS}}>")
	]])
	if parameters.release_options == "" then
		compilerOptionsRelease = ""
	end

	cmake = 
		versionRequired .. newlineIf(cxxStandard) .. 
		cxxStandard .. newlineIf(defaultBuildType) ..
		defaultBuildType .. "\n" ..
		project .. "\n" .. 
		files .. "\n" ..
		addBinary .. newlineIf(binaryName) .. 
		binaryName
		
	if compilerOptionsDebug ~= "" and compilerOptionsRelease ~= "" then
		cmake = cmake .. "\n# Compiler Options\n" ..
			compilerOptionsDebug ..
			compilerOptionsRelease
	end

	return cmake
end

if debugging then
	print(runWizard(getParameters()))
end