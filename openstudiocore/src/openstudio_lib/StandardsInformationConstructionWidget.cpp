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

#include "StandardsInformationConstructionWidget.hpp"

#include "OSItem.hpp"

#include "../shared_gui_components/OSComboBox.hpp"
#include "../shared_gui_components/OSLineEdit.hpp"
#include "../shared_gui_components/OSSwitch.hpp"

#include "../utilities/core/Assert.hpp"

#include <QComboBox>
#include <QGridLayout>
#include <QLabel>

#include <QStackedWidget>

namespace openstudio {

// ConstructionBaseInspectorView

StandardsInformationConstructionWidget::StandardsInformationConstructionWidget(bool isIP, QGridLayout* mainGridLayout, int& row)
  : QWidget(mainGridLayout->parentWidget())
{
  bool test;

  QVBoxLayout * vLayout = nullptr;

  QLabel * label = nullptr;

  // Measure Tags
  QFrame * line;
  line = new QFrame();
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow(QFrame::Sunken);
  mainGridLayout->addWidget(line, row, 0, 1, 3);

  ++row;

  label = new QLabel();
  label->setText("Measure Tags (Optional):");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label, row, 0);

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

  mainGridLayout->addLayout(vLayout, row, 0);

  //test = connect(m_standard, &QComboBox::editTextChanged, this, &openstudio::StandardsInformationConstructionWidget::standardChanged);
  //OS_ASSERT(test); 
  test = connect(m_standard, &QComboBox::currentTextChanged, this, &openstudio::StandardsInformationConstructionWidget::standardChanged);
  OS_ASSERT(test);

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

  mainGridLayout->addLayout(vLayout, row, 1);

  //test = connect(m_standardSource, &QComboBox::editTextChanged, this, &openstudio::StandardsInformationConstructionWidget::standardSourceChanged);
  //OS_ASSERT(test);
  test = connect(m_standardSource, &QComboBox::currentTextChanged, this, &openstudio::StandardsInformationConstructionWidget::standardSourceChanged);
  OS_ASSERT(test);

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

  mainGridLayout->addLayout(vLayout, row, 0);

  test = connect(m_intendedSurfaceType, &QComboBox::currentTextChanged, this, &openstudio::StandardsInformationConstructionWidget::intendedSurfaceTypeChanged);
  OS_ASSERT(test);

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

  mainGridLayout->addLayout(vLayout, row, 1);

  //test = connect(m_standardsConstructionType, &QComboBox::editTextChanged, this, &openstudio::StandardsInformationConstructionWidget::standardsConstructionTypeChanged);
  //OS_ASSERT(test);
  test = connect(m_standardsConstructionType, &QComboBox::currentTextChanged, this, &openstudio::StandardsInformationConstructionWidget::standardsConstructionTypeChanged);
  OS_ASSERT(test);

  ++row;

  // Fenestration Type
  vLayout = new QVBoxLayout();

  m_fenestrationTypeLabel = new QLabel();
  m_fenestrationTypeLabel->setText("Fenestration Type: ");
  m_fenestrationTypeLabel->setObjectName("StandardsInfo");
  vLayout->addWidget(m_fenestrationTypeLabel);

  m_fenestrationType = new OSComboBox2();
  m_fenestrationType->setFixedWidth(OSItem::ITEM_WIDTH);
  vLayout->addWidget(m_fenestrationType);

  mainGridLayout->addLayout(vLayout, row, 0);

  // Fenestration Assembly Context
  vLayout = new QVBoxLayout();

  m_fenestrationAssemblyContextLabel = new QLabel();
  m_fenestrationAssemblyContextLabel->setText("Fenestration Assembly Context: ");
  m_fenestrationAssemblyContextLabel->setObjectName("StandardsInfo");
  vLayout->addWidget(m_fenestrationAssemblyContextLabel);

  m_fenestrationAssemblyContext = new OSComboBox2();
  m_fenestrationAssemblyContext->setFixedWidth(OSItem::ITEM_WIDTH);
  vLayout->addWidget(m_fenestrationAssemblyContext);

  mainGridLayout->addLayout(vLayout, row, 1);

  ++row;

  // Fenestration Number of Panes
  vLayout = new QVBoxLayout();

  m_fenestrationNumberOfPanesLabel = new QLabel();
  m_fenestrationNumberOfPanesLabel->setText("Fenestration Number of Panes: ");
  m_fenestrationNumberOfPanesLabel->setObjectName("StandardsInfo");
  vLayout->addWidget(m_fenestrationNumberOfPanesLabel);

  m_fenestrationNumberOfPanes = new OSComboBox2();
  m_fenestrationNumberOfPanes->setFixedWidth(OSItem::ITEM_WIDTH);
  vLayout->addWidget(m_fenestrationNumberOfPanes);

  mainGridLayout->addLayout(vLayout, row, 0);

  // Fenestration Frame Type
  vLayout = new QVBoxLayout();

  m_fenestrationFrameTypeLabel = new QLabel();
  m_fenestrationFrameTypeLabel->setText("Fenestration Frame Type: ");
  m_fenestrationFrameTypeLabel->setObjectName("StandardsInfo");
  vLayout->addWidget(m_fenestrationFrameTypeLabel);

  m_fenestrationFrameType = new OSComboBox2();
  m_fenestrationFrameType->setFixedWidth(OSItem::ITEM_WIDTH);
  vLayout->addWidget(m_fenestrationFrameType);

  mainGridLayout->addLayout(vLayout, row, 1);

  ++row;

  // Fenestration Divider Type
  vLayout = new QVBoxLayout();

  m_fenestrationDividerTypeLabel = new QLabel();
  m_fenestrationDividerTypeLabel->setText("Fenestration Divider Type: ");
  m_fenestrationDividerTypeLabel->setObjectName("StandardsInfo");
  vLayout->addWidget(m_fenestrationDividerTypeLabel);

  m_fenestrationDividerType = new OSComboBox2();
  m_fenestrationDividerType->setFixedWidth(OSItem::ITEM_WIDTH);
  vLayout->addWidget(m_fenestrationDividerType);

  mainGridLayout->addLayout(vLayout, row, 0);

  // Fenestration Tint
  vLayout = new QVBoxLayout();

  m_fenestrationTintLabel = new QLabel();
  m_fenestrationTintLabel->setText("Fenestration Tint: ");
  m_fenestrationTintLabel->setObjectName("StandardsInfo");
  vLayout->addWidget(m_fenestrationTintLabel);

  m_fenestrationTint = new OSComboBox2();
  m_fenestrationTint->setFixedWidth(OSItem::ITEM_WIDTH);
  vLayout->addWidget(m_fenestrationTint);

  mainGridLayout->addLayout(vLayout, row, 1);

  ++row;    

  // Fenestration Gas Fill
  vLayout = new QVBoxLayout();

    m_fenestrationGasFillLabel = new QLabel();
    m_fenestrationGasFillLabel->setText("Fenestration Gas Fill: ");
  m_fenestrationGasFillLabel->setObjectName("StandardsInfo");
  vLayout->addWidget(m_fenestrationGasFillLabel);

  m_fenestrationGasFill = new OSComboBox2();
  m_fenestrationGasFill->setFixedWidth(OSItem::ITEM_WIDTH);
  vLayout->addWidget(m_fenestrationGasFill);

  mainGridLayout->addLayout(vLayout, row, 0);

  // Fenestration Gas Fill
  vLayout = new QVBoxLayout();

  m_fenestrationLowEmissivityCoatingLabel = new QLabel();
  m_fenestrationLowEmissivityCoatingLabel->setText("Fenestration Low Emissivity Coating: ");
  m_fenestrationLowEmissivityCoatingLabel->setObjectName("StandardsInfo");
  vLayout->addWidget(m_fenestrationLowEmissivityCoatingLabel);

  m_fenestrationLowEmissivityCoating = new OSSwitch2();
  vLayout->addWidget(m_fenestrationLowEmissivityCoating);

  mainGridLayout->addLayout(vLayout, row, 1);

  ++row;

  // Border
  line = new QFrame();
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow(QFrame::Sunken);
  mainGridLayout->addWidget(line, row, 0, 1, 3);

  ++row;
}

StandardsInformationConstructionWidget::~StandardsInformationConstructionWidget()
{}

void StandardsInformationConstructionWidget::showFenestration()
{
  m_fenestrationType->show();
  m_fenestrationAssemblyContext->show();
  m_fenestrationNumberOfPanes->show();
  m_fenestrationFrameType->show();
  m_fenestrationDividerType->show();
  m_fenestrationTint->show();
  m_fenestrationGasFill->show();
  m_fenestrationLowEmissivityCoating->show();

  m_fenestrationTypeLabel->show();
  m_fenestrationAssemblyContextLabel->show();
  m_fenestrationNumberOfPanesLabel->show();
  m_fenestrationFrameTypeLabel->show();
  m_fenestrationDividerTypeLabel->show();
  m_fenestrationTintLabel->show();
  m_fenestrationGasFillLabel->show();
  m_fenestrationLowEmissivityCoatingLabel->show();
}

void StandardsInformationConstructionWidget::hideFenestration()
{
  m_fenestrationType->hide();
  m_fenestrationAssemblyContext->hide();
  m_fenestrationNumberOfPanes->hide();
  m_fenestrationFrameType->hide();
  m_fenestrationDividerType->hide();
  m_fenestrationTint->hide();
  m_fenestrationGasFill->hide();
  m_fenestrationLowEmissivityCoating->hide();

  m_fenestrationTypeLabel->hide();
  m_fenestrationAssemblyContextLabel->hide();
  m_fenestrationNumberOfPanesLabel->hide();
  m_fenestrationFrameTypeLabel->hide();
  m_fenestrationDividerTypeLabel->hide();
  m_fenestrationTintLabel->hide();
  m_fenestrationGasFillLabel->hide();
  m_fenestrationLowEmissivityCoatingLabel->hide();
}

void StandardsInformationConstructionWidget::enableFenestration()
{
  m_fenestrationType->setEnabled(true);
  m_fenestrationAssemblyContext->setEnabled(true);
  m_fenestrationNumberOfPanes->setEnabled(true);
  m_fenestrationFrameType->setEnabled(true);
  m_fenestrationDividerType->setEnabled(true);
  m_fenestrationTint->setEnabled(true);
  m_fenestrationGasFill->setEnabled(true);
  m_fenestrationLowEmissivityCoating->setEnabled(true);

  m_fenestrationType->bind<std::string>(
    *m_standardsInformation,
    static_cast<std::string(*)(const std::string&)>(&openstudio::toString),
    std::bind(&openstudio::model::StandardsInformationConstruction::fenestrationTypeValues),
    std::function<boost::optional<std::string>()>(std::bind(&openstudio::model::StandardsInformationConstruction::fenestrationType, m_standardsInformation.get_ptr())),
    std::bind(&openstudio::model::StandardsInformationConstruction::setFenestrationType, m_standardsInformation.get_ptr(), std::placeholders::_1),
    NoFailAction(std::bind(&model::StandardsInformationConstruction::resetFenestrationType, m_standardsInformation.get_ptr())));

  m_fenestrationAssemblyContext->bind<std::string>(
    *m_standardsInformation,
    static_cast<std::string(*)(const std::string&)>(&openstudio::toString),
    std::bind(&openstudio::model::StandardsInformationConstruction::fenestrationAssemblyContextValues),
    std::function<boost::optional<std::string>()>(std::bind(&openstudio::model::StandardsInformationConstruction::fenestrationAssemblyContext, m_standardsInformation.get_ptr())),
    std::bind(&openstudio::model::StandardsInformationConstruction::setFenestrationAssemblyContext, m_standardsInformation.get_ptr(), std::placeholders::_1),
    NoFailAction(std::bind(&model::StandardsInformationConstruction::resetFenestrationAssemblyContext, m_standardsInformation.get_ptr())));

  m_fenestrationNumberOfPanes->bind<std::string>(
    *m_standardsInformation,
    static_cast<std::string(*)(const std::string&)>(&openstudio::toString),
    std::bind(&openstudio::model::StandardsInformationConstruction::fenestrationNumberOfPanesValues),
    std::function<boost::optional<std::string>()>(std::bind(&openstudio::model::StandardsInformationConstruction::fenestrationNumberOfPanes, m_standardsInformation.get_ptr())),
    std::bind(&openstudio::model::StandardsInformationConstruction::setFenestrationNumberOfPanes, m_standardsInformation.get_ptr(), std::placeholders::_1),
    NoFailAction(std::bind(&model::StandardsInformationConstruction::resetFenestrationNumberOfPanes, m_standardsInformation.get_ptr())));

  m_fenestrationFrameType->bind<std::string>(
    *m_standardsInformation,
    static_cast<std::string(*)(const std::string&)>(&openstudio::toString),
    std::bind(&openstudio::model::StandardsInformationConstruction::fenestrationFrameTypeValues),
    std::function<boost::optional<std::string>()>(std::bind(&openstudio::model::StandardsInformationConstruction::fenestrationFrameType, m_standardsInformation.get_ptr())),
    std::bind(&openstudio::model::StandardsInformationConstruction::setFenestrationFrameType, m_standardsInformation.get_ptr(), std::placeholders::_1),
    NoFailAction(std::bind(&model::StandardsInformationConstruction::resetFenestrationFrameType, m_standardsInformation.get_ptr())));

  m_fenestrationDividerType->bind<std::string>(
    *m_standardsInformation,
    static_cast<std::string(*)(const std::string&)>(&openstudio::toString),
    std::bind(&openstudio::model::StandardsInformationConstruction::fenestrationDividerTypeValues),
    std::function<boost::optional<std::string>()>(std::bind(&openstudio::model::StandardsInformationConstruction::fenestrationDividerType, m_standardsInformation.get_ptr())),
    std::bind(&openstudio::model::StandardsInformationConstruction::setFenestrationDividerType, m_standardsInformation.get_ptr(), std::placeholders::_1),
    NoFailAction(std::bind(&model::StandardsInformationConstruction::resetFenestrationDividerType, m_standardsInformation.get_ptr())));

  m_fenestrationTint->bind<std::string>(
    *m_standardsInformation,
    static_cast<std::string(*)(const std::string&)>(&openstudio::toString),
    std::bind(&openstudio::model::StandardsInformationConstruction::fenestrationTintValues),
    std::function<boost::optional<std::string>()>(std::bind(&openstudio::model::StandardsInformationConstruction::fenestrationTint, m_standardsInformation.get_ptr())),
    std::bind(&openstudio::model::StandardsInformationConstruction::setFenestrationTint, m_standardsInformation.get_ptr(), std::placeholders::_1),
    NoFailAction(std::bind(&model::StandardsInformationConstruction::resetFenestrationTint, m_standardsInformation.get_ptr())));

  m_fenestrationGasFill->bind<std::string>(
    *m_standardsInformation,
    static_cast<std::string(*)(const std::string&)>(&openstudio::toString),
    std::bind(&openstudio::model::StandardsInformationConstruction::fenestrationGasFillValues),
    std::function<boost::optional<std::string>()>(std::bind(&openstudio::model::StandardsInformationConstruction::fenestrationGasFill, m_standardsInformation.get_ptr())),
    std::bind(&openstudio::model::StandardsInformationConstruction::setFenestrationGasFill, m_standardsInformation.get_ptr(), std::placeholders::_1),
    NoFailAction(std::bind(&model::StandardsInformationConstruction::resetFenestrationGasFill, m_standardsInformation.get_ptr())));

  m_fenestrationLowEmissivityCoating->bind(
   *m_standardsInformation,
    std::bind(&openstudio::model::StandardsInformationConstruction::fenestrationLowEmissivityCoating, m_standardsInformation.get_ptr()),
    BoolSetter(std::bind(&openstudio::model::StandardsInformationConstruction::setFenestrationLowEmissivityCoating, m_standardsInformation.get_ptr(), std::placeholders::_1)),
    NoFailAction(std::bind(&model::StandardsInformationConstruction::resetFenestrationLowEmissivityCoating, m_standardsInformation.get_ptr())));
}

void StandardsInformationConstructionWidget::disableFenestration()
{
  m_fenestrationType->setEnabled(false);
  m_fenestrationAssemblyContext->setEnabled(false);
  m_fenestrationNumberOfPanes->setEnabled(false);
  m_fenestrationFrameType->setEnabled(false);
  m_fenestrationDividerType->setEnabled(false);
  m_fenestrationTint->setEnabled(false);
  m_fenestrationGasFill->setEnabled(false);
  m_fenestrationLowEmissivityCoating->setEnabled(false);

  m_fenestrationType->unbind();
  m_fenestrationAssemblyContext->unbind();
  m_fenestrationNumberOfPanes->unbind();
  m_fenestrationFrameType->unbind();
  m_fenestrationDividerType->unbind();
  m_fenestrationTint->unbind();
  m_fenestrationGasFill->unbind();
  m_fenestrationLowEmissivityCoating->unbind();
}

void StandardsInformationConstructionWidget::toggleUnits(bool displayIP)
{
  m_isIP = displayIP;
}

void StandardsInformationConstructionWidget::attach(openstudio::model::ConstructionBase & construction)
{
  detach();

  m_construction = construction;
  m_standardsInformation = construction.standardsInformation();

  m_standard->setEnabled(true);

  m_standardSource->setEnabled(true);

  m_intendedSurfaceType->bind<std::string>(
    *m_standardsInformation,
    static_cast<std::string(*)(const std::string&)>(&openstudio::toString),
    std::bind(&openstudio::model::StandardsInformationConstruction::intendedSurfaceTypeValues),
    std::function<boost::optional<std::string>()>(std::bind(&openstudio::model::StandardsInformationConstruction::intendedSurfaceType, m_standardsInformation.get_ptr())),
    std::bind(&openstudio::model::StandardsInformationConstruction::setIntendedSurfaceType, m_standardsInformation.get_ptr(), std::placeholders::_1),
    NoFailAction(std::bind(&model::StandardsInformationConstruction::resetIntendedSurfaceType, m_standardsInformation.get_ptr())));

  m_standardsConstructionType->setEnabled(true);

  // do not bind fenestration fields here, that is done in enableFenestration
  if (construction.isFenestration()){
    enableFenestration();
  } else {
    disableFenestration();
  }

  populateStandards();
  populateStandardSources();
  populateStandardsConstructionType();
}

void StandardsInformationConstructionWidget::detach()
{
  m_construction.reset(); 

  if (m_standardsInformation){
    disconnect(m_standardsInformation->getImpl<openstudio::model::detail::ModelObject_Impl>().get(), 0, this, 0);
    m_standardsInformation.reset();
  }

  m_standard->setEnabled(false);

  m_standardSource->setEnabled(false);

  m_intendedSurfaceType->unbind();

  m_standardsConstructionType->setEnabled(false);

  disableFenestration();
}

void StandardsInformationConstructionWidget::standardChanged(const QString& text)
{
  if (m_standardsInformation){
    std::string standard = toString(text);
    if (standard.empty()){
      m_standardsInformation->resetConstructionStandard();
    } else{
      m_standardsInformation->setConstructionStandard(standard);
    }
    populateStandards();
    populateStandardSources();
  }
}

void StandardsInformationConstructionWidget::populateStandards()
{
  m_standard->blockSignals(true);

  m_standard->clear();
  if (m_standardsInformation){
    m_standard->addItem("");
    std::vector<std::string> suggestedConstructionStandards = m_standardsInformation->suggestedConstructionStandards();
    for (const std::string& suggestedConstructionStandard : suggestedConstructionStandards) {
      m_standard->addItem(toQString(suggestedConstructionStandard));
    }
    boost::optional<std::string> constructionStandard = m_standardsInformation->constructionStandard();
    if (constructionStandard){
      OS_ASSERT(!suggestedConstructionStandards.empty());
      m_standard->setCurrentIndex(1);
    } else{
      m_standard->setCurrentIndex(0);
    }
  }

  m_standard->blockSignals(false);
}

void StandardsInformationConstructionWidget::standardSourceChanged(const QString& text)
{
  if (m_standardsInformation){
    std::string standardSource = toString(text);
    if (standardSource.empty()){
      m_standardsInformation->resetConstructionStandardSource();
    } else{
      m_standardsInformation->setConstructionStandardSource(standardSource);
    }
    populateStandardSources();
  }
}

void StandardsInformationConstructionWidget::populateStandardSources()
{
  m_standardSource->blockSignals(true);

  m_standardSource->clear();
  if (m_standardsInformation){
    m_standardSource->addItem("");
    std::vector<std::string> suggestedConstructionStandardSources = m_standardsInformation->suggestedConstructionStandardSources();
    for (const std::string& suggestedConstructionStandardSource : suggestedConstructionStandardSources) {
      m_standardSource->addItem(toQString(suggestedConstructionStandardSource));
    }
    boost::optional<std::string> constructionStandardSource = m_standardsInformation->constructionStandardSource();
    if (constructionStandardSource){
      OS_ASSERT(!suggestedConstructionStandardSources.empty());
      m_standardSource->setCurrentIndex(1);
    } else{
      m_standardSource->setCurrentIndex(0);
    }
  }

  m_standardSource->blockSignals(false);
}

void StandardsInformationConstructionWidget::intendedSurfaceTypeChanged(const QString& text)
{
  populateStandardsConstructionType();
}

void StandardsInformationConstructionWidget::standardsConstructionTypeChanged(const QString & text)
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

void StandardsInformationConstructionWidget::populateStandardsConstructionType()
{
  m_standardsConstructionType->blockSignals(true);

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

  m_standardsConstructionType->blockSignals(false);
}





} // openstudio
