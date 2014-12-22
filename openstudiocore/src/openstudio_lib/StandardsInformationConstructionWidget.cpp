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

#include "../utilities/core/Assert.hpp"

#include <QComboBox>
#include <QGridLayout>
#include <QLabel>

#include <QStackedWidget>

namespace openstudio {

// ConstructionBaseInspectorView

  StandardsInformationConstructionWidget::StandardsInformationConstructionWidget(bool isIP, QWidget * parent)
  : QWidget(parent),
    m_standard(nullptr),
    m_standardSource(nullptr),
    m_intendedSurfaceType(nullptr),
    m_standardsConstructionType(nullptr),
    m_fenestrationLabel(nullptr),
    m_fenestration(nullptr),
    m_isIP(isIP),
    m_construction(boost::none),
    m_standardsInformation(boost::none)
{
}

void StandardsInformationConstructionWidget::addToLayout(QGridLayout* mainGridLayout, int& row)
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

  mainGridLayout->addLayout(vLayout, row, 0);

  ++row;

  line = new QFrame();
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow(QFrame::Sunken);
  mainGridLayout->addWidget(line, row, 0, 1, 3);

  ++row;

}

void StandardsInformationConstructionWidget::showFenestration()
{
  m_fenestrationLabel->show();
  m_fenestration->show();
}

void StandardsInformationConstructionWidget::hideFenestration()
{
  m_fenestrationLabel->hide();
  m_fenestration->hide();
}

void StandardsInformationConstructionWidget::enableFenestration()
{
  m_fenestration->setEnabled(true);
}

void StandardsInformationConstructionWidget::disableFenestration()
{
  m_fenestration->setEnabled(false);
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
