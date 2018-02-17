#include "project_impl.hpp"

namespace MinIDE
{
//#####################################################################################################################
    ProjectImpl::ProjectImpl(GlobalPersistence* settings)
        : settings{settings}
        , rootDir{}
        , files{}
        , directories{}
        , cb{}
        , projectFile{}
        , localFile{}
    {
    }
//#####################################################################################################################
}
