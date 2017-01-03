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

  connect(component, &Component::clicked, this, &ComponentList::componentClicked);

  connect(component, &Component::clicked, this, &ComponentList::on_componentClicked);
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

