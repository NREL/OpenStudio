/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef UTILITIES_IDF_IDFOBJECTWATCHER_HPP
#define UTILITIES_IDF_IDFOBJECTWATCHER_HPP

#include <nano/nano_signal_slot.hpp>  // Signal-Slot replacement
#include <utilities/UtilitiesAPI.hpp>
#include <utilities/idf/IdfObject.hpp>

namespace openstudio {

/** IdfObjectWatcher watches an IdfObject for signals emitted on changes.  These signals will cause
 *  the watcher to mark the IdfObject as dirty.  The dirty flag will persist until reset by the
 *  user.  Virtual methods onChangeIdfObject and onDataFieldChange are also available to override.
 *  The former is called every time the IdfObject changes in any way; the latter is called whenever
 *  a data field changes (not triggered by comment changes, and is superceded by
 *  onRelationshipChange for WorkspaceObject pointer fields.) The virtual method onBecomeDirty is
 *  called only after dirty transitions from false to true.  Alternatively, the virtual method
 *  onBecomeClean is called only after dirty transitions from true to false.
 *
 *  IdfObjectWatcher is an atypical object because it is designed to be stack allocated.  In many
 *  cases it would be preferred to connect your own heap allocated object to the Workspace signals
 *  directly rather than using this convenience class. **/
class UTILITIES_API IdfObjectWatcher : public Nano::Observer<>
{

 public:
  /// constructor with IdfObject to watch
  IdfObjectWatcher(const IdfObject& idfObject);

  /// virtual destructor
  virtual ~IdfObjectWatcher();

  /// is the watcher currently enabled
  bool enabled() const;

  /// enable the watcher
  void enable();

  // disable watcher, returns true if it was enabled
  bool disable();

  /// true if object has changed
  bool dirty() const;

  /// if dirty, then true if data field changed, false if some other change
  bool dataChanged() const;

  /// if dirty, then true if name field changed, false otherwise
  bool nameChanged() const;

  /// reset dirty, dataChanged, and nameChanged
  virtual void clearState();

  /// called each time the IdfObject changes if watcher enabled
  virtual void onChangeIdfObject();

  /// called each time one of the IdfObject's data fields changes if watcher enabled
  virtual void onDataFieldChange();

  /// called each time the IdfObject's name changes if watcher enabled
  virtual void onNameChange();

  /// called after dirty transitions from false to true if watcher enabled
  virtual void onBecomeDirty();

  /// called after dirty transitions from true to false if watcher enabled
  virtual void onBecomeClean();

  // public slots:
  void change();
  void dataChange();
  void nameChange();

 protected:
  bool m_enabled;
  bool m_dirty;
  bool m_dataChanged;
  bool m_nameChanged;
};

}  // namespace openstudio

#endif  // UTILITIES_IDF_IDFOBJECTWATCHER_HPP
