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

#include "PathWatcher.hpp"
#include "Application.hpp"
#include "Checksum.hpp"
#include "Assert.hpp"
#include "Logger.hpp"

#include <QFileSystemWatcher>
#include <QTimer>

namespace openstudio {

  /// constructor
  PathWatcher::PathWatcher(const openstudio::path& p, int msec)
    : m_impl(new QFileSystemWatcher()),
    m_enabled(true), m_isDirectory(boost::filesystem::is_directory(p) || toString(p.filename())=="." || toString(p.filename())=="/"), 
    m_exists(boost::filesystem::exists(p)), m_dirty(false),
    m_checksum(openstudio::checksum(p)), m_path(p), m_msec(msec)
  {
    // make sure a QApplication exists
    openstudio::Application::instance().application();
    openstudio::Application::instance().processEvents();

    if (m_isDirectory){

      if (!m_exists){
        LOG_FREE_AND_THROW("openstudio.PathWatcher", "Directory '" << openstudio::toString(p) << "' does not exist, cannot be watched");
      }

      bool connected = this->connect(m_impl.get(), SIGNAL(directoryChanged(const QString&)), SLOT(directoryChanged(const QString&)));
      OS_ASSERT(connected);
      m_impl->addPath(openstudio::toQString(p));

    }else{
      m_timer = std::shared_ptr<QTimer>(new QTimer());
      bool connected = this->connect(m_timer.get(), SIGNAL(timeout()), SLOT(checkFile()));
      OS_ASSERT(connected);
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
    m_exists = boost::filesystem::exists(m_path);
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
    bool exists = boost::filesystem::exists(m_path);

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
    bool exists = boost::filesystem::exists(m_path);
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
