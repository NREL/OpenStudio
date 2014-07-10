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

#include "OSLineEdit.hpp"
#include "../model/ModelObject.hpp"
#include "../model/ModelObject_Impl.hpp"
#include "../utilities/core/Assert.hpp"
#include <boost/optional.hpp>
#include <QString>

namespace openstudio {

OSLineEdit2::OSLineEdit2( QWidget * parent )
  : QLineEdit(parent)
{
  this->setAcceptDrops(false);
  setEnabled(false);
}

void OSLineEdit2::bind(model::ModelObject& modelObject,
                       StringGetter get,
                       boost::optional<StringSetter> set,
                       boost::optional<NoFailAction> reset,
                       boost::optional<BasicQuery> isDefaulted)
{
  m_modelObject = modelObject;
  m_get = get;
  m_set = set;
  m_reset = reset;
  m_isDefaulted = isDefaulted;

  completeBind();
}

void OSLineEdit2::bind(model::ModelObject& modelObject, 
                       OptionalStringGetter get,
                       boost::optional<StringSetter> set,
                       boost::optional<NoFailAction> reset,
                       boost::optional<BasicQuery> isDefaulted)
{
  m_modelObject = modelObject;
  m_getOptional = get;
  m_set = set;
  m_reset = reset;
  m_isDefaulted = isDefaulted;

  completeBind();
}

void OSLineEdit2::bind(model::ModelObject& modelObject,
                       OptionalStringGetterBoolArg get,
                       boost::optional<StringSetterOptionalStringReturn> set,
                       boost::optional<NoFailAction> reset,
                       boost::optional<BasicQuery> isDefaulted)
{
  m_modelObject = modelObject;
  m_getOptionalBoolArg = get;
  m_setOptionalStringReturn = set;
  m_reset = reset;
  m_isDefaulted = isDefaulted;

  completeBind();
}

void OSLineEdit2::completeBind() {
  setEnabled(true);

  bool isConnected = false;
  isConnected = connect( m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get(),SIGNAL(onChange()),
                         this,SLOT(onModelObjectChange()) );
  OS_ASSERT(isConnected);

  isConnected = connect( m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get(),SIGNAL(onRemoveFromWorkspace(Handle)),
                         this,SLOT(onModelObjectRemove(Handle)) );
  OS_ASSERT(isConnected);

  isConnected = connect( this, SIGNAL(editingFinished()), this, SLOT(onEditingFinished()) );
  OS_ASSERT(isConnected);

  onModelObjectChange();
}

void OSLineEdit2::unbind()
{
  if (m_modelObject){
    this->disconnect(m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get());
    m_modelObject.reset();
    m_get.reset();
    m_getOptional.reset();
    m_getOptionalBoolArg.reset();
    m_set.reset();
    m_setOptionalStringReturn.reset();
    m_reset.reset();
    m_isDefaulted.reset();
    setEnabled(false);
  }
}

void OSLineEdit2::onEditingFinished() {
  if(m_modelObject && m_set) {
    bool result = (*m_set)(this->text().toStdString());
    if (!result){
      //restore
      onModelObjectChange();
    }
  }
}

void OSLineEdit2::onModelObjectChange() {
  if( m_modelObject ) {
    OptionalString value;
    if (m_get) {
      value = (*m_get)();
    }
    else if (m_getOptional){
      value = (*m_getOptional)();
    }
    else if (m_getOptionalBoolArg) {
      value = (*m_getOptionalBoolArg)(true); // TODO may want to pass a variable
    }
    else{
      // unhandled
      OS_ASSERT(false);
    }
    std::string text;
    if (value) {
      text = *value;
    }
    setText(QString::fromStdString(text));
  }
}

void OSLineEdit2::onModelObjectRemove(Handle handle)
{
  unbind();
}

OSLineEdit::OSLineEdit( QWidget * parent )
  : QLineEdit(parent)
{
  this->setAcceptDrops(false);
  setEnabled(false);
}

void OSLineEdit::bind(model::ModelObject & modelObject, const char * property)
{
  m_modelObject = modelObject;

  m_property = property;

  setEnabled(true);

  bool isConnected = false;
  isConnected = connect( m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get(),SIGNAL(onChange()),
                         this,SLOT(onModelObjectChange()) );
  OS_ASSERT(isConnected);

  isConnected = connect( m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get(),SIGNAL(onRemoveFromWorkspace(Handle)),
                         this,SLOT(onModelObjectRemove(Handle)) );
  OS_ASSERT(isConnected);

  isConnected = connect( this, SIGNAL(editingFinished()), this, SLOT(onEditingFinished()) );
  OS_ASSERT(isConnected);

  onModelObjectChange();
}

void OSLineEdit::unbind()
{
  if (m_modelObject){
    this->disconnect(m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get());
    m_modelObject.reset();
    m_property = "";
    setEnabled(false);
  }
}

void OSLineEdit::onEditingFinished()
{
  if( m_modelObject )
  {
    std::string stdstring = this->text().toStdString();

    QVariant var;
    var.setValue<std::string>(stdstring);

    if( ! m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>()->setProperty(m_property.c_str(),var) )
    {
      boost::optional<std::string> ostring(stdstring);

      QVariant ovar;
      ovar.setValue<boost::optional<std::string> >(ostring);

      m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>()->setProperty(m_property.c_str(),ovar);
    }
    
  }
}

void OSLineEdit::onModelObjectChange()
{
  if( m_modelObject )
  {
    QVariant variant = m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>()->property(m_property.c_str());

    if( variant.canConvert<std::string>() )
    {
      setText(QString::fromStdString(variant.value<std::string>()));
    }
    else if( variant.canConvert<boost::optional<std::string> >() )
    {
      if( boost::optional<std::string> string = variant.value<boost::optional<std::string> >() )
      {
        if( string )
        {
          setText(QString::fromStdString(string.get()));
        }
      }
    }
  }
}

void OSLineEdit::onModelObjectRemove(Handle handle)
{
  m_modelObject.reset();
  m_property = "";
  setEnabled(false);
}

} // openstudio

