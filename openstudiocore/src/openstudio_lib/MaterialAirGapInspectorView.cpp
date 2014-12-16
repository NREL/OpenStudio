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

#include "MaterialAirGapInspectorView.hpp"

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
  : MaterialBaseInspectorView(isIP, model, parent),
    m_thermalResistance(nullptr)
{
  createLayout();
}

void MaterialAirGapInspectorView::createLayout()
{
  QLabel * label = nullptr;

  unsigned row = m_mainGridLayout->rowCount();

  // Thermal Resistance

  label = new QLabel("Thermal Resistance: ");
  label->setObjectName("H2");
  m_mainGridLayout->addWidget(label,row++,0);

  m_thermalResistance = new OSQuantityEdit(m_isIP);
  connect(this, &MaterialAirGapInspectorView::toggleUnitsClicked, m_thermalResistance, &OSQuantityEdit::onUnitSystemChange);
  m_mainGridLayout->addWidget(m_thermalResistance,row++,0,1,3);

  // Stretch

  m_mainGridLayout->setRowStretch(100,100);

  m_mainGridLayout->setColumnStretch(100,100);
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
  m_nameEdit->bind(airGap,"name");
  m_thermalResistance->bind(airGap,"thermalResistance",m_isIP);

  this->stackedWidget()->setCurrentIndex(1);
}

void MaterialAirGapInspectorView::detach()
{
  this->stackedWidget()->setCurrentIndex(0);

  m_nameEdit->unbind();
  m_thermalResistance->unbind();
}

void MaterialAirGapInspectorView::refresh()
{
}

} // openstudio
