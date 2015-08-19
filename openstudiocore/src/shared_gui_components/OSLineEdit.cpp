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

  connect(m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get(), &openstudio::model::detail::ModelObject_Impl::onChange, this, &OSLineEdit2::onModelObjectChange);

  connect(m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get(), &openstudio::model::detail::ModelObject_Impl::onRemoveFromWorkspace, this, &OSLineEdit2::onModelObjectRemove);

  connect(this, &OSLineEdit2::editingFinished, this, &OSLineEdit2::onEditingFinished);

  m_timer.setSingleShot(true);
  connect(&m_timer, &QTimer::timeout, this, &OSLineEdit2::emitItemClicked);

  onModelObjectChangeInternal(true);

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
  // This m_item code is only relevant if we are building in
  // the context of openstudio_lib
#ifdef openstudio_lib_EXPORTS
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
#endif
}

void OSLineEdit2::onModelObjectRemove(Handle handle)
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

#ifdef openstudio_lib_EXPORTS
    auto mouseOverInspectorView = OSAppBase::instance()->currentDocument()->mainRightColumnController()->inspectorController()->inspectorView()->mouseOverInspectorView();
    if (!mouseOverInspectorView) {
      emit itemClicked(nullptr);
    }
#endif

  }

  QLineEdit::focusOutEvent(e);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

  connect(m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get(), &openstudio::model::detail::ModelObject_Impl::onChange, this, &OSLineEdit::onModelObjectChange);

  connect(m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get(), &openstudio::model::detail::ModelObject_Impl::onRemoveFromWorkspace, this, &OSLineEdit::onModelObjectRemove);

  connect(this, &OSLineEdit::editingFinished, this, &OSLineEdit::onEditingFinished);

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

    QVariant variant = m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>()->property(m_property.c_str());

    if( variant.canConvert<std::string>() ) {
      QVariant var;
      var.setValue<std::string>(stdstring);

      m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>()->setProperty(m_property.c_str(), var);
    } else if( variant.canConvert<boost::optional<std::string> >() ) {
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

