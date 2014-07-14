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

#ifndef UTILITIES_IDF_WORKSPACEWATCHER_HPP
#define UTILITIES_IDF_WORKSPACEWATCHER_HPP

#include <utilities/UtilitiesAPI.hpp>
#include <utilities/idf/IdfObject.hpp>
#include <utilities/idf/Workspace.hpp>
#include <QObject>

namespace openstudio{

/** WorkspaceWatcher watches a Workspace for signals emitted on changes.  This signals will cause the 
 *  watcher to mark the Workspace  as dirty.  The dirty flag will persist until reset by the user.  A virtual
 *  method onChangeWorkspace is also available to override, it is called everytime the Workspace changes. The 
 *  virtual method onBecomeDirty is called only after dirty transitions from false to true.  Alternatively, the 
 *  virtual method onBecomeClean is called only after dirty transitions from true to false.
 *
 *  WorkspaceWatcher an atypical QObject because it is designed to be stack allocated.  In many cases it 
 *  would be preferred to connect your own heap allocated QObject to the Workspace signals directly rather
 *  than using this convenience class.
 **/
class UTILITIES_API WorkspaceWatcher : public QObject {
  Q_OBJECT;
 public:

  WorkspaceWatcher(const Workspace& workspace);

  virtual ~WorkspaceWatcher();

  /// is the watcher currently enabled
  bool enabled() const;

  /// enable the watcher
  void enable();

  // disable watcher, returns true if it was enabled
  bool disable();

  /// true if workspace has changed 
  bool dirty() const;

  /// true if an object has been added 
  bool objectAdded() const;

  /// true if an object has been removed 
  bool objectRemoved() const;

  /// resets dirty, objectAdded, and objectRemoved
  virtual void clearState();

  /// called each time the IdfObject changes if watcher enabled
  virtual void onChangeWorkspace();

  /// called after dirty transitions from false to true if watcher enabled
  virtual void onBecomeDirty();

  /// called after dirty transitions from true to false if watcher enabled
  virtual void onBecomeClean();

  /// called when a new object is added to the workspace if watcher enabled
  /// this may be called when an object is restored after a failed remove
  virtual void onObjectAdd(const WorkspaceObject& addedObject);

  /// called immediately before an object is removed from the workspace if watcher enabled
  /// the object is still valid when this method is called
  virtual void onObjectRemove(const WorkspaceObject& removedObject);

 public slots:

  void change();

  void objectAdd(const WorkspaceObject& addedObject);

  void processAddedObjects();

  void objectRemove(const WorkspaceObject& removedObject);

 private:

  bool m_enabled;
  bool m_dirty;
  bool m_objectAdded;
  bool m_objectRemoved;

  std::vector<WorkspaceObject> m_addedObjects;

};

}
#endif
