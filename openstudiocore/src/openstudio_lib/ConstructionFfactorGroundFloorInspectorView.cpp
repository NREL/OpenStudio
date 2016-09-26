/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
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

