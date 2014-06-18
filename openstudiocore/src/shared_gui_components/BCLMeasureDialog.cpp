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

#include "BCLMeasureDialog.hpp"

#include "../utilities/core/Assert.hpp"

#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QGroupBox>

namespace openstudio {
  

BCLMeasureDialog::BCLMeasureDialog(QWidget* parent)
  : OSDialog(false, parent)
{
  setWindowTitle("Create New Measure");

  init();

  m_nameLineEdit->setText("New Measure");
  m_descriptionTextEdit->setText("Replace this text with an explanation of what the measure does in terms that can be understood by a general building professional audience (building owners, architects, engineers, contractors, etc.).  This description will be used to create reports aimed at convincing the owner and/or design team to implement the measure in the actual building design.  For this reason, the description may include details about how the measure would be implemented, along with explanations of qualitative benefits associated with the measure.  It is good practice to include citations in the measure if the description is taken from a known source or if specific benefits are listed.");
  m_modelerDescriptionTextEdit->setText("Replace this text with an explanation for the energy modeler specifically.  It should explain how the measure is modeled, including any requirements about how the baseline model must be set up, major assumptions, citations of references to applicable modeling resources, etc.  The energy modeler should be able to read this description and understand what changes the measure is making to the model and why these changes are being made.  Because the Modeler Description is written for an expert audience, using common abbreviations for brevity is good practice.");

  m_taxonomyFirstLevelComboBox->setCurrentIndex(0);
  firstLevelTaxonomyChanged(m_taxonomyFirstLevelComboBox->currentText());
}

BCLMeasureDialog::BCLMeasureDialog(const BCLMeasure& bclMeasure, QWidget* parent)
  : m_bclMeasureToCopy(bclMeasure)
{
  setWindowTitle("Copy Measure");

  init();

  m_nameLineEdit->setText(toQString(bclMeasure.name() + " Copy"));
  m_descriptionTextEdit->setText(toQString(bclMeasure.description()));
  m_modelerDescriptionTextEdit->setText(toQString(bclMeasure.modelerDescription()));

  QString measureTypeString;
  MeasureType measureType = bclMeasure.measureType();
  if ( measureType == MeasureType::ModelMeasure){
    measureTypeString = "OpenStudio Measure";
  }else if ( measureType == MeasureType::EnergyPlusMeasure){
    measureTypeString = "EnergyPlus Measure";
  }else if ( measureType == MeasureType::UtilityMeasure){
    measureTypeString = "Utility Measure";
  }else if ( measureType == MeasureType::ReportingMeasure){
    measureTypeString = "Reporting Measure";
  }
  int index = m_measureTypeComboBox->findText(measureTypeString);
  m_measureTypeComboBox->setCurrentIndex(index);

  QStringList taxonomyParts = toQString(bclMeasure.taxonomyTag()).split('.');
  if (taxonomyParts.size() > 0){
    index = m_taxonomyFirstLevelComboBox->findText(taxonomyParts[0]);
    m_taxonomyFirstLevelComboBox->setCurrentIndex(index);
    firstLevelTaxonomyChanged(m_taxonomyFirstLevelComboBox->currentText());
  }
  if (taxonomyParts.size() > 1){
    index = m_taxonomySecondLevelComboBox->findText(taxonomyParts[1]);
    m_taxonomySecondLevelComboBox->setCurrentIndex(index);
  }

  // todo: initialize uses sketchup api and requires e+ results
}

BCLMeasureDialog::~BCLMeasureDialog()
{
}

QSize BCLMeasureDialog::sizeHint() const
{
  return QSize(770,540);
}

boost::optional<openstudio::BCLMeasure> BCLMeasureDialog::createMeasure()
{
  std::string name = toString(m_nameLineEdit->text());
  std::string className = BCLMeasure::className(name);
  std::string description = toString(m_descriptionTextEdit->toPlainText());
  std::string modelerDescription = toString(m_modelerDescriptionTextEdit->toPlainText());

  std::string measureTypeStr = toString(m_measureTypeComboBox->currentText());
  MeasureType measureType;
  if ( measureTypeStr == "OpenStudio Measure"){
    measureType = MeasureType::ModelMeasure;
  }else if ( measureTypeStr == "EnergyPlus Measure"){
    measureType = MeasureType::EnergyPlusMeasure;
  }else if ( measureTypeStr == "Utility Measure"){
    measureType = MeasureType::UtilityMeasure;
  }else if ( measureTypeStr == "Reporting Measure"){
    measureType = MeasureType::ReportingMeasure;
  }

  bool usesSketchUpAPI = false; //disabled for now, m_usesSketchUpAPI->isChecked();

  openstudio::path userMeasuresDir = BCLMeasure::userMeasuresDir();
  QString folderName = toQString(className).append("/");
  openstudio::path measureDir = userMeasuresDir / toPath(folderName);

  // prompt user ???
  int i = 1;
  while (boost::filesystem::exists(measureDir)){
    folderName = toQString(className).append(" ").append(QString::number(i)).append("/");
    measureDir = userMeasuresDir / toPath(folderName);
    ++i;
  }

  QStringList taxonomyParts;
  QString taxonomyFirstLevel = m_taxonomyFirstLevelComboBox->currentText();
  QString taxonomySecondLevel = m_taxonomySecondLevelComboBox->currentText();
  taxonomyParts << taxonomyFirstLevel;
  if (!taxonomySecondLevel.isEmpty()){
    taxonomyParts << taxonomySecondLevel;
  }
  std::string taxonomyTag = toString(taxonomyParts.join("."));

  boost::optional<BCLMeasure> result;
  if (m_bclMeasureToCopy){
    // have measure to copy, use clone
    result = m_bclMeasureToCopy->clone(measureDir);
    if (result){
      result->changeUID();
      result->setName(name);
      // todo: change class name? this would require opening the ruby files
      result->setDescription(description);
      result->setModelerDescription(modelerDescription);
      result->setTaxonomyTag(taxonomyTag);
      result->setMeasureType(measureType);
      result->setUsesSketchUpAPI(usesSketchUpAPI);
      result->save();
    }
  }else{
    try{
    // starting new measure
    result = BCLMeasure(name, className, measureDir, taxonomyTag,
                        measureType, usesSketchUpAPI);
    result->setDescription(description);
    result->setModelerDescription(modelerDescription);
    result->save();
    }catch(std::exception&){
    }
  }

  return result;
}

void BCLMeasureDialog::nameChanged(const QString& newName)
{
  std::string className = BCLMeasure::className(toString(newName));
  m_classNameLabel->setText(toQString(className));
}

void BCLMeasureDialog::measureTypeChanged(const QString& newName)
{
  if (newName == "Reporting Measure"){
    int index = m_taxonomyFirstLevelComboBox->findText("Reporting");
    m_taxonomyFirstLevelComboBox->setCurrentIndex(index);
  }else{
    m_taxonomyFirstLevelComboBox->setCurrentIndex(0);
  }
}

void BCLMeasureDialog::firstLevelTaxonomyChanged(const QString& newName)
{
  m_taxonomySecondLevelComboBox->clear();
  m_taxonomySecondLevelComboBox->setEnabled(false);

  if (newName == "Envelope"){
    m_taxonomySecondLevelComboBox->addItem("Form");
    m_taxonomySecondLevelComboBox->addItem("Opaque");
    m_taxonomySecondLevelComboBox->addItem("Fenestration");
    m_taxonomySecondLevelComboBox->addItem("Construction Sets");
    m_taxonomySecondLevelComboBox->addItem("Daylighting");
    m_taxonomySecondLevelComboBox->addItem("Infiltration");
    m_taxonomySecondLevelComboBox->setCurrentIndex(0);
    m_taxonomySecondLevelComboBox->setEnabled(true);
  }else if (newName == "Electric Lighting"){
    m_taxonomySecondLevelComboBox->addItem("Electric Lighting Controls");
    m_taxonomySecondLevelComboBox->addItem("Lighting Equipment");
    m_taxonomySecondLevelComboBox->setCurrentIndex(0);
    m_taxonomySecondLevelComboBox->setEnabled(true);
  }else if (newName == "Equipment"){
    m_taxonomySecondLevelComboBox->addItem("Equipment Controls");
    m_taxonomySecondLevelComboBox->addItem("Electric Equipment");
    m_taxonomySecondLevelComboBox->addItem("Gas Equipment");
    m_taxonomySecondLevelComboBox->setCurrentIndex(0);
    m_taxonomySecondLevelComboBox->setEnabled(true);
  }else if (newName == "People"){
    m_taxonomySecondLevelComboBox->addItem("Characteristics");
    m_taxonomySecondLevelComboBox->addItem("People Schedules");
    m_taxonomySecondLevelComboBox->setCurrentIndex(0);
    m_taxonomySecondLevelComboBox->setEnabled(true);
  }else if (newName == "HVAC"){
    m_taxonomySecondLevelComboBox->addItem("HVAC Controls");
    m_taxonomySecondLevelComboBox->addItem("Heating");
    m_taxonomySecondLevelComboBox->addItem("Cooling");
    m_taxonomySecondLevelComboBox->addItem("Heat Rejection");
    m_taxonomySecondLevelComboBox->addItem("Energy Recovery");
    m_taxonomySecondLevelComboBox->addItem("Distribution");
    m_taxonomySecondLevelComboBox->addItem("Ventilation");
    m_taxonomySecondLevelComboBox->addItem("Whole System");
    m_taxonomySecondLevelComboBox->setCurrentIndex(0);
    m_taxonomySecondLevelComboBox->setEnabled(true);
  }else if (newName == "Refrigeration"){
    m_taxonomySecondLevelComboBox->addItem("Refrigeration Controls");
    m_taxonomySecondLevelComboBox->addItem("Cases and Walkins");
    m_taxonomySecondLevelComboBox->addItem("Compressors");
    m_taxonomySecondLevelComboBox->addItem("Condensers");
    m_taxonomySecondLevelComboBox->addItem("Heat Reclaim");
    m_taxonomySecondLevelComboBox->setCurrentIndex(0);
    m_taxonomySecondLevelComboBox->setEnabled(true);    
  }else if (newName == "Service Water Heating"){
    m_taxonomySecondLevelComboBox->addItem("Water Use");
    m_taxonomySecondLevelComboBox->addItem("Water Heating");
    m_taxonomySecondLevelComboBox->addItem("Distribution");
    m_taxonomySecondLevelComboBox->setCurrentIndex(0);
    m_taxonomySecondLevelComboBox->setEnabled(true);
  }else if (newName == "Onsite Power Generation"){
    m_taxonomySecondLevelComboBox->addItem("Photovoltaic");
    m_taxonomySecondLevelComboBox->setCurrentIndex(0);
    m_taxonomySecondLevelComboBox->setEnabled(true);
  }else if (newName == "Whole Building"){
    m_taxonomySecondLevelComboBox->addItem("Whole Building Schedules");
    m_taxonomySecondLevelComboBox->addItem("Space Types");
    m_taxonomySecondLevelComboBox->setCurrentIndex(0);
    m_taxonomySecondLevelComboBox->setEnabled(true);
  }else if (newName == "Economics"){
    m_taxonomySecondLevelComboBox->addItem("Life Cycle Cost Analysis");
    m_taxonomySecondLevelComboBox->setCurrentIndex(0);
    m_taxonomySecondLevelComboBox->setEnabled(true);
  }else if (newName == "Reporting"){
    m_taxonomySecondLevelComboBox->addItem("QAQC");
    m_taxonomySecondLevelComboBox->addItem("Troubleshooting");
    m_taxonomySecondLevelComboBox->setCurrentIndex(0);
    m_taxonomySecondLevelComboBox->setEnabled(true);
  }
}

void BCLMeasureDialog::init()
{
#ifdef Q_OS_MAC
  int btnHeight = 44;
#else
  int btnHeight = 34;
#endif

  QPushButton* cancelButton = this->cancelButton();
  cancelButton->setText("Cancel");
  cancelButton->setMinimumHeight(btnHeight);
  cancelButton->setDefault(true);

  QPushButton* okButton = this->okButton();
  okButton->setText("Create Measure and \n Open for Editing");
  okButton->setMinimumHeight(btnHeight);

  auto vLayout = new QVBoxLayout;
  vLayout->setContentsMargins(QMargins(0,0,0,0));

  auto label = new QLabel;
  label->setText("Name:");
  label->setObjectName("H2");
  vLayout->addWidget(label);
  m_nameLineEdit = new QLineEdit(this);
  vLayout->addWidget(m_nameLineEdit);

  auto tempHLayout = new QHBoxLayout;
  label = new QLabel;
  label->setText("Class Name:");
  label->setObjectName("H2");
  tempHLayout->addWidget(label);
  m_classNameLabel = new QLabel(this);
  tempHLayout->addWidget(m_classNameLabel);
  tempHLayout->addStretch();
  vLayout->addLayout(tempHLayout);
  vLayout->addSpacing(10);

  label = new QLabel;
  label->setText("Description:");
  label->setObjectName("H2");
  vLayout->addWidget(label);
  m_descriptionTextEdit = new QTextEdit(this);
  m_descriptionTextEdit->setAcceptRichText(false);
  m_descriptionTextEdit->setTabChangesFocus(true);
  vLayout->addWidget(m_descriptionTextEdit);
  vLayout->addSpacing(10);

  label = new QLabel;
  label->setText("Modeler Description:");
  label->setObjectName("H2");
  vLayout->addWidget(label);
  m_modelerDescriptionTextEdit = new QTextEdit(this);
  m_modelerDescriptionTextEdit->setAcceptRichText(false);
  m_modelerDescriptionTextEdit->setTabChangesFocus(true);
  vLayout->addWidget(m_modelerDescriptionTextEdit);
  vLayout->addSpacing(10);

  auto vLayout2 = new QVBoxLayout;

  label = new QLabel;
  label->setText("Measure Type:");
  label->setObjectName("H2");
  vLayout2->addWidget(label);
  m_measureTypeComboBox = new QComboBox(this);
  m_measureTypeComboBox->addItem("OpenStudio Measure");
  m_measureTypeComboBox->addItem("EnergyPlus Measure");
  //m_measureTypeComboBox->addItem("Utility Measure"); // Disable for now
  m_measureTypeComboBox->addItem("Reporting Measure");
  m_measureTypeComboBox->setCurrentIndex(0);
  vLayout2->addWidget(m_measureTypeComboBox);
  vLayout2->addSpacing(10);

  label = new QLabel;
  label->setText("Taxonomy:");
  label->setObjectName("H2");
  vLayout2->addWidget(label);
  tempHLayout = new QHBoxLayout;
  m_taxonomyFirstLevelComboBox = new QComboBox(this);
  m_taxonomyFirstLevelComboBox->addItem("Envelope");
  m_taxonomyFirstLevelComboBox->addItem("Electric Lighting");
  m_taxonomyFirstLevelComboBox->addItem("Equipment");
  m_taxonomyFirstLevelComboBox->addItem("People");
  m_taxonomyFirstLevelComboBox->addItem("HVAC");
  m_taxonomyFirstLevelComboBox->addItem("Refrigeration");
  m_taxonomyFirstLevelComboBox->addItem("Service Water Heating");
  m_taxonomyFirstLevelComboBox->addItem("Onsite Power Generation");
  m_taxonomyFirstLevelComboBox->addItem("Whole Building");
  m_taxonomyFirstLevelComboBox->addItem("Economics");
  m_taxonomyFirstLevelComboBox->addItem("Reporting");
  tempHLayout->addWidget(m_taxonomyFirstLevelComboBox);
  m_taxonomySecondLevelComboBox = new QComboBox(this);
  tempHLayout->addWidget(m_taxonomySecondLevelComboBox);
  vLayout2->addLayout(tempHLayout);
  vLayout2->addSpacing(10);

  /* Disable for now 
  m_usesSketchUpAPI = new QRadioButton(this);
  m_usesSketchUpAPI->setText("Yes");
  m_usesSketchUpAPI->setChecked(false);
  QRadioButton* notUsesSketchUpAPI = new QRadioButton(this);
  notUsesSketchUpAPI->setText("No");
  notUsesSketchUpAPI->setChecked(true);
  tempHLayout = new QHBoxLayout;
  tempHLayout->addWidget(m_usesSketchUpAPI);
  tempHLayout->addWidget(notUsesSketchUpAPI);
  tempHLayout->addStretch();
  groupBox = new QGroupBox(this);
  groupBox->setTitle("Uses SketchUp API");
  groupBox->setLayout(tempHLayout);
  vLayout2->addWidget(groupBox);
  vLayout2->addSpacing(10);
  */

  auto hLayout = new QHBoxLayout;
  hLayout->addLayout(vLayout2);
  hLayout->addStretch();
  vLayout->addLayout(hLayout);

  QBoxLayout* upperLayout = this->upperLayout();
  upperLayout->addLayout(vLayout);
  upperLayout->addStretch();

  bool test = connect(m_nameLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(nameChanged(const QString&)));
  OS_ASSERT(test);

  test = connect(m_measureTypeComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(measureTypeChanged(const QString&)));
  OS_ASSERT(test);
  
  test = connect(m_taxonomyFirstLevelComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(firstLevelTaxonomyChanged(const QString&)));
  OS_ASSERT(test);

  this->setWindowModality(Qt::ApplicationModal);
  //this->setSizeGripEnabled(true);

  #ifdef Q_OS_MAC
    setWindowFlags(Qt::FramelessWindowHint);
  #else
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
  #endif
}


} // openstudio
