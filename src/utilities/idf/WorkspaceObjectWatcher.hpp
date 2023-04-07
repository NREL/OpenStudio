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
