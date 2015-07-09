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
