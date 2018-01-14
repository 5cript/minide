#pragma once

#define NONCOPY(name) \
    name& operator=(name const&) = delete; \
    name(name const&) = delete

#define NONMOVE(name) \
    name& operator=(name&&) = delete; \
    name(name&&) = delete
