/**********************************************************************
*  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#include "OSSwitch.hpp"
#include "../model/ModelObject.hpp"
#include "../model/ModelObject_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <boost/optional.hpp>
#include <QString>

namespace openstudio {

OSSwitch2::OSSwitch2( QWidget * parent )
  : QPushButton(parent)
{
  this->setAcceptDrops(false);
  setFlat(true);
  setFixedSize(63,21);

  makeOnOff();

  this->setCheckable(true);
}

void OSSwitch2::makeOnOff()
{
  setObjectName("OnOffSliderButton");
}

void OSSwitch2::makeTrueFalse()
{
  setObjectName("TrueFalseSliderButton");
}

void OSSwitch2::bind(model::ModelObject & modelObject,
                     BoolGetter get,
                     boost::optional<BoolSetter> set,
                     boost::optional<NoFailAction> reset,
                     boost::optional<BasicQuery> isDefaulted)
{
  m_modelObject = modelObject;
  m_get = get;
  m_set = set;
  m_reset = reset;
  m_isDefaulted = isDefaulted;

  connect(m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get(), &openstudio::model::detail::ModelObject_Impl::onChange, this, &OSSwitch2::onModelObjectChange);

  connect(m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get(), &openstudio::model::detail::ModelObject_Impl::onRemoveFromWorkspace, this, &OSSwitch2::onModelObjectRemove);

  connect(this, &OSSwitch2::clicked, this, &OSSwitch2::onClicked);

  onModelObjectChange();
}

void OSSwitch2::unbind()
{
  if (m_modelObject){
    this->disconnect(m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get());
    m_modelObject.reset();
    m_get.reset();
    m_set.reset();
    m_reset.reset();
    m_isDefaulted.reset();
  }
}

void OSSwitch2::onClicked(bool checked)
{
  if(m_modelObject && m_set) {
    if ((*m_get)() != checked) {
      (*m_set)(checked);
    }
  }
}

void OSSwitch2::onModelObjectChange()
{
  if( m_modelObject ) {
    if ((*m_get)() != this->isChecked()) {
      this->blockSignals(true);
      this->setChecked((*m_get)());
      this->blockSignals(false);
    }
  }
}

void OSSwitch2::onModelObjectRemove(Handle handle) {
  unbind();
}

OSSwitch::OSSwitch( QWidget * parent )
  : QPushButton(parent)
{
  this->setAcceptDrops(false);
  setFlat(true);
  setFixedSize(63,21);

  setObjectName("OnOffSliderButton");

  this->setCheckable(true);
}

void OSSwitch::bind(model::ModelObject & modelObject, const char * property)
{
  m_modelObject = modelObject;

  m_property = property;

  connect(m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get(), &openstudio::model::detail::ModelObject_Impl::onChange, this, &OSSwitch::onModelObjectChange);

  connect(m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get(), &openstudio::model::detail::ModelObject_Impl::onRemoveFromWorkspace, this, &OSSwitch::onModelObjectRemove);

  connect(this, &OSSwitch::clicked, this, &OSSwitch::onClicked);

  onModelObjectChange();
}

void OSSwitch::unbind()
{
  if (m_modelObject){
    this->disconnect(m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get());
    m_modelObject.reset();
    m_property = "";
  }
}

void OSSwitch::onClicked(bool checked)
{
  if( m_modelObject )
  {
    m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>()->setProperty(m_property.c_str(),checked);
  }
}

void OSSwitch::onModelObjectChange()
{
  if( m_modelObject )
  {
    QVariant variant = m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>()->property(m_property.c_str());

    if( variant.canConvert<bool>() )
    {
      this->setChecked(variant.value<bool>());
    }
  }
}

void OSSwitch::onModelObjectRemove(Handle handle)
{
  m_modelObject.reset();
  m_property = "";
}

} // openstudio

