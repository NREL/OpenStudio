/**********************************************************************
*  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#ifndef UTILITIES_IDF_WORKSPACEOBJECTWATCHER_HPP
#define UTILITIES_IDF_WORKSPACEOBJECTWATCHER_HPP

#include <utilities/idf/IdfObjectWatcher.hpp>
#include <utilities/idf/WorkspaceObject.hpp>

namespace openstudio {

class UTILITIES_API WorkspaceObjectWatcher : public IdfObjectWatcher {
  Q_OBJECT;
 public:

  /// constructor with IdfObject to watch
  WorkspaceObjectWatcher(const WorkspaceObject& workspaceObject);

  /// virtual destructor
  virtual ~WorkspaceObjectWatcher();

  /// true if a pointer field changed
  bool relationshipChanged() const;

  /// true if the object was removed from its workspace
  bool removedFromWorkspace() const;

  /// reset relationshipChanged and removedFromWorkspace as well as flags from IdfObjectWatcher
  virtual void clearState() override;

  /// called each time a pointer field changes if watcher enabled
  virtual void onRelationshipChange(int index,Handle newHandle,Handle oldHandle);

  /// called when the object is removed from its workspace if watcher enabled
  virtual void onRemoveFromWorkspace(Handle handle);

 public slots:

  void relationshipChange(int index,Handle newHandle,Handle oldHandle);

  void removedFromWorkspace(Handle handle);

 protected:
  bool m_relationshipChanged;
  bool m_removedFromWorkspace;
};

} // openstudio

#endif // UTILITIES_IDF_WORKSPACEOBJECTWATCHER_HPP
