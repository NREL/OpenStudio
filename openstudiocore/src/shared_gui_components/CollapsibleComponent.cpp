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

#include "CollapsibleComponent.hpp"
#include "CollapsibleComponentHeader.hpp"
#include "ComponentList.hpp"

#include "../utilities/core/Assert.hpp"

#include <QBoxLayout>
#include <QButtonGroup>
#include <QLabel>
#include <QPainter>
#include <QResizeEvent>
#include <QStyleOption>

namespace openstudio {

CollapsibleComponent::CollapsibleComponent(CollapsibleComponentHeader * collapsibleComponentHeader,
                                           ComponentList * componentList,
                                           QWidget * parent)
  : QAbstractButton(parent),
    m_collapsibleComponentHeader(collapsibleComponentHeader),
    m_componentList(componentList),
    m_mainLayout(nullptr)
{

  OS_ASSERT(m_collapsibleComponentHeader);
  OS_ASSERT(m_componentList);

  setObjectName("CollapsibleComponent");

  m_mainLayout = new QVBoxLayout();
  m_mainLayout->setSpacing(0);
  m_mainLayout->setContentsMargins(0,0,0,0);
  setLayout(m_mainLayout);

  // collapsible header
  m_mainLayout->addWidget(m_collapsibleComponentHeader);

  connect(collapsibleComponentHeader, &CollapsibleComponentHeader::clicked, this, &CollapsibleComponent::headerClicked);

  connect(collapsibleComponentHeader, &CollapsibleComponentHeader::clicked, this, &CollapsibleComponent::on_headerClicked);

  connect(collapsibleComponentHeader, &CollapsibleComponentHeader::getComponentsByPage, this, &CollapsibleComponent::getComponentsByPage);

  connect(collapsibleComponentHeader, &CollapsibleComponentHeader::getComponentsByPage, this, &CollapsibleComponent::on_getComponentsByPage);

  // component list
  m_mainLayout->addWidget(m_componentList);
  
  connect(componentList, &ComponentList::componentClicked, this, &CollapsibleComponent::componentClicked);

  connect(componentList, &ComponentList::componentClicked, this, &CollapsibleComponent::on_componentClicked);

  m_mainLayout->addStretch();

}

CollapsibleComponentHeader * CollapsibleComponent::collapsibleComponentHeader() const
{
  return m_collapsibleComponentHeader;
}

ComponentList * CollapsibleComponent::componentList() const
{
  return m_componentList;
}

bool CollapsibleComponent::expanded() const
{
  return this->m_collapsibleComponentHeader->isChecked();
}

void CollapsibleComponent::setExpanded(bool expanded)
{
  this->m_collapsibleComponentHeader->setChecked(expanded);
  this->m_componentList->setVisible(expanded);
}

void CollapsibleComponent::paintEvent(QPaintEvent * event)
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void CollapsibleComponent::setText(const QString& text)
{
  m_collapsibleComponentHeader->setText(text);
}

void CollapsibleComponent::setNumResults(int numResults)
{
  m_collapsibleComponentHeader->setNumResults(numResults);
}

void CollapsibleComponent::setNumPages(int numPages)
{
  m_collapsibleComponentHeader->setNumPages(numPages);
}

void CollapsibleComponent::firstPage()
{
  m_collapsibleComponentHeader->firstPage();
}

///! SLOTS

void CollapsibleComponent::on_headerClicked(bool checked)
{
  setExpanded(checked);
}

void CollapsibleComponent::on_componentClicked(bool checked)
{
}

void CollapsibleComponent::on_getComponentsByPage(int pageIdx)
{
}

} // openstudio
