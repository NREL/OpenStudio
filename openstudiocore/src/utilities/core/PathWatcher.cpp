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

#include "PathWatcher.hpp"
#include "Application.hpp"
#include "Checksum.hpp"
#include "Assert.hpp"

#include <QFileSystemWatcher>
#include <QTimer>

namespace openstudio {

  /// constructor
  PathWatcher::PathWatcher(const openstudio::path& p, int msec)
    : m_impl(new QFileSystemWatcher()),
    m_enabled(true), m_isDirectory(openstudio::filesystem::is_directory(p) || toString(p.filename())=="." || toString(p.filename())=="/"),
    m_exists(openstudio::filesystem::exists(p)), m_dirty(false),
    m_checksum(openstudio::checksum(p)), m_path(p), m_msec(msec)
  {
    // make sure a QApplication exists
    openstudio::Application::instance().application(false);
    openstudio::Application::instance().processEvents();

    if (m_isDirectory){

      if (!m_exists){
        LOG_FREE_AND_THROW("openstudio.PathWatcher", "Directory '" << openstudio::toString(p) << "' does not exist, cannot be watched");
      }

      connect(m_impl.get(), &QFileSystemWatcher::directoryChanged, this, &PathWatcher::directoryChanged);
      m_impl->addPath(openstudio::toQString(p));

    }else{
      m_timer = std::shared_ptr<QTimer>(new QTimer());
      connect(m_timer.get(), &QTimer::timeout, this, &PathWatcher::checkFile);
      m_timer->start(m_msec);

      // DLM: do not use QFileSystemWatcher to watch individual files, was acting glitchy
      //connected = this->connect(m_impl.get(), SIGNAL(fileChanged(const QString&)), SLOT(fileChanged(const QString&)));
      //OS_ASSERT(connected);
      //m_impl->addPath(openstudio::toQString(p));
    }

  }

  PathWatcher::~PathWatcher()
  {}

  bool PathWatcher::enabled() const
  {
    return m_enabled;
  }

  void PathWatcher::enable()
  {
    m_enabled = true;

    if (!m_timer->isActive()){
      m_timer->start(m_msec);
    }
  }

  bool PathWatcher::disable()
  {
    if (m_timer->isActive()){
      m_timer->stop();
    }

    bool result = m_enabled;
    m_enabled = false;
    return result;
  }

  openstudio::path PathWatcher::path() const
  {
    return m_path;
  }

  bool PathWatcher::dirty() const
  {
    return m_dirty;
  }

  void PathWatcher::clearState()
  {
    m_exists = openstudio::filesystem::exists(m_path);
    m_dirty = false;
    m_checksum = openstudio::checksum(m_path);
  }

  void PathWatcher::onPathAdded()
  {
  }

  void PathWatcher::onPathChanged()
  {
  }

  void PathWatcher::onPathRemoved()
  {
  }

  void PathWatcher::directoryChanged(const QString & path)
  {
    bool exists = openstudio::filesystem::exists(m_path);

    if (m_exists && exists){

      // regular change
      m_dirty = true;

      if (m_enabled){
        onPathChanged();
      }

    }else if (m_exists && !exists){

      // used to exist, now does not
      m_exists = exists;
      m_dirty = true;

      if (m_enabled){
        onPathRemoved();
      }

    }else if (!m_exists && exists){

      // did not exist, now does
      m_exists = exists;
      m_dirty = true;

      if (m_enabled){
        onPathAdded();
      }

    }else{
      // should never get this
    }
  }

  void PathWatcher::fileChanged(const QString & path)
  {
    checkFile();
  }

  void PathWatcher::checkFile()
  {
    bool exists = openstudio::filesystem::exists(m_path);
    std::string checksum = openstudio::checksum(m_path);

    if (checksum == "00000000"){
      exists = false;
    }

    if (m_exists && exists){

      // first check checksum
      if (checksum != m_checksum){
        m_dirty = true;
        m_checksum = checksum;

        // regular change
        if (m_enabled){
          onPathChanged();
        }
      }

    }else if (m_exists && !exists){

      // used to exist, now does not
      m_dirty = true;
      m_exists = exists;
      m_checksum = openstudio::checksum(m_path);

      if (m_enabled){
        onPathRemoved();
      }

    }else if (!m_exists && exists){

      // did not exist, now does
      m_dirty = true;
      m_exists = exists;
      m_checksum = openstudio::checksum(m_path);

      if (m_enabled){
        onPathAdded();
      }

    }else{
      // !m_exists && !exists
      // no change
    }
  }

}
