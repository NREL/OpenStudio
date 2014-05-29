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

#include "BuildingComponentDialogCentralWidget.hpp"
#include "BuildingComponentDialog.hpp"
#include "CollapsibleComponent.hpp"
#include "CollapsibleComponentHeader.hpp"
#include "CollapsibleComponentList.hpp"
#include "Component.hpp"
#include "ComponentList.hpp"

#include "../utilities/bcl/BCL.hpp"
#include "../utilities/bcl/LocalBCL.hpp"
#include "../utilities/bcl/RemoteBCL.hpp"
#include "../utilities/data/Attribute.hpp"
#include "../utilities/core/Assert.hpp"

#include <QApplication>
#include <QBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QProgressBar>
#include <QPushButton>
#include <QSettings>

namespace openstudio {

BuildingComponentDialogCentralWidget::BuildingComponentDialogCentralWidget(QWidget * parent)
  : QWidget(parent),
  m_tid(0),
  m_collapsibleComponentList(nullptr),
  m_componentList(nullptr), // TODO cruft to be removed
  m_progressBar(nullptr),
  m_pendingDownloads(std::set<std::string>()),
  m_pageIdx(0),
  m_searchString(QString()),
  m_showNewComponents(false)
{
  init();
}

BuildingComponentDialogCentralWidget::BuildingComponentDialogCentralWidget(int tid, QWidget * parent)
  : QWidget(parent),
  m_tid(0),
  m_collapsibleComponentList(nullptr),
  m_componentList(nullptr), // TODO cruft to be removed
  m_progressBar(nullptr),
  m_pendingDownloads(std::set<std::string>()),
  m_pageIdx(0),
  m_searchString(QString())
{
  init();
}

void BuildingComponentDialogCentralWidget::init()
{
  createLayout();
}

void BuildingComponentDialogCentralWidget::createLayout()
{
  bool isConnected = false;

  QLabel * label = new QLabel("Sort by:");
  label->hide(); // TODO remove this hack when we have sorts to do

  auto comboBox = new QComboBox(this);
  comboBox->hide(); // TODO remove this hack when we have sorts to do

  isConnected = connect(comboBox, SIGNAL(currentIndexChanged(const QString &)),
                             this, SLOT(comboBoxIndexChanged(const QString &)));
  OS_ASSERT(isConnected);

  QPushButton * upperPushButton = new QPushButton("Check All");
  isConnected = connect(upperPushButton, SIGNAL(clicked()),
                        this, SLOT(upperPushButtonClicked()));
  OS_ASSERT(isConnected);

  auto upperLayout = new QHBoxLayout();
  upperLayout->addWidget(label);
  upperLayout->addWidget(comboBox);
  upperLayout->addStretch();
  upperLayout->addWidget(upperPushButton);

  m_collapsibleComponentList = new CollapsibleComponentList();

  isConnected = connect(m_collapsibleComponentList, SIGNAL(headerClicked(bool)),
                        this, SIGNAL(headerClicked(bool)));
  OS_ASSERT(isConnected);

  isConnected = connect(m_collapsibleComponentList, SIGNAL(headerClicked(bool)),
                        this, SLOT(on_headerClicked(bool)));
  OS_ASSERT(isConnected);

  isConnected = connect(m_collapsibleComponentList, SIGNAL(componentClicked(bool)),
                        this, SIGNAL(componentClicked(bool)));
  OS_ASSERT(isConnected);

  isConnected = connect(m_collapsibleComponentList, SIGNAL(componentClicked(bool)),
                        this, SLOT(on_componentClicked(bool)));
  OS_ASSERT(isConnected);

  isConnected = connect(m_collapsibleComponentList, SIGNAL(collapsibleComponentClicked(bool)),
                        this, SIGNAL(collapsibleComponentClicked(bool)));
  OS_ASSERT(isConnected);

  isConnected = connect(m_collapsibleComponentList, SIGNAL(collapsibleComponentClicked(bool)),
                        this, SLOT(on_collapsibleComponentClicked(bool)));
  OS_ASSERT(isConnected);

  isConnected = connect(m_collapsibleComponentList, SIGNAL(getComponentsByPage(int)),
                        this, SIGNAL(getComponentsByPage(int)));
  OS_ASSERT(isConnected);

  isConnected = connect(m_collapsibleComponentList, SIGNAL(getComponentsByPage(int)),
                        this, SLOT(on_getComponentsByPage(int)));
  OS_ASSERT(isConnected);

  //*******************************************************************
  // Hack code to be removed (TODO)
  m_componentList = new ComponentList();  // TODO refactor and remove

  CollapsibleComponentHeader * collapsibleComponentHeader = nullptr;
  collapsibleComponentHeader = new CollapsibleComponentHeader("Constructions",100,5);

  CollapsibleComponent * collapsibleComponent = nullptr;
  collapsibleComponent = new CollapsibleComponent(collapsibleComponentHeader,m_componentList);

  m_collapsibleComponentList->addCollapsibleComponent(collapsibleComponent);
  //*******************************************************************
  
  m_progressBar = new QProgressBar(this);
  m_progressBar->setVisible(false);

  QPushButton * lowerPushButton = new QPushButton("Download");
  isConnected = connect(lowerPushButton, SIGNAL(clicked()),
                        this, SLOT(lowerPushButtonClicked()));
  OS_ASSERT(isConnected);

  auto lowerLayout = new QHBoxLayout();
  lowerLayout->addStretch();
  lowerLayout->addWidget(m_progressBar);
  lowerLayout->addWidget(lowerPushButton);

  auto mainLayout = new QVBoxLayout();
  mainLayout->addLayout(upperLayout);

  mainLayout->addWidget(m_collapsibleComponentList,0,Qt::AlignTop);
  mainLayout->addLayout(lowerLayout);
  setLayout(mainLayout);
}

int BuildingComponentDialogCentralWidget::tid()
{
  return m_tid;
}

int BuildingComponentDialogCentralWidget::pageIdx()
{
  return m_pageIdx;
}

void BuildingComponentDialogCentralWidget::setTid()
{
  requestComponents(m_filterType, m_tid, m_pageIdx, m_searchString);
}

// Note: don't call this directly if the "wait" screen is desired
void BuildingComponentDialogCentralWidget::setTid(const std::string& filterType,
                                                  int tid,
                                                  int pageIdx,
                                                  const QString & title,
                                                  const QString & searchString)
{

  if(m_tid != tid || m_searchString != searchString){
    m_collapsibleComponentList->firstPage();
  }

  m_filterType = filterType;

  m_tid = tid;

  m_searchString = searchString;

  //std::vector<Component *> components = m_collapsibleComponentList->components();
  std::vector<Component *> components = m_componentList->components();  // TODO replace with code above

  for( auto & comp : components)
  {
    delete comp;
  }

  RemoteBCL remoteBCL;
  std::vector<BCLSearchResult> responses;
  if (filterType == "components")
  {
    responses = remoteBCL.searchComponentLibrary(searchString.toStdString(),tid,pageIdx);
  }
  else if (filterType == "measures")
  {
    responses = remoteBCL.searchMeasureLibrary(searchString.toStdString(),tid,pageIdx);
  }

  for( const auto & response : responses)
  {
    auto component = new Component(response);
    
    // TODO replace with a componentList owned by m_collapsibleComponentList
    m_componentList->addComponent(component);
  }

  // the parent taxonomy
  m_collapsibleComponentList->setText(title);

  // the total number of results
  int lastTotalResults = remoteBCL.lastTotalResults();
  m_collapsibleComponentList->setNumResults(lastTotalResults);

  // the number of pages of results
  int numResultPages = remoteBCL.numResultPages();
  m_collapsibleComponentList->setNumPages(numResultPages);

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

  emit componentsReady();

}

///! Slots

void BuildingComponentDialogCentralWidget::upperPushButtonClicked()
{
  for (Component* component : m_collapsibleComponentList->components()) {
    if (component->checkBox()->isEnabled()){
      component->checkBox()->setChecked(true);
    }
  }
}

void BuildingComponentDialogCentralWidget::lowerPushButtonClicked()
{
  for (Component* component : m_collapsibleComponentList->components()) {
    if (component->checkBox()->isChecked() && component->checkBox()->isEnabled()){
      
      auto remoteBCL = new RemoteBCL();

      if (m_filterType == "components")
      {
        bool isConnected = connect(remoteBCL, SIGNAL(componentDownloaded(const std::string&, const boost::optional<BCLComponent>&)),
                                   this, SLOT(componentDownloadComplete(const std::string&, const boost::optional<BCLComponent>&)));
        OS_ASSERT(isConnected);

        bool downloadStarted = remoteBCL->downloadComponent(component->uid());
        if (downloadStarted){

          component->checkBox()->setEnabled(false);
          component->msg()->setHidden(true);
          m_pendingDownloads.insert(component->uid());

          // show busy
          m_progressBar->setValue(1);
          m_progressBar->setMinimum(0);
          m_progressBar->setMaximum(0);
          m_progressBar->setVisible(true);

        }else{

          delete remoteBCL;

          // todo: show error

        }
      }
      else if (m_filterType == "measures")
      {
        bool isConnected = connect(remoteBCL, SIGNAL(measureDownloaded(const std::string&, const boost::optional<BCLMeasure>&)),
                                   this, SLOT(measureDownloadComplete(const std::string&, const boost::optional<BCLMeasure>&)));
        OS_ASSERT(isConnected);

        bool downloadStarted = remoteBCL->downloadMeasure(component->uid());
        if (downloadStarted){

          component->checkBox()->setEnabled(false);
          component->msg()->setHidden(true);
          m_pendingDownloads.insert(component->uid());

          // show busy
          m_progressBar->setValue(1);
          m_progressBar->setMinimum(0);
          m_progressBar->setMaximum(0);
          m_progressBar->setVisible(true);

        }else{

          delete remoteBCL;

          // todo: show error

        }
      }
    }
  }
}

void BuildingComponentDialogCentralWidget::comboBoxIndexChanged(const QString & text)
{
}

void BuildingComponentDialogCentralWidget::componentDownloadComplete(const std::string& uid, const boost::optional<BCLComponent>& component)
{
  QObject* sender = this->sender();
  if (sender){
    sender->deleteLater();
  }

  if (component){
    // good
    // remove old component
    boost::optional<BCLComponent> oldComponent = LocalBCL::instance().getComponent(component->uid());
    if (oldComponent && oldComponent->versionId() != component->versionId()){
      LocalBCL::instance().removeComponent(*oldComponent);
    }
  }else{
    // error downloading component
    // find component in list by uid and re-enable
    for (Component* component : m_collapsibleComponentList->components()) {
      if (component->uid() == uid){
        component->checkBox()->setEnabled(true);
        break;
      }
    }
  }

  m_pendingDownloads.erase(uid);
  if (m_pendingDownloads.empty()){
    // show not busy
    m_progressBar->setValue(0);
    m_progressBar->setMinimum(0);
    m_progressBar->setMaximum(0);
    m_progressBar->setVisible(false);
    m_showNewComponents = true;
  }
}

void BuildingComponentDialogCentralWidget::measureDownloadComplete(const std::string& uid, const boost::optional<BCLMeasure>& measure)
{
  QObject* sender = this->sender();
  if (sender){
    sender->deleteLater();
  }

  if (measure){
    // good
    // remove old measure
    boost::optional<BCLMeasure> oldMeasure = LocalBCL::instance().getMeasure(measure->uid());
    if (oldMeasure && oldMeasure->versionId() != measure->versionId()){
      LocalBCL::instance().removeMeasure(*oldMeasure);
    }
  }else{
    // error downloading measure
    // find measure in list by uid and re-enable
    for (Component* component : m_collapsibleComponentList->components()) {
      if (component->uid() == uid){
        component->checkBox()->setEnabled(true);
        break;
      }
    }
  }

  m_pendingDownloads.erase(uid);
  if (m_pendingDownloads.empty()){
    // show not busy
    m_progressBar->setValue(0);
    m_progressBar->setMinimum(0);
    m_progressBar->setMaximum(0);
    m_progressBar->setVisible(false);
    m_showNewComponents = true;
  }
}

Component * BuildingComponentDialogCentralWidget::checkedComponent() const
{
  return m_collapsibleComponentList->checkedComponent();
}

bool BuildingComponentDialogCentralWidget::showNewComponents()
{
  return m_showNewComponents;
}

void BuildingComponentDialogCentralWidget::setShowNewComponents(bool showNewComponents)
{
  m_showNewComponents = showNewComponents;
}

///! SLOTS

void BuildingComponentDialogCentralWidget::on_headerClicked(bool checked)
{
}

void BuildingComponentDialogCentralWidget::on_componentClicked(bool checked)
{
}

void BuildingComponentDialogCentralWidget::on_collapsibleComponentClicked(bool checked)
{
}

void BuildingComponentDialogCentralWidget::on_getComponentsByPage(int pageIdx)
{
  m_pageIdx = pageIdx;
  setTid();
}

} // namespace openstudio
