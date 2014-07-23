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

#include "IdfObjectWatcher.hpp"
#include "IdfObject_Impl.hpp"
#include "../core/Application.hpp"
#include "../core/Assert.hpp"

namespace openstudio {

IdfObjectWatcher::IdfObjectWatcher(const IdfObject& idfObject)
  : m_enabled(true), m_dirty(false), m_dataChanged(false), m_nameChanged(false)
{
  // make sure a QApplication exists
  openstudio::Application::instance().application();
  
  detail::IdfObject_ImplPtr objectImpl = idfObject.getImpl<openstudio::detail::IdfObject_Impl>();
  bool connected = this->connect(objectImpl.get(), SIGNAL(onChange()), SLOT(change()));
  connected = connected && this->connect(objectImpl.get(), SIGNAL(onDataChange()), SLOT(dataChange()));
  connected = connected && this->connect(objectImpl.get(), SIGNAL(onNameChange()), SLOT(nameChange()));
  
  OS_ASSERT(connected);
}

IdfObjectWatcher::~IdfObjectWatcher()
{
}

bool IdfObjectWatcher::enabled() const
{
  return m_enabled;
}

void IdfObjectWatcher::enable()
{
  m_enabled = true;
}

bool IdfObjectWatcher::disable()
{
  bool result = m_enabled;
  m_enabled = false;
  return result;
}

bool IdfObjectWatcher::dirty() const
{
  return m_dirty;
}

bool IdfObjectWatcher::dataChanged() const 
{
  return m_dataChanged;
}

bool IdfObjectWatcher::nameChanged() const 
{
  return m_nameChanged;
}

void IdfObjectWatcher::clearState()
{
  bool wasDirty = m_dirty;
  m_dirty = false;
  m_dataChanged = false;
  m_nameChanged = false;
  
  if (enabled()){
    if (wasDirty){ 
      onBecomeClean(); 
    }
  }
}

void IdfObjectWatcher::onChangeIdfObject()
{
}

void IdfObjectWatcher::onDataFieldChange() 
{
}

void IdfObjectWatcher::onNameChange()
{
}

void IdfObjectWatcher::onBecomeDirty()
{
}

void IdfObjectWatcher::onBecomeClean()
{
}

void IdfObjectWatcher::change() 
{
  bool wasDirty = m_dirty;
  m_dirty = true;

  if (enabled()){
    if (!wasDirty){ 
      onBecomeDirty(); 
    }
    onChangeIdfObject();
  }
}

void IdfObjectWatcher::dataChange() 
{
  m_dataChanged = true;

  if (enabled()){
    onDataFieldChange();
  }
}

void IdfObjectWatcher::nameChange() 
{
  m_nameChanged = true;

  if (enabled()){
    onNameChange();
  }
}
  
} // openstudio
