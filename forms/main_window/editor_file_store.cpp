#include "editor_file_store.hpp"

#include <fstream>

#include <nana/gui.hpp>
#include <nana/gui/msgbox.hpp>

namespace MinIDE
{
//#####################################################################################################################
    FileStoreEntry::FileStoreEntry(std::string displayName, path associatedFile, std::string volatileContents)
        : displayName_{std::move(displayName)}
        , associatedFile_{std::move(associatedFile)}
        , volatileContents_{std::move(volatileContents_)}
        , synchronized_{true}
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    void FileStoreEntry::synchronizeToDisk() const
    {
        std::ofstream writer(associatedFile_.string(), std::ios_base::binary);
        writer.write(volatileContents_.data(), volatileContents_.size());
        synchronized_ = true;
    }
//---------------------------------------------------------------------------------------------------------------------
    void FileStoreEntry::reset()
    {
        std::ifstream reader(associatedFile_.string(), std::ios_base::binary);
        reader.seekg(0ul, std::ios::end);
        volatileContents_.resize(reader.tellg());
        reader.seekg(0ul);
        // casting away const on std::string::data is guaranteed to be safe as of C++11.
        reader.read(const_cast <char*> (volatileContents_.data()), volatileContents_.size());
        synchronized_ = true;
    }
//---------------------------------------------------------------------------------------------------------------------
    std::string FileStoreEntry::fileName() const
    {
        return associatedFile_.filename().string();
    }
//---------------------------------------------------------------------------------------------------------------------
    std::string FileStoreEntry::data() const
    {
        return volatileContents_;
    }
//---------------------------------------------------------------------------------------------------------------------
    void FileStoreEntry::data(std::string const& data)
    {
        synchronized_ = false;
        volatileContents_ = data;
    }
//---------------------------------------------------------------------------------------------------------------------
    std::string FileStoreEntry::displayName() const
    {
        return displayName_;
    }
//---------------------------------------------------------------------------------------------------------------------
    path FileStoreEntry::file() const
    {
        return associatedFile_;
    }
//---------------------------------------------------------------------------------------------------------------------
    bool FileStoreEntry::isSynchronized() const
    {
        return synchronized_;
    }
//#####################################################################################################################
    FileStore::FileStore()
        : selected_{0}
        , files_{}
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    int FileStore::add(path const& projectRoot, path const& relativeToRootName)
    {
        auto absolute = projectRoot / relativeToRootName;

        if (!filesystem::exists(absolute))
            return -1;

        if (filesystem::is_directory(absolute))
            return -1;

        bool foundDuplicateName = false;
        for (std::size_t i = 0; i != files_.size(); ++i)
        {
            auto& file = files_[i];

            if (file.displayName() == relativeToRootName.filename().string())
            {
                foundDuplicateName = true;
                if (file.file() == absolute)
                {
                    select(i);
                    return i;
                }
            }
            else if (file.displayName() == relativeToRootName.string() && file.file() == absolute)
            {
                select(i);
                return i;
            }
        }

        FileStoreEntry entry{
            (foundDuplicateName ? relativeToRootName : relativeToRootName.filename()).string(),
            absolute,
            {}
        };
        entry.reset();

        files_.push_back(entry);
        select(size() - 1);
        return index();
    }
//---------------------------------------------------------------------------------------------------------------------
    void FileStore::synchronizeAllToDisk()
    {
        for (auto const& file : files_)
        {
            file.synchronizeToDisk();
        }
    }
//---------------------------------------------------------------------------------------------------------------------
    std::size_t FileStore::index() const
    {
        return selected_;
    }
//---------------------------------------------------------------------------------------------------------------------
    bool FileStore::remove(std::size_t which)
    {
        if (which < 0 || which >= size())
            return false;

        // ISSUE PROMPT
        if (!files_[which].isSynchronized())
        {
            nana::msgbox box(nullptr, "Data Loss Possible", nana::msgbox::yes_no_cancel);
            box.icon(box.icon_warning);
            box << "Do you want to save the file first?";
            auto result = box.show();
            if (result == nana::msgbox::pick_yes)
                files_[which].synchronizeToDisk();
            else if (result == nana::msgbox::pick_cancel)
                return false;
        }

        bool mustReselect = (selected_ == which);

        files_.erase(std::begin(files_) + which);

        if (mustReselect)
            select(selected_);

        return true;
    }
//---------------------------------------------------------------------------------------------------------------------
    bool FileStore::remove(std::string const& filename)
    {
        std::size_t counter = 0;
        for (auto const& i : files_)
        {
            if (i.file().string() == filename)
                break;
            ++counter;
        }
        if (counter == size())
            return false;
        remove(counter);
        return true;
    }
//---------------------------------------------------------------------------------------------------------------------
    void FileStore::select(std::size_t which)
    {
        if (empty())
            return;
        selected_ = std::max(which, std::size_t{0}) % files_.size();
    }
//---------------------------------------------------------------------------------------------------------------------
    std::size_t FileStore::size()
    {
        return files_.size();
    }
//---------------------------------------------------------------------------------------------------------------------
    FileStoreEntry& FileStore::selected()
    {
        return files_[selected_];
    }
//---------------------------------------------------------------------------------------------------------------------
    bool FileStore::empty()
    {
        return files_.empty();
    }
//#####################################################################################################################
}
