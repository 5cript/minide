if debugging == nil then
    debugging = true
end

local wizardParameters = {
	project_name = {
		value = "",
		isOptional = false,
		prettyName = "Project Name",
		description = "The name of the project",
		orderHint = 0
	},
	cmake_required = {
		value = "3.11",
		isOptional = false,
		prettyName = "CMake Minimum Version",
		description = "The minimum required CMake version",
		orderHint = 8
	},
	cxx_standard = {
		value = "17",
		isOptional = true,
		prettyName = "Default C++ Standard",
		description = "The default C++ standard for this project, empty = dont include default cxx standard",
		orderHint = 7
	},
	default_build_type = {
		value = "Release",
		isOptional = true,
		prettyName = "Default Build Type",
		description = "Adds a section to set a default build type, empty = omit section",
		orderHint = 4
	},
	is_library = {
		value = false,
		isOptional = false,
		prettyName = "Is Library?",
		description = "Generate Library? (or executable?)",
		orderHint = 5
	},
	library_type = {
		value = "",
		isOptional = true,
		prettyName = "Static/Shared",
		description = "Leave empty if no lib. STATIC/SHARED",
		orderHint = 6
	},
	binary_name = {
		value = "",
		isOptional = true,
		prettyName = "Name of Binary",
		description = "Specify name for binary output, leave empty if binary_name=project_name",
		orderHint = 1
	},
	debug_options = {
		value = "-fexecptions -g -Wall -pedantic-errors -pedantic",
		isOptional = true,
		prettyName = "Debug Build Options",
		description = "Build options for debug build, empty = dont set",
		orderHint = 2
	},
	release_options = {
		value = "-fexceptions -O3 -Wall -pedantic",
		isOptional = true,
		prettyName = "Release Build Options",
		description = "Build options for release build, empty = dont set",
		orderHint = 3
	}
}

function getParameters()
	return wizardParameters
end

local function extractValue(param)
	return param.value
end

function extractValues(parameters)
	local valueMap = {}
	for key, value in pairs(parameters) do
		valueMap[key] = value.value
	end
	return valueMap
end

-- types are: directory and file
function getType()
	return "directory"
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

	if parameters.library_type == "" then
		parameters.library_type = wizardParameters.library_type
	end
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

	local result = {}
	result[0] = {
		name = "CMakeLists.txt",
		content = cmake,
		type = "file"
	}

	return result
end

if debugging then
	print(runWizard(extractValues(getParameters()))[0].content)
end