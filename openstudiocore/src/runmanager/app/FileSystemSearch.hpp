/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#ifndef RUNMANAGER_APP_FILESYSTEMSEARCH_HPP
#define RUNMANAGER_APP_FILESYSTEMSEARCH_HPP


#include <vector>
#include <string>
#include "../../utilities/core/Path.hpp"
#include <QStandardItemModel>
#include <boost/regex.hpp>
#include "../../utilities/core/Logger.hpp"
#include <QThread>
#include <boost/filesystem.hpp>

namespace openstudio {
namespace runmanager {

  namespace detail {
    class FileSystemSearchThread : public QThread
    {
      Q_OBJECT;

      public:
        template<typename InItr>
          FileSystemSearchThread(
              const openstudio::path &rootpath,
              InItr begin,
              const InItr &end,
              const QRegExp &regex, const std::string &extension)
            : m_canceled(false),
              m_fileBuilder(std::bind(&FileSystemSearchThread::buildFileList<InItr>,
                  this, rootpath, begin, end, regex, extension))
          {
          }


        void cancel()
        {
          m_canceled = true;
        }


      signals:
        void fileFound(const QString &);

      protected:
        virtual void run();

      private:
        REGISTER_LOGGER("openstudio.runmanager.detail.FileSystemSearchThread");

        void no_push(boost::filesystem::recursive_directory_iterator &i)
        {
          i.no_push();
        }

        void no_push(boost::filesystem::directory_iterator &)
        {
          // the simple iterator does not have nopush - only recursive has it
        }

        void pop(boost::filesystem::recursive_directory_iterator &i)
        {
          i.pop();
        }

        void pop(boost::filesystem::directory_iterator &)
        {
          // the simple iterator does not have pop - only recursive has it
          // but calling it on non-recursive will not help us here
          throw std::runtime_error("Invalid call of pop on non-recursive directory iterator");
        }

        int level(boost::filesystem::recursive_directory_iterator &i)
        {
          return i.level();
        }

        int level(boost::filesystem::directory_iterator &)
        {
          // the simple iterator does not have level - return 0
          return 0;
        }


        /// Build a list of files from the given iterators that match
        /// the requirements
        template<typename InItr>
          void buildFileList(
              openstudio::path rootpath,
              InItr begin,
              InItr end,
              QRegExp regex, std::string extension)
          {
            //          LOG(Debug, "Building File List regex: " << regex << " empty? " << regex.empty() << " extension " 
            //              << extension << " empty? " << extension.empty());
            std::set<QString> foundFiles;

            int rootlength = std::distance(rootpath.begin(), rootpath.end());

            if (!extension.empty())
            {
              if (extension[0] != '.')
              {
                extension = "." + extension;
              }
            }

            while (!m_canceled && begin != end)
            {
              try {
                // We do not want to recurse into symlink dirs
                if (boost::filesystem::is_directory(*begin)
                    && boost::filesystem::is_symlink(*begin))
                {
                  no_push(begin);
                }


                // We only want files, not directories
                if (!boost::filesystem::is_directory(*begin))
                {
                  openstudio::path pstart = *begin;
                  openstudio::path::iterator pathbegin = pstart.begin();
                  openstudio::path::iterator pathend = pstart.end();

                  std::advance(pathbegin, rootlength);
                  openstudio::path p;

                  while (pathbegin != pathend)
                  {
                    p /= *pathbegin;
                    ++pathbegin;
                  }

                  QString filestring = toQString(p.native());

                  if (   (extension.empty() || toString(p.extension()) == extension)
                      && (regex.isEmpty() || regex.exactMatch(filestring))
                     )
                  {
//                    LOG(Debug, "FileFound " << toString(p) << " thread: " << QThread::currentThreadId());
                    if (foundFiles.count(filestring) != 0)
                    {
                      assert(!"This shouldn't happen");
                    } else {
                      foundFiles.insert(filestring);
                      emit fileFound(filestring);
                    }

                  }
                }
              } catch (const boost::filesystem::filesystem_error &e) {
                //Ignore FS errors
                LOG(Debug, "FileSystem Error: " << e.what());
              }

              try {
                ++begin;
              } catch (const boost::filesystem::filesystem_error &) {
                no_push(begin);
                ++begin;
              }
            }
          }

        volatile bool m_canceled;
        std::function<void ()> m_fileBuilder;
    };
  }

  class FileSystemSearch : public QObject
  {
    Q_OBJECT;

    public:
      /// Constructor
      FileSystemSearch(const openstudio::path &t_rootPath, bool t_recursive,
          const std::string &t_fileExtension,
          const QRegExp &t_regex);

      /// Returns the recursive nature of the search
      /// \sa setRecursive
      bool recursive() const;

      /// Sets the recursive nature of the search. If recursive is true, all matching
      /// files are presented to the user
      void setRecursive(bool);

      /// Returns the root path that the file list is built from
      openstudio::path rootPath() const;

      /// Sets the root path that the file list is built from
      void setRootPath(const openstudio::path &);

      /// Returns the regex that is used to build file list. The regex
      /// is a partial match against the entire file name (relative path included)
      /// if any part matches, the match is good. Default settings for boost::regex will be used
      QRegExp regex() const;

      /// sets the regex to match when building the file list
      void setRegex(const QRegExp &);

      /// returns the fileExtension that is matched while building file list
      std::string fileExtension() const;

      /// sets the fileExtension to match
      void setFileExtension(const std::string &);

      /// Returns the QAbstractItemModel for connecting to model viewer in the UI
      QAbstractItemModel *getQItemModel();

      /// Returns the set of all files selected by the user
      std::vector<openstudio::path> selectedFiles();

      /// Returns the set of all files found with the current search criteria
      std::vector<openstudio::path> foundFiles();

      /// Select all currently found files
      void selectAll();

      /// Deselect all currently found files
      void selectNone();

      /// Invert the selection state of all found files
      void invertSelection();
     
      /// Select all files in the given QModelIndexList
      void select(const QModelIndexList &mil); 

      /// Returns the path of the given file index
      openstudio::path getFile(const QModelIndex &idx);

    signals:
      void errorInRegex(const QString &);
      void rootPathChanged(const openstudio::path &);
      void searchStarted();
      void searchComplete();

    private slots:
      void fileFoundSlot(const QString &);
      void threadFinished();
    //  void itemChanged(QStandardItem *);

    private:
      REGISTER_LOGGER("openstudio.runmanager.FileSystemSearch");

      /// Update the UI model
      void updateModel();


      openstudio::path m_rootPath;
      bool m_recursive;
      std::string m_fileExtension;
      std::shared_ptr<detail::FileSystemSearchThread> m_thread;

      QRegExp m_regex;

      std::vector<openstudio::path> m_foundFiles;

      QStandardItemModel m_model;
      QStandardItem *m_header;
  };

}
}


#endif // RUNMANAGER_APP_FILESYSTEMSEARCH_HPP
