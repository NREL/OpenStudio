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

#include "OSCheckBox.hpp"
#include "../model/ModelObject.hpp"
#include "../model/ModelObject_Impl.hpp"
#include <QString>

namespace openstudio {

  OSCheckBox3::OSCheckBox3(QWidget * parent)
    : QCheckBox(parent)
  {
    this->setCheckable(true);

    setEnabled(false);
  }

  void OSCheckBox3::bind(model::ModelObject & modelObject,
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

    setEnabled(true);

    connect(m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get(), &openstudio::model::detail::ModelObject_Impl::onChange, this, &OSCheckBox3::onModelObjectChange);

    connect(m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get(), &openstudio::model::detail::ModelObject_Impl::onRemoveFromWorkspace, this, &OSCheckBox3::onModelObjectRemove);

    connect(this, &OSCheckBox3::toggled, this, &OSCheckBox3::onToggled);
    bool checked = (*m_get)();

    this->setChecked(checked);
  }

  void OSCheckBox3::bind(model::ModelObject & modelObject,
    BoolGetter get,
    boost::optional<BoolSetterBoolReturn> set,
    boost::optional<NoFailAction> reset,
    boost::optional<BasicQuery> isDefaulted)
  {
    m_modelObject = modelObject;
    m_get = get;
    m_setBoolReturn = set;
    m_reset = reset;
    m_isDefaulted = isDefaulted;

    setEnabled(true);

    connect(m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get(), &openstudio::model::detail::ModelObject_Impl::onChange, this, &OSCheckBox3::onModelObjectChange);

    connect(m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get(), &openstudio::model::detail::ModelObject_Impl::onRemoveFromWorkspace, this, &OSCheckBox3::onModelObjectRemove);

    connect(this, &OSCheckBox3::toggled, this, &OSCheckBox3::onToggled);
    bool checked = (*m_get)();

    this->setChecked(checked);
  }

  void OSCheckBox3::unbind()
  {
    if (m_modelObject){
      this->disconnect(m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get());
      m_get.reset();
      m_set.reset();
      m_setBoolReturn.reset();
      m_reset.reset();
      m_isDefaulted.reset();
      setEnabled(false);
    }
  }

  void OSCheckBox3::onToggled(bool checked)
  {
    if (m_modelObject && m_set) {
      if ((*m_get)() != checked) {
        (*m_set)(checked);
      }
    }
    else if (m_modelObject && m_setBoolReturn) {
      if ((*m_get)() != checked) {
        (*m_setBoolReturn)(checked);
      }
    }
  }

  void OSCheckBox3::onModelObjectChange()
  {
    if (m_modelObject) {
      if ((*m_get)() != this->isChecked()) {
        this->blockSignals(true);
        this->setChecked((*m_get)());
        this->blockSignals(false);
      }
    }
  }

  void OSCheckBox3::onModelObjectRemove(Handle handle)
  {
    unbind();
  }

OSCheckBox2::OSCheckBox2( QWidget * parent )
  : QPushButton(parent)
{
  setObjectName("StandardGrayButton");
  this->setAcceptDrops(false);

  this->setCheckable(true);
  
  setEnabled(false);
}

void OSCheckBox2::bind(model::ModelObject & modelObject,
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

  setEnabled(true);

  connect(m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get(), &openstudio::model::detail::ModelObject_Impl::onChange, this, &OSCheckBox2::onModelObjectChange);

  connect(m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get(), &openstudio::model::detail::ModelObject_Impl::onRemoveFromWorkspace, this, &OSCheckBox2::onModelObjectRemove);

  connect(this, &OSCheckBox2::toggled, this, &OSCheckBox2::onToggled);
  bool checked = (*m_get)();

  this->setChecked(checked);
}

void OSCheckBox2::unbind()
{
  if (m_modelObject){
    this->disconnect(m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get());
    m_get.reset();
    m_set.reset();
    m_reset.reset();
    m_isDefaulted.reset();
    setEnabled(false);
  }
}

void OSCheckBox2::onToggled(bool checked)
{
  if(m_modelObject && m_set) {
    if ((*m_get)() != checked) {
      (*m_set)(checked);
    }
  }
}

void OSCheckBox2::onModelObjectChange()
{
  if( m_modelObject ) {
    this->setChecked((*m_get)());
  }
}

void OSCheckBox2::onModelObjectRemove(Handle handle)
{
  unbind();
}

OSCheckBox::OSCheckBox( QWidget * parent )
  : QPushButton(parent)
{
  setObjectName("StandardGrayButton");
  this->setAcceptDrops(false);

  this->setCheckable(true);
  
  setEnabled(false);
}

void OSCheckBox::bind(model::ModelObject & modelObject, const char * property)
{
  m_modelObject = modelObject;

  m_property = property;

  setEnabled(true);

  connect(m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get(), &openstudio::model::detail::ModelObject_Impl::onChange, this, &OSCheckBox::onModelObjectChange);

  connect(m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get(), &openstudio::model::detail::ModelObject_Impl::onRemoveFromWorkspace, this, &OSCheckBox::onModelObjectRemove);

  connect(this, &OSCheckBox::toggled, this, &OSCheckBox::onToggled);
  bool checked = m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>()->property(m_property.c_str()).toBool();

  this->setChecked(checked);
}

void OSCheckBox::unbind()
{
  if (m_modelObject){
    this->disconnect(m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get());
    m_modelObject.reset();
    m_property = "";
    setEnabled(false);
  }
}

void OSCheckBox::onToggled(bool checked)
{
  if( m_modelObject )
  {
    m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>()->setProperty(m_property.c_str(),checked);
  }
}

void OSCheckBox::onModelObjectChange()
{
  if( m_modelObject )
  {
    bool checked = m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>()->property(m_property.c_str()).toBool();

    this->setChecked(checked);
  }
}

void OSCheckBox::onModelObjectRemove(Handle handle)
{
  m_modelObject.reset();
  m_property = "";
  setEnabled(false);
}

} // openstudio

