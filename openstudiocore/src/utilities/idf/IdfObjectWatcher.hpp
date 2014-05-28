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

#ifndef UTILITIES_IDF_IDFOBJECTWATCHER_HPP
#define UTILITIES_IDF_IDFOBJECTWATCHER_HPP

#include <utilities/UtilitiesAPI.hpp>
#include <utilities/idf/IdfObject.hpp>

#include <QObject>

namespace openstudio{

/** IdfObjectWatcher watches an IdfObject for signals emitted on changes.  These signals will cause 
 *  the watcher to mark the IdfObject as dirty.  The dirty flag will persist until reset by the 
 *  user.  Virtual methods onChangeIdfObject and onDataFieldChange are also available to override. 
 *  The former is called every time the IdfObject changes in any way; the latter is called whenever 
 *  a data field changes (not triggered by comment changes, and is superceded by 
 *  onRelationshipChange for WorkspaceObject pointer fields.) The virtual method onBecomeDirty is 
 *  called only after dirty transitions from false to true.  Alternatively, the virtual method 
 *  onBecomeClean is called only after dirty transitions from true to false.
 *
 *  IdfObjectWatcher is an atypical QObject because it is designed to be stack allocated.  In many 
 *  cases it would be preferred to connect your own heap allocated QObject to the Workspace signals 
 *  directly rather than using this convenience class. **/
class UTILITIES_API IdfObjectWatcher : public QObject {
  Q_OBJECT;
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

 public slots:
  void change();
  void dataChange();
  void nameChange();

 protected:
  bool m_enabled;
  bool m_dirty;
  bool m_dataChanged;
  bool m_nameChanged;
};

} // openstudio

#endif // UTILITIES_IDF_IDFOBJECTWATCHER_HPP
