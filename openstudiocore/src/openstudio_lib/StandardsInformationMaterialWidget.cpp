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

#include "StandardsInformationMaterialWidget.hpp"

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

// StandardsInformationMaterialWidget

StandardsInformationMaterialWidget::StandardsInformationMaterialWidget(bool isIP, QWidget * parent)
  : QWidget(parent),
    m_isIP(isIP)
{
}

void StandardsInformationMaterialWidget::addToLayout(QGridLayout * mainGridLayout, int & row)
{
  QVBoxLayout * vLayout = nullptr;

  QLabel * label = nullptr;

  // Measure Tags
  QFrame * line;
  line = new QFrame();
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow(QFrame::Sunken);
  mainGridLayout->addWidget(line, row++, 0, 1, 3);

  label = new QLabel();
  label->setText("Measure Tags (Optional):");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label, row++, 0);

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

  mainGridLayout->addLayout(vLayout, row, 0);

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

  mainGridLayout->addLayout(vLayout, row++, 1);

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

  mainGridLayout->addLayout(vLayout, row, 0);

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

  mainGridLayout->addLayout(vLayout, row++, 1);

  line = new QFrame();
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow(QFrame::Sunken);
  mainGridLayout->addWidget(line, row++, 0, 1, 3);  
}

void StandardsInformationMaterialWidget::attach(openstudio::model::Material & material)
{
  detach();

  m_material = material;
  m_standardsInformation = material.standardsInformation();

  connect(m_standardsInformation->getImpl<openstudio::model::detail::ModelObject_Impl>().get(), &openstudio::model::detail::ModelObject_Impl::onChange, this, &openstudio::StandardsInformationMaterialWidget::populateMaterialStandard);
}

void StandardsInformationMaterialWidget::detach()
{
  m_material.reset();

  if (m_standardsInformation){
    disconnect(m_standardsInformation->getImpl<openstudio::model::detail::ModelObject_Impl>().get(), 0, this, 0);
    m_standardsInformation.reset();
  }
}

void StandardsInformationMaterialWidget::materialStandardChanged(const QString & text)
{
  if (m_standardsInformation){
    std::string materialStandard = toString(text);
    if (materialStandard.empty()){
      m_standardsInformation->resetMaterialStandard();
    }
    else{
      m_standardsInformation->setMaterialStandard(materialStandard);
    }
    populateMaterialStandard();
  }
}

void StandardsInformationMaterialWidget::editMaterialStandard(const QString & text)
{
  if (m_standardsInformation){
    std::string materialStandard = toString(text);
    if (materialStandard.empty()){
      m_standardsInformation->resetMaterialStandard();
    }
    else{
      m_standardsInformation->setMaterialStandard(materialStandard);
    }
  }
}

void StandardsInformationMaterialWidget::populateMaterialStandard()
{
  // TODO
}

void StandardsInformationMaterialWidget::toggleUnits(bool displayIP)
{
  m_isIP = displayIP;
}

} // openstudio
