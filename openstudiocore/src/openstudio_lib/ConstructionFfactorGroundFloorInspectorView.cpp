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

#include "ConstructionFfactorGroundFloorInspectorView.hpp"
#include "StandardsInformationConstructionWidget.hpp"
#include "OSItem.hpp"

#include "../shared_gui_components/OSLineEdit.hpp"
#include "../shared_gui_components/OSQuantityEdit.hpp"
#include "../shared_gui_components/OSComboBox.hpp"

#include "../model/FFactorGroundFloorConstruction.hpp"
#include "../model/FFactorGroundFloorConstruction_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <QGridLayout>
#include <QLabel>
#include <QStackedWidget>
#include <QComboBox>

namespace openstudio {

ConstructionFfactorGroundFloorInspectorView::ConstructionFfactorGroundFloorInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent)
  : ModelObjectInspectorView(model, true, parent),
    m_isIP(isIP),
    m_nameEdit(nullptr),
    m_standardsInformationWidget(nullptr),
    m_ffactorEdit(nullptr),
    m_areaEdit(nullptr),
    m_perimeterExposedEdit(nullptr)
{
  createLayout();
}

void ConstructionFfactorGroundFloorInspectorView::createLayout()
{
  QWidget* hiddenWidget = new QWidget();
  this->stackedWidget()->addWidget(hiddenWidget);

  QWidget* visibleWidget = new QWidget();
  this->stackedWidget()->addWidget(visibleWidget);

  QGridLayout* mainGridLayout = new QGridLayout();
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

  // F-Factor

  label = new QLabel("F-Factor: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label, row, 0);

  ++row;

  m_ffactorEdit = new OSQuantityEdit(m_isIP);
  connect(this, &ConstructionFfactorGroundFloorInspectorView::toggleUnitsClicked, m_ffactorEdit, &OSQuantityEdit::onUnitSystemChange);
  mainGridLayout->addWidget(m_ffactorEdit, row, 0);

  ++row;

  // Area

  label = new QLabel("Area: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label, row, 0);

  ++row;

  m_areaEdit = new OSQuantityEdit(m_isIP);
  connect(this, &ConstructionFfactorGroundFloorInspectorView::toggleUnitsClicked, m_areaEdit, &OSQuantityEdit::onUnitSystemChange);
  mainGridLayout->addWidget(m_areaEdit, row, 0);

  ++row;

  // Perimeter Exposed

  label = new QLabel("Perimeter Exposed: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label, row, 0);

  ++row;

  m_perimeterExposedEdit = new OSQuantityEdit(m_isIP);
  connect(this, &ConstructionFfactorGroundFloorInspectorView::toggleUnitsClicked, m_perimeterExposedEdit, &OSQuantityEdit::onUnitSystemChange);
  mainGridLayout->addWidget(m_perimeterExposedEdit, row, 0);

  ++row;

  // Stretch

  mainGridLayout->setRowStretch(row, 100);

  mainGridLayout->setColumnStretch(100, 100);
}

void ConstructionFfactorGroundFloorInspectorView::onClearSelection()
{
  detach();

  this->stackedWidget()->setCurrentIndex(0);
}

void ConstructionFfactorGroundFloorInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  detach();
  model::FFactorGroundFloorConstruction fFactorGroundFloorConstruction = modelObject.cast<model::FFactorGroundFloorConstruction>();
  attach(fFactorGroundFloorConstruction);

  this->stackedWidget()->setCurrentIndex(1);
}

void ConstructionFfactorGroundFloorInspectorView::onUpdate()
{
}

void ConstructionFfactorGroundFloorInspectorView::attach(openstudio::model::FFactorGroundFloorConstruction & fFactorGroundFloorConstruction)
{
  m_nameEdit->bind(fFactorGroundFloorConstruction,"name");
  m_ffactorEdit->bind(fFactorGroundFloorConstruction,"fFactor",m_isIP);
  m_areaEdit->bind(fFactorGroundFloorConstruction,"area",m_isIP);
  m_perimeterExposedEdit->bind(fFactorGroundFloorConstruction,"perimeterExposed",m_isIP);
  
  m_standardsInformationWidget->attach(fFactorGroundFloorConstruction);
}

void ConstructionFfactorGroundFloorInspectorView::detach()
{
  m_ffactorEdit->unbind();
  m_areaEdit->unbind();
  m_perimeterExposedEdit->unbind();

  m_standardsInformationWidget->detach();
}

} // openstudio

