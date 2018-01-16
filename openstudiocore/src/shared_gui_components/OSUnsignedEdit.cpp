/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include "OSUnsignedEdit.hpp"

#include "../model/ModelObject_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Containers.hpp"
#include "../utilities/data/Attribute.hpp"

#include <iomanip>

#include <QFocusEvent>
#include <QIntValidator>

using openstudio::model::ModelObject;

namespace openstudio {

OSUnsignedEdit2::OSUnsignedEdit2( QWidget * parent )
  : m_isScientific(false)
{
  this->setFixedWidth(90);
  this->setAcceptDrops(false);
  setEnabled(false);

  m_intValidator = new QIntValidator();
  m_intValidator->setBottom(0);
  //this->setValidator(m_intValidator);
}

OSUnsignedEdit2::~OSUnsignedEdit2()
{
  unbind();
}

void OSUnsignedEdit2::bind(model::ModelObject& modelObject,
                          UnsignedGetter get,
                          boost::optional<UnsignedSetter> set,
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

void OSUnsignedEdit2::bind(model::ModelObject& modelObject,
                          OptionalUnsignedGetter get,
                          boost::optional<UnsignedSetter> set,
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

void OSUnsignedEdit2::bind(model::ModelExtensibleGroup& modelExtensibleGroup,
                           UnsignedGetter get,
                           boost::optional<UnsignedSetter> set,
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

void OSUnsignedEdit2::bind(model::ModelExtensibleGroup& modelExtensibleGroup,
                           OptionalUnsignedGetter get,
                           boost::optional<UnsignedSetter> set,
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

void OSUnsignedEdit2::completeBind() {

  // only let one of autosize/autocalculate
  if ((m_isAutosized && m_isAutocalculated) ||
      (m_isAutosized && m_autocalculate) ||
      (m_isAutocalculated && m_autosize))
  {
    LOG_AND_THROW("A field can only be autosized or autocalculated, it cannot be both.");
  }

  setEnabled(true);

  connect(this, &OSUnsignedEdit2::editingFinished, this, &OSUnsignedEdit2::onEditingFinished);

  m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get()->onChange.connect<OSUnsignedEdit2, &OSUnsignedEdit2::onModelObjectChange>(this);
  m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get()->onRemoveFromWorkspace.connect<OSUnsignedEdit2, &OSUnsignedEdit2::onModelObjectRemove>(this);

  refreshTextAndLabel();
}

void OSUnsignedEdit2::unbind() {
  if (m_modelObject){
    m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get()->onChange.disconnect<OSUnsignedEdit2, &OSUnsignedEdit2::onModelObjectChange>(this);
    m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get()->onRemoveFromWorkspace.disconnect<OSUnsignedEdit2, &OSUnsignedEdit2::onModelObjectRemove>(this);
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

void OSUnsignedEdit2::onEditingFinished() {

  emit inFocus(true, hasData());

  QString text = this->text();
  if (m_text == text) return;

  int pos = 0;
  QValidator::State state = m_intValidator->validate(text, pos);
  bool isAuto = false;
  if (state != QValidator::Acceptable){
    if (text.isEmpty()){
      // ok
    } else{
      boost::regex autore("[aA][uU][tT][oO]");
      isAuto = boost::regex_search(text.toStdString(), autore);
      if (isAuto){
        // ok
      } else{
        // not ok
        refreshTextAndLabel();
        return;
      }
    }
  }
  if (m_modelObject) {
    std::string str = text.toStdString();
    ModelObject modelObject = m_modelObject.get();

    if (str.empty()) {
      if (m_reset) {
        (*m_reset)();
      } else{
        refreshTextAndLabel();
      }
    }
    else if (isAuto) {
      if (m_isAutosized) {
        if (m_autosize) {
          (*m_autosize)();
        } else if (m_reset) {
          (*m_reset)();
        } else {
          refreshTextAndLabel();
        }
      }else if (m_isAutocalculated) {
        if (m_autocalculate) {
          (*m_autocalculate)();
        }
        else if (m_reset) {
          (*m_reset)();
        } else {
          refreshTextAndLabel();
        }
      } else {
        refreshTextAndLabel();
      }
    }
    else {
      try {
        int value = boost::lexical_cast<int>(str);
        setPrecision(str);
        if (m_set) {
          bool result = (*m_set)(value);
          if (!result){
            // restore
            refreshTextAndLabel();
          }
        }
      }
      catch (...)
      {
        // restore
        refreshTextAndLabel();
      }
    }
  }
}

void OSUnsignedEdit2::onModelObjectChange() {
  if (m_modelExtensibleGroup){
    if (m_modelExtensibleGroup->empty()){
      // this is equivalent to onModelObjectRemove for the extensible group
      unbind();
      return;
    }
  }
  refreshTextAndLabel();
}

void OSUnsignedEdit2::onModelObjectRemove(const Handle& handle) {
  unbind();
}

void OSUnsignedEdit2::refreshTextAndLabel() {

  QString text = this->text();

  //if (m_text == text) return;

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
    } else {
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

    if (m_text != textValue || text != textValue){
      m_text = textValue;
      this->blockSignals(true);
      this->setText(m_text);
      this->blockSignals(false);
    }

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

void OSUnsignedEdit2::setPrecision(const std::string& str) {
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

void OSUnsignedEdit2::focusInEvent(QFocusEvent * e)
{
  if (e->reason() == Qt::MouseFocusReason && m_hasClickFocus)
  {
    QString style("QLineEdit { background: #ffc627; }");
    setStyleSheet(style);

    emit inFocus(true, hasData());
  }

  QLineEdit::focusInEvent(e);
}

void OSUnsignedEdit2::focusOutEvent(QFocusEvent * e)
{
  if (e->reason() == Qt::MouseFocusReason && m_hasClickFocus)
  {
    QString style("QLineEdit { background: white; }");
    setStyleSheet(style);

    emit inFocus(false, false);
  }

  QLineEdit::focusOutEvent(e);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// OSUnsignedEdit::OSUnsignedEdit( QWidget * parent )
//   : m_isScientific(false)
// {
//   this->setFixedWidth(90);
//   this->setAcceptDrops(false);
//   setEnabled(false);

//   m_intValidator = new QIntValidator();
//   m_intValidator->setBottom(0);
//   this->setValidator(m_intValidator);
// }

// void OSUnsignedEdit::bind(model::ModelObject& modelObject,
//                           const char* property,
//                           const boost::optional<std::string>& isDefaultedProperty,
//                           const boost::optional<std::string>& isAutosizedProperty,
//                           const boost::optional<std::string>& isAutocalculatedProperty)
// {
//   m_modelObject = modelObject;
//   m_property = property;
//   m_isDefaultedProperty = isDefaultedProperty;
//   m_isAutosizedProperty = isAutosizedProperty;
//   m_isAutocalculatedProperty = isAutocalculatedProperty;

//   // only let one of autosize/autocalculate
//   if (isAutosizedProperty && isAutocalculatedProperty) {
//     LOG_AND_THROW("A field can only be autosized or autocalculated, it cannot be both.");
//   }

//   // check for attribute existence
//   StringVector attributeNames = modelObject.attributeNames();
//   StringVector::const_iterator anb(attributeNames.begin()),ane(attributeNames.end());
//   OS_ASSERT(std::find(anb,ane,m_property) != ane);
//   if (m_isDefaultedProperty) {
//     OS_ASSERT(std::find(anb,ane,*m_isDefaultedProperty) != ane);
//   }
//   if (m_isAutosizedProperty) {
//     OS_ASSERT(std::find(anb,ane,*m_isAutosizedProperty) != ane);
//   }
//   if (m_isAutocalculatedProperty) {
//     OS_ASSERT(std::find(anb,ane,*m_isAutocalculatedProperty) != ane);
//   }

//   setEnabled(true);

//   connect(this, &OSUnsignedEdit::editingFinished, this, &OSUnsignedEdit::onEditingFinished);

//   m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get()->openstudio::model::detail::ModelObject_Impl::onChange.connect<OSUnsignedEdit, &OSUnsignedEdit::onModelObjectChange>(this);

//   m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get()->openstudio::model::detail::ModelObject_Impl::onRemoveFromWorkspace.connect<OSUnsignedEdit, &OSUnsignedEdit::onModelObjectRemove>(this);

//   refreshTextAndLabel();
// }

// void OSUnsignedEdit::unbind() {
//   if (m_modelObject){
//     this->disconnect(m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get());
//     m_modelObject.reset();
//     m_property = "";
//     setEnabled(false);
//   }
// }

// void OSUnsignedEdit::onEditingFinished() {
//   if (m_modelObject) {
//     std::string str = this->text().toStdString();
//     boost::regex autore("[aA][uU][tT][oO]");
//     ModelObject modelObject = m_modelObject.get();

//     if (str.empty()) {
//       modelObject.resetAttribute(m_property);
//     }
//     else if (boost::regex_search(str,autore)) {
//       if (m_isAutosizedProperty) {
//         if (modelObject.isSettableAttribute(*m_isAutosizedProperty)) {
//           modelObject.setAttribute(*m_isAutosizedProperty,true);
//         }
//         else {
//           modelObject.resetAttribute(m_property);
//         }
//       }
//       if (m_isAutocalculatedProperty) {
//         if (modelObject.isSettableAttribute(*m_isAutocalculatedProperty)) {
//           modelObject.setAttribute(*m_isAutocalculatedProperty,true);
//         }
//         else {
//           modelObject.resetAttribute(m_property);
//         }
//       }
//     }
//     else {
//       try {
//         int value = boost::lexical_cast<int>(str);
//         setPrecision(str);
//         modelObject.setAttribute(m_property,value);
//       }
//       catch (...) {}
//     }
//   }
// }

// void OSUnsignedEdit::onModelObjectChange() {
//   refreshTextAndLabel();
// }

// void OSUnsignedEdit::onModelObjectRemove(const Handle& handle) {
//   m_modelObject.reset();
//   m_property = "";
//   setEnabled(false);
// }

// void OSUnsignedEdit::refreshTextAndLabel() {
//   if (m_modelObject) {
//     QString textValue;
//     ModelObject modelObject = m_modelObject.get();
//     std::stringstream ss;

//     if (m_isAutosizedProperty) {
//       Attribute autosized = modelObject.getAttribute(*m_isAutosizedProperty).get();
//       if (autosized.valueAsBoolean()) {
//         textValue = QString("autosize");
//       }
//     }

//     if (m_isAutocalculatedProperty) {
//       Attribute autocalculated = modelObject.getAttribute(*m_isAutocalculatedProperty).get();
//       if (autocalculated.valueAsBoolean()) {
//         textValue = QString("autocalculate");
//       }
//     }

//     OptionalAttribute attribute = modelObject.getAttribute(m_property);
//     if (attribute) {
//       int value = attribute->valueAsInteger();
//       if (m_isScientific) {
//         ss << std::scientific;
//       }
//       else {
//         ss << std::fixed;
//       }
//       if (m_precision) {
//         ss << std::setprecision(*m_precision);
//       }
//       ss << value;
//       textValue = toQString(ss.str());
//       ss.str("");
//     }

//     this->setText(textValue);

//     if (m_isDefaultedProperty) {
//       Attribute defaulted = modelObject.getAttribute(*m_isDefaultedProperty).get();
//       if (defaulted.valueAsBoolean()) {
//         this->setStyleSheet("color:green");
//       }
//       else {
//         this->setStyleSheet("color:black");
//       }
//     }
//   }
// }

// void OSUnsignedEdit::setPrecision(const std::string& str) {
//   boost::regex rgx("-?([[:digit:]]*)(\\.)?([[:digit:]]+)([EDed][-\\+]?[[:digit:]]+)?");
//   boost::smatch m;
//   if (boost::regex_match(str,m,rgx)) {
//     std::string sci, prefix, postfix;
//     if (m[1].matched) {
//       prefix = std::string(m[1].first,m[1].second);
//     }
//     if (m[3].matched) {
//       postfix = std::string(m[3].first,m[3].second);
//     }
//     if (m[4].matched) {
//       sci = std::string(m[4].first,m[4].second);
//     }
//     m_isScientific = !sci.empty();

//     if (m_isScientific) {
//       m_precision = prefix.size() + postfix.size() - 1;
//     }
//     else {
//       m_precision = 0;
//     }
//   }
//   else {
//     m_isScientific = false;
//     m_precision.reset();
//   }
// }

} // openstudio

