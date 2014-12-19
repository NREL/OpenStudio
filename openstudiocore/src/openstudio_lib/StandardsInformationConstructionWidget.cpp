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

#include "StandardsInformationConstructionWidget.hpp"

#include "OSItem.hpp"

#include "../shared_gui_components/OSComboBox.hpp"
#include "../shared_gui_components/OSLineEdit.hpp"

#include "../model/Construction.hpp"
#include "../model/Construction_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <QComboBox>
#include <QGridLayout>
#include <QLabel>

#include <QStackedWidget>

namespace openstudio {

// ConstructionBaseInspectorView

ConstructionBaseInspectorView::ConstructionBaseInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent)
  : ModelObjectInspectorView(model, true, parent),
    m_nameEdit(nullptr),
    m_isIP(isIP)
{
  createLayout();
}

void ConstructionBaseInspectorView::createLayout()
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
  label = new QLabel("Name: ");
  label->setObjectName("H2");
  m_mainGridLayout->addWidget(label, row, 0);

  ++row;

  m_nameEdit = new OSLineEdit();
  m_mainGridLayout->addWidget(m_nameEdit, row, 0, 1, 3);

  ++row;

  // Measure Tags
  QFrame * line;
  line = new QFrame();
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow(QFrame::Sunken);
  m_mainGridLayout->addWidget(line, row, 0, 1, 3);

  ++row;

  label = new QLabel();
  label->setText("Measure Tags (Optional):");
  label->setObjectName("H2");
  m_mainGridLayout->addWidget(label, row, 0);

  ++row;

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

  m_mainGridLayout->addLayout(vLayout, row, 1);

  ++row;

  // Intended Surface Type
  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Intended Surface Type: ");
  label->setObjectName("StandardsInfo");
  vLayout->addWidget(label);

  m_intendedSurfaceType = new OSComboBox2();
  m_intendedSurfaceType->setFixedWidth(OSItem::ITEM_WIDTH);
  vLayout->addWidget(m_intendedSurfaceType);

  m_mainGridLayout->addLayout(vLayout, row, 0);

  // Standards Construction Type
  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Standards Construction Type: ");
  label->setObjectName("StandardsInfo");
  vLayout->addWidget(label);

  m_standardsConstructionType = new QComboBox();
  m_standardsConstructionType->setEditable(true);
  m_standardsConstructionType->setDuplicatesEnabled(false);
  m_standardsConstructionType->setFixedWidth(OSItem::ITEM_WIDTH);
  vLayout->addWidget(m_standardsConstructionType);

  m_mainGridLayout->addLayout(vLayout, row, 1);

  ++row;

  // Fenestrations
  vLayout = new QVBoxLayout();

  m_fenestrationLabel = new QLabel();
  m_fenestrationLabel->setText("Fenestrations: ");
  m_fenestrationLabel->setObjectName("StandardsInfo");
  m_fenestrationLabel->hide();
  vLayout->addWidget(m_fenestrationLabel);

  m_fenestration = new QComboBox();
  m_fenestration->setEditable(true);
  m_fenestration->setDuplicatesEnabled(false);
  m_fenestration->setFixedWidth(OSItem::ITEM_WIDTH);
  m_fenestration->hide();
  vLayout->addWidget(m_fenestration);

  m_mainGridLayout->addLayout(vLayout, row, 0);

  ++row;

  line = new QFrame();
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow(QFrame::Sunken);
  m_mainGridLayout->addWidget(line, row, 0, 1, 3);

  ++row;

}

void ConstructionBaseInspectorView::showFenestration()
{
  m_fenestrationLabel->show();
  m_fenestration->show();
}

void ConstructionBaseInspectorView::hideFenestration()
{
  m_fenestrationLabel->hide();
  m_fenestration->hide();
}

void ConstructionBaseInspectorView::enableFenestration()
{
  m_fenestration->setEnabled(true);
}

void ConstructionBaseInspectorView::disableFenestration()
{
  m_fenestration->setEnabled(false);
}

void ConstructionBaseInspectorView::onClearSelection()
{
  ModelObjectInspectorView::onClearSelection(); // call parent implementation
}

void ConstructionBaseInspectorView::onUpdate()
{
  refresh();
}

void ConstructionBaseInspectorView::standardsConstructionTypeChanged(const QString & text)
{
  if (m_standardsInformation){
    std::string standardsConstructionType = toString(text);
    if (standardsConstructionType.empty()){
      m_standardsInformation->resetStandardsConstructionType();
    }else{
      m_standardsInformation->setStandardsConstructionType(standardsConstructionType);
    }
    populateStandardsConstructionType();
  }
}

void ConstructionBaseInspectorView::editStandardsConstructionType(const QString & text)
{
  if (m_standardsInformation){
    std::string standardsConstructionType = toString(text);
    if (standardsConstructionType.empty()){
      m_standardsInformation->resetStandardsConstructionType();
    }else{
      m_standardsInformation->setStandardsConstructionType(standardsConstructionType);
    }
  }
}

void ConstructionBaseInspectorView::populateStandardsConstructionType()
{
  disconnect(m_standardsConstructionType, 0, this, 0);

  m_standardsConstructionType->clear();
  if (m_standardsInformation){
    m_standardsConstructionType->addItem("");
    std::vector<std::string> suggestedStandardsConstructionTypes = m_standardsInformation->suggestedStandardsConstructionTypes();
    for (const std::string& standardsConstructionType : suggestedStandardsConstructionTypes) {
      m_standardsConstructionType->addItem(toQString(standardsConstructionType));
    }
    boost::optional<std::string> standardsConstructionType = m_standardsInformation->standardsConstructionType();
    if (standardsConstructionType){
      OS_ASSERT(!suggestedStandardsConstructionTypes.empty());
      m_standardsConstructionType->setCurrentIndex(1);
    }else{
      m_standardsConstructionType->setCurrentIndex(0);
    }
  }
}

void ConstructionBaseInspectorView::detach()
{
  this->stackedWidget()->setCurrentIndex(0);

  m_nameEdit->unbind();

  if (m_standardsInformation){
    disconnect(m_standardsInformation->getImpl<openstudio::model::detail::ModelObject_Impl>().get(), 0, this, 0);
    m_standardsInformation.reset();
  }

  m_intendedSurfaceType->unbind();

  disconnect(m_standardsConstructionType, 0, this, 0);
  m_standardsConstructionType->setEnabled(false);
}

void ConstructionBaseInspectorView::refresh()
{
}

void ConstructionBaseInspectorView::toggleUnits(bool displayIP)
{
  m_isIP = displayIP;
}

} // openstudio
