#include "global_persistence.hpp"
#include <special-paths/special_paths.hpp>

#include <SimpleJSON/stringify/jss.hpp>
#include <SimpleJSON/parse/jsd.hpp>
#include <SimpleJSON/utility/fill_missing.hpp>

namespace MinIDE
{
//#####################################################################################################################
    IMPLEMENT_SERIALIZABLE(GlobalPersistence, true)
//---------------------------------------------------------------------------------------------------------------------
    void GlobalPersistence::load()
    {
        auto file = getFileName();
        if (filesystem::exists(file))
            load(file);
        else
            save();
    }
//---------------------------------------------------------------------------------------------------------------------
    void GlobalPersistence::save()
    {
        save(getFileName());
    }
//---------------------------------------------------------------------------------------------------------------------
    filesystem::path GlobalPersistence::getFileName() const
    {
        if (homeMemo_.empty())
        {
            auto home = path{SpecialPaths::getHome()};
            if (!filesystem::exists(home / ".minIDE"))
            {
                filesystem::create_directory(home / ".minIDE");
            }
            homeMemo_ = home / ".minIDE" / "persistence.json";
        }
        return homeMemo_;
    }
//#####################################################################################################################

}
