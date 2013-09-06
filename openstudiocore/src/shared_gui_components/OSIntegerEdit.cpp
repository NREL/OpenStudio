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

#include <shared_gui_components/OSIntegerEdit.hpp>

#include <model/ModelObject_Impl.hpp>

#include <utilities/core/Assert.hpp>
#include <utilities/core/Containers.hpp>
#include <utilities/data/Attribute.hpp>

#include <iomanip>

using openstudio::model::ModelObject;

namespace openstudio {

OSIntegerEdit2::OSIntegerEdit2( QWidget * parent )
  : m_isScientific(false)
{
  this->setAcceptDrops(false);
  setEnabled(false);
}

void OSIntegerEdit2::bind(model::ModelObject& modelObject,
                          IntGetter get,
                          boost::optional<IntSetter> set,
                          boost::optional<NoFailAction> reset,
                          boost::optional<NoFailAction> autosize,
                          boost::optional<NoFailAction> autocalculate,
                          boost::optional<BasicQuery> isDefaulted,
                          boost::optional<BasicQuery> isAutosized,
                          boost::optional<BasicQuery> isAutocalculated)
{
  m_modelObject = modelObject;
  m_get = get;
  m_set = set;
  m_reset = reset;
  m_autosize = autosize;
  m_autocalculate = autocalculate;
  m_isDefaulted = isDefaulted;
  m_isAutosized = isAutosized;
  m_isAutocalculated = isAutocalculated;

  completeBind();
}

void OSIntegerEdit2::bind(model::ModelObject& modelObject,
                          OptionalIntGetter get,
                          boost::optional<IntSetter> set,
                          boost::optional<NoFailAction> reset,
                          boost::optional<NoFailAction> autosize,
                          boost::optional<NoFailAction> autocalculate,
                          boost::optional<BasicQuery> isDefaulted,
                          boost::optional<BasicQuery> isAutosized,
                          boost::optional<BasicQuery> isAutocalculated)
{
  m_modelObject = modelObject;
  m_getOptional = get;
  m_set = set;
  m_reset = reset;
  m_autosize = autosize;
  m_autocalculate = autocalculate;
  m_isDefaulted = isDefaulted;
  m_isAutosized = isAutosized;
  m_isAutocalculated = isAutocalculated;

  completeBind();
}

void OSIntegerEdit2::bind(model::ModelExtensibleGroup& modelExtensibleGroup,
                          IntGetter get,
                          boost::optional<IntSetter> set,
                          boost::optional<NoFailAction> reset,
                          boost::optional<NoFailAction> autosize,
                          boost::optional<NoFailAction> autocalculate,
                          boost::optional<BasicQuery> isDefaulted,
                          boost::optional<BasicQuery> isAutosized,
                          boost::optional<BasicQuery> isAutocalculated)
{
  m_modelExtensibleGroup = modelExtensibleGroup;
  m_modelObject = modelExtensibleGroup.getOptionalObject<model::ModelObject>();
  m_get = get;
  m_set = set;
  m_reset = reset;
  m_autosize = autosize;
  m_autocalculate = autocalculate;
  m_isDefaulted = isDefaulted;
  m_isAutosized = isAutosized;
  m_isAutocalculated = isAutocalculated;

  completeBind();
}

void OSIntegerEdit2::bind(model::ModelExtensibleGroup& modelExtensibleGroup,
                          OptionalIntGetter get,
                          boost::optional<IntSetter> set,
                          boost::optional<NoFailAction> reset,
                          boost::optional<NoFailAction> autosize,
                          boost::optional<NoFailAction> autocalculate,
                          boost::optional<BasicQuery> isDefaulted,
                          boost::optional<BasicQuery> isAutosized,
                          boost::optional<BasicQuery> isAutocalculated)
{
  m_modelExtensibleGroup = modelExtensibleGroup;
  m_modelObject = modelExtensibleGroup.getOptionalObject<model::ModelObject>();
  m_getOptional = get;
  m_set = set;
  m_reset = reset;
  m_autosize = autosize;
  m_autocalculate = autocalculate;
  m_isDefaulted = isDefaulted;
  m_isAutosized = isAutosized;
  m_isAutocalculated = isAutocalculated;

  completeBind();
}

void OSIntegerEdit2::completeBind() {

  // only let one of autosize/autocalculate
  if ((m_isAutosized && m_isAutocalculated) || 
      (m_isAutosized && m_autocalculate) || 
      (m_isAutocalculated && m_autosize)) 
  {
    LOG_AND_THROW("A field can only be autosized or autocalculated, it cannot be both.");
  }

  setEnabled(true);

  bool isConnected = false;
  isConnected = connect( this, SIGNAL(editingFinished()), this, SLOT(onEditingFinished()) );
  OS_ASSERT(isConnected);

  isConnected = connect( m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get(),SIGNAL(onChange()),
                         this,SLOT(onModelObjectChange()) );
  OS_ASSERT(isConnected);

  isConnected = connect( m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get(),SIGNAL(onRemoveFromWorkspace(Handle)),
                         this,SLOT(onModelObjectRemove(Handle)) );
  OS_ASSERT(isConnected);

  refreshTextAndLabel();
}

void OSIntegerEdit2::unbind() {
  if (m_modelObject){
    this->disconnect(m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get());
    m_modelObject.reset();
    m_modelExtensibleGroup.reset();
    m_get.reset();
    m_getOptional.reset();
    m_set.reset();
    m_reset.reset();
    m_autosize.reset();
    m_autocalculate.reset();
    m_isDefaulted.reset();
    m_isAutosized.reset();
    m_isAutocalculated.reset();
    setEnabled(false);
  }
}

void OSIntegerEdit2::onEditingFinished() {
  if (m_modelObject) {
    std::string str = this->text().toStdString();
    boost::regex autore("[aA][uU][tT][oO]");
    ModelObject modelObject = m_modelObject.get();

    if (str.empty()) {
      if (m_reset) {
        (*m_reset)();
      }
    }
    else if (boost::regex_search(str,autore)) {
      if (m_isAutosized) {
        if (m_autosize) {
          (*m_autosize)();
        }
        else if (m_reset) {
          (*m_reset)();
        }
      }
      if (m_isAutocalculated) {
        if (m_autocalculate) {
          (*m_autocalculate)();
        }
        else if (m_reset) {
          (*m_reset)();
        }
      }
    }
    else {
      try {
        int value = boost::lexical_cast<int>(str);
        setPrecision(str);
        if (m_set) {
          (*m_set)(value);
        }
      }
      catch (...) {}
    }
  }
}

void OSIntegerEdit2::onModelObjectChange() {
  if (m_modelExtensibleGroup){
    if (m_modelExtensibleGroup->empty()){
      // this is equivalent to onModelObjectRemove for the extensible group
      unbind();
      return;
    }
  }
  refreshTextAndLabel();
}

void OSIntegerEdit2::onModelObjectRemove(Handle handle) {
  unbind();
}

void OSIntegerEdit2::refreshTextAndLabel() {
  if (m_modelObject) {
    QString textValue;
    ModelObject modelObject = m_modelObject.get();
    std::stringstream ss;

    if (m_isAutosized && (*m_isAutosized)()) {
      textValue = QString("autosize");
    }

    if (m_isAutocalculated && (*m_isAutocalculated)()) {
      textValue = QString("autocalculate");
    }

    OptionalInt oi;
    if (m_get) {
      oi = (*m_get)();
    }
    else {
      OS_ASSERT(m_getOptional);
      oi = (*m_getOptional)();
    }
    if (oi) {
      int value = *oi;
      if (m_isScientific) {
        ss << std::scientific;
      }
      else {
        ss << std::fixed;
      }
      if (m_precision) {
        ss << std::setprecision(*m_precision);
      }
      ss << value;
      textValue = toQString(ss.str());
      ss.str("");
    }

    this->setText(textValue);

    if (m_isDefaulted) {
      if ((*m_isDefaulted)()) {
        this->setStyleSheet("color:green");
      }
      else {
        this->setStyleSheet("color:black");
      }
    }
  }
}

void OSIntegerEdit2::setPrecision(const std::string& str) {
  boost::regex rgx("-?([[:digit:]]*)(\\.)?([[:digit:]]+)([EDed][-\\+]?[[:digit:]]+)?");
  boost::smatch m;
  if (boost::regex_match(str,m,rgx)) {
    std::string sci, prefix, postfix;
    if (m[1].matched) {
      prefix = std::string(m[1].first,m[1].second);
    }
    if (m[3].matched) {
      postfix = std::string(m[3].first,m[3].second);
    }
    if (m[4].matched) {
      sci = std::string(m[4].first,m[4].second);
    }
    m_isScientific = !sci.empty();

    if (m_isScientific) {
      m_precision = prefix.size() + postfix.size() - 1;
    }
    else {
      m_precision = 0;
    }
  }
  else {
    m_isScientific = false;
    m_precision.reset();
  }
}

OSIntegerEdit::OSIntegerEdit( QWidget * parent )
  : m_isScientific(false)
{
  this->setAcceptDrops(false);
  setEnabled(false);
}

void OSIntegerEdit::bind(model::ModelObject& modelObject,
                          const char* property,
                          const boost::optional<std::string>& isDefaultedProperty,
                          const boost::optional<std::string>& isAutosizedProperty,
                          const boost::optional<std::string>& isAutocalculatedProperty)
{
  m_modelObject = modelObject;
  m_property = property;
  m_isDefaultedProperty = isDefaultedProperty;
  m_isAutosizedProperty = isAutosizedProperty;
  m_isAutocalculatedProperty = isAutocalculatedProperty;

  // only let one of autosize/autocalculate
  if (isAutosizedProperty && isAutocalculatedProperty) {
    LOG_AND_THROW("A field can only be autosized or autocalculated, it cannot be both.");
  }

  // check for attribute existence
  StringVector attributeNames = modelObject.attributeNames();
  StringVector::const_iterator anb(attributeNames.begin()),ane(attributeNames.end());
  OS_ASSERT(std::find(anb,ane,m_property) != ane);
  if (m_isDefaultedProperty) {
    OS_ASSERT(std::find(anb,ane,*m_isDefaultedProperty) != ane);
  }
  if (m_isAutosizedProperty) {
    OS_ASSERT(std::find(anb,ane,*m_isAutosizedProperty) != ane);
  }
  if (m_isAutocalculatedProperty) {
    OS_ASSERT(std::find(anb,ane,*m_isAutocalculatedProperty) != ane);
  }

  setEnabled(true);

  bool isConnected = false;
  isConnected = connect( this, SIGNAL(editingFinished()), this, SLOT(onEditingFinished()) );
  OS_ASSERT(isConnected);

  isConnected = connect( m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get(),SIGNAL(onChange()),
                         this,SLOT(onModelObjectChange()) );
  OS_ASSERT(isConnected);

  isConnected = connect( m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get(),SIGNAL(onRemoveFromWorkspace(Handle)),
                         this,SLOT(onModelObjectRemove(Handle)) );
  OS_ASSERT(isConnected);

  refreshTextAndLabel();
}

void OSIntegerEdit::unbind() {
  if (m_modelObject){
    this->disconnect(m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get());
    m_modelObject.reset();
    m_property = "";
    setEnabled(false);
  }
}

void OSIntegerEdit::onEditingFinished() {
  if (m_modelObject) {
    std::string str = this->text().toStdString();
    boost::regex autore("[aA][uU][tT][oO]");
    ModelObject modelObject = m_modelObject.get();

    if (str.empty()) {
      modelObject.resetAttribute(m_property);
    }
    else if (boost::regex_search(str,autore)) {
      if (m_isAutosizedProperty) {
        if (modelObject.isSettableAttribute(*m_isAutosizedProperty)) {
          modelObject.setAttribute(*m_isAutosizedProperty,true);
        }
        else {
          modelObject.resetAttribute(m_property);
        }
      }
      if (m_isAutocalculatedProperty) {
        if (modelObject.isSettableAttribute(*m_isAutocalculatedProperty)) {
          modelObject.setAttribute(*m_isAutocalculatedProperty,true);
        }
        else {
          modelObject.resetAttribute(m_property);
        }
      }
    }
    else {
      try {
        int value = boost::lexical_cast<int>(str);
        setPrecision(str);
        modelObject.setAttribute(m_property,value);
      }
      catch (...) {}
    }
  }
}

void OSIntegerEdit::onModelObjectChange() {
  refreshTextAndLabel();
}

void OSIntegerEdit::onModelObjectRemove(Handle handle) {
  m_modelObject.reset();
  m_property = "";
  setEnabled(false);
}

void OSIntegerEdit::refreshTextAndLabel() {
  if (m_modelObject) {
    QString textValue;
    ModelObject modelObject = m_modelObject.get();
    std::stringstream ss;

    if (m_isAutosizedProperty) {
      Attribute autosized = modelObject.getAttribute(*m_isAutosizedProperty).get();
      if (autosized.valueAsBoolean()) {
        textValue = QString("autosize");
      }
    }

    if (m_isAutocalculatedProperty) {
      Attribute autocalculated = modelObject.getAttribute(*m_isAutocalculatedProperty).get();
      if (autocalculated.valueAsBoolean()) {
        textValue = QString("autocalculate");
      }
    }

    OptionalAttribute attribute = modelObject.getAttribute(m_property);
    if (attribute) {
      int value = attribute->valueAsInteger();
      if (m_isScientific) {
        ss << std::scientific;
      }
      else {
        ss << std::fixed;
      }
      if (m_precision) {
        ss << std::setprecision(*m_precision);
      }
      ss << value;
      textValue = toQString(ss.str());
      ss.str("");
    }

    this->setText(textValue);

    if (m_isDefaultedProperty) {
      Attribute defaulted = modelObject.getAttribute(*m_isDefaultedProperty).get();
      if (defaulted.valueAsBoolean()) {
        this->setStyleSheet("color:green");
      }
      else {
        this->setStyleSheet("color:black");
      }
    }
  }
}

void OSIntegerEdit::setPrecision(const std::string& str) {
  boost::regex rgx("-?([[:digit:]]*)(\\.)?([[:digit:]]+)([EDed][-\\+]?[[:digit:]]+)?");
  boost::smatch m;
  if (boost::regex_match(str,m,rgx)) {
    std::string sci, prefix, postfix;
    if (m[1].matched) {
      prefix = std::string(m[1].first,m[1].second);
    }
    if (m[3].matched) {
      postfix = std::string(m[3].first,m[3].second);
    }
    if (m[4].matched) {
      sci = std::string(m[4].first,m[4].second);
    }
    m_isScientific = !sci.empty();

    if (m_isScientific) {
      m_precision = prefix.size() + postfix.size() - 1;
    }
    else {
      m_precision = 0;
    }
  }
  else {
    m_isScientific = false;
    m_precision.reset();
  }
}

} // openstudio

