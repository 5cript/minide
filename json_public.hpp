#pragma once

#include <SimpleJSON/stringify/jss_fusion_adapted_struct.hpp>
#include <SimpleJSON/parse/jsd_fusion_adapted_struct.hpp>
#include <SimpleJSON/parse/jsd_convenience.hpp>

#include "filesystem.hpp"

namespace MinIDE
{
    struct Serializable
    {
        virtual void save(filesystem::path const& p) const = 0;
        virtual void load(filesystem::path const& p) = 0;
    };
}

#define SERIALIZABLE \
    void save(path const& p) const override; \
    void load(path const& p) override

#define IMPLEMENT_SERIALIZABLE(name) \
    void name::save(filesystem::path const& p) const { \
        JSON::StringificationOptions opts; \
        opts.strings_are_binary = true; \
        auto writer = std::ofstream{p.string(), std::ios_base::binary}; \
        JSON::stringify(writer, "minide", *this, opts); \
    } \
    void name::load(filesystem::path const& p) { \
        JSON::ParsingOptions opts; \
        opts.strings_are_binary = true; \
        auto stream = std::ifstream{p.string(), std::ios_base::binary}; \
        auto tree = JSON::parse_json(stream); \
        JSON::fill_missing <decltype(*this)> ("", tree); \
        JSON::parse(*this, "", tree, opts); \
    }
