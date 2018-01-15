#pragma once

#include "copyability.hpp"
#include "filesystem.hpp"
#include <string>
#include <vector>

namespace MinIDE
{
    class FileStoreEntry
    {
    public:
        FileStoreEntry(std::string displayName, path associatedFile, std::string volatileContents);

        /**
         *  save volatile contents to disk.
         */
        void synchronizeToDisk() const;

        /**
         *  load from file into memory.
         */
        void reset();

        /**
         *  Get the file name.
         */
        std::string fileName() const;

        /**
         *  Get associated file.
         */
        path file() const;

        /**
         *  Get display name.
         */
        std::string displayName() const;

        /**
         *  Get volatile contents.
         */
        std::string data() const;

        /**
         *  Set data.
         */
        void data(std::string const& data);

        /**
         *  is synchronized?
         */
        bool isSynchronized() const;

    private:
        std::string displayName_;
        path associatedFile_;
        std::string volatileContents_; // utf8
        mutable bool synchronized_;
    };

    class FileStore
    {
    public:
        FileStore();
        ~FileStore() = default;

        /**
         *  Add a file to the store, unless its already in the store.
         *
         *  @param projectRoot The root path to the project the file belongs to.
         *  @param relativeToRootName The relative path to the file.
         *
         *  @return Returns the new selected index, the currently selected index, if duplicate add,
         *          or returns -1 on failure.
         */
        int add(path const& projectRoot, path const& relativeToRootName);

        /**
         *  Remove selected file from the store.
         *  Will issue a prompt for save/discard changes.
         *
         *  @return Return False if remove was rejected!
         */
        bool remove(std::size_t which);

        /**
         *  Remove selected file from the store.
         *  Will issue a prompt for save/discard changes.
         *
         *  @return Return False if remove was rejected!
         */
        bool remove(std::string const& filename);

        /**
         *  Selects a file in the store, will loop if which > size.
         */
        void select(std::size_t which);

        /**
         *  Get the selected entry.
         */
        FileStoreEntry& selected();

        /**
         *  Gets a file store at the specified index.
         */
        FileStoreEntry& operator[](std::size_t index)
        {
            return files_[index];
        }

        /**
         *  Save memory to disk for all files.
         */
        void synchronizeAllToDisk();

        /**
         *  Return the index.
         */
        std::size_t index() const;

        /**
         *  Returns the size of the file store.
         */
        std::size_t size();

        /**
         *  Returns whether or not the file store is empty.
         */
        bool empty();

    private:
        std::size_t selected_;
        std::vector <FileStoreEntry> files_;
    };
}
