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

#include "SyncMeasuresDialogCentralWidget.hpp"

#include "../shared_gui_components/CollapsibleComponent.hpp"
#include "../shared_gui_components/CollapsibleComponentHeader.hpp"
#include "../shared_gui_components/CollapsibleComponentList.hpp"
#include "../shared_gui_components/Component.hpp"
#include "../shared_gui_components/ComponentList.hpp"
#include "../shared_gui_components/MeasureManager.hpp"
#include "../shared_gui_components/SyncMeasuresDialog.hpp"

#include "../utilities/plot/ProgressBar.hpp"
#include "../utilities/core/Assert.hpp"

#include <QBoxLayout>
#include <QCheckBox>
#include <QPushButton>

#define NUM_COMPONENTS_DISPLAYED 10

namespace openstudio {

SyncMeasuresDialogCentralWidget::SyncMeasuresDialogCentralWidget(const WorkflowJSON& workflow,
  MeasureManager * measureManager,
  QWidget * parent)
  : QWidget(parent),
  m_collapsibleComponentList(nullptr),
  m_componentList(nullptr),
  m_pageIdx(0),
  m_workflow(workflow),
  m_measureManager(measureManager)
{
  init();
}

void SyncMeasuresDialogCentralWidget::init()
{
  createLayout();
}

void SyncMeasuresDialogCentralWidget::createLayout()
{

  QPushButton * upperPushButton = new QPushButton("Check All");
  connect(upperPushButton, &QPushButton::clicked, this, &SyncMeasuresDialogCentralWidget::upperPushButtonClicked);

  auto upperLayout = new QHBoxLayout();
  upperLayout->addStretch();
  upperLayout->addWidget(upperPushButton);

  m_collapsibleComponentList = new CollapsibleComponentList();

  connect(m_collapsibleComponentList, &CollapsibleComponentList::componentClicked, this, &SyncMeasuresDialogCentralWidget::componentClicked);

  connect(m_collapsibleComponentList, &CollapsibleComponentList::getComponentsByPage, this, &SyncMeasuresDialogCentralWidget::getComponentsByPage);

  connect(m_collapsibleComponentList, &CollapsibleComponentList::getComponentsByPage, this, &SyncMeasuresDialogCentralWidget::on_getComponentsByPage);

  //*******************************************************************

  m_componentList = new ComponentList();

  CollapsibleComponentHeader * collapsibleComponentHeader = nullptr;
  collapsibleComponentHeader = new CollapsibleComponentHeader("Updates",100,5);

  CollapsibleComponent * collapsibleComponent = nullptr;
  collapsibleComponent = new CollapsibleComponent(collapsibleComponentHeader,m_componentList);

  m_collapsibleComponentList->addCollapsibleComponent(collapsibleComponent);

  //*******************************************************************

  progressBar = new QProgressBar(this);
  progressBar->setOrientation(Qt::Horizontal);
  progressBar->setVisible(false);
  progressBar->setTextVisible(false);

  lowerPushButton = new QPushButton("Update");
  connect(lowerPushButton, &QPushButton::clicked, this, &SyncMeasuresDialogCentralWidget::lowerPushButtonClicked);

  auto lowerLayout = new QHBoxLayout();
  lowerLayout->addWidget(progressBar);
  lowerLayout->addStretch();
  lowerLayout->addWidget(lowerPushButton);

  auto mainLayout = new QVBoxLayout();
  mainLayout->addLayout(upperLayout);

  mainLayout->addWidget(m_collapsibleComponentList,0,Qt::AlignTop);
  mainLayout->addLayout(lowerLayout);
  setLayout(mainLayout);
}

void SyncMeasuresDialogCentralWidget::setMeasures(const std::vector<BCLMeasure> & measures)
{
  m_measures = measures;

  // the total number of results
  m_collapsibleComponentList->setNumResults(m_measures.size());

  // the number of pages of results
  int numResultPages = m_measures.size() / NUM_COMPONENTS_DISPLAYED;
  if (m_measures.size() % NUM_COMPONENTS_DISPLAYED != 0 ){
    numResultPages++;
  }
  m_collapsibleComponentList->setNumPages(numResultPages);

  m_collapsibleComponentList->firstPage();
  
  m_pageIdx = 0;

  displayMeasures(m_pageIdx);

}

void SyncMeasuresDialogCentralWidget::displayMeasures(int pageIdx)
{
  std::vector<Component *> components = m_componentList->components();

  for( auto it = components.begin();
       it != components.end();
       ++it )
  {
    delete *it;
  }

  int startPoint = pageIdx * NUM_COMPONENTS_DISPLAYED;
  int endPoint = (pageIdx + 1) * NUM_COMPONENTS_DISPLAYED - 1;

  if(endPoint >= static_cast<int>(m_measures.size())){
    endPoint = m_measures.size() - 1;
  }

  for( int i = startPoint;
       i <= endPoint;
       ++i )
  {
    auto component = new Component(m_measures.at(i));
    
    m_componentList->addComponent(component);
  }

  // make sure the header is expanded
  if(m_collapsibleComponentList->checkedCollapsibleComponent()){
    m_collapsibleComponentList->checkedCollapsibleComponent()->setExpanded(true);
  }

  // select the first component
  if(m_componentList->firstComponent()){
    m_componentList->firstComponent()->setChecked(true);
  }
  else{
    emit noComponents();
  }

  // check all the components for download
  upperPushButtonClicked();
}

Component * SyncMeasuresDialogCentralWidget::checkedComponent() const
{
  return m_collapsibleComponentList->checkedComponent();
}

///! SLOTS

void SyncMeasuresDialogCentralWidget::upperPushButtonClicked()
{
  for (Component* component : m_collapsibleComponentList->components()){
    if (component->checkBox()->isEnabled()){
      component->checkBox()->setChecked(true);
    }
  }
}

void SyncMeasuresDialogCentralWidget::lowerPushButtonClicked()
{
  std::vector<BCLMeasure> newMeasures;

  // Must convert from the checked component to the appropriate measure for updating
  unsigned index = 0;
  for (Component* component : m_collapsibleComponentList->components()){
    if (component->checkBox()->isChecked() && component->checkBox()->isEnabled()){
      newMeasures.push_back(m_measures.at(m_pageIdx * NUM_COMPONENTS_DISPLAYED + index));
    }
    ++index;
  }

  if(!newMeasures.empty()){
    bool showMessage = true;
    m_measureManager->updateMeasures(newMeasures,showMessage);

    // clear the internal state of measure manager 
    // DLM: this could be more selective in the future
    m_measureManager->reset();
  }

  emit closeDlg();

}

void SyncMeasuresDialogCentralWidget::on_getComponentsByPage(int pageIdx)
{
  m_pageIdx = pageIdx;

  displayMeasures(m_pageIdx);
}

} // namespace openstudio
