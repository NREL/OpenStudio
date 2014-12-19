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

#include "ConstructionFfactorGroundFloorInspectorView.hpp"

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
  : ModelObjectInspectorView(model, parent),
    m_isIP(isIP),
    m_nameEdit(nullptr),
    m_ffactorEdit(nullptr),
    m_areaEdit(nullptr),
    m_perimeterExposedEdit(nullptr)
{
  createLayout();
}

void ConstructionFfactorGroundFloorInspectorView::createLayout()
{
  QWidget* visibleWidget = new QWidget();
  this->stackedWidget()->addWidget(visibleWidget);

  QGridLayout* mainGridLayout = new QGridLayout();
  mainGridLayout->setContentsMargins(7, 7, 7, 7);
  mainGridLayout->setSpacing(14);
  visibleWidget->setLayout(mainGridLayout);

  int row = mainGridLayout->rowCount();

  QLabel * label = nullptr;

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
  /*
  m_standardsInformation = fFactorGroundFloorConstruction.standardsInformation();
  if (!m_standardsInformation->intendedSurfaceType()){
    m_standardsInformation->setIntendedSurfaceType("GroundContactFloor");
  }

  m_intendedSurfaceType->bind<std::string>(
      *m_standardsInformation,
      static_cast<std::string (*)(const std::string&)>(&openstudio::toString),
      std::bind(&openstudio::model::StandardsInformationConstruction::intendedSurfaceTypeValues),
      std::function<boost::optional<std::string> ()>(std::bind(&openstudio::model::StandardsInformationConstruction::intendedSurfaceType,m_standardsInformation.get_ptr())),
      std::bind(&openstudio::model::StandardsInformationConstruction::setIntendedSurfaceType,m_standardsInformation.get_ptr(),std::placeholders::_1),
      NoFailAction(std::bind(&model::StandardsInformationConstruction::resetIntendedSurfaceType,m_standardsInformation.get_ptr())));

  connect(m_standardsInformation->getImpl<openstudio::model::detail::ModelObject_Impl>().get(), &openstudio::model::detail::ModelObject_Impl::onChange, this, &ConstructionFfactorGroundFloorInspectorView::populateStandardsConstructionType);

  m_standardsConstructionType->setEnabled(true);
  populateStandardsConstructionType();
  */
}

void ConstructionFfactorGroundFloorInspectorView::detach()
{
  m_ffactorEdit->unbind();
  m_areaEdit->unbind();
  m_perimeterExposedEdit->unbind();
}

} // openstudio

