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

#include "BuildingComponentDialogCentralWidget.hpp"
#include "BuildingComponentDialog.hpp"
#include "CollapsibleComponent.hpp"
#include "CollapsibleComponentHeader.hpp"
#include "CollapsibleComponentList.hpp"
#include "Component.hpp"
#include "ComponentList.hpp"
#include "BaseApp.hpp"
#include "MeasureManager.hpp"

#include "../measure/OSArgument.hpp"

#include "../utilities/bcl/BCL.hpp"
#include "../utilities/bcl/LocalBCL.hpp"
#include "../utilities/bcl/RemoteBCL.hpp"
#include "../utilities/data/Attribute.hpp"
#include "../utilities/core/Application.hpp"
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

  QLabel * label = new QLabel("Sort by:");
  label->hide(); // TODO remove this hack when we have sorts to do

  auto comboBox = new QComboBox(this);
  comboBox->hide(); // TODO remove this hack when we have sorts to do

  connect(comboBox, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged), this, &BuildingComponentDialogCentralWidget::comboBoxIndexChanged);

  QPushButton * upperPushButton = new QPushButton("Check All");
  connect(upperPushButton, &QPushButton::clicked, this, &BuildingComponentDialogCentralWidget::upperPushButtonClicked);

  auto upperLayout = new QHBoxLayout();
  upperLayout->addWidget(label);
  upperLayout->addWidget(comboBox);
  upperLayout->addStretch();
  upperLayout->addWidget(upperPushButton);

  m_collapsibleComponentList = new CollapsibleComponentList();

  connect(m_collapsibleComponentList, &CollapsibleComponentList::headerClicked, this, &BuildingComponentDialogCentralWidget::headerClicked);

  connect(m_collapsibleComponentList, &CollapsibleComponentList::headerClicked, this, &BuildingComponentDialogCentralWidget::on_headerClicked);

  connect(m_collapsibleComponentList, &CollapsibleComponentList::componentClicked, this, &BuildingComponentDialogCentralWidget::componentClicked);

  connect(m_collapsibleComponentList, &CollapsibleComponentList::componentClicked, this, &BuildingComponentDialogCentralWidget::on_componentClicked);

  connect(m_collapsibleComponentList, &CollapsibleComponentList::collapsibleComponentClicked, this, &BuildingComponentDialogCentralWidget::collapsibleComponentClicked);

  connect(m_collapsibleComponentList, &CollapsibleComponentList::collapsibleComponentClicked, this, &BuildingComponentDialogCentralWidget::on_collapsibleComponentClicked);

  connect(m_collapsibleComponentList, &CollapsibleComponentList::getComponentsByPage, this, &BuildingComponentDialogCentralWidget::getComponentsByPage);

  connect(m_collapsibleComponentList, &CollapsibleComponentList::getComponentsByPage, this, &BuildingComponentDialogCentralWidget::on_getComponentsByPage);

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
  connect(lowerPushButton, &QPushButton::clicked, this, &BuildingComponentDialogCentralWidget::lowerPushButtonClicked);

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
        connect(remoteBCL, &RemoteBCL::componentDownloaded, this, &BuildingComponentDialogCentralWidget::componentDownloadComplete);

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
        connect(remoteBCL, &RemoteBCL::measureDownloaded, this, &BuildingComponentDialogCentralWidget::measureDownloadComplete);

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

  BaseApp* app = dynamic_cast<BaseApp*>(Application::instance().application());
  if (app){
    if (measure){
      try{
        app->measureManager().getArguments(*measure);
      } catch (const std::exception&){

      }
    }
    // DLM: handled in OSDocument::on_closeMeasuresBclDlg
    // app->measureManager().updateMeasuresLists();
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
