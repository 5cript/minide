if debugging == nil then
    debugging = true
end

local wizardParameters = {
	project_name = {
		value = "",
		isOptional = false,
		prettyName = "Project Name",
		description = "The name of the project",
		orderHint = 0,
		validate = function() return true end
	},
	cmake_required = {
		value = "3.11",
		isOptional = false,
		prettyName = "CMake Minimum Version",
		description = "The minimum required CMake version",
		orderHint = 8,
		validate = function() return true end
	},
	cxx_standard = {
		value = "17",
		isOptional = true,
		prettyName = "Default C++ Standard",
		description = "The default C++ standard for this project, empty = dont include default cxx standard",
		orderHint = 7,
		validate = (function()
			return value == ""
		end)
	},
	default_build_type = {
		value = "Release",
		isOptional = true,
		prettyName = "Default Build Type",
		description = "Adds a section to set a default build type, empty = omit section",
		orderHint = 4,
		validate = function() return true end
	},
	is_library = {
		value = false,
		isOptional = false,
		prettyName = "Is Library?",
		description = "Generate Library? (or executable?)",
		orderHint = 5,
		validate = (function() 
			return (
				value:lower() == "true" or 
				value:lower() == "false" or 
				value == "1" or 
				value == "0" or
				value == true or
				value == false
			)
		end)
	},
	library_type = {
		value = "",
		isOptional = true,
		prettyName = "Static/Shared",
		description = "Leave empty if no lib. STATIC/SHARED",
		orderHint = 6,
		validate = (function()
			return value:lower() == "static" or value:lower() == "shared"
		end)
	},
	binary_name = {
		value = "",
		isOptional = true,
		prettyName = "Name of Binary",
		description = "Specify name for binary output, leave empty if binary_name=project_name",
		orderHint = 1,
		validate = function() return true end
	},
	debug_options = {
		value = "-fexecptions -g -Wall -pedantic-errors -pedantic",
		isOptional = true,
		prettyName = "Debug Build Options",
		description = "Build options for debug build, empty = dont set",
		orderHint = 2,
		validate = function() return true end
	},
	release_options = {
		value = "-fexceptions -O3 -Wall -pedantic",
		isOptional = true,
		prettyName = "Release Build Options",
		description = "Build options for release build, empty = dont set",
		orderHint = 3,
		validate = function() return true end
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

	-- takes values out or parameter pack.
	local unziped = extractValues(parameters)

	-- Libraries
	local Pretty = require "pl.pretty"
	local Template = require ("pl.text").Template
	local dedent = require("pl.text").dedent

	-- if debugging then
		Pretty.dump(unziped)
	-- end

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

	local versionRequired = substitute(unziped, [[
		# Version Check
		cmake_minimum_required(VERSION ${cmake_required})
	]])

	local cxxStandard = substitute(unziped, [[
		# Default CXX Standard
		if (NOT "${${CMAKE_CXX_STANDARD}}")
			set(CMAKE_CXX_STANDARD ${cxx_standard})
		endif()
	]])
	if unziped.cxx_standard == 0 then
		cxxStandard = ""
	end

	local defaultBuildType = substitute(unziped, [[
		# Default Build Type
		if (NOT EXISTS ${${CMAKE_BINARY_DIR}}/CMakeCache.txt)
			if (NOT CMAKE_BUILD_TYPE)
				set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE STRING "" FORCE)
			endif()
		endif()
	]])
	if unziped.default_build_type == "" then
		defaultBuildType = ""
	end

	local project = substitute(unziped, [[
		# Project
		project(${project_name})
	]])

	local files = substitute(unziped, [[
		# Add Files
		file(GLOB sources "*.cpp")
	]])

	if unziped.library_type == "" then
		unziped.library_type = wizardParameters.library_type.value
	end
	local addLibrary = substitute(unziped, [[
		# Add Library
		add_library(${project_name} ${library_type} ${${sources}})
	]])

	local addExecutable = substitute(unziped, [[
		# Add Executable
		add_executable(${project_name} ${${sources}})
	]])

	local addBinary = ""
	if unziped.is_library == "true" or unziped.is_library == "True" or unziped.is_library == true or unziped.is_library == 1 then
		addBinary = addLibrary
	else
		addBinary = addExecutable
	end

	local binaryName = substitute(unziped, [[
		# Rename Binary
		set_target_properties(${project_name} PROPERTIES OUTPUT_NAME "${binary_name}")
	]])
	if unziped.binary_name == "" then
		binaryName = ""
	end

	local compilerOptionsDebug = substitute(unziped, [[
		set(DEBUG_OPTIONS ${debug_options})
		target_compile_options(${project_name} PUBLIC "$<$<CONFIG:DEBUG>:${${DEBUG_OPTIONS}}>")
	]])
	if unziped.debug_options == "" then
		compilerOptionsDebug = ""
	end

	local compilerOptionsRelease = substitute(unziped, [[
		set(RELEASE_OPTIONS ${release_options})
		target_compile_options(${project_name} PUBLIC "$<$<CONFIG:RELEASE>:${${RELEASE_OPTIONS}}>")
	]])
	if unziped.release_options == "" then
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
	result[1] = {
		name = "main.cpp",
		content = "#include <iostream>\n\n" ..
			"int main(int argv, char** argc)\n" ..
			"{\n" ..
			"}",
		type = "file"
	}

	return result
end

if debugging then
	print(runWizard(extractValues(getParameters()))[0].content)
end