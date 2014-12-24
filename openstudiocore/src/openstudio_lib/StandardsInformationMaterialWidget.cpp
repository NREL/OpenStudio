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

  bool isConnected = false;

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

  isConnected = connect(m_standard, &QComboBox::currentTextChanged, this, &openstudio::StandardsInformationMaterialWidget::standardChanged);
  OS_ASSERT(isConnected);

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

  isConnected = connect(m_standardSource, &QComboBox::currentTextChanged, this, &openstudio::StandardsInformationMaterialWidget::standardSourceChanged);
  OS_ASSERT(isConnected);

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

  isConnected = connect(m_standardsCategory, &QComboBox::currentTextChanged, this, &openstudio::StandardsInformationMaterialWidget::standardsCategoryChanged);
  OS_ASSERT(isConnected);

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

  isConnected = connect(m_standardsIdentifier, &QComboBox::currentTextChanged, this, &openstudio::StandardsInformationMaterialWidget::standardsIdentifierChanged);
  OS_ASSERT(isConnected);

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

  m_standard->setEnabled(true);

  m_standardSource->setEnabled(true);

  m_standardsCategory->setEnabled(true);

  m_standardsIdentifier->setEnabled(true);

  populateStandards();
  populateStandardSources();
  populateStandardsCategories();
  populateStandardsIdentifier();

}

void StandardsInformationMaterialWidget::detach()
{
  m_material.reset();

  if (m_standardsInformation){ // TODO
    disconnect(m_standardsInformation->getImpl<openstudio::model::detail::ModelObject_Impl>().get(), 0, this, 0);
    m_standardsInformation.reset();
  }

  m_standard->setEnabled(false);

  m_standardSource->setEnabled(false);

  m_standardsCategory->setEnabled(false);

  m_standardsIdentifier->setEnabled(false);

}

void StandardsInformationMaterialWidget::standardChanged(const QString& text)
{
  if (m_standardsInformation){
    std::string str = toString(text);
    if (str.empty()){
      m_standardsInformation->resetMaterialStandard();
    }
    else{
      m_standardsInformation->setMaterialStandard(str);
    }
    populateStandards();
    populateStandardSources();
  }
}

void StandardsInformationMaterialWidget::populateStandards()
{
  m_standard->blockSignals(true);

  m_standard->clear();
  if (m_standardsInformation){
    m_standard->addItem("");
    std::vector<std::string> suggestedMaterialStandards = m_standardsInformation->suggestedMaterialStandards();
    for (const std::string& suggestedMaterialStandard : suggestedMaterialStandards) {
      m_standard->addItem(toQString(suggestedMaterialStandard));
    }
    boost::optional<std::string> materialStandard = m_standardsInformation->materialStandard();
    if (materialStandard){
      OS_ASSERT(!suggestedMaterialStandards.empty());
      m_standard->setCurrentIndex(1);
    }
    else{
      m_standard->setCurrentIndex(0);
    }
  }

  m_standard->blockSignals(false);
}

void StandardsInformationMaterialWidget::standardSourceChanged(const QString& text)
{
  if (m_standardsInformation){
    std::string str = toString(text);
    if (str.empty()){
      m_standardsInformation->resetMaterialStandardSource();
    }
    else{
      m_standardsInformation->setMaterialStandardSource(str);
    }
    populateStandardSources();
  }
}

void StandardsInformationMaterialWidget::populateStandardSources()
{
  m_standardSource->blockSignals(true);

  m_standardSource->clear();
  if (m_standardsInformation){
    m_standardSource->addItem("");
    std::vector<std::string> suggestedMaterialStandardSources = m_standardsInformation->suggestedMaterialStandardSources();
    for (const std::string& suggestedMaterialStandardSource : suggestedMaterialStandardSources) {
      m_standardSource->addItem(toQString(suggestedMaterialStandardSource));
    }
    boost::optional<std::string> materialStandardSource = m_standardsInformation->materialStandardSource();
    if (materialStandardSource){
      OS_ASSERT(!suggestedMaterialStandardSources.empty());
      m_standardSource->setCurrentIndex(1);
    }
    else{
      m_standardSource->setCurrentIndex(0);
    }
  }

  m_standardSource->blockSignals(false);
}

void StandardsInformationMaterialWidget::standardsCategoryChanged(const QString& text)
{
  if (m_standardsInformation){
    std::string str = toString(text);
    if (str.empty()){
      m_standardsInformation->resetStandardsCategory();
    }
    else{
      m_standardsInformation->setStandardsCategory(str);
    }
    populateStandardsCategories();
    populateStandardsIdentifier();
  }
}

void StandardsInformationMaterialWidget::populateStandardsCategories()
{
  m_standardsCategory->blockSignals(true);

  m_standardsCategory->clear();
  if (m_standardsInformation){
    m_standardsCategory->addItem("");
    std::vector<std::string> suggestedStandardsCategories = m_standardsInformation->suggestedStandardsCategories();
    for (const std::string& suggestedStandardsCategory : suggestedStandardsCategories) {
      m_standardsCategory->addItem(toQString(suggestedStandardsCategory));
    }
    boost::optional<std::string> standardsCategory = m_standardsInformation->standardsCategory();
    if (standardsCategory){
      OS_ASSERT(!suggestedStandardsCategories.empty());
      m_standardsCategory->setCurrentIndex(1);
    }
    else{
      m_standardsCategory->setCurrentIndex(0);
    }
  }

  m_standardsCategory->blockSignals(false);
}

void StandardsInformationMaterialWidget::standardsIdentifierChanged(const QString& text)
{
  if (m_standardsInformation){
    std::string str = toString(text);
    if (str.empty()){
      m_standardsInformation->resetStandardsIdentifier();
    }
    else{
      m_standardsInformation->setStandardsIdentifier(str);
    }
    populateStandardsIdentifier();
  }
}

void StandardsInformationMaterialWidget::populateStandardsIdentifier()
{
  m_standardsIdentifier->blockSignals(true);

  m_standardsIdentifier->clear();
  if (m_standardsInformation){
    m_standardsIdentifier->addItem("");
    std::vector<std::string> suggestedStandardsIdentifiers = m_standardsInformation->suggestedStandardsIdentifiers();
    for (const std::string& suggestedStandardsIdentifier : suggestedStandardsIdentifiers) {
      m_standardsIdentifier->addItem(toQString(suggestedStandardsIdentifier));
    }
    boost::optional<std::string> standardsIdentifier = m_standardsInformation->standardsIdentifier();
    if (standardsIdentifier){
      OS_ASSERT(!suggestedStandardsIdentifiers.empty());
      m_standardsIdentifier->setCurrentIndex(1);
    }
    else{
      m_standardsIdentifier->setCurrentIndex(0);
    }
  }

  m_standardsIdentifier->blockSignals(false);
}

void StandardsInformationMaterialWidget::toggleUnits(bool displayIP)
{
  m_isIP = displayIP;
}

} // openstudio
