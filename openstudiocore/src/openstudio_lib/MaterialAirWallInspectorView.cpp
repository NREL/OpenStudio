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

#include "MaterialAirWallInspectorView.hpp"

#include "../shared_gui_components/OSLineEdit.hpp"
#include "../shared_gui_components/OSQuantityEdit.hpp"

#include "../model/AirWallMaterial.hpp"
#include "../model/AirWallMaterial_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <QGridLayout>
#include <QLabel>
#include <QStackedWidget>

namespace openstudio {

// MaterialAirWallInspectorView

MaterialAirWallInspectorView::MaterialAirWallInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent)
  : ModelObjectInspectorView(model, true, parent),
    m_nameEdit(NULL),
    m_isIP(isIP)
{
  createLayout();
}

void MaterialAirWallInspectorView::createLayout()
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

  // Stretch

  mainGridLayout->setRowStretch(100,100);

  mainGridLayout->setColumnStretch(100,100);
}

void MaterialAirWallInspectorView::onClearSelection()
{
  ModelObjectInspectorView::onClearSelection(); // call parent implementation
  detach();
}

void MaterialAirWallInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  detach();
  model::AirWallMaterial airWallMaterial = modelObject.cast<model::AirWallMaterial>();
  attach(airWallMaterial);
  refresh();
}

void MaterialAirWallInspectorView::onUpdate()
{
  refresh();
}

void MaterialAirWallInspectorView::attach(openstudio::model::AirWallMaterial & airWallMaterial)
{
  m_nameEdit->bind(airWallMaterial,"name");

  this->stackedWidget()->setCurrentIndex(1);
}

void MaterialAirWallInspectorView::detach()
{
  this->stackedWidget()->setCurrentIndex(0);

  m_nameEdit->unbind();
}

void MaterialAirWallInspectorView::refresh()
{
}

void MaterialAirWallInspectorView::toggleUnits(bool displayIP)
{
  m_isIP = displayIP;
}

} // openstudio
