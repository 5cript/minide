#include "project_impl.hpp"

namespace MinIDE
{
//#####################################################################################################################
    ProjectImpl::ProjectImpl(GlobalPersistence* settings)
        : settings{settings}
        , localPersistence{}
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
