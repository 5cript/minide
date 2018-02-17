#include "environment.hpp"

#include <cstdio>

extern char **environ;

namespace MinIDE
{
//#####################################################################################################################
    std::vector <std::string> getEnvironmentVariables()
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    void setEnvironment(Environment const& environment)
    {
        char *s = *environ;
        for (int i = 1; s; i++)
        {
            s = *(environ+i);
        }
    }
//#####################################################################################################################
}
