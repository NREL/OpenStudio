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

#include <openstudio_lib/ConstructionCfactorUndergroundWallInspectorView.hpp>

#include "../shared_gui_components/OSLineEdit.hpp"
#include "../shared_gui_components/OSQuantityEdit.hpp"

#include <model/CFactorUndergroundWallConstruction.hpp>
#include <model/CFactorUndergroundWallConstruction_Impl.hpp>

#include <utilities/core/Assert.hpp>

#include <QGridLayout>
#include <QLabel>
#include <QStackedWidget>

namespace openstudio {

ConstructionCfactorUndergroundWallInspectorView::ConstructionCfactorUndergroundWallInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent)
  : ModelObjectInspectorView(model, true, parent),
    m_nameEdit(NULL),
    m_cfactorEdit(NULL),
    m_heightEdit(NULL),
    m_isIP(isIP)
{
  createLayout();
}

void ConstructionCfactorUndergroundWallInspectorView::createLayout()
{
  QWidget* visibleWidget = new QWidget();
  this->stackedWidget()->addWidget(visibleWidget);

  QGridLayout* mainGridLayout = new QGridLayout();
  mainGridLayout->setContentsMargins(7,7,7,7);
  mainGridLayout->setSpacing(14);
  visibleWidget->setLayout(mainGridLayout);

  // Name

  QLabel* label = new QLabel("Name: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,0,0);

  m_nameEdit = new OSLineEdit();
  mainGridLayout->addWidget(m_nameEdit,1,0,1,3);

  // C-Factor

  label = new QLabel("C-Factor: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,2,0);

  m_cfactorEdit = new OSQuantityEdit(m_isIP);
  bool isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_cfactorEdit, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_cfactorEdit,3,0);

  // Height

  label = new QLabel("Height: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,4,0);

  m_heightEdit = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_heightEdit, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_heightEdit,5,0);

  // Stretch

  mainGridLayout->setRowStretch(100,100);

  mainGridLayout->setColumnStretch(100,100);
}

void ConstructionCfactorUndergroundWallInspectorView::onClearSelection()
{
  ModelObjectInspectorView::onClearSelection(); // call parent implementation
  detach();
}

void ConstructionCfactorUndergroundWallInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  detach();
  model::CFactorUndergroundWallConstruction cFactorUndergroundWallConstruction = modelObject.cast<model::CFactorUndergroundWallConstruction>();
  attach(cFactorUndergroundWallConstruction);
  refresh();
}

void ConstructionCfactorUndergroundWallInspectorView::onUpdate()
{
  refresh();
}

void ConstructionCfactorUndergroundWallInspectorView::attach(openstudio::model::CFactorUndergroundWallConstruction & cFactorUndergroundWallConstruction)
{
  m_nameEdit->bind(cFactorUndergroundWallConstruction,"name");
  m_cfactorEdit->bind(cFactorUndergroundWallConstruction,"cFactor",m_isIP);
  m_heightEdit->bind(cFactorUndergroundWallConstruction,"height",m_isIP);

  this->stackedWidget()->setCurrentIndex(1);
}

void ConstructionCfactorUndergroundWallInspectorView::detach()
{
  this->stackedWidget()->setCurrentIndex(0);

  m_nameEdit->unbind();
  m_cfactorEdit->unbind();
  m_heightEdit->unbind();
}

void ConstructionCfactorUndergroundWallInspectorView::refresh()
{
}

void ConstructionCfactorUndergroundWallInspectorView::toggleUnits(bool displayIP)
{
  m_isIP = displayIP;
}

} // openstudio

