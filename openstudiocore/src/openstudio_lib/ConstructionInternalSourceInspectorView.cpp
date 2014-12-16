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

#include "ConstructionInternalSourceInspectorView.hpp"

#include "ConstructionObjectVectorController.hpp"
#include "ModelObjectItem.hpp"
#include "OSAppBase.hpp"
#include "OSDocument.hpp"
#include "OSDropZone.hpp"
#include "../shared_gui_components/OSIntegerEdit.hpp"
#include "../shared_gui_components/OSLineEdit.hpp"
#include "../shared_gui_components/OSQuantityEdit.hpp"
#include "../shared_gui_components/OSComboBox.hpp"

#include "../model/ConstructionWithInternalSource.hpp"
#include "../model/ConstructionWithInternalSource_Impl.hpp"
#include "../model/Material.hpp"
#include "../model/Material_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include <utilities/idd/OS_Construction_InternalSource_FieldEnums.hxx>

#include <QGridLayout>
#include <QLabel>
#include <QStackedWidget>
#include <QComboBox>

namespace openstudio {

// ConstructionInternalSourceInspectorView

ConstructionInternalSourceInspectorView::ConstructionInternalSourceInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent)
  : ConstructionBaseInspectorView(isIP, model, parent),
    m_constructionDZ(nullptr),
    m_sourcePresentAfterLayerNumberEdit(nullptr),
    m_temperatureCalculationRequestedAfterLayerNumberEdit(nullptr),
    m_dimensionsForTheCTFCalculationEdit(nullptr),
    m_tubeSpacingEdit(nullptr)
{
  createLayout();
}

void ConstructionInternalSourceInspectorView::createLayout()
{
  ConstructionBaseInspectorView::createLayout();

  int row = m_mainGridLayout->rowCount();

  QLabel * label = nullptr;

  // Layer

  label = new QLabel("Layer: ");
  label->setObjectName("H2");
  m_mainGridLayout->addWidget(label,row,0);

  ++row;

  label = new QLabel("Outside");
  label->setObjectName("H2");
  m_mainGridLayout->addWidget(label,row,0);

  ++row;

  m_constructionVC = new ConstructionObjectVectorController(this);
  m_constructionDZ = new OSDropZone(m_constructionVC,"Drag From Library",QSize(0,0),false);
  m_constructionDZ->setMinItems(0);
  m_constructionDZ->setMaxItems(16);
  m_constructionDZ->setItemsAcceptDrops(true);
  m_constructionDZ->setFixedSize(QSize(OSItem::ITEM_WIDTH + 20,600));
  m_mainGridLayout->addWidget(m_constructionDZ,row,0);

  ++row;

  label = new QLabel("Inside");
  label->setObjectName("H2");
  m_mainGridLayout->addWidget(label,row,0);

  ++row;

  // Source Present After Layer

  label = new QLabel("Source Present After Layer: ");
  label->setObjectName("H2");
  m_mainGridLayout->addWidget(label,row,0);

  ++row;

  m_sourcePresentAfterLayerNumberEdit = new OSIntegerEdit();
  m_mainGridLayout->addWidget(m_sourcePresentAfterLayerNumberEdit,row,0);

  ++row;

  // Temperature Calculation Requested After Layer Number

  label = new QLabel("Temperature Calculation Requested After Layer Number: ");
  label->setObjectName("H2");
  m_mainGridLayout->addWidget(label,row,0,1,3);

  ++row;

  m_temperatureCalculationRequestedAfterLayerNumberEdit = new OSIntegerEdit();
  m_mainGridLayout->addWidget(m_temperatureCalculationRequestedAfterLayerNumberEdit,row,0);

  ++row;

  // Dimensions for the CTF Calculation

  label = new QLabel("Dimensions for the CTF Calculation: ");
  label->setObjectName("H2");
  m_mainGridLayout->addWidget(label,row,0,1,3);

  ++row;

  m_dimensionsForTheCTFCalculationEdit = new OSIntegerEdit();
  m_mainGridLayout->addWidget(m_dimensionsForTheCTFCalculationEdit,row,0);

  ++row;

  // Tube Spacing

  label = new QLabel("Tube Spacing: ");
  label->setObjectName("H2");
  m_mainGridLayout->addWidget(label,row,0);

  ++row;

  m_tubeSpacingEdit = new OSQuantityEdit(m_isIP);
  connect(this, &ConstructionInternalSourceInspectorView::toggleUnitsClicked, m_tubeSpacingEdit, &OSQuantityEdit::onUnitSystemChange);
  m_mainGridLayout->addWidget(m_tubeSpacingEdit,row,0);

  ++row;

  // Stretch

  m_mainGridLayout->setRowStretch(row,100);

  m_mainGridLayout->setColumnStretch(100,100);
}

void ConstructionInternalSourceInspectorView::onClearSelection()
{
  ConstructionBaseInspectorView::onClearSelection(); // call parent implementation
  detach();
}

void ConstructionInternalSourceInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  detach();
  model::ConstructionWithInternalSource constructionWithInternalSource = modelObject.cast<model::ConstructionWithInternalSource>();
  attach(constructionWithInternalSource);
  refresh();
}

void ConstructionInternalSourceInspectorView::onUpdate()
{
  refresh();
}

void ConstructionInternalSourceInspectorView::standardsConstructionTypeChanged(const QString & text)
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

void ConstructionInternalSourceInspectorView::editStandardsConstructionType(const QString & text)
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

void ConstructionInternalSourceInspectorView::populateStandardsConstructionType()
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

  connect(m_standardsConstructionType, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged), this, &ConstructionInternalSourceInspectorView::standardsConstructionTypeChanged);
  connect(m_standardsConstructionType, &QComboBox::editTextChanged, this, &ConstructionInternalSourceInspectorView::editStandardsConstructionType);
}

void ConstructionInternalSourceInspectorView::attach(openstudio::model::ConstructionWithInternalSource & constructionWithInternalSource)
{
  m_nameEdit->bind(constructionWithInternalSource,"name");

  m_sourcePresentAfterLayerNumberEdit->bind(constructionWithInternalSource,"sourcePresentAfterLayerNumber");
  m_temperatureCalculationRequestedAfterLayerNumberEdit->bind(constructionWithInternalSource,"temperatureCalculationRequestedAfterLayerNumber");
  m_dimensionsForTheCTFCalculationEdit->bind(constructionWithInternalSource,"dimensionsForTheCTFCalculation");

  m_tubeSpacingEdit->bind(constructionWithInternalSource,"tubeSpacing",m_isIP);

  m_constructionVC->attach(constructionWithInternalSource);
  m_constructionVC->reportItems();

  m_standardsInformation = constructionWithInternalSource.standardsInformation();

  m_intendedSurfaceType->bind<std::string>(
      *m_standardsInformation,
      static_cast<std::string (*)(const std::string&)>(&openstudio::toString),
      std::bind(&openstudio::model::StandardsInformationConstruction::intendedSurfaceTypeValues),
      std::function<boost::optional<std::string> ()>(std::bind(&openstudio::model::StandardsInformationConstruction::intendedSurfaceType,m_standardsInformation.get_ptr())),
      std::bind(&openstudio::model::StandardsInformationConstruction::setIntendedSurfaceType,m_standardsInformation.get_ptr(),std::placeholders::_1),
      NoFailAction(std::bind(&model::StandardsInformationConstruction::resetIntendedSurfaceType,m_standardsInformation.get_ptr())));

  connect(m_standardsInformation->getImpl<openstudio::model::detail::ModelObject_Impl>().get(), &openstudio::model::detail::ModelObject_Impl::onChange, this, &ConstructionInternalSourceInspectorView::populateStandardsConstructionType);

  m_standardsConstructionType->setEnabled(true);
  populateStandardsConstructionType();

  this->stackedWidget()->setCurrentIndex(1);
}

void ConstructionInternalSourceInspectorView::detach()
{
  this->stackedWidget()->setCurrentIndex(0);

  m_constructionVC->detach();

  m_nameEdit->unbind();
  m_sourcePresentAfterLayerNumberEdit->unbind();
  m_temperatureCalculationRequestedAfterLayerNumberEdit->unbind();
  m_dimensionsForTheCTFCalculationEdit->unbind();
  m_tubeSpacingEdit->unbind();

  if (m_standardsInformation){
    disconnect(m_standardsInformation->getImpl<openstudio::model::detail::ModelObject_Impl>().get(), 0, this, 0);
    m_standardsInformation.reset();
  }

  m_intendedSurfaceType->unbind();

  disconnect(m_standardsConstructionType, 0, this, 0);
  m_standardsConstructionType->setEnabled(false);
}

void ConstructionInternalSourceInspectorView::refresh()
{
}

} // openstudio
