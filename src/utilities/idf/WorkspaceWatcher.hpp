/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_IDF_WORKSPACEWATCHER_HPP
#define UTILITIES_IDF_WORKSPACEWATCHER_HPP

#include <utilities/UtilitiesAPI.hpp>
#include <utilities/idf/IdfObject.hpp>
#include <utilities/idf/Workspace.hpp>

namespace openstudio {

/** WorkspaceWatcher watches a Workspace for signals emitted on changes.  This signals will cause the
 *  watcher to mark the Workspace  as dirty.  The dirty flag will persist until reset by the user.  A virtual
 *  method onChangeWorkspace is also available to override, it is called everytime the Workspace changes. The
 *  virtual method onBecomeDirty is called only after dirty transitions from false to true.  Alternatively, the
 *  virtual method onBecomeClean is called only after dirty transitions from true to false.
 *
 *  WorkspaceWatcher an atypical object because it is designed to be stack allocated.  In many cases it
 *  would be preferred to connect your own heap allocated object to the Workspace signals directly rather
 *  than using this convenience class.
 **/
class UTILITIES_API WorkspaceWatcher
{

 public:
  WorkspaceWatcher(const Workspace& workspace);

  virtual ~WorkspaceWatcher() = default;

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

  /// called immediately when a new object is added to the workspace if watcher enabled
  /// the object may not be fully constructed
  /// this may be called when an object is restored after a failed remove
  virtual void onObjectAdd(const WorkspaceObject& addedObject);

  /// called immediately before an object is removed from the workspace if watcher enabled
  /// the object is still valid when this method is called
  virtual void onObjectRemove(const WorkspaceObject& removedObject);

  // public slots:

  void change();

  // Note: Args 2 & 3 are simply to comply with Nano::Signal template parameters
  void objectAdd(const WorkspaceObject& addedObject, const openstudio::IddObjectType& type, const openstudio::UUID& uuid);

  // Note: Args 2 & 3 are simply to comply with Nano::Signal template parameters
  void objectRemove(const WorkspaceObject& removedObject, const openstudio::IddObjectType& type, const openstudio::UUID& uuid);

 private:
  bool m_enabled;
  bool m_dirty;
  bool m_objectAdded;
  bool m_objectRemoved;
};

}  // namespace openstudio
#endif
