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

#include "WindowMaterialSimpleGlazingSystemInspectorView.hpp"

#include "StandardsInformationMaterialWidget.hpp"

#include "../shared_gui_components/OSLineEdit.hpp"
#include "../shared_gui_components/OSQuantityEdit.hpp"

#include "../model/SimpleGlazing.hpp"
#include "../model/SimpleGlazing_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <QGridLayout>
#include <QLabel>
#include <QStackedWidget>

namespace openstudio {

// WindowMaterialSimpleGlazingSystemInspectorView

WindowMaterialSimpleGlazingSystemInspectorView::WindowMaterialSimpleGlazingSystemInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent)
  : ModelObjectInspectorView(model, true, parent),
    m_isIP(isIP)
{
  createLayout();
}

void WindowMaterialSimpleGlazingSystemInspectorView::createLayout()
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

  m_standardsInformationWidget = new StandardsInformationMaterialWidget(m_isIP, mainGridLayout, row);

  ++row;

  // U-Factor

  label = new QLabel("U-Factor: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_uFactor = new OSQuantityEdit(m_isIP);
  connect(this, &WindowMaterialSimpleGlazingSystemInspectorView::toggleUnitsClicked, m_uFactor, &OSQuantityEdit::onUnitSystemChange);
  mainGridLayout->addWidget(m_uFactor,row++,0,1,3);

  // Solar Heat Gain Coefficient

  label = new QLabel("Solar Heat Gain Coefficient: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_solarHeatGainCoefficient = new OSQuantityEdit(m_isIP);
  connect(this, &WindowMaterialSimpleGlazingSystemInspectorView::toggleUnitsClicked, m_solarHeatGainCoefficient, &OSQuantityEdit::onUnitSystemChange);
  mainGridLayout->addWidget(m_solarHeatGainCoefficient,row++,0,1,3);

  // Visible Transmittance

  label = new QLabel("Visible Transmittance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_visibleTransmittance = new OSQuantityEdit(m_isIP);
  connect(this, &WindowMaterialSimpleGlazingSystemInspectorView::toggleUnitsClicked, m_visibleTransmittance, &OSQuantityEdit::onUnitSystemChange);
  mainGridLayout->addWidget(m_visibleTransmittance,row++,0,1,3);

  // Stretch

  mainGridLayout->setRowStretch(100,100);

  mainGridLayout->setColumnStretch(100,100);
}

void WindowMaterialSimpleGlazingSystemInspectorView::onClearSelection()
{
  ModelObjectInspectorView::onClearSelection(); // call parent implementation
  detach();
}

void WindowMaterialSimpleGlazingSystemInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  detach();
  model::SimpleGlazing simpleGlazing = modelObject.cast<model::SimpleGlazing>();
  attach(simpleGlazing);
  refresh();
}

void WindowMaterialSimpleGlazingSystemInspectorView::onUpdate()
{
  refresh();
}

void WindowMaterialSimpleGlazingSystemInspectorView::attach(openstudio::model::SimpleGlazing & simpleGlazing)
{
  m_nameEdit->bind(simpleGlazing,"name");
  m_uFactor->bind(simpleGlazing,"uFactor",m_isIP);
  m_solarHeatGainCoefficient->bind(simpleGlazing,"solarHeatGainCoefficient",m_isIP);
  m_visibleTransmittance->bind(simpleGlazing,"visibleTransmittance",m_isIP);

  m_standardsInformationWidget->attach(simpleGlazing);

  this->stackedWidget()->setCurrentIndex(1);
}

void WindowMaterialSimpleGlazingSystemInspectorView::detach()
{
  this->stackedWidget()->setCurrentIndex(0);

  m_nameEdit->unbind();
  m_uFactor->unbind();
  m_solarHeatGainCoefficient->unbind();
  m_visibleTransmittance->unbind();

  m_standardsInformationWidget->detach();
}

void WindowMaterialSimpleGlazingSystemInspectorView::refresh()
{
}

} // openstudio
