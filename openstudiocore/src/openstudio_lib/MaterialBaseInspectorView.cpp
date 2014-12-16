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

#include "MaterialBaseInspectorView.hpp"

#include "OSItem.hpp"

#include "../shared_gui_components/OSComboBox.hpp"
#include "../shared_gui_components/OSLineEdit.hpp"

#include "../model/Material.hpp"
#include "../model/Material_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <QGridLayout>
#include <QLabel>
#include <QStackedWidget>

namespace openstudio {

// MaterialBaseInspectorView

MaterialBaseInspectorView::MaterialBaseInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent)
  : ModelObjectInspectorView(model, true, parent),
    m_isIP(isIP)
{
  createLayout();
}

void MaterialBaseInspectorView::createLayout()
{
  QWidget* visibleWidget = new QWidget();
  this->stackedWidget()->addWidget(visibleWidget);

  m_mainGridLayout  = new QGridLayout();
  m_mainGridLayout->setContentsMargins(7, 7, 7, 7);
  m_mainGridLayout->setSpacing(14);
  visibleWidget->setLayout(m_mainGridLayout);

  QVBoxLayout * vLayout = nullptr;

  QLabel * label = nullptr;

  unsigned row = 0;

  // Name
  vLayout = new QVBoxLayout();

  label = new QLabel("Name: ");
  label->setObjectName("H2");
  vLayout->addWidget(label);

  m_nameEdit = new OSLineEdit();
  vLayout->addWidget(m_nameEdit);

  m_mainGridLayout->addLayout(vLayout, row++, 0, 1, 3);

  // Measure Tags
  QFrame * line;
  line = new QFrame();
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow(QFrame::Sunken);
  m_mainGridLayout->addWidget(line, row++, 0, 1, 3);

  label = new QLabel();
  label->setText("Measure Tags (Optional):");
  label->setObjectName("H2");
  m_mainGridLayout->addWidget(label, row++, 0);

  // Standard
  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Standard: ");
  label->setObjectName("StandardsInfo");
  vLayout->addWidget(label);

  m_standard = new QComboBox();
  m_standard->setEditable(true);
  m_standard->setDuplicatesEnabled(false);
  m_standard->setFixedWidth(OSItem::ITEM_WIDTH);
  vLayout->addWidget(m_standard);

  m_mainGridLayout->addLayout(vLayout, row, 0);

  // Standard Source
  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Standard Source: ");
  label->setObjectName("StandardsInfo");
  vLayout->addWidget(label);

  m_standardSource = new QComboBox();
  m_standardSource->setEditable(true);
  m_standardSource->setDuplicatesEnabled(false);
  m_standardSource->setFixedWidth(OSItem::ITEM_WIDTH);
  vLayout->addWidget(m_standardSource);

  m_mainGridLayout->addLayout(vLayout, row++, 1);

  // Standards Category
  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Standards Category: ");
  label->setObjectName("StandardsInfo");
  vLayout->addWidget(label);

  m_standardsCategory = new QComboBox();
  m_standardsCategory->setEditable(true);
  m_standardsCategory->setDuplicatesEnabled(false);
  m_standardsCategory->setFixedWidth(OSItem::ITEM_WIDTH);
  vLayout->addWidget(m_standardsCategory);

  m_mainGridLayout->addLayout(vLayout, row, 0);

  // Standards Identifier
  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Standards Identifier: ");
  label->setObjectName("StandardsInfo");
  vLayout->addWidget(label);

  m_standardsIdentifier = new QComboBox();
  m_standardsIdentifier->setEditable(true);
  m_standardsIdentifier->setDuplicatesEnabled(false);
  m_standardsIdentifier->setFixedWidth(OSItem::ITEM_WIDTH);
  vLayout->addWidget(m_standardsIdentifier);

  m_mainGridLayout->addLayout(vLayout, row++, 1);

  line = new QFrame();
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow(QFrame::Sunken);
  m_mainGridLayout->addWidget(line, row++, 0, 1, 3);  
}

void MaterialBaseInspectorView::onClearSelection()
{
  ModelObjectInspectorView::onClearSelection(); // call parent implementation
  detach();
}

void MaterialBaseInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  detach();
  refresh();
}

void MaterialBaseInspectorView::onUpdate()
{
  refresh();
}

void MaterialBaseInspectorView::attach(openstudio::model::Material & material)
{
  m_nameEdit->bind(material, "name");

  this->stackedWidget()->setCurrentIndex(1);
}

void MaterialBaseInspectorView::detach()
{
  this->stackedWidget()->setCurrentIndex(0);

  m_nameEdit->unbind();
}

void MaterialBaseInspectorView::refresh()
{
}

void MaterialBaseInspectorView::toggleUnits(bool displayIP)
{
  m_isIP = displayIP;
}

} // openstudio
