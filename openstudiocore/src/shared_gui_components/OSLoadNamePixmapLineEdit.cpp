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

#include "OSLoadNamePixmapLineEdit.hpp"

#include "OSLineEdit.hpp"

#include "../openstudio_lib/IconLibrary.hpp"
#include "../openstudio_lib/OSItem.hpp"

#include "../model/ModelObject.hpp"
#include "../model/ModelObject_Impl.hpp"

#include <QBoxLayout>
#include <QLabel>
#include <QPixmap>

#define MINI_ICON_SIZE 24

namespace openstudio {

OSLoadNamePixmapLineEdit::OSLoadNamePixmapLineEdit( QWidget * parent )
  : QWidget(parent)
{
  createWidgets();
}

void OSLoadNamePixmapLineEdit::createWidgets()
{
  QPixmap m_pixmap(MINI_ICON_SIZE, MINI_ICON_SIZE);

  m_label = new QLabel();
  m_label->setFixedSize(MINI_ICON_SIZE, MINI_ICON_SIZE);
  m_label->setPixmap(m_pixmap);
  
  m_lineEdit = new OSLineEdit2();

  bool isConnected = connect(m_lineEdit, SIGNAL(itemClicked(OSItem*)), this, SIGNAL(itemClicked(OSItem*)));
  OS_ASSERT(isConnected);

  isConnected = connect(m_lineEdit, SIGNAL(objectRemoved(boost::optional<model::ParentObject>)), this, SIGNAL(objectRemoved(boost::optional<model::ParentObject>)));
  OS_ASSERT(isConnected);

  auto layout = new QHBoxLayout();
  layout->setContentsMargins(0,0,0,0);
  layout->addWidget(m_label);
  layout->addWidget(m_lineEdit);
  this->setLayout(layout);
}

void OSLoadNamePixmapLineEdit::setIcon()
{
  if (m_modelObject)
  {
    auto pixmap = IconLibrary::Instance().findMiniIcon(m_modelObject->iddObjectType().value());
    if (pixmap)
    {
      m_label->setPixmap(*pixmap);
    }
  }
}

void OSLoadNamePixmapLineEdit::bind(model::ModelObject& modelObject,
                       StringGetter get,
                       boost::optional<StringSetter> set,
                       boost::optional<NoFailAction> reset,
                       boost::optional<BasicQuery> isDefaulted)
{
  m_modelObject = modelObject;

  m_lineEdit->bind(modelObject,get,set,reset,isDefaulted);

  completeBind();
}

void OSLoadNamePixmapLineEdit::bind(model::ModelObject& modelObject, 
                       OptionalStringGetter get,
                       boost::optional<StringSetter> set,
                       boost::optional<NoFailAction> reset,
                       boost::optional<BasicQuery> isDefaulted)
{
  m_modelObject = modelObject;

  m_lineEdit->bind(modelObject, get, set, reset, isDefaulted);

  completeBind();
}

void OSLoadNamePixmapLineEdit::bind(model::ModelObject& modelObject,
                       OptionalStringGetterBoolArg get,
                       boost::optional<StringSetterOptionalStringReturn> set,
                       boost::optional<NoFailAction> reset,
                       boost::optional<BasicQuery> isDefaulted)
{
  m_modelObject = modelObject;

  m_lineEdit->bind(modelObject, get, set, reset, isDefaulted);

  completeBind();
}

void OSLoadNamePixmapLineEdit::completeBind() {

  setIcon();

  bool isConnected = false;

  isConnected = connect( m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get(),SIGNAL(onChange()),
                         this,SLOT(onModelObjectChange()) );
  OS_ASSERT(isConnected);
}

void OSLoadNamePixmapLineEdit::unbind()
{
  m_lineEdit->unbind();
}

void OSLoadNamePixmapLineEdit::onModelObjectChange()
{
  setIcon();
}

void OSLoadNamePixmapLineEdit::enableClickFocus()
{
  m_lineEdit->enableClickFocus();
}

} // openstudio
