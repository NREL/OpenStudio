/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "IdfObjectWatcher.hpp"
#include "IdfObject_Impl.hpp"

namespace openstudio {

IdfObjectWatcher::IdfObjectWatcher(const IdfObject& idfObject)
  : m_enabled(true), m_dirty(false), m_dataChanged(false), m_nameChanged(false)
{
  detail::IdfObject_ImplPtr objectImpl = idfObject.getImpl<detail::IdfObject_Impl>();
  objectImpl.get()->detail::IdfObject_Impl::onChange.connect<IdfObjectWatcher, &IdfObjectWatcher::change>(this);
  objectImpl.get()->detail::IdfObject_Impl::onDataChange.connect<IdfObjectWatcher, &IdfObjectWatcher::dataChange>(this);
  objectImpl.get()->detail::IdfObject_Impl::onNameChange.connect<IdfObjectWatcher, &IdfObjectWatcher::nameChange>(this);
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
