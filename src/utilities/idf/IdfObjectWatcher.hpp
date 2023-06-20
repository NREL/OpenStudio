/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
class UTILITIES_API IdfObjectWatcher : public Nano::Observer
{

 public:
  /// constructor with IdfObject to watch
  IdfObjectWatcher(const IdfObject& idfObject);

  /// virtual destructor
  virtual ~IdfObjectWatcher() = default;

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
