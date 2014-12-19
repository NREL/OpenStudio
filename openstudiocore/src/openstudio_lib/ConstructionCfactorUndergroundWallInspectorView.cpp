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
  : ModelObjectInspectorView(model, parent),
    m_isIP(isIP),
    m_nameEdit(nullptr),
    m_cfactorEdit(nullptr),
    m_heightEdit(nullptr)
{
  createLayout();
}

void ConstructionCfactorUndergroundWallInspectorView::createLayout()
{
  QWidget* visibleWidget = new QWidget();
  this->stackedWidget()->addWidget(visibleWidget);

  QGridLayout* mainGridLayout = new QGridLayout();
  mainGridLayout->setContentsMargins(7, 7, 7, 7);
  mainGridLayout->setSpacing(14);
  visibleWidget->setLayout(mainGridLayout);

  int row = mainGridLayout->rowCount();

  QLabel * label = nullptr;

  // C-Factor

  label = new QLabel("C-Factor: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label, row, 0);

  ++row;

  m_cfactorEdit = new OSQuantityEdit(m_isIP);
  connect(this, &ConstructionCfactorUndergroundWallInspectorView::toggleUnitsClicked, m_cfactorEdit, &OSQuantityEdit::onUnitSystemChange);
  mainGridLayout->addWidget(m_cfactorEdit, row, 0);

  ++row;

  // Height

  label = new QLabel("Height: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label, row, 0);

  ++row;

  m_heightEdit = new OSQuantityEdit(m_isIP);
  connect(this, &ConstructionCfactorUndergroundWallInspectorView::toggleUnitsClicked, m_heightEdit, &OSQuantityEdit::onUnitSystemChange);
  mainGridLayout->addWidget(m_heightEdit, row, 0);

  ++row;

  // Stretch

  mainGridLayout->setRowStretch(row, 100);

  mainGridLayout->setColumnStretch(100, 100);
}

void ConstructionCfactorUndergroundWallInspectorView::onClearSelection()
{
  detach();

  this->stackedWidget()->setCurrentIndex(0);
}

void ConstructionCfactorUndergroundWallInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  detach();
  model::CFactorUndergroundWallConstruction construction = modelObject.cast<model::CFactorUndergroundWallConstruction>();
  attach(construction);


  this->stackedWidget()->setCurrentIndex(1);
}

void ConstructionCfactorUndergroundWallInspectorView::onUpdate()
{
}

void ConstructionCfactorUndergroundWallInspectorView::attach(openstudio::model::CFactorUndergroundWallConstruction & cFactorUndergroundWallConstruction)
{
  m_nameEdit->bind(cFactorUndergroundWallConstruction,"name");
  m_cfactorEdit->bind(cFactorUndergroundWallConstruction,"cFactor",m_isIP);
  m_heightEdit->bind(cFactorUndergroundWallConstruction,"height",m_isIP);
  /*
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
  */
  
}

void ConstructionCfactorUndergroundWallInspectorView::detach()
{
  m_cfactorEdit->unbind();
  m_heightEdit->unbind();
}

} // openstudio

