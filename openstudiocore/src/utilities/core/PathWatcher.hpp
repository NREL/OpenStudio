/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

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
