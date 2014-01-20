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

#include <openstudio_lib/ConstructionInternalSourceInspectorView.hpp>

#include <openstudio_lib/ConstructionObjectVectorController.hpp>
#include <openstudio_lib/ModelObjectItem.hpp>
#include <openstudio_lib/OSAppBase.hpp>
#include <openstudio_lib/OSDocument.hpp>
#include <openstudio_lib/OSDropZone.hpp>
#include "../shared_gui_components/OSIntegerEdit.hpp"
#include "../shared_gui_components/OSLineEdit.hpp"
#include "../shared_gui_components/OSQuantityEdit.hpp"

#include <model/ConstructionWithInternalSource.hpp>
#include <model/ConstructionWithInternalSource_Impl.hpp>
#include <model/Material.hpp>
#include <model/Material_Impl.hpp>

#include <utilities/core/Assert.hpp>
#include <utilities/idd/OS_Construction_InternalSource_FieldEnums.hxx>

#include <QGridLayout>
#include <QLabel>
#include <QStackedWidget>

namespace openstudio {

// ConstructionInternalSourceInspectorView

ConstructionInternalSourceInspectorView::ConstructionInternalSourceInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent)
  : ModelObjectInspectorView(model, true, parent),
    m_constructionDZ(NULL),
    m_nameEdit(NULL),
    m_sourcePresentAfterLayerNumberEdit(NULL),
    m_temperatureCalculationRequestedAfterLayerNumberEdit(NULL),
    m_dimensionsForTheCTFCalculationEdit(NULL),
    m_tubeSpacingEdit(NULL),
    m_isIP(isIP)
{
  createLayout();
}

void ConstructionInternalSourceInspectorView::createLayout()
{
  QWidget* visibleWidget = new QWidget();
  this->stackedWidget()->addWidget(visibleWidget);

  QGridLayout* mainGridLayout = new QGridLayout();
  mainGridLayout->setContentsMargins(7,7,7,7);
  mainGridLayout->setSpacing(14);
  visibleWidget->setLayout(mainGridLayout);

  // Name

  QLabel* label = new QLabel("Name: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,0,0);

  m_nameEdit = new OSLineEdit();
  mainGridLayout->addWidget(m_nameEdit,1,0,1,3);

  // Layer

  label = new QLabel("Layer: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,2,0);

  label = new QLabel("Outside");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,3,0);

  m_constructionVC = new ConstructionObjectVectorController(this);
  m_constructionDZ = new OSDropZone(m_constructionVC,false);
  m_constructionDZ->setMinItems(0);
  m_constructionDZ->setMaxItems(16);
  m_constructionDZ->setItemsAcceptDrops(true);
  m_constructionDZ->setFixedSize(QSize(OSItem::ITEM_WIDTH + 20,600));
  mainGridLayout->addWidget(m_constructionDZ,4,0);

  label = new QLabel("Inside");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,5,0);

  // Source Present After Layer

  label = new QLabel("Source Present After Layer: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,6,0);

  bool isConnected = false;

  m_sourcePresentAfterLayerNumberEdit = new OSIntegerEdit();
  mainGridLayout->addWidget(m_sourcePresentAfterLayerNumberEdit,7,0);

  // Temperature Calculation Requested After Layer Number

  label = new QLabel("Temperature Calculation Requested After Layer Number: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,8,0);

  m_temperatureCalculationRequestedAfterLayerNumberEdit = new OSIntegerEdit();
  mainGridLayout->addWidget(m_temperatureCalculationRequestedAfterLayerNumberEdit,9,0);

  // Dimensions for the CTF Calculation

  label = new QLabel("Dimensions for the CTF Calculation: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,10,0);

  m_dimensionsForTheCTFCalculationEdit = new OSIntegerEdit();
  mainGridLayout->addWidget(m_dimensionsForTheCTFCalculationEdit,11,0);

  // Tube Spacing

  label = new QLabel("Tube Spacing: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,12,0);

  m_tubeSpacingEdit = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_tubeSpacingEdit, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_tubeSpacingEdit,13,0);

  // Stretch

  mainGridLayout->setRowStretch(100,100);

  mainGridLayout->setColumnStretch(100,100);
}

void ConstructionInternalSourceInspectorView::onClearSelection()
{
  ModelObjectInspectorView::onClearSelection(); // call parent implementation
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

void ConstructionInternalSourceInspectorView::attach(openstudio::model::ConstructionWithInternalSource & constructionWithInternalSource)
{
  m_nameEdit->bind(constructionWithInternalSource,"name");

  m_sourcePresentAfterLayerNumberEdit->bind(constructionWithInternalSource,"sourcePresentAfterLayerNumber");
  m_temperatureCalculationRequestedAfterLayerNumberEdit->bind(constructionWithInternalSource,"temperatureCalculationRequestedAfterLayerNumber");
  m_dimensionsForTheCTFCalculationEdit->bind(constructionWithInternalSource,"dimensionsForTheCTFCalculation");

  m_tubeSpacingEdit->bind(constructionWithInternalSource,"tubeSpacing",m_isIP);

  m_constructionVC->attach(constructionWithInternalSource);
  m_constructionVC->reportItems();

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
}

void ConstructionInternalSourceInspectorView::refresh()
{
}

void ConstructionInternalSourceInspectorView::toggleUnits(bool displayIP)
{
  m_isIP = displayIP;
}

} // openstudio
