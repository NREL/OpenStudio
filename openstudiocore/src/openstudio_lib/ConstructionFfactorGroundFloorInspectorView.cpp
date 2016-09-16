/**********************************************************************
*  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

  m_nameEdit = new OSLineEdit2();
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

  m_ffactorEdit = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &ConstructionFfactorGroundFloorInspectorView::toggleUnitsClicked, m_ffactorEdit, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_ffactorEdit, row, 0);

  ++row;

  // Area

  label = new QLabel("Area: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label, row, 0);

  ++row;

  m_areaEdit = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &ConstructionFfactorGroundFloorInspectorView::toggleUnitsClicked, m_areaEdit, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_areaEdit, row, 0);

  ++row;

  // Perimeter Exposed

  label = new QLabel("Perimeter Exposed: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label, row, 0);

  ++row;

  m_perimeterExposedEdit = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &ConstructionFfactorGroundFloorInspectorView::toggleUnitsClicked, m_perimeterExposedEdit, &OSQuantityEdit2::onUnitSystemChange);
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
  m_fFactorGroundFloorConstruction = fFactorGroundFloorConstruction;

  m_nameEdit->bind(
    *m_fFactorGroundFloorConstruction,
    OptionalStringGetter(std::bind(&model::FFactorGroundFloorConstruction::name, m_fFactorGroundFloorConstruction.get_ptr(),true)),
    boost::optional<StringSetter>(std::bind(&model::FFactorGroundFloorConstruction::setName, m_fFactorGroundFloorConstruction.get_ptr(),std::placeholders::_1))
  );

  m_ffactorEdit->bind(
    m_isIP,
    *m_fFactorGroundFloorConstruction,
    DoubleGetter(std::bind(&model::FFactorGroundFloorConstruction::fFactor, m_fFactorGroundFloorConstruction.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::FFactorGroundFloorConstruction::*)(double)>(&model::FFactorGroundFloorConstruction::setFFactor), m_fFactorGroundFloorConstruction.get_ptr(), std::placeholders::_1))
  );

  m_areaEdit->bind(
    m_isIP,
    *m_fFactorGroundFloorConstruction,
    DoubleGetter(std::bind(&model::FFactorGroundFloorConstruction::area, m_fFactorGroundFloorConstruction.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::FFactorGroundFloorConstruction::*)(double)>(&model::FFactorGroundFloorConstruction::setArea), m_fFactorGroundFloorConstruction.get_ptr(), std::placeholders::_1))
  );

  m_perimeterExposedEdit->bind(
    m_isIP,
    *m_fFactorGroundFloorConstruction,
    DoubleGetter(std::bind(&model::FFactorGroundFloorConstruction::perimeterExposed, m_fFactorGroundFloorConstruction.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::FFactorGroundFloorConstruction::*)(double)>(&model::FFactorGroundFloorConstruction::setPerimeterExposed), m_fFactorGroundFloorConstruction.get_ptr(), std::placeholders::_1))
  );
  
  m_standardsInformationWidget->attach(m_fFactorGroundFloorConstruction.get());
}

void ConstructionFfactorGroundFloorInspectorView::detach()
{
  m_ffactorEdit->unbind();
  m_areaEdit->unbind();
  m_perimeterExposedEdit->unbind();

  m_standardsInformationWidget->detach();

  m_fFactorGroundFloorConstruction = boost::none;
}

} // openstudio

