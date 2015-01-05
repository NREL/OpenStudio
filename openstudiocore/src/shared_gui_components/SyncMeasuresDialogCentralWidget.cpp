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

#include "SyncMeasuresDialogCentralWidget.hpp"

#include "../shared_gui_components/CollapsibleComponent.hpp"
#include "../shared_gui_components/CollapsibleComponentHeader.hpp"
#include "../shared_gui_components/CollapsibleComponentList.hpp"
#include "../shared_gui_components/Component.hpp"
#include "../shared_gui_components/ComponentList.hpp"
#include "../shared_gui_components/MeasureManager.hpp"
#include "../shared_gui_components/SyncMeasuresDialog.hpp"

#include "../analysisdriver/SimpleProject.hpp"

#include "../utilities/plot/ProgressBar.hpp"
#include "../utilities/core/Assert.hpp"

#include <QBoxLayout>
#include <QCheckBox>
#include <QPushButton>

#define NUM_COMPONENTS_DISPLAYED 10

namespace openstudio {

SyncMeasuresDialogCentralWidget::SyncMeasuresDialogCentralWidget(analysisdriver::SimpleProject * project,
  MeasureManager * measureManager,
  QWidget * parent)
  : QWidget(parent),
  m_collapsibleComponentList(nullptr),
  m_componentList(nullptr),
  m_pageIdx(0),
  m_project(project),
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

  QHBoxLayout * upperLayout = new QHBoxLayout();
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

  QHBoxLayout * lowerLayout = new QHBoxLayout();
  lowerLayout->addWidget(progressBar);
  lowerLayout->addStretch();
  lowerLayout->addWidget(lowerPushButton);

  QVBoxLayout * mainLayout = new QVBoxLayout();
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

  for( std::vector<Component *>::iterator it = components.begin();
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
    Component * component = new Component(m_measures.at(i));
    
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
    m_measureManager->updateMeasures(*m_project,newMeasures,showMessage);
  }

  emit closeDlg();

}

void SyncMeasuresDialogCentralWidget::on_getComponentsByPage(int pageIdx)
{
  m_pageIdx = pageIdx;

  displayMeasures(m_pageIdx);
}

} // namespace openstudio
