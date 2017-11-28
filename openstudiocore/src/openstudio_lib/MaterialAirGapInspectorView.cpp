/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "MaterialAirGapInspectorView.hpp"

#include "StandardsInformationMaterialWidget.hpp"

#include "../shared_gui_components/OSLineEdit.hpp"
#include "../shared_gui_components/OSQuantityEdit.hpp"

#include "../model/AirGap.hpp"
#include "../model/AirGap_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <QGridLayout>
#include <QLabel>
#include <QStackedWidget>

namespace openstudio {

// MaterialAirGapInspectorView

MaterialAirGapInspectorView::MaterialAirGapInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent)
  : ModelObjectInspectorView(model, true, parent),
    m_isIP(isIP),
    m_thermalResistance(nullptr)
{
  createLayout();
}

void MaterialAirGapInspectorView::createLayout()
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

  m_standardsInformationWidget = new StandardsInformationMaterialWidget(m_isIP, mainGridLayout, row);

  ++row;

  // Thermal Resistance

  label = new QLabel("Thermal Resistance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_thermalResistance = new OSQuantityEdit2("m^2*K/W","m^2*K/W","ft^2*h*R/Btu", m_isIP);
  connect(this, &MaterialAirGapInspectorView::toggleUnitsClicked, m_thermalResistance, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_thermalResistance,row++,0,1,3);

  // Stretch

  mainGridLayout->setRowStretch(100,100);

  mainGridLayout->setColumnStretch(100,100);
}

void MaterialAirGapInspectorView::onClearSelection()
{
  ModelObjectInspectorView::onClearSelection(); // call parent implementation
  detach();
}

void MaterialAirGapInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  detach();
  model::AirGap airGap = modelObject.cast<model::AirGap>();
  attach(airGap);
  refresh();
}

void MaterialAirGapInspectorView::onUpdate()
{
  refresh();
}

void MaterialAirGapInspectorView::attach(openstudio::model::AirGap & airGap)
{
  m_airGap = airGap;

  // m_nameEdit->bind(airGap,"name");
  m_nameEdit->bind(
    *m_airGap,
    OptionalStringGetter(std::bind(&model::AirGap::name, m_airGap.get_ptr(),true)),
    boost::optional<StringSetter>(std::bind(&model::AirGap::setName, m_airGap.get_ptr(),std::placeholders::_1))
  );

  // m_thermalResistance->bind(airGap,"thermalResistance",m_isIP);
  m_thermalResistance->bind(
    m_isIP,
    *m_airGap,
    DoubleGetter(std::bind(&model::AirGap::thermalResistance, m_airGap.get_ptr())),
    //static_cast<void(Client::*)(int)>(&Client::foobar)
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::AirGap::*)(double)>(&model::AirGap::setThermalResistance), m_airGap.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::AirGap::resetThermalResistance, m_airGap.get_ptr()))
  );

  m_standardsInformationWidget->attach(airGap);

  this->stackedWidget()->setCurrentIndex(1);
}

void MaterialAirGapInspectorView::detach()
{
  this->stackedWidget()->setCurrentIndex(0);

  m_nameEdit->unbind();
  m_thermalResistance->unbind();

  m_airGap = boost::none;

  m_standardsInformationWidget->detach();
}

void MaterialAirGapInspectorView::refresh()
{
}

} // openstudio
