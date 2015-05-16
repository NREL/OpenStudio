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

#include "ConstructionInternalSourceInspectorView.hpp"
#include "StandardsInformationConstructionWidget.hpp"
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
  : ModelObjectInspectorView(model, true, parent),
    m_isIP(isIP),
    m_nameEdit(nullptr),
    m_standardsInformationWidget(nullptr),
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
  auto hiddenWidget = new QWidget();
  this->stackedWidget()->addWidget(hiddenWidget);

  auto visibleWidget = new QWidget();
  this->stackedWidget()->addWidget(visibleWidget);

  auto mainGridLayout = new QGridLayout();
  mainGridLayout->setContentsMargins(7, 7, 7, 7);
  mainGridLayout->setSpacing(14);
  visibleWidget->setLayout(mainGridLayout);

  int row = mainGridLayout->rowCount();

  QLabel * label = nullptr;

  // Name

  label = new QLabel("Name: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label, row, 0);

  ++row;

  m_nameEdit = new OSLineEdit();
  mainGridLayout->addWidget(m_nameEdit, row, 0, 1, 3);

  ++row;

  // Standards Information

  m_standardsInformationWidget = new StandardsInformationConstructionWidget(m_isIP, mainGridLayout, row);
  m_standardsInformationWidget->hideFenestration();
  m_standardsInformationWidget->disableFenestration();

  ++row;

  // Layer

  label = new QLabel("Layer: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label, row, 0);

  ++row;

  label = new QLabel("Outside");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label, row, 0);

  ++row;

  m_constructionVC = new ConstructionObjectVectorController(this);
  m_constructionDZ = new OSDropZone(m_constructionVC,"Drag From Library",QSize(0,0),false);
  m_constructionDZ->setMinItems(0);
  m_constructionDZ->setMaxItems(16);
  m_constructionDZ->setItemsAcceptDrops(true);
  m_constructionDZ->setFixedSize(QSize(OSItem::ITEM_WIDTH + 20,600));
  mainGridLayout->addWidget(m_constructionDZ, row, 0);

  ++row;

  label = new QLabel("Inside");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label, row, 0);

  ++row;

  // Source Present After Layer

  label = new QLabel("Source Present After Layer: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label, row, 0);

  ++row;

  m_sourcePresentAfterLayerNumberEdit = new OSIntegerEdit();
  mainGridLayout->addWidget(m_sourcePresentAfterLayerNumberEdit, row, 0);

  ++row;

  // Temperature Calculation Requested After Layer Number

  label = new QLabel("Temperature Calculation Requested After Layer Number: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label, row, 0, 1, 3);

  ++row;

  m_temperatureCalculationRequestedAfterLayerNumberEdit = new OSIntegerEdit();
  mainGridLayout->addWidget(m_temperatureCalculationRequestedAfterLayerNumberEdit, row, 0);

  ++row;

  // Dimensions for the CTF Calculation

  label = new QLabel("Dimensions for the CTF Calculation: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label, row, 0, 1, 3);

  ++row;

  m_dimensionsForTheCTFCalculationEdit = new OSIntegerEdit();
  mainGridLayout->addWidget(m_dimensionsForTheCTFCalculationEdit, row, 0);

  ++row;

  // Tube Spacing

  label = new QLabel("Tube Spacing: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label, row, 0);

  ++row;

  m_tubeSpacingEdit = new OSQuantityEdit(m_isIP);
  connect(this, &ConstructionInternalSourceInspectorView::toggleUnitsClicked, m_tubeSpacingEdit, &OSQuantityEdit::onUnitSystemChange);
  mainGridLayout->addWidget(m_tubeSpacingEdit, row, 0);

  ++row;

  // Stretch

  mainGridLayout->setRowStretch(row, 100);

  mainGridLayout->setColumnStretch(100, 100);
}

void ConstructionInternalSourceInspectorView::onClearSelection()
{
  detach();

  this->stackedWidget()->setCurrentIndex(0);
}

void ConstructionInternalSourceInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  detach();
  model::ConstructionWithInternalSource constructionWithInternalSource = modelObject.cast<model::ConstructionWithInternalSource>();
  attach(constructionWithInternalSource);

  this->stackedWidget()->setCurrentIndex(1);
}

void ConstructionInternalSourceInspectorView::onUpdate()
{
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
  
  m_standardsInformationWidget->attach(constructionWithInternalSource);
}

void ConstructionInternalSourceInspectorView::detach()
{
  m_constructionVC->detach();

  m_sourcePresentAfterLayerNumberEdit->unbind();
  m_temperatureCalculationRequestedAfterLayerNumberEdit->unbind();
  m_dimensionsForTheCTFCalculationEdit->unbind();
  m_tubeSpacingEdit->unbind();

  m_standardsInformationWidget->detach();
}

} // openstudio
