/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <shared_gui_components/OSSwitch.hpp>
#include <model/ModelObject.hpp>
#include <model/ModelObject_Impl.hpp>
#include <boost/optional.hpp>
#include <QString>

namespace openstudio {

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

  bool isConnected = false;
  isConnected = connect( m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get(),SIGNAL(onChange()),
                         this,SLOT(onModelObjectChange()) );
  BOOST_ASSERT(isConnected);

  isConnected = connect( m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get(),SIGNAL(onRemoveFromWorkspace(Handle)),
                         this,SLOT(onModelObjectRemove(Handle)) );
  BOOST_ASSERT(isConnected);

  isConnected = connect( this, SIGNAL(clicked(bool)), this, SLOT(onClicked(bool)) );
  BOOST_ASSERT(isConnected);

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

