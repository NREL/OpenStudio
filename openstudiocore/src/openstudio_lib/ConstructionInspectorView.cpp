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

#include <openstudio_lib/ConstructionInspectorView.hpp>

#include <openstudio_lib/ConstructionObjectVectorController.hpp>
#include <openstudio_lib/ModelObjectItem.hpp>
#include <openstudio_lib/OSAppBase.hpp>
#include <openstudio_lib/OSDocument.hpp>
#include <openstudio_lib/OSDropZone.hpp>
#include "../shared_gui_components/OSLineEdit.hpp"

#include <model/Construction.hpp>
#include <model/Construction_Impl.hpp>
#include <model/Material.hpp>
#include <model/Material_Impl.hpp>

#include <utilities/core/Assert.hpp>
#include <utilities/idd/OS_Construction_FieldEnums.hxx>

#include <QGridLayout>
#include <QLabel>
#include <QMessageBox>
#include <QStackedWidget>

namespace openstudio {

// ConstructionInspectorView

ConstructionInspectorView::ConstructionInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent)
  : ModelObjectInspectorView(model, true, parent),
    m_constructionDZ(NULL),
    m_nameEdit(NULL),
    m_isIP(isIP)
{
  createLayout();
}

void ConstructionInspectorView::createLayout()
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

  // Layer

  label = new QLabel("Layer: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,2,0);

  label = new QLabel("Outside");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,3,0);

  m_constructionVC = new ConstructionObjectVectorController(this);
  m_constructionDZ = new OSDropZone(m_constructionVC,false);
  m_constructionDZ->setMinItems(0);
  m_constructionDZ->setMaxItems(12);
  m_constructionDZ->setItemsAcceptDrops(true);
  m_constructionDZ->setFixedSize(QSize(OSItem::ITEM_WIDTH + 20,600));
  mainGridLayout->addWidget(m_constructionDZ,4,0);

  label = new QLabel("Inside");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,5,0);

  // Stretch

  mainGridLayout->setRowStretch(100,100);

  mainGridLayout->setColumnStretch(100,100);
}

void ConstructionInspectorView::onClearSelection()
{
  ModelObjectInspectorView::onClearSelection(); // call parent implementation
  detach();
}

void ConstructionInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  detach();
  model::Construction construction = modelObject.cast<model::Construction>();
  attach(construction);
  refresh();
}

void ConstructionInspectorView::onUpdate()
{
  refresh();
}

void ConstructionInspectorView::attach(openstudio::model::Construction & construction)
{
  m_nameEdit->bind(construction,"name");

  m_constructionVC->attach(construction);
  m_constructionVC->reportItems();

  this->stackedWidget()->setCurrentIndex(1);
}

void ConstructionInspectorView::detach()
{
  this->stackedWidget()->setCurrentIndex(0);

  m_constructionVC->detach();

  m_nameEdit->unbind();
}

void ConstructionInspectorView::refresh()
{
}

void ConstructionInspectorView::toggleUnits(bool displayIP)
{
  m_isIP = displayIP;
}

} // openstudio
