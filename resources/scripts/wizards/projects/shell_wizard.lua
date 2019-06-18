if debugging == nil then
    debugging = true
end

local wizardParameters = {
    script_name = {
        value = "build.sh",
		isOptional = false,
		prettyName = "Shell Script Name",
		description = "The name of the shell script",
		orderHint = 0,
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

	local result = {}
	result[0] = {
		name = unziped.script_name,
		content = "#!/bin/bash",
		type = "file"
	}

	return result
end