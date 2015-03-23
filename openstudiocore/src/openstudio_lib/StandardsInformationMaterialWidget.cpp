/**********************************************************************
*  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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
#include <QTimer>

namespace openstudio {

// StandardsInformationMaterialWidget

StandardsInformationMaterialWidget::StandardsInformationMaterialWidget(bool isIP, QGridLayout * mainGridLayout, int & row)
  : QWidget(mainGridLayout->parentWidget())
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

  // Composite Framing Material
  vLayout = new QVBoxLayout();

  m_compositeFramingMaterialLabel = new QLabel();
  m_compositeFramingMaterialLabel->setText("Composite Framing Material: ");
  m_compositeFramingMaterialLabel->setObjectName("StandardsInfo");
  vLayout->addWidget(m_compositeFramingMaterialLabel);

  m_compositeFramingMaterial = new QComboBox();
  m_compositeFramingMaterial->setEditable(true);
  m_compositeFramingMaterial->setDuplicatesEnabled(false);
  m_compositeFramingMaterial->setFixedWidth(OSItem::ITEM_WIDTH);
  vLayout->addWidget(m_compositeFramingMaterial);

  mainGridLayout->addLayout(vLayout, row, 0);

  isConnected = connect(m_compositeFramingMaterial, &QComboBox::currentTextChanged, this, &openstudio::StandardsInformationMaterialWidget::compositeFramingMaterialChanged);
  OS_ASSERT(isConnected);

  // Composite Framing Configuration
  vLayout = new QVBoxLayout();

  m_compositeFramingConfigurationLabel = new QLabel();
  m_compositeFramingConfigurationLabel->setText("Composite Framing Configuration: ");
  m_compositeFramingConfigurationLabel->setObjectName("StandardsInfo");
  vLayout->addWidget(m_compositeFramingConfigurationLabel);

  m_compositeFramingConfiguration = new QComboBox();
  m_compositeFramingConfiguration->setEditable(true);
  m_compositeFramingConfiguration->setDuplicatesEnabled(false);
  m_compositeFramingConfiguration->setFixedWidth(OSItem::ITEM_WIDTH);
  vLayout->addWidget(m_compositeFramingConfiguration);

  mainGridLayout->addLayout(vLayout, row++, 1);

  isConnected = connect(m_compositeFramingConfiguration, &QComboBox::currentTextChanged, this, &openstudio::StandardsInformationMaterialWidget::compositeFramingConfigurationChanged);
  OS_ASSERT(isConnected);

  // Composite Framing Depth
  vLayout = new QVBoxLayout();

  m_compositeFramingDepthLabel = new QLabel();
  m_compositeFramingDepthLabel->setText("Composite Framing Depth: ");
  m_compositeFramingDepthLabel->setObjectName("StandardsInfo");
  vLayout->addWidget(m_compositeFramingDepthLabel);

  m_compositeFramingDepth = new QComboBox();
  m_compositeFramingDepth->setEditable(true);
  m_compositeFramingDepth->setDuplicatesEnabled(false);
  m_compositeFramingDepth->setFixedWidth(OSItem::ITEM_WIDTH);
  vLayout->addWidget(m_compositeFramingDepth);

  mainGridLayout->addLayout(vLayout, row, 0);

  isConnected = connect(m_compositeFramingDepth, &QComboBox::currentTextChanged, this, &openstudio::StandardsInformationMaterialWidget::compositeFramingDepthChanged);
  OS_ASSERT(isConnected);

  // Composite Framing Size
  vLayout = new QVBoxLayout();

  m_compositeFramingSizeLabel = new QLabel();
  m_compositeFramingSizeLabel->setText("Composite Framing Size: ");
  m_compositeFramingSizeLabel->setObjectName("StandardsInfo");
  vLayout->addWidget(m_compositeFramingSizeLabel);

  m_compositeFramingSize = new QComboBox();
  m_compositeFramingSize->setEditable(true);
  m_compositeFramingSize->setDuplicatesEnabled(false);
  m_compositeFramingSize->setFixedWidth(OSItem::ITEM_WIDTH);
  vLayout->addWidget(m_compositeFramingSize);

  mainGridLayout->addLayout(vLayout, row++, 1);

  isConnected = connect(m_compositeFramingSize, &QComboBox::currentTextChanged, this, &openstudio::StandardsInformationMaterialWidget::compositeFramingSizeChanged);
  OS_ASSERT(isConnected);

  // Composite Cavity Insulation
  vLayout = new QVBoxLayout();

  m_compositeCavityInsulationLabel = new QLabel();
  m_compositeCavityInsulationLabel->setText("Composite Cavity Insulation: ");
  m_compositeCavityInsulationLabel->setObjectName("StandardsInfo");
  vLayout->addWidget(m_compositeCavityInsulationLabel);

  m_compositeCavityInsulation = new QComboBox();
  m_compositeCavityInsulation->setEditable(true);
  m_compositeCavityInsulation->setDuplicatesEnabled(false);
  m_compositeCavityInsulation->setFixedWidth(OSItem::ITEM_WIDTH);
  vLayout->addWidget(m_compositeCavityInsulation);

  mainGridLayout->addLayout(vLayout, row++, 0);

  isConnected = connect(m_compositeCavityInsulation, &QComboBox::currentTextChanged, this, &openstudio::StandardsInformationMaterialWidget::compositeCavityInsulationChanged);
  OS_ASSERT(isConnected);

  line = new QFrame();
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow(QFrame::Sunken);
  mainGridLayout->addWidget(line, row++, 0, 1, 3);

  hideComposite();
}

StandardsInformationMaterialWidget::~StandardsInformationMaterialWidget()
{
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

  if (m_standardsInformation->isCompositeMaterial()){
    enableComposite();
  } else {
    disableComposite();
  }

  requestPopulateFields();
}

void StandardsInformationMaterialWidget::detach()
{
  m_material.reset();

  if (m_standardsInformation){ 
    // DLM: this was never attached to anything
    disconnect(m_standardsInformation->getImpl<openstudio::model::detail::ModelObject_Impl>().get(), 0, this, 0);
    m_standardsInformation.reset();
  }

  m_standard->setEnabled(false);

  m_standardSource->setEnabled(false);

  m_standardsCategory->setEnabled(false);

  m_standardsIdentifier->setEnabled(false);

  disableComposite();
}

void StandardsInformationMaterialWidget::showComposite()
{
  m_compositeFramingMaterial->show();
  m_compositeFramingConfiguration->show();
  m_compositeFramingDepth->show();
  m_compositeFramingSize->show();
  m_compositeCavityInsulation->show();

  m_compositeFramingMaterialLabel->show();
  m_compositeFramingConfigurationLabel->show();
  m_compositeFramingDepthLabel->show();
  m_compositeFramingSizeLabel->show();
  m_compositeCavityInsulationLabel->show();
}

void StandardsInformationMaterialWidget::hideComposite()
{
  m_compositeFramingMaterial->hide();
  m_compositeFramingConfiguration->hide();
  m_compositeFramingDepth->hide();
  m_compositeFramingSize->hide();
  m_compositeCavityInsulation->hide();

  m_compositeFramingMaterialLabel->hide();
  m_compositeFramingConfigurationLabel->hide();
  m_compositeFramingDepthLabel->hide();
  m_compositeFramingSizeLabel->hide();
  m_compositeCavityInsulationLabel->hide();
}

void StandardsInformationMaterialWidget::enableComposite()
{
  m_compositeFramingMaterial->setEnabled(true);
  m_compositeFramingConfiguration->setEnabled(true);
  m_compositeFramingDepth->setEnabled(true);
  m_compositeFramingSize->setEnabled(true);
  m_compositeCavityInsulation->setEnabled(true);
}

void StandardsInformationMaterialWidget::disableComposite()
{
  m_compositeFramingMaterial->setEnabled(false);
  m_compositeFramingConfiguration->setEnabled(false);
  m_compositeFramingDepth->setEnabled(false);
  m_compositeFramingSize->setEnabled(false);
  m_compositeCavityInsulation->setEnabled(false);
}

void StandardsInformationMaterialWidget::requestPopulateFields()
{
  this;

  if (!m_populateFieldsRequested){
    m_populateFieldsRequested = true;

    //populateFields();
    QTimer::singleShot(0, this, SLOT(populateFields()));
    //QTimer::singleShot(0, this, &openstudio::StandardsInformationMaterialWidget::populateFields); // Qt 5.4 only
  }
}

void StandardsInformationMaterialWidget::populateFields()
{
  this;

  m_populateFieldsRequested = false;

  populateStandards();
  populateStandardSources();
  populateStandardsCategories();
  populateStandardsIdentifier();
  populateCompositeFramingMaterial();
  populateCompositeFramingConfiguration();
  populateCompositeFramingDepth();
  populateCompositeFramingSize();
  populateCompositeCavityInsulation();

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
    requestPopulateFields();
  }
}

void StandardsInformationMaterialWidget::populateStandards()
{
  m_standard->blockSignals(true);

  m_standard->clear();
  
  if (m_standardsInformation){
    unsigned i = 0;
    m_standard->addItem("");

    std::vector<std::string> suggestedMaterialStandards = m_standardsInformation->suggestedMaterialStandards();
    for (const std::string& suggestedMaterialStandard : suggestedMaterialStandards) {
      QString value = toQString(suggestedMaterialStandard);
      m_standard->addItem(value);
      m_standard->setItemData(++i, value, Qt::ToolTipRole);
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
    //requestPopulateFields();
  }
}

void StandardsInformationMaterialWidget::populateStandardSources()
{
  m_standardSource->blockSignals(true);

  m_standardSource->clear();
  if (m_standardsInformation){
    unsigned i = 0;
    m_standardSource->addItem("");

    std::vector<std::string> suggestedMaterialStandardSources = m_standardsInformation->suggestedMaterialStandardSources();
    for (const std::string& suggestedMaterialStandardSource : suggestedMaterialStandardSources) {
      QString value = toQString(suggestedMaterialStandardSource);
      m_standardSource->addItem(value);
      m_standardSource->setItemData(++i, value, Qt::ToolTipRole);
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

    if (m_standardsInformation->isCompositeMaterial()){
      enableComposite();
    } else {
      disableComposite();
    }

    requestPopulateFields();
  }
}

void StandardsInformationMaterialWidget::populateStandardsCategories()
{
  m_standardsCategory->blockSignals(true);

  m_standardsCategory->clear();
  if (m_standardsInformation){
    unsigned i = 0;
    m_standardsCategory->addItem("");

    std::vector<std::string> suggestedStandardsCategories = m_standardsInformation->suggestedStandardsCategories();
    for (const std::string& suggestedStandardsCategory : suggestedStandardsCategories) {
      QString value = toQString(suggestedStandardsCategory);
      m_standardsCategory->addItem(value);
      m_standardsCategory->setItemData(++i, value, Qt::ToolTipRole);
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
    //requestPopulateFields();
  }
}

void StandardsInformationMaterialWidget::populateStandardsIdentifier()
{
  m_standardsIdentifier->blockSignals(true);

  m_standardsIdentifier->clear();
  if (m_standardsInformation){
    unsigned i = 0;
    m_standardsIdentifier->addItem("");

    std::vector<std::string> suggestedStandardsIdentifiers = m_standardsInformation->suggestedStandardsIdentifiers();
    for (const std::string& suggestedStandardsIdentifier : suggestedStandardsIdentifiers) {
      QString value = toQString(suggestedStandardsIdentifier);
      m_standardsIdentifier->addItem(value);
      m_standardsIdentifier->setItemData(++i, value, Qt::ToolTipRole);
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

void StandardsInformationMaterialWidget::compositeFramingMaterialChanged(const QString& text)
{
  if (m_standardsInformation){
    std::string str = toString(text);
    if (str.empty()){
      m_standardsInformation->resetCompositeFramingMaterial();
    }
    else{
      m_standardsInformation->setCompositeFramingMaterial(str);
    }
    //requestPopulateFields();
  }
}

void StandardsInformationMaterialWidget::populateCompositeFramingMaterial()
{
  m_compositeFramingMaterial->blockSignals(true);

  m_compositeFramingMaterial->clear();
  if (m_standardsInformation){
    unsigned i = 0;
    m_compositeFramingMaterial->addItem("");

    std::vector<std::string> suggestedCompositeFramingMaterials = m_standardsInformation->suggestedCompositeFramingMaterials();
    for (const std::string& suggestedCompositeFramingMaterial : suggestedCompositeFramingMaterials) {
      QString value = toQString(suggestedCompositeFramingMaterial);
      m_compositeFramingMaterial->addItem(value);
      m_compositeFramingMaterial->setItemData(++i, value, Qt::ToolTipRole);
    }
    boost::optional<std::string> compositeFramingMaterial = m_standardsInformation->compositeFramingMaterial();
    if (compositeFramingMaterial){
      OS_ASSERT(!suggestedCompositeFramingMaterials.empty());
      m_compositeFramingMaterial->setCurrentIndex(1);
    }
    else{
      m_compositeFramingMaterial->setCurrentIndex(0);
    }
  }

  m_compositeFramingMaterial->blockSignals(false);
}

void StandardsInformationMaterialWidget::compositeFramingConfigurationChanged(const QString& text)
{
  if (m_standardsInformation){
    std::string str = toString(text);
    if (str.empty()){
      m_standardsInformation->resetCompositeFramingConfiguration();
    }
    else{
      m_standardsInformation->setCompositeFramingConfiguration(str);
    }
    //requestPopulateFields();
  }
}

void StandardsInformationMaterialWidget::populateCompositeFramingConfiguration()
{
  m_compositeFramingConfiguration->blockSignals(true);

  m_compositeFramingConfiguration->clear();
  if (m_standardsInformation){
    unsigned i = 0;
    m_compositeFramingConfiguration->addItem("");

    std::vector<std::string> suggestedCompositeFramingConfigurations = m_standardsInformation->suggestedCompositeFramingConfigurations();
    for (const std::string& suggestedCompositeFramingConfiguration : suggestedCompositeFramingConfigurations) {
      QString value = toQString(suggestedCompositeFramingConfiguration);
      m_compositeFramingConfiguration->addItem(value);
      m_compositeFramingConfiguration->setItemData(++i, value, Qt::ToolTipRole);
    }
    boost::optional<std::string> compositeFramingConfiguration = m_standardsInformation->compositeFramingConfiguration();
    if (compositeFramingConfiguration){
      OS_ASSERT(!suggestedCompositeFramingConfigurations.empty());
      m_compositeFramingConfiguration->setCurrentIndex(1);
    }
    else{
      m_compositeFramingConfiguration->setCurrentIndex(0);
    }
  }

  m_compositeFramingConfiguration->blockSignals(false);
}

void StandardsInformationMaterialWidget::compositeFramingDepthChanged(const QString& text)
{
  if (m_standardsInformation){
    std::string str = toString(text);
    if (str.empty()){
      m_standardsInformation->resetCompositeFramingDepth();
    }
    else{
      m_standardsInformation->setCompositeFramingDepth(str);
    }
    //requestPopulateFields();
  }
}

void StandardsInformationMaterialWidget::populateCompositeFramingDepth()
{
  m_compositeFramingDepth->blockSignals(true);

  m_compositeFramingDepth->clear();
  if (m_standardsInformation){
    unsigned i = 0;
    m_compositeFramingDepth->addItem("");

    std::vector<std::string> suggestedCompositeFramingDepths = m_standardsInformation->suggestedCompositeFramingDepths();
    for (const std::string& suggestedCompositeFramingDepth : suggestedCompositeFramingDepths) {
      QString value = toQString(suggestedCompositeFramingDepth);
      m_compositeFramingDepth->addItem(value);
      m_compositeFramingDepth->setItemData(++i, value, Qt::ToolTipRole);
    }
    boost::optional<std::string> compositeFramingDepth = m_standardsInformation->compositeFramingDepth();
    if (compositeFramingDepth){
      OS_ASSERT(!suggestedCompositeFramingDepths.empty());
      m_compositeFramingDepth->setCurrentIndex(1);
    }
    else{
      m_compositeFramingDepth->setCurrentIndex(0);
    }
  }

  m_compositeFramingDepth->blockSignals(false);
}

void StandardsInformationMaterialWidget::compositeFramingSizeChanged(const QString& text)
{
  if (m_standardsInformation){
    std::string str = toString(text);
    if (str.empty()){
      m_standardsInformation->resetCompositeFramingSize();
    }
    else{
      m_standardsInformation->setCompositeFramingSize(str);
    }
    //requestPopulateFields();
  }
}

void StandardsInformationMaterialWidget::populateCompositeFramingSize()
{
  m_compositeFramingSize->blockSignals(true);

  m_compositeFramingSize->clear();
  if (m_standardsInformation){
    unsigned i = 0;
    m_compositeFramingSize->addItem("");

    std::vector<std::string> suggestedCompositeFramingSizes = m_standardsInformation->suggestedCompositeFramingSizes();
    for (const std::string& suggestedCompositeFramingSize : suggestedCompositeFramingSizes) {
      QString value = toQString(suggestedCompositeFramingSize);
      m_compositeFramingSize->addItem(value);
      m_compositeFramingSize->setItemData(++i, value, Qt::ToolTipRole);
    }
    boost::optional<std::string> compositeFramingSize = m_standardsInformation->compositeFramingSize();
    if (compositeFramingSize){
      OS_ASSERT(!suggestedCompositeFramingSizes.empty());
      m_compositeFramingSize->setCurrentIndex(1);
    }
    else{
      m_compositeFramingSize->setCurrentIndex(0);
    }
  }

  m_compositeFramingSize->blockSignals(false);
}

void StandardsInformationMaterialWidget::compositeCavityInsulationChanged(const QString& text)
{
  if (m_standardsInformation){
    std::string str = toString(text);
    if (str.empty()){
      m_standardsInformation->resetCompositeCavityInsulation();
    }
    else{
      m_standardsInformation->setCompositeCavityInsulation(str);
    }
    //requestPopulateFields();
  }
}

void StandardsInformationMaterialWidget::populateCompositeCavityInsulation()
{
  m_compositeCavityInsulation->blockSignals(true);

  m_compositeCavityInsulation->clear();
  if (m_standardsInformation){
    unsigned i = 0;
    m_compositeCavityInsulation->addItem("");

    std::vector<std::string> suggestedCompositeCavityInsulations = m_standardsInformation->suggestedCompositeCavityInsulations();
    for (const std::string& suggestedCompositeCavityInsulation : suggestedCompositeCavityInsulations) {
      QString value = toQString(suggestedCompositeCavityInsulation);
      m_compositeCavityInsulation->addItem(value);
      m_compositeCavityInsulation->setItemData(++i, value, Qt::ToolTipRole);
    }
    boost::optional<std::string> compositeCavityInsulation = m_standardsInformation->compositeCavityInsulation();
    if (compositeCavityInsulation){
      OS_ASSERT(!suggestedCompositeCavityInsulations.empty());
      m_compositeCavityInsulation->setCurrentIndex(1);
    }
    else{
      m_compositeCavityInsulation->setCurrentIndex(0);
    }
  }

  m_compositeCavityInsulation->blockSignals(false);
}

void StandardsInformationMaterialWidget::toggleUnits(bool displayIP)
{
  m_isIP = displayIP;
}

} // openstudio
