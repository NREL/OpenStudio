/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "OSLineEdit.hpp"

#include "../openstudio_lib/InspectorController.hpp"
#include "../openstudio_lib/InspectorView.hpp"
#include "../openstudio_lib/MainRightColumnController.hpp"
#include "../openstudio_lib/ModelObjectItem.hpp"
#include "../openstudio_lib/OSAppBase.hpp"
#include "../openstudio_lib/OSDocument.hpp"
#include "../openstudio_lib/OSItem.hpp"

#include "../model/ModelObject.hpp"
#include "../model/ModelObject_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <boost/optional.hpp>

#include <QFocusEvent>
#include <QMouseEvent>
#include <QString>

#if !(_DEBUG || (__GNUC__ && !NDEBUG))
#define TIMEOUT_INTERVAL 500
#else
#define TIMEOUT_INTERVAL 2000
#endif

namespace openstudio {

OSLineEdit2::OSLineEdit2( QWidget * parent )
  : QLineEdit(parent)
{
  this->setAcceptDrops(false);
  setEnabled(false);
}

OSLineEdit2::~OSLineEdit2()
{ 
  unbind();
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

void OSLineEdit2::bind(model::ModelObject& modelObject,
  StringGetter get,
  boost::optional<StringSetterVoidReturn> set,
  boost::optional<NoFailAction> reset,
  boost::optional<BasicQuery> isDefaulted)
{
  m_modelObject = modelObject;
  m_get = get;
  m_setVoidReturn = set;
  m_reset = reset;
  m_isDefaulted = isDefaulted;

  completeBind();
}

void OSLineEdit2::completeBind() {
  setEnabled(true);

  if (!m_set && !m_setOptionalStringReturn && !m_setVoidReturn)
  {
    setReadOnly(true);
  }

  m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get()->onChange.connect<OSLineEdit2, &OSLineEdit2::onModelObjectChange>(this);

  m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get()->onRemoveFromWorkspace.connect<OSLineEdit2, &OSLineEdit2::onModelObjectRemove>(this);

  connect(this, &OSLineEdit2::editingFinished, this, &OSLineEdit2::onEditingFinished);

  m_timer.setSingleShot(true);
  connect(&m_timer, &QTimer::timeout, this, &OSLineEdit2::emitItemClicked);

  onModelObjectChangeInternal(true);

}

void OSLineEdit2::unbind()
{
  if (m_modelObject){
    m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get()->onChange.disconnect<OSLineEdit2, &OSLineEdit2::onModelObjectChange>(this);
    m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get()->onRemoveFromWorkspace.disconnect<OSLineEdit2, &OSLineEdit2::onModelObjectRemove>(this);

    m_modelObject.reset();
    m_get.reset();
    m_getOptional.reset();
    m_getOptionalBoolArg.reset();
    m_set.reset();
    m_setOptionalStringReturn.reset();
    m_setVoidReturn.reset();
    m_reset.reset();
    m_isDefaulted.reset();
    setEnabled(false);
  }
}

void OSLineEdit2::onEditingFinished() {
  if (m_modelObject && (m_set || m_setOptionalStringReturn || m_setVoidReturn)) {
    if (m_text != this->text().toStdString()) {
      m_text = this->text().toStdString();
      auto result = false;
      if (m_set) {
        result = (*m_set)(m_text);
      }
      else if (m_setOptionalStringReturn) {
        auto optionalStringReturn = (*m_setOptionalStringReturn)(m_text);
        if (optionalStringReturn) {
          result = true; // TODO
        }
        result = true;
      }
      else if (m_setVoidReturn) {
        (*m_setVoidReturn)(m_text);
        result = true;
      }
      if (!result){
        //restore
        onModelObjectChange();
      }
      else {
        emit inFocus(true, hasData());
        adjustWidth();
      }
    }
  }
}

void OSLineEdit2::adjustWidth()
{
  if (m_modelObject) {
    // Adjust the width to accommodate the text
    QFont myFont;
    QFontMetrics fm(myFont);
    auto width = fm.width(toQString(m_text));
    if (width < 80) width = 80;
    setFixedWidth(width + 10);
  }
}

void OSLineEdit2::onModelObjectChange()
{
  onModelObjectChangeInternal(false);
}

void OSLineEdit2::onModelObjectChangeInternal(bool startingup) {
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
      if (m_text != text) {
        m_text = text;
        this->blockSignals(true);
        this->setText(QString::fromStdString(m_text));
        this->blockSignals(false);
        if (!startingup) m_timer.start(TIMEOUT_INTERVAL);
      }
    }
  }
}

void OSLineEdit2::emitItemClicked()
{
  if (!m_item && m_modelObject) {
    m_item = OSItem::makeItem(modelObjectToItemId(*m_modelObject, false));
    OS_ASSERT(m_item);
    m_item->setParent(this);
    connect(m_item, &OSItem::itemRemoveClicked, this, &OSLineEdit2::onItemRemoveClicked);
  }

  if (m_item){
    // Tell EditView to display this object
    emit itemClicked(m_item);
  }
}

void OSLineEdit2::onModelObjectRemove(const Handle& handle)
{
  unbind();
}

void OSLineEdit2::mouseReleaseEvent(QMouseEvent * event)
{
  if (event->button() == Qt::LeftButton){
    event->accept();

    m_timer.start(TIMEOUT_INTERVAL);
  }
}

void OSLineEdit2::onItemRemoveClicked()
{
  if (m_reset)
  {
    boost::optional<model::ParentObject> parent = boost::none;
    if (m_modelObject) {
      parent = m_modelObject->parent();
    }
    (*m_reset)();
    if (m_deleteObject) {
      m_modelObject->remove();
    }
    emit objectRemoved(parent);
  }
}

void OSLineEdit2::focusInEvent(QFocusEvent * e)
{
  if (e->reason() == Qt::MouseFocusReason && m_hasClickFocus)
  {
    QString style("QLineEdit { background: #ffc627; }");
    setStyleSheet(style);

    emit inFocus(true, hasData());
  }

  QLineEdit::focusInEvent(e);
}

void OSLineEdit2::focusOutEvent(QFocusEvent * e)
{
  if (e->reason() == Qt::MouseFocusReason && m_hasClickFocus)
  {
    QString style("QLineEdit { background: white; }");
    setStyleSheet(style);

    emit inFocus(false, false);

    auto mouseOverInspectorView = OSAppBase::instance()->currentDocument()->mainRightColumnController()->inspectorController()->inspectorView()->mouseOverInspectorView();
    if (!mouseOverInspectorView) {
      emit itemClicked(nullptr);
    }

  }

  QLineEdit::focusOutEvent(e);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// OSLineEdit::OSLineEdit( QWidget * parent )
//   : QLineEdit(parent)
// {
//   this->setAcceptDrops(false);
//   setEnabled(false);
// }

// void OSLineEdit::bind(model::ModelObject & modelObject, const char * property)
// {
//   m_modelObject = modelObject;

//   m_property = property;

//   setEnabled(true);

//   m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get()->onChange.connect<OSLineEdit, &OSLineEdit::onModelObjectChange>(this);

//   m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get()->onRemoveFromWorkspace.connect<OSLineEdit, &OSLineEdit::onModelObjectRemove>(this);

//   connect(this, &OSLineEdit::editingFinished, this, &OSLineEdit::onEditingFinished);

//   onModelObjectChange();
// }

// void OSLineEdit::unbind()
// {
//   if (m_modelObject){
//     m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get()->onChange.disconnect<OSLineEdit, &OSLineEdit::onModelObjectChange>(this);
//     m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get()->onRemoveFromWorkspace.disconnect<OSLineEdit, &OSLineEdit::onModelObjectRemove>(this);

//     m_modelObject.reset();
//     m_property = "";
//     setEnabled(false);
//   }
// }

// void OSLineEdit::onEditingFinished()
// {
//   if( m_modelObject )
//   {
//     std::string stdstring = this->text().toStdString();

//     QVariant variant = m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>()->property(m_property.c_str());

//     if( variant.canConvert<std::string>() ) {
//       QVariant var;
//       var.setValue<std::string>(stdstring);

//       m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>()->setProperty(m_property.c_str(), var);
//     } else if( variant.canConvert<boost::optional<std::string> >() ) {
//       boost::optional<std::string> ostring(stdstring);

//       QVariant ovar;
//       ovar.setValue<boost::optional<std::string> >(ostring);

//       m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>()->setProperty(m_property.c_str(),ovar);
//     }
    
//   }
// }

// void OSLineEdit::onModelObjectChange()
// {
//   if( m_modelObject )
//   {
//     QVariant variant = m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>()->property(m_property.c_str());

//     if( variant.canConvert<std::string>() )
//     {
//       setText(QString::fromStdString(variant.value<std::string>()));
//     }
//     else if( variant.canConvert<boost::optional<std::string> >() )
//     {
//       if( boost::optional<std::string> string = variant.value<boost::optional<std::string> >() )
//       {
//         if( string )
//         {
//           setText(QString::fromStdString(string.get()));
//         }
//       }
//     }
//   }
// }

// void OSLineEdit::onModelObjectRemove(const Handle& handle)
// {
//   m_modelObject.reset();
//   m_property = "";
//   setEnabled(false);
// }

} // openstudio

