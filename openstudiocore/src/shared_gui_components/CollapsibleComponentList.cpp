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

#include "CollapsibleComponentList.hpp"
#include "CollapsibleComponent.hpp"
#include "CollapsibleComponentHeader.hpp"
#include "Component.hpp"
#include "ComponentList.hpp"

#include "../utilities/core/Assert.hpp"

#include <QBoxLayout>
#include <QButtonGroup>
#include <QPainter>
#include <QResizeEvent>
#include <QStyleOption>

namespace openstudio {

CollapsibleComponentList::CollapsibleComponentList(QWidget * parent)
  : QWidget(parent),
  m_mainLayout(nullptr),
  m_contentLayout(nullptr),
  m_collapsibleComponentGroup(nullptr)
{
  createLayout();
}

CollapsibleComponentList::CollapsibleComponentList(const std::vector<CollapsibleComponent *> & collapsibleComponents,
                                                   QWidget * parent)
  : QWidget(parent),
  m_mainLayout(nullptr),
  m_contentLayout(nullptr),
  m_collapsibleComponentGroup(nullptr)

{
  createLayout();
  setCollapsibleComponents(collapsibleComponents);
}

void CollapsibleComponentList::createLayout()
{
  this->setObjectName("GrayWidget"); 

  auto outerVLayout = new QVBoxLayout();
  outerVLayout->setContentsMargins(0,0,0,0);
  this->setLayout(outerVLayout);

  auto outerWidget = new QWidget();

  outerVLayout->addWidget(outerWidget);

  m_mainLayout = new QVBoxLayout();
  outerWidget->setLayout(m_mainLayout);
  m_mainLayout->setContentsMargins(0,0,0,0);
  m_mainLayout->setSpacing(0);
  m_mainLayout->addStretch(10);

  ///! QButtonGroup is exclusive, by default
  m_collapsibleComponentGroup = new QButtonGroup(this);
}

Component * CollapsibleComponentList::checkedComponent() const
{
  Component * result = nullptr;
  if (checkedCollapsibleComponent()){
    result = checkedCollapsibleComponent()->componentList()->checkedComponent();
  }
  return result;
}

CollapsibleComponent * CollapsibleComponentList::checkedCollapsibleComponent() const
{
  return qobject_cast<CollapsibleComponent *>(m_collapsibleComponentGroup->button(0)); // TODO fix this hack
}

CollapsibleComponent * CollapsibleComponentList::firstCollapsibleComponent()
{
  return qobject_cast<CollapsibleComponent *>(m_collapsibleComponentGroup->button(0));
}

CollapsibleComponent * CollapsibleComponentList::lastCollapsibleComponent()
{
  return qobject_cast<CollapsibleComponent *>(m_collapsibleComponentGroup->button(m_collapsibleComponentGroup->buttons().size() - 1));
}

std::vector<CollapsibleComponent *> CollapsibleComponentList::collapsibleComponents()
{
  std::vector<CollapsibleComponent *> result;

  for (QAbstractButton * button : m_collapsibleComponentGroup->buttons().toVector().toStdVector()) {
    result.push_back(qobject_cast<CollapsibleComponent *>(button));
  }

  return result;
}

std::vector<Component *> CollapsibleComponentList::components()
{
  std::vector<Component *> result;
  std::vector<Component *> components;

  for (QAbstractButton * button : m_collapsibleComponentGroup->buttons().toVector().toStdVector()) {
    components = qobject_cast<CollapsibleComponent *>(button)->componentList()->components();
    for (Component * component : components) {
      result.push_back(component);
    }
    components.clear();
  }

  return result;
}

void CollapsibleComponentList::addCollapsibleComponent(CollapsibleComponent * collapsibleComponent)
{
  OS_ASSERT(collapsibleComponent);

  m_mainLayout->addWidget(collapsibleComponent);
  m_collapsibleComponentGroup->addButton(collapsibleComponent,m_collapsibleComponentGroup->buttons().size());

  connect(collapsibleComponent, &CollapsibleComponent::headerClicked, this, &CollapsibleComponentList::headerClicked);

  connect(collapsibleComponent, &CollapsibleComponent::headerClicked, this, &CollapsibleComponentList::on_headerClicked);

  connect(collapsibleComponent, &CollapsibleComponent::componentClicked, this, &CollapsibleComponentList::componentClicked);

  connect(collapsibleComponent, &CollapsibleComponent::componentClicked, this, &CollapsibleComponentList::on_componentClicked);

  connect(collapsibleComponent, &CollapsibleComponent::clicked, this, &CollapsibleComponentList::collapsibleComponentClicked);

  connect(collapsibleComponent, &CollapsibleComponent::clicked, this, &CollapsibleComponentList::on_collapsibleComponentClicked);

  connect(collapsibleComponent, &CollapsibleComponent::getComponentsByPage, this, &CollapsibleComponentList::getComponentsByPage);

  connect(collapsibleComponent, &CollapsibleComponent::getComponentsByPage, this, &CollapsibleComponentList::on_getComponentsByPage);

  collapsibleComponent->setChecked(true);

  QString style;
  style.append( "QWidget#CollapsibleComponentList {" );
  style.append( "background: #F2F2F2; ");
  style.append( "border-bottom: 1px solid black; " );
  style.append( "}" );
  style.append( "QWidget#SideBar {background: #EEDEDE;}" );

  collapsibleComponent->setStyleSheet(style);
}

void CollapsibleComponentList::paintEvent(QPaintEvent * event)
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void CollapsibleComponentList::setCollapsibleComponents(const std::vector<CollapsibleComponent *> & collapsibleComponents)
{
  for (CollapsibleComponent * collapsibleComponent : collapsibleComponents)
  {
    addCollapsibleComponent(collapsibleComponent);
  }
}

void CollapsibleComponentList::setText(const QString& text)
{
  checkedCollapsibleComponent()->setText(text);
}

void CollapsibleComponentList::setNumResults(int numResults)
{
  checkedCollapsibleComponent()->setNumResults(numResults);
}

void CollapsibleComponentList::setNumPages(int numPages)
{
  checkedCollapsibleComponent()->setNumPages(numPages);
}

void CollapsibleComponentList::firstPage()
{
  checkedCollapsibleComponent()->firstPage();
}

///! SLOTS

void CollapsibleComponentList::on_headerClicked(bool checked)
{
}

void CollapsibleComponentList::on_componentClicked(bool checked)
{
}

void CollapsibleComponentList::on_collapsibleComponentClicked(bool checked)
{
}

void CollapsibleComponentList::on_getComponentsByPage(int pageIdx)
{
}

} // openstudio
