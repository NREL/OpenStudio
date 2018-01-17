/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "WindowMaterialDaylightRedirectionDeviceInspectorView.hpp"

#include "StandardsInformationMaterialWidget.hpp"

#include "../shared_gui_components/OSComboBox.hpp"
#include "../shared_gui_components/OSLineEdit.hpp"
#include "../shared_gui_components/OSQuantityEdit.hpp"

#include "../model/DaylightRedirectionDevice.hpp"
#include "../model/DaylightRedirectionDevice_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <QGridLayout>
#include <QLabel>
#include <QStackedWidget>

namespace openstudio {

// WindowMaterialDaylightRedirectionDeviceInspectorView

WindowMaterialDaylightRedirectionDeviceInspectorView::WindowMaterialDaylightRedirectionDeviceInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent)
  : ModelObjectInspectorView(model, true, parent),
    m_isIP(isIP)
{
  createLayout();
}

void WindowMaterialDaylightRedirectionDeviceInspectorView::createLayout()
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

  // Daylight Redirection Device Type

  label = new QLabel("Daylight Redirection Device Type: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label, row, 0);

  ++row;

  m_daylightRedirectionDeviceType = new OSComboBox2();
  mainGridLayout->addWidget(m_daylightRedirectionDeviceType, row, 0, 1, 3);

  ++row;

  // Stretch

  mainGridLayout->setRowStretch(100,100);

  mainGridLayout->setColumnStretch(100,100);
}

void WindowMaterialDaylightRedirectionDeviceInspectorView::onClearSelection()
{
  ModelObjectInspectorView::onClearSelection(); // call parent implementation
  detach();
}

void WindowMaterialDaylightRedirectionDeviceInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  detach();
  model::DaylightRedirectionDevice material = modelObject.cast<model::DaylightRedirectionDevice>();
  attach(material);
  refresh();
}

void WindowMaterialDaylightRedirectionDeviceInspectorView::onUpdate()
{
  refresh();
}

void WindowMaterialDaylightRedirectionDeviceInspectorView::attach(openstudio::model::DaylightRedirectionDevice & material)
{
  m_material = material;

  m_nameEdit->bind(
    *m_material,
    OptionalStringGetter(std::bind(&model::DaylightRedirectionDevice::name, m_material.get_ptr(), true)),
    boost::optional<StringSetter>(std::bind(&model::DaylightRedirectionDevice::setName, m_material.get_ptr(), std::placeholders::_1))
  );

  m_daylightRedirectionDeviceType->bind<std::string>(
    *m_material,
    static_cast<std::string(*)(const std::string&)>(&openstudio::toString),
    std::bind(&openstudio::model::DaylightRedirectionDevice::daylightRedirectionDeviceTypeValues),
    StringGetter(std::bind(&openstudio::model::DaylightRedirectionDevice::daylightRedirectionDeviceType, m_material.get_ptr())),
    StringSetter(std::bind(&openstudio::model::DaylightRedirectionDevice::setDaylightRedirectionDeviceType, m_material.get_ptr(), std::placeholders::_1)),
    NoFailAction(std::bind(&model::DaylightRedirectionDevice::resetDaylightRedirectionDeviceType, m_material.get_ptr()))
  );


  m_standardsInformationWidget->attach(material);

  this->stackedWidget()->setCurrentIndex(1);
}

void WindowMaterialDaylightRedirectionDeviceInspectorView::detach()
{
  m_nameEdit->unbind();
  m_daylightRedirectionDeviceType->unbind();

  this->stackedWidget()->setCurrentIndex(0);

  m_standardsInformationWidget->detach();

  m_material.reset();
}

void WindowMaterialDaylightRedirectionDeviceInspectorView::refresh()
{
}

} // openstudio
