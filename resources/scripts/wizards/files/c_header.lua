if debugging == nil then
    debugging = true
end

local wizardParameters = {
	include_guard = {
		value = "",
		isOptional = false,
		prettyName = "Include Guard",
		description = "The include guard for this file",
		orderHint = 0,
		validate = function() return true end
	},
	use_pragma_once = {
		value = "false",
		isOptional = true,
		prettyName = "Pragma Once",
		description = "Use pragma once instead of include guard",
		orderHint = 1,
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
	return "file"
end

function getFilters()
	local filters = {
		{
			name = "C Header File", 
			filter = "*.h"
		}
	}
	return filters;
end

function runWizard(parameters)
    --[[
	if parameters == nil then
		if not debugging then
			return 1
		end
    end
    --]]
	local Pretty = require "pl.pretty"
	local Template = require ("pl.text").Template
	local dedent = require("pl.text").dedent

	function substitute(params, str)
		return dedent(Template(str):safe_substitute(params))
	end

	-- takes values out or parameter pack.
	local unziped = extractValues(parameters)

	local header = ""
	if unziped.use_pragma_once == "true" or unziped.use_pragma_once == "True" or unziped.use_pragma_once == true or unziped.use_pragma_once == 1 then
		header = "#pragma once\n\n"
	else
		local includeGuardStart = substitute(unziped, [[
			#ifndef ${include_guard}
			#define ${include_guard}
		]])

		local includeGuardEnd = substitute(unziped, [[
			#endif // ${include_guard}
		]])

		header = 		
			includeGuardStart .. "\n\n\n\n" ..
			includeGuardEnd
	end

	return header
end