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

#include "ComponentList.hpp"
#include "Component.hpp"

#include "../utilities/core/Assert.hpp"

#include <QAbstractButton>
#include <QBoxLayout>
#include <QButtonGroup>
#include <QPainter>
#include <QPaintEvent>
#include <QStyleOption>

namespace openstudio {

ComponentList::ComponentList(QWidget * parent)
  : QWidget(parent),
    m_mainLayout(nullptr),
    m_componentGroup(nullptr)
{
  createLayout();
}

ComponentList::ComponentList(const std::vector<Component *> & components,
                             QWidget * parent)
  : QWidget(parent),
    m_mainLayout(nullptr),
    m_componentGroup(nullptr)
{
  createLayout();
  setComponents(components);
}

void ComponentList::createLayout()
{
  this->setObjectName("GrayWidget"); 

  QString style;

  style.append("QWidget#GrayWidget {");
  style.append(" background: #E6E6E6;");
  style.append(" border-bottom: 1px solid black;");
  style.append("}");

  setStyleSheet(style);

  auto outerVLayout = new QVBoxLayout();
  outerVLayout->setContentsMargins(0,0,0,0);
  this->setLayout(outerVLayout);

  auto outerWidget = new QWidget();

  outerVLayout->addWidget(outerWidget);

  m_mainLayout = new QVBoxLayout();
  outerWidget->setLayout(m_mainLayout);
  m_mainLayout->setContentsMargins(0,0,0,0);
  m_mainLayout->setSpacing(0);
  m_mainLayout->addStretch();

  ///! QButtonGroup is exclusive, by default
  m_componentGroup = new QButtonGroup(this);
}

Component * ComponentList::checkedComponent() const
{
  return qobject_cast<Component *>(m_componentGroup->checkedButton());
}

Component * ComponentList::firstComponent()
{
  return qobject_cast<Component *>(m_componentGroup->button(0));
}

Component * ComponentList::lastComponent()
{
  return qobject_cast<Component *>(m_componentGroup->button(m_componentGroup->buttons().size() - 1));
}

std::vector<Component *> ComponentList::components()
{
  std::vector<Component *> result;

  for (QAbstractButton * button : m_componentGroup->buttons().toVector().toStdVector()) {
    result.push_back(qobject_cast<Component *>(button));
  }

  return result;
}

void ComponentList::addComponent(Component * component)
{
  OS_ASSERT(component);

  m_mainLayout->addWidget(component);
  m_componentGroup->addButton(component,m_componentGroup->buttons().size());

  bool isConnected = false;

  isConnected = connect(component, SIGNAL(clicked(bool)),
                        this, SIGNAL(componentClicked(bool)));
  OS_ASSERT(isConnected);

  isConnected = connect(component, SIGNAL(clicked(bool)),
                        this, SLOT(on_componentClicked(bool)));
  OS_ASSERT(isConnected);
}

void ComponentList::paintEvent( QPaintEvent * event )
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void ComponentList::setComponents(const std::vector<Component *> & components)
{
  for (Component * component : components)
  {
    addComponent(component);
  }
}

///! SLOTS

void ComponentList::on_componentClicked(bool)
{
}


} // openstudio

