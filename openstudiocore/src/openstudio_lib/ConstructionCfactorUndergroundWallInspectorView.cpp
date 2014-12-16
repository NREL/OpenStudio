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
  : ConstructionBaseInspectorView(isIP, model, parent),
    m_cfactorEdit(nullptr),
    m_heightEdit(nullptr)
{
  createLayout();
}

void ConstructionCfactorUndergroundWallInspectorView::createLayout()
{
  ConstructionBaseInspectorView::createLayout();

  int row = m_mainGridLayout->rowCount();

  QLabel * label = nullptr;

  // C-Factor

  label = new QLabel("C-Factor: ");
  label->setObjectName("H2");
  m_mainGridLayout->addWidget(label,row,0);

  ++row;

  m_cfactorEdit = new OSQuantityEdit(m_isIP);
  connect(this, &ConstructionCfactorUndergroundWallInspectorView::toggleUnitsClicked, m_cfactorEdit, &OSQuantityEdit::onUnitSystemChange);
  m_mainGridLayout->addWidget(m_cfactorEdit,row,0);

  ++row;

  // Height

  label = new QLabel("Height: ");
  label->setObjectName("H2");
  m_mainGridLayout->addWidget(label,row,0);

  ++row;

  m_heightEdit = new OSQuantityEdit(m_isIP);
  connect(this, &ConstructionCfactorUndergroundWallInspectorView::toggleUnitsClicked, m_heightEdit, &OSQuantityEdit::onUnitSystemChange);
  m_mainGridLayout->addWidget(m_heightEdit,row,0);

  ++row;

  // Stretch

  m_mainGridLayout->setRowStretch(row,100);

  m_mainGridLayout->setColumnStretch(100,100);
}

void ConstructionCfactorUndergroundWallInspectorView::onClearSelection()
{
  ConstructionBaseInspectorView::onClearSelection(); // call parent implementation
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

  connect(m_standardsConstructionType, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged), this, &ConstructionCfactorUndergroundWallInspectorView::standardsConstructionTypeChanged);
  connect(m_standardsConstructionType, &QComboBox::editTextChanged, this, &ConstructionCfactorUndergroundWallInspectorView::editStandardsConstructionType);
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

  connect(m_standardsInformation->getImpl<openstudio::model::detail::ModelObject_Impl>().get(), &openstudio::model::detail::ModelObject_Impl::onChange, this, &ConstructionCfactorUndergroundWallInspectorView::populateStandardsConstructionType);

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

} // openstudio

