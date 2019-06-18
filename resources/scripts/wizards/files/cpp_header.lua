if debugging == nil then
    debugging = true
end

local wizardParameters = {}

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
			name = "C++ Header File", 
			filter = "*.hpp"
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

	-- takes values out or parameter pack.
	-- local unziped = extractValues(parameters)

	return "#pragma once\n\n"
end