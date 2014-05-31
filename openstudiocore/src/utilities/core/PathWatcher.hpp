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

#ifndef UTILITIES_CORE_PATHWATCHER_HPP
#define UTILITIES_CORE_PATHWATCHER_HPP

#include "../UtilitiesAPI.hpp"
#include "Path.hpp"

#include <QObject>
#include <QString>

// forward declarations
class QFileSystemWatcher;
class QTimer;

namespace openstudio {

  /** Class for watching either a file or directory, QFileSystemWatcher has issues when watching 
   **  many files so it is not recommended to use too many of these objects.
   **/
  class UTILITIES_API PathWatcher : public QObject{

    Q_OBJECT;

    public:

      /// constructor with path

      /// if path is a directory it must exist at time of construction, no periodic checks are performed for directory
      /// if path is not a directory it is assumed to be a regular file which may or may not exist at construction, 
      /// a timer is used to periodically check for changes to the file
      /// msec is the timer delay to check for updates to the file, msec does not apply if the path is a directory
      PathWatcher(const openstudio::path& p, int msec = 1000);

      /// virtual destructor
      virtual ~PathWatcher();

      /// is the watcher currently enabled
      bool enabled() const;

      /// enable the watcher
      void enable();

      // disable watcher, returns true if it was enabled
      bool disable();

      /// path that is being watched
      openstudio::path path() const;

      /// true if path has been changed
      bool dirty() const;

      /// reset state to clean
      void clearState(); 

      /// for files, called when watched file is added if watcher enabled
      virtual void onPathAdded();

      /// for files, called when watched file is modified if watcher enabled
      /// for directories, called when file/directory is added or removed to the watched directory if watcher enabled
      virtual void onPathChanged();

      /// for files, called when watched file is removed if watcher enabled
      /// for directories, called when the watched directory is removed if watcher enabled
      virtual void onPathRemoved();

    public slots:

      /// called when file/directory is added or removed or when directory is removed 
      void directoryChanged(const QString& path);

      /// called when file is modified or removed
      void fileChanged(const QString& path);

      /// periodically check for changes
      void checkFile();

    private:

      /// impl
      std::shared_ptr<QFileSystemWatcher> m_impl;
      std::shared_ptr<QTimer> m_timer;

      bool m_enabled;
      bool m_isDirectory;
      bool m_exists;
      bool m_dirty;
      std::string m_checksum;
      openstudio::path m_path;
      int m_msec;

  };

} // openstudio

#endif // UTILITIES_CORE_PATHWATCHER_HPP
