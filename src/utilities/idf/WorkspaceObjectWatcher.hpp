/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_IDF_WORKSPACEOBJECTWATCHER_HPP
#define UTILITIES_IDF_WORKSPACEOBJECTWATCHER_HPP

#include <utilities/idf/IdfObjectWatcher.hpp>
#include <utilities/idf/WorkspaceObject.hpp>

namespace openstudio {

class UTILITIES_API WorkspaceObjectWatcher : public IdfObjectWatcher
{
 public:
  /// constructor with IdfObject to watch
  WorkspaceObjectWatcher(const WorkspaceObject& workspaceObject);

  /// virtual destructor
  virtual ~WorkspaceObjectWatcher() = default;

  /// true if a pointer field changed
  bool relationshipChanged() const;

  /// true if the object was removed from its workspace
  bool removedFromWorkspace() const;

  /// reset relationshipChanged and removedFromWorkspace as well as flags from IdfObjectWatcher
  virtual void clearState() override;

  /// called each time a pointer field changes if watcher enabled
  virtual void onRelationshipChange(int index, Handle newHandle, Handle oldHandle);

  /// called when the object is removed from its workspace if watcher enabled
  virtual void onRemoveFromWorkspace(Handle handle);

  // public slots:

  void relationshipChange(int index, Handle newHandle, Handle oldHandle);

  void removedFromWorkspace(const Handle& handle);

 protected:
  bool m_relationshipChanged;
  bool m_removedFromWorkspace;
};

}  // namespace openstudio

#endif  // UTILITIES_IDF_WORKSPACEOBJECTWATCHER_HPP
