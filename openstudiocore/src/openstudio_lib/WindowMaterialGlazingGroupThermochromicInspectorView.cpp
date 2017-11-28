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

#include "WindowMaterialGlazingGroupThermochromicInspectorView.hpp"

#include "StandardsInformationMaterialWidget.hpp"

#include "../shared_gui_components/OSLineEdit.hpp"
#include "../shared_gui_components/OSQuantityEdit.hpp"

#include "../model/ThermochromicGlazing.hpp"
#include "../model/ThermochromicGlazing_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <QGridLayout>
#include <QLabel>
#include <QStackedWidget>

namespace openstudio {

// WindowMaterialGlazingGroupThermochromicInspectorView

WindowMaterialGlazingGroupThermochromicInspectorView::WindowMaterialGlazingGroupThermochromicInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent)
  : ModelObjectInspectorView(model, true, parent),
    m_isIP(isIP)
{
  createLayout();
}

void WindowMaterialGlazingGroupThermochromicInspectorView::createLayout()
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

  // Optical Data Temperature

  label = new QLabel("Optical Data Temperature: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);
  label->hide();

  m_opticalDataTemperature = new OSQuantityEdit2("C", "C", "F", m_isIP);
  connect(this, &WindowMaterialGlazingGroupThermochromicInspectorView::toggleUnitsClicked, m_opticalDataTemperature, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_opticalDataTemperature,row++,0,1,3);
  m_opticalDataTemperature->hide();

  // Window ThermochromicGlazing Glazing Name

  label = new QLabel("Window ThermochromicGlazing Glazing Name: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_windowMaterialGlazingName = new OSLineEdit2();
  mainGridLayout->addWidget(m_windowMaterialGlazingName,row++,0,1,3);

  // Stretch

  mainGridLayout->setRowStretch(100,100);

  mainGridLayout->setColumnStretch(100,100);
}

void WindowMaterialGlazingGroupThermochromicInspectorView::onClearSelection()
{
  ModelObjectInspectorView::onClearSelection(); // call parent implementation
  detach();
}

void WindowMaterialGlazingGroupThermochromicInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  detach();
  model::ThermochromicGlazing thermochromicGlazing = modelObject.cast<model::ThermochromicGlazing>();
  attach(thermochromicGlazing);
  refresh();
}

void WindowMaterialGlazingGroupThermochromicInspectorView::onUpdate()
{
  refresh();
}

void WindowMaterialGlazingGroupThermochromicInspectorView::attach(openstudio::model::ThermochromicGlazing & thermochromicGlazing)
{
  // m_nameEdit->bind(thermochromicGlazing,"name");
  m_thermochromicGlazing = thermochromicGlazing;
  m_nameEdit->bind(
    *m_thermochromicGlazing,
    OptionalStringGetter(std::bind(&model::ThermochromicGlazing::name, m_thermochromicGlazing.get_ptr(),true)),
    boost::optional<StringSetter>(std::bind(&model::ThermochromicGlazing::setName, m_thermochromicGlazing.get_ptr(),std::placeholders::_1))
  );

  //m_opticalDataTemperature->bind( // TODO
  //  m_isIP,
  //  thermochromicGlazing,
  //  DoubleGetter(std::bind(&model::ThermochromicGlazing::opticalDataTemperature,thermochromicGlazing)),
  //  DoubleSetterVoidReturn(std::bind(&model::ThermochromicGlazing::setOpticalDataTemperature,thermochromicGlazing,_1)));

  // m_windowMaterialGlazingName->bind(thermochromicGlazing,"windowMaterialGlazingName");

  // TODO: Reimplement when "windowMaterialGlazingName" is found
  // m_windowMaterialGlazingName->bind(
  //   *m_thermochromicGlazing,
  //   StringGetter(std::bind(&model::ThermochromicGlazing::windowMaterialGlazingName, m_thermochromicGlazing.get_ptr())),
  //   boost::optional<StringSetter>(std::bind(&model::ThermochromicGlazing::setWindowMaterialGlazingName, m_thermochromicGlazing.get_ptr(),std::placeholders::_1)),
  //   boost::optional<NoFailAction>(std::bind(&model::ThermochromicGlazing::resetWindowMaterialGlazingName, m_thermochromicGlazing.get_ptr())),
  //   boost::optional<BasicQuery>(std::bind(&model::ThermochromicGlazing::isWindowMaterialGlazingNameDefaulted, m_thermochromicGlazing.get_ptr()))
  // );

  m_standardsInformationWidget->attach(thermochromicGlazing);

  this->stackedWidget()->setCurrentIndex(1);
}

void WindowMaterialGlazingGroupThermochromicInspectorView::detach()
{
  this->stackedWidget()->setCurrentIndex(0);

  m_nameEdit->unbind();
  m_opticalDataTemperature->unbind();
  m_windowMaterialGlazingName->unbind();

  m_thermochromicGlazing = boost::none;

  m_standardsInformationWidget->detach();
}

void WindowMaterialGlazingGroupThermochromicInspectorView::refresh()
{
}

} // openstudio
