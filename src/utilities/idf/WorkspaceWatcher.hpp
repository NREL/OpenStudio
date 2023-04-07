/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
