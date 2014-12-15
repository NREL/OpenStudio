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

#include "MaterialInfraredTransparentInspectorView.hpp"

#include "../shared_gui_components/OSLineEdit.hpp"
#include "../shared_gui_components/OSQuantityEdit.hpp"

#include "../model/InfraredTransparentMaterial.hpp"
#include "../model/InfraredTransparentMaterial_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <QGridLayout>
#include <QLabel>
#include <QStackedWidget>

namespace openstudio {

// MaterialInfraredTransparentInspectorView

MaterialInfraredTransparentInspectorView::MaterialInfraredTransparentInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent)
  : MaterialBaseInspectorView(isIP, model, parent)
{
  createLayout();
}

void MaterialInfraredTransparentInspectorView::createLayout()
{
  MaterialBaseInspectorView::createLayout(); // call parent implementation

  QVBoxLayout * vLayout = nullptr;

  QLabel * label = nullptr;

  unsigned row = m_mainGridLayout->rowCount();

  // Stretch

  m_mainGridLayout->setRowStretch(100,100);

  m_mainGridLayout->setColumnStretch(100,100);
}

void MaterialInfraredTransparentInspectorView::onClearSelection()
{
  ModelObjectInspectorView::onClearSelection(); // call parent implementation
  detach();
}

void MaterialInfraredTransparentInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  detach();
  model::InfraredTransparentMaterial infraredTransparentMaterial = modelObject.cast<model::InfraredTransparentMaterial>();
  attach(infraredTransparentMaterial);
  refresh();
}

void MaterialInfraredTransparentInspectorView::onUpdate()
{
  refresh();
}

void MaterialInfraredTransparentInspectorView::attach(openstudio::model::InfraredTransparentMaterial & infraredTransparentMaterial)
{
  m_nameEdit->bind(infraredTransparentMaterial,"name");

  this->stackedWidget()->setCurrentIndex(1);
}

void MaterialInfraredTransparentInspectorView::detach()
{
  this->stackedWidget()->setCurrentIndex(0);

  m_nameEdit->unbind();
}

void MaterialInfraredTransparentInspectorView::refresh()
{
}

} // openstudio
