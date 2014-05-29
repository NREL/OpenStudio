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

#include "ConstructionCfactorUndergroundWallInspectorView.hpp"
#include "OSItem.hpp"

#include "../shared_gui_components/OSLineEdit.hpp"
#include "../shared_gui_components/OSQuantityEdit.hpp"
#include "../shared_gui_components/OSComboBox.hpp"

#include "../model/CFactorUndergroundWallConstruction.hpp"
#include "../model/CFactorUndergroundWallConstruction_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <QGridLayout>
#include <QLabel>
#include <QStackedWidget>
#include <QComboBox>

namespace openstudio {

ConstructionCfactorUndergroundWallInspectorView::ConstructionCfactorUndergroundWallInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent)
  : ModelObjectInspectorView(model, true, parent),
    m_nameEdit(NULL),
    m_cfactorEdit(NULL),
    m_heightEdit(NULL),
    m_isIP(isIP)
{
  createLayout();
}

void ConstructionCfactorUndergroundWallInspectorView::createLayout()
{
  QWidget* visibleWidget = new QWidget();
  this->stackedWidget()->addWidget(visibleWidget);

  QGridLayout* mainGridLayout = new QGridLayout();
  mainGridLayout->setContentsMargins(7,7,7,7);
  mainGridLayout->setSpacing(14);
  visibleWidget->setLayout(mainGridLayout);

  int row = 0;

  // Name

  QLabel* label = new QLabel("Name: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row,0);

  ++row;

  m_nameEdit = new OSLineEdit();
  mainGridLayout->addWidget(m_nameEdit,row,0,1,3);

  ++row;

  // Standards
  QFrame * line;
  line = new QFrame();
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow(QFrame::Sunken);
  mainGridLayout->addWidget(line,row,0,1,3);

  ++row;

  label = new QLabel();
  label->setText("Measure Tags (Optional):");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row,0);

  ++row;

  QVBoxLayout* vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Intended Surface Type: ");
  label->setObjectName("StandardsInfo");
  vLayout->addWidget(label);

  m_intendedSurfaceType = new OSComboBox2();
  m_intendedSurfaceType->setFixedWidth(OSItem::ITEM_WIDTH);
  vLayout->addWidget(m_intendedSurfaceType);

  mainGridLayout->addLayout(vLayout,row,0);

  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Standards Construction Type: ");
  label->setObjectName("StandardsInfo");
  vLayout->addWidget(label);

  m_standardsConstructionType = new QComboBox();
  m_standardsConstructionType->setEditable(true);
  m_standardsConstructionType->setDuplicatesEnabled(false);
  m_standardsConstructionType->setFixedWidth(OSItem::ITEM_WIDTH);
  vLayout->addWidget(m_standardsConstructionType);

  mainGridLayout->addLayout(vLayout,row,1);

  ++row;

  line = new QFrame();
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow(QFrame::Sunken);
  mainGridLayout->addWidget(line,row,0,1,3);

  ++row;

  // C-Factor

  label = new QLabel("C-Factor: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row,0);

  ++row;

  bool isConnected = false;

  m_cfactorEdit = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_cfactorEdit, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_cfactorEdit,row,0);

  ++row;

  // Height

  label = new QLabel("Height: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row,0);

  ++row;

  m_heightEdit = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_heightEdit, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_heightEdit,row,0);

  ++row;

  // Stretch

  mainGridLayout->setRowStretch(row,100);

  mainGridLayout->setColumnStretch(100,100);
}

void ConstructionCfactorUndergroundWallInspectorView::onClearSelection()
{
  ModelObjectInspectorView::onClearSelection(); // call parent implementation
  detach();
}

void ConstructionCfactorUndergroundWallInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  detach();
  model::CFactorUndergroundWallConstruction cFactorUndergroundWallConstruction = modelObject.cast<model::CFactorUndergroundWallConstruction>();
  attach(cFactorUndergroundWallConstruction);
  refresh();
}

void ConstructionCfactorUndergroundWallInspectorView::onUpdate()
{
  refresh();
}

void ConstructionCfactorUndergroundWallInspectorView::standardsConstructionTypeChanged(const QString & text)
{
  if (m_standardsInformation){
    std::string standardsConstructionType = toString(text);
    if (standardsConstructionType.empty()){
      m_standardsInformation->resetStandardsConstructionType();
    }else{
      m_standardsInformation->setStandardsConstructionType(standardsConstructionType);
    }
    populateStandardsConstructionType();
  }
}

void ConstructionCfactorUndergroundWallInspectorView::editStandardsConstructionType(const QString & text)
{
  if (m_standardsInformation){
    std::string standardsConstructionType = toString(text);
    if (standardsConstructionType.empty()){
      m_standardsInformation->resetStandardsConstructionType();
    }else{
      m_standardsInformation->setStandardsConstructionType(standardsConstructionType);
    }
  }
}

void ConstructionCfactorUndergroundWallInspectorView::populateStandardsConstructionType()
{

  disconnect(m_standardsConstructionType, 0, this, 0);

  m_standardsConstructionType->clear();
  if (m_standardsInformation){
    m_standardsConstructionType->addItem("");
    std::vector<std::string> suggestedStandardsConstructionTypes = m_standardsInformation->suggestedStandardsConstructionTypes();
    for (const std::string& standardsConstructionType : suggestedStandardsConstructionTypes) {
      m_standardsConstructionType->addItem(toQString(standardsConstructionType));
    }
    boost::optional<std::string> standardsConstructionType = m_standardsInformation->standardsConstructionType();
    if (standardsConstructionType){
      OS_ASSERT(!suggestedStandardsConstructionTypes.empty());
      m_standardsConstructionType->setCurrentIndex(1);
    }else{
      m_standardsConstructionType->setCurrentIndex(0);
    }
  }

  bool isConnected = false;
  isConnected = connect(m_standardsConstructionType, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(standardsConstructionTypeChanged(const QString&)));
  OS_ASSERT(isConnected);
  isConnected = connect(m_standardsConstructionType, SIGNAL(editTextChanged(const QString&)), this, SLOT(editStandardsConstructionType(const QString&)));
  OS_ASSERT(isConnected);
}

void ConstructionCfactorUndergroundWallInspectorView::attach(openstudio::model::CFactorUndergroundWallConstruction & cFactorUndergroundWallConstruction)
{
  m_nameEdit->bind(cFactorUndergroundWallConstruction,"name");
  m_cfactorEdit->bind(cFactorUndergroundWallConstruction,"cFactor",m_isIP);
  m_heightEdit->bind(cFactorUndergroundWallConstruction,"height",m_isIP);

  m_standardsInformation = cFactorUndergroundWallConstruction.standardsInformation();
  if (!m_standardsInformation->intendedSurfaceType()){
    m_standardsInformation->setIntendedSurfaceType("GroundContactWall");
  }

  m_intendedSurfaceType->bind<std::string>(
      *m_standardsInformation,
      static_cast<std::string (*)(const std::string&)>(&openstudio::toString),
      &openstudio::model::StandardsInformationConstruction::intendedSurfaceTypeValues,
      std::function<boost::optional<std::string> ()>(std::bind(&openstudio::model::StandardsInformationConstruction::intendedSurfaceType,m_standardsInformation.get_ptr())),
      std::bind(&openstudio::model::StandardsInformationConstruction::setIntendedSurfaceType,m_standardsInformation.get_ptr(),std::placeholders::_1),
      NoFailAction(std::bind(&model::StandardsInformationConstruction::resetIntendedSurfaceType,m_standardsInformation.get_ptr())));

  bool test = connect(m_standardsInformation->getImpl<openstudio::model::detail::ModelObject_Impl>().get(), SIGNAL(onChange()), this, SLOT(populateStandardsConstructionType()));
  OS_ASSERT(test);

  m_standardsConstructionType->setEnabled(true);
  populateStandardsConstructionType();

  this->stackedWidget()->setCurrentIndex(1);
}

void ConstructionCfactorUndergroundWallInspectorView::detach()
{
  this->stackedWidget()->setCurrentIndex(0);

  m_nameEdit->unbind();
  m_cfactorEdit->unbind();
  m_heightEdit->unbind();

  if (m_standardsInformation){
    disconnect(m_standardsInformation->getImpl<openstudio::model::detail::ModelObject_Impl>().get(), 0, this, 0);
    m_standardsInformation.reset();
  }

  m_intendedSurfaceType->unbind();

  disconnect(m_standardsConstructionType, 0, this, 0);
  m_standardsConstructionType->setEnabled(false);
}

void ConstructionCfactorUndergroundWallInspectorView::refresh()
{
}

void ConstructionCfactorUndergroundWallInspectorView::toggleUnits(bool displayIP)
{
  m_isIP = displayIP;
}

} // openstudio

