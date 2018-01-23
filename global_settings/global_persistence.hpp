#pragma once

#include "../json_public.hpp"
#include "environment.hpp"
#include "cmake_base_settings.hpp"
#include "tools.hpp"

namespace MinIDE
{
    struct GlobalPersistence : public Serializable
                             , public JSON::Stringifiable <GlobalPersistence>
                             , public JSON::Parsable <GlobalPersistence>
    {
        SERIALIZABLE;

        std::unordered_map <std::string, Environment> environments;
        std::unordered_map <std::string, Tools> tooling;
        CMakeBaseSettings cmakeBaseSettings;

        void load();
        void save();

    private:
        mutable filesystem::path homeMemo_;
        filesystem::path getFileName() const;
    };
}

BOOST_FUSION_ADAPT_STRUCT
(
    MinIDE::GlobalPersistence,
    environments, tooling, cmakeBaseSettings
)
