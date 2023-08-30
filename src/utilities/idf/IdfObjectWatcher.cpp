/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "IdfObjectWatcher.hpp"
#include "IdfObject_Impl.hpp"

namespace openstudio {

IdfObjectWatcher::IdfObjectWatcher(const IdfObject& idfObject) : m_enabled(true), m_dirty(false), m_dataChanged(false), m_nameChanged(false) {
  detail::IdfObject_ImplPtr objectImpl = idfObject.getImpl<detail::IdfObject_Impl>();
  objectImpl.get()->detail::IdfObject_Impl::onChange.connect<IdfObjectWatcher, &IdfObjectWatcher::change>(this);
  objectImpl.get()->detail::IdfObject_Impl::onDataChange.connect<IdfObjectWatcher, &IdfObjectWatcher::dataChange>(this);
  objectImpl.get()->detail::IdfObject_Impl::onNameChange.connect<IdfObjectWatcher, &IdfObjectWatcher::nameChange>(this);
}

bool IdfObjectWatcher::enabled() const {
  return m_enabled;
}

void IdfObjectWatcher::enable() {
  m_enabled = true;
}

bool IdfObjectWatcher::disable() {
  bool result = m_enabled;
  m_enabled = false;
  return result;
}

bool IdfObjectWatcher::dirty() const {
  return m_dirty;
}

bool IdfObjectWatcher::dataChanged() const {
  return m_dataChanged;
}

bool IdfObjectWatcher::nameChanged() const {
  return m_nameChanged;
}

void IdfObjectWatcher::clearState() {
  bool wasDirty = m_dirty;
  m_dirty = false;
  m_dataChanged = false;
  m_nameChanged = false;

  if (enabled()) {
    if (wasDirty) {
      onBecomeClean();
    }
  }
}

void IdfObjectWatcher::onChangeIdfObject() {}

void IdfObjectWatcher::onDataFieldChange() {}

void IdfObjectWatcher::onNameChange() {}

void IdfObjectWatcher::onBecomeDirty() {}

void IdfObjectWatcher::onBecomeClean() {}

void IdfObjectWatcher::change() {
  bool wasDirty = m_dirty;
  m_dirty = true;

  if (enabled()) {
    if (!wasDirty) {
      onBecomeDirty();
    }
    onChangeIdfObject();
  }
}

void IdfObjectWatcher::dataChange() {
  m_dataChanged = true;

  if (enabled()) {
    onDataFieldChange();
  }
}

void IdfObjectWatcher::nameChange() {
  m_nameChanged = true;

  if (enabled()) {
    onNameChange();
  }
}

}  // namespace openstudio
