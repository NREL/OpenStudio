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

#include "WindowMaterialGlazingGroupThermochromicInspectorView.hpp"

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
    m_nameEdit(NULL),
    m_opticalDataTemperature(NULL),
    m_windowMaterialGlazingName(NULL),
    m_isIP(isIP)
{
  createLayout();
}

void WindowMaterialGlazingGroupThermochromicInspectorView::createLayout()
{
  QWidget* visibleWidget = new QWidget();
  this->stackedWidget()->addWidget(visibleWidget);

  QGridLayout* mainGridLayout = new QGridLayout();
  mainGridLayout->setContentsMargins(7,7,7,7);
  mainGridLayout->setSpacing(14);
  visibleWidget->setLayout(mainGridLayout);

  unsigned row = 0;
  unsigned col = 0;

  // Name

  QLabel * label = new QLabel("Name: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_nameEdit = new OSLineEdit();
  mainGridLayout->addWidget(m_nameEdit,row++,0,1,3);

  // Optical Data Temperature

  label = new QLabel("Optical Data Temperature: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);
  label->hide();

  m_opticalDataTemperature = new OSQuantityEdit2("C", "C", "F", m_isIP);
  bool isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_opticalDataTemperature, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_opticalDataTemperature,row++,0,1,3);
  m_opticalDataTemperature->hide();

  // Window ThermochromicGlazing Glazing Name

  label = new QLabel("Window ThermochromicGlazing Glazing Name: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_windowMaterialGlazingName = new OSLineEdit();
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
  m_nameEdit->bind(thermochromicGlazing,"name");

  //m_opticalDataTemperature->bind( // TODO
  //  m_isIP,
  //  thermochromicGlazing,
  //  DoubleGetter(std::bind(&model::ThermochromicGlazing::opticalDataTemperature,thermochromicGlazing)),
  //  DoubleSetterVoidReturn(std::bind(&model::ThermochromicGlazing::setOpticalDataTemperature,thermochromicGlazing,_1)));

  m_windowMaterialGlazingName->bind(thermochromicGlazing,"windowMaterialGlazingName");

  this->stackedWidget()->setCurrentIndex(1);
}

void WindowMaterialGlazingGroupThermochromicInspectorView::detach()
{
  this->stackedWidget()->setCurrentIndex(0);

  m_nameEdit->unbind();
  m_opticalDataTemperature->unbind();
  m_windowMaterialGlazingName->unbind();
}

void WindowMaterialGlazingGroupThermochromicInspectorView::refresh()
{
}

void WindowMaterialGlazingGroupThermochromicInspectorView::toggleUnits(bool displayIP)
{
  m_isIP = displayIP;
}

} // openstudio
