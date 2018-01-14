#pragma once

#define NONCOPY(name) \
    name& operator=(name const&) = delete; \
    name(name const&) = delete

#define NONMOVE(name) \
    name& operator=(name&&) = delete; \
    name(name&&) = delete

#define COPYABLE(name) \
    name& operator=(name const&) = default; \
    name(name const&) = default

#define MOVEABLE(name) \
    name& operator=(name&&) = default; \
    name(name&&) = default
