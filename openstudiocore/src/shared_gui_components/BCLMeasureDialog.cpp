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

#include "BCLMeasureDialog.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/StringHelpers.hpp"
#include "../utilities/core/PathHelpers.hpp"

#include <QBoxLayout>
#include <QComboBox>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QPushButton>
#include <QRadioButton>
#include <QTextEdit>

namespace openstudio {
  

BCLMeasureDialog::BCLMeasureDialog(QWidget* parent)
  : OSDialog(false, parent)
{
  setWindowTitle("Create New Measure");
  setWindowModality(Qt::ApplicationModal);

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

  m_nameLineEdit->setText(toQString(bclMeasure.displayName() + " Copy"));
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

  std::vector<std::string> intendedSoftwareTools = bclMeasure.intendedSoftwareTools();
  QList<QListWidgetItem *> items = m_intendedSoftwareToolListWidget->findItems(".*", Qt::MatchRegExp);
  for (QListWidgetItem * item : items){
    std::string intendedSoftwareTool = toString(item->text());
    if (std::find(intendedSoftwareTools.begin(), intendedSoftwareTools.end(), intendedSoftwareTool) == intendedSoftwareTools.end()){
      item->setCheckState(Qt::Unchecked);
    } else {
      item->setCheckState(Qt::Checked);
    }
  }

  std::vector<std::string> intendedUseCases = bclMeasure.intendedUseCases();
  items = m_intendedUseCaseListWidget->findItems(".*", Qt::MatchRegExp);
  for (QListWidgetItem * item : items){
    std::string intendedUseCase = toString(item->text());
    if (std::find(intendedUseCases.begin(), intendedUseCases.end(), intendedUseCase) == intendedUseCases.end()){
      item->setCheckState(Qt::Unchecked);
    } else {
      item->setCheckState(Qt::Checked);
    }
  }
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
  openstudio::path userMeasuresDir = BCLMeasure::userMeasuresDir();

  if (isNetworkPath(userMeasuresDir) && !isNetworkPathAvailable(userMeasuresDir)) {
    QMessageBox::information(this, "Cannot Create Measure", "Your My Measures Directory appears to be on a network drive that is not currently available.\nYou can change your specified My Measures Directory using 'Preferences->Change My Measures Directory'.", QMessageBox::Ok);
    return boost::optional<openstudio::BCLMeasure>();
  }

  std::string name = toString(m_nameLineEdit->text());
  std::string className = BCLMeasure::makeClassName(name);
  std::string lowerClassName = toUnderscoreCase(className);
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

  QString folderName = toQString(lowerClassName).append("/");
  openstudio::path measureDir = userMeasuresDir / toPath(folderName);

  // prompt user ???
  if (boost::filesystem::exists(measureDir)){
    int i = 1;
    while (boost::filesystem::exists(measureDir)){
      folderName = toQString(lowerClassName).append(" ").append(QString::number(i)).append("/");
      measureDir = userMeasuresDir / toPath(folderName);
      ++i;
    }
    // DLM: do we want to alter the class name to indicate this copy?
    //className = className + toString(QString::number(i));
    //lowerClassName = lowerClassName + toString(QString::number(i));
  }

  QStringList taxonomyParts;
  QString taxonomyFirstLevel = m_taxonomyFirstLevelComboBox->currentText();
  QString taxonomySecondLevel = m_taxonomySecondLevelComboBox->currentText();
  taxonomyParts << taxonomyFirstLevel;
  if (!taxonomySecondLevel.isEmpty()){
    taxonomyParts << taxonomySecondLevel;
  }
  std::string taxonomyTag = toString(taxonomyParts.join("."));

  std::vector<Attribute> attributes;

  QList<QListWidgetItem *> items = m_intendedSoftwareToolListWidget->findItems(".*", Qt::MatchRegExp);
  for (QListWidgetItem * item : items){
    if (item->checkState() == Qt::Checked){
      std::string intendedSoftwareTool = toString(item->text());
      attributes.push_back(Attribute("Intended Software Tool", intendedSoftwareTool));
    }
  }

  items = m_intendedUseCaseListWidget->findItems(".*", Qt::MatchRegExp);
  for (QListWidgetItem * item : items){
    if (item->checkState() == Qt::Checked){
      std::string intendedUseCase = toString(item->text());
      attributes.push_back(Attribute("Intended Use Case", intendedUseCase));
    }
  }

  boost::optional<BCLMeasure> result;
  if (m_bclMeasureToCopy){
    // have measure to copy, use clone
    result = m_bclMeasureToCopy->clone(measureDir);
    if (result){
      result->changeUID();

      // change the files on disk
      result->updateMeasureScript(m_bclMeasureToCopy->measureType(), measureType, 
                                  m_bclMeasureToCopy->className(), className, 
                                  name, description, modelerDescription);

      result->updateMeasureTests(m_bclMeasureToCopy->className(), className);

      result->checkForUpdatesFiles();

      // change the xml
      std::string lowerClassName = toUnderscoreCase(className);

      result->setName(lowerClassName);
      result->setClassName(className);
      result->setDisplayName(name);
      result->setDescription(description);
      result->setModelerDescription(modelerDescription);
      result->setArguments(m_bclMeasureToCopy->arguments());
      result->setTaxonomyTag(taxonomyTag);
      result->setMeasureType(measureType);

      // xml checksum is out of date

      for (const Attribute& attribute : attributes){
        result->addAttribute(attribute);
      }

      result->save();
    }
  }else{
    try{
    // starting new measure
    result = BCLMeasure(name, className, measureDir, taxonomyTag, measureType, description, modelerDescription);

    for (const Attribute& attribute : attributes){
      result->addAttribute(attribute);
    }

    result->save();
    }catch(std::exception&){
    }
  }

  return result;
}

void BCLMeasureDialog::nameChanged(const QString& newName)
{
  std::string className = BCLMeasure::makeClassName(toString(newName));
  m_classNameLabel->setText(toQString(className));
}

void BCLMeasureDialog::measureTypeChanged(const QString& newName)
{
  if (newName == "Reporting Measure"){
    int index = m_taxonomyFirstLevelComboBox->findText("Reporting");
    m_taxonomyFirstLevelComboBox->setCurrentIndex(index);
  }else{
    // DLM: do we want to toggle this back?
    m_taxonomyFirstLevelComboBox->setCurrentIndex(0);
  }

  if (newName == "OpenStudio Measure"){
    // DLM: do we want to toggle this back?
    QList<QListWidgetItem *> items = m_intendedSoftwareToolListWidget->findItems("Apply Measure Now", Qt::MatchFixedString);
    for (QListWidgetItem * item : items){
      item->setCheckState(Qt::Checked);
    }
  }else{
    QList<QListWidgetItem *> items = m_intendedSoftwareToolListWidget->findItems("Apply Measure Now", Qt::MatchFixedString);
    for (QListWidgetItem * item : items){
      item->setCheckState(Qt::Unchecked);
    }
  }
}

void BCLMeasureDialog::firstLevelTaxonomyChanged(const QString& newName)
{
  m_taxonomySecondLevelComboBox->clear();
  m_taxonomySecondLevelComboBox->setEnabled(false);

  std::vector<std::string> secondLevelTerms = BCLMeasure::suggestedSecondLevelTaxonomyTerms(toString(newName));

  if (!secondLevelTerms.empty()){
    for (const std::string& secondLevelTerm : secondLevelTerms){
      m_taxonomySecondLevelComboBox->addItem(toQString(secondLevelTerm));
    }
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

  tempHLayout = new QHBoxLayout;
  vLayout->addLayout(tempHLayout);

  auto tempVLayout = new QVBoxLayout;

  label = new QLabel;
  label->setText("Measure Type:");
  label->setObjectName("H2");
  tempVLayout->addWidget(label);
  m_measureTypeComboBox = new QComboBox(this);
  m_measureTypeComboBox->addItem("OpenStudio Measure");
  m_measureTypeComboBox->addItem("EnergyPlus Measure");
  //m_measureTypeComboBox->addItem("Utility Measure"); // Disable for now
  m_measureTypeComboBox->addItem("Reporting Measure");
  m_measureTypeComboBox->setCurrentIndex(0);
  tempVLayout->addWidget(m_measureTypeComboBox);
  tempVLayout->addSpacing(10);
  tempHLayout->addLayout(tempVLayout);

  tempVLayout = new QVBoxLayout;

  label = new QLabel;
  label->setText("Taxonomy:");
  label->setObjectName("H2");
  tempVLayout->addWidget(label);
  m_taxonomyFirstLevelComboBox = new QComboBox(this);

  std::vector<std::string> firstLevelTerms = BCLMeasure::suggestedFirstLevelTaxonomyTerms();
  for (const std::string& firstLevelTerm : firstLevelTerms){
    m_taxonomyFirstLevelComboBox->addItem(toQString(firstLevelTerm));
  }

  m_taxonomySecondLevelComboBox = new QComboBox(this);
  auto tempHLayout2 = new QHBoxLayout;
  tempHLayout2->addWidget(m_taxonomyFirstLevelComboBox);
  tempHLayout2->addWidget(m_taxonomySecondLevelComboBox);
  tempVLayout->addLayout(tempHLayout2);
  tempVLayout->addSpacing(10);
  tempHLayout->addLayout(tempVLayout);

  tempHLayout = new QHBoxLayout;
  vLayout->addLayout(tempHLayout);

  tempVLayout = new QVBoxLayout;

  label = new QLabel;
  label->setText("Intended Software Tools:");
  label->setObjectName("H2");
  tempVLayout->addWidget(label);
  m_intendedSoftwareToolListWidget = new QListWidget(this);
  tempVLayout->addWidget(m_intendedSoftwareToolListWidget);
  QStringList intendedSoftwareTools;
  for (const std::string& suggestedTool : BCLMeasure::suggestedIntendedSoftwareTools()){
    intendedSoftwareTools.append(toQString(suggestedTool));
  }
  QStringListIterator it(intendedSoftwareTools);
  while (it.hasNext()){
    QString intendedSoftwareTool = it.next();
    auto listItem = new QListWidgetItem(intendedSoftwareTool, m_intendedSoftwareToolListWidget);
    // DLM: defaults per David
    if (intendedSoftwareTool == "Analysis Spreadsheet"){
      listItem->setCheckState(Qt::Unchecked);
    }else{
      listItem->setCheckState(Qt::Checked);
    }
    listItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    m_intendedSoftwareToolListWidget->addItem(listItem);
  }
  tempHLayout->addLayout(tempVLayout);
  
  tempVLayout = new QVBoxLayout;

  label = new QLabel;
  label->setText("Intended Use Cases:");
  label->setObjectName("H2");
  tempVLayout->addWidget(label);
  m_intendedUseCaseListWidget = new QListWidget();
  tempVLayout->addWidget(m_intendedUseCaseListWidget);
  QStringList intendedUseCases;
  for (const std::string& suggestedUseCase : BCLMeasure::suggestedIntendedUseCases()){
    intendedUseCases.append(toQString(suggestedUseCase));
  }
  it = QStringListIterator(intendedUseCases);
  while (it.hasNext()){
    QString intendedUseCase = it.next();
    auto listItem = new QListWidgetItem(intendedUseCase, m_intendedUseCaseListWidget);
    // DLM: default to unchecked per David
    listItem->setCheckState(Qt::Unchecked);
    listItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    m_intendedUseCaseListWidget->addItem(listItem);
  }
  tempHLayout->addLayout(tempVLayout);

  QBoxLayout* upperLayout = this->upperLayout();
  upperLayout->addLayout(vLayout);
  upperLayout->addStretch();

  connect(m_nameLineEdit, &QLineEdit::textChanged, this, &BCLMeasureDialog::nameChanged);

  connect(m_measureTypeComboBox, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
    this, &BCLMeasureDialog::measureTypeChanged);
  
  connect(m_taxonomyFirstLevelComboBox, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
    this, &BCLMeasureDialog::firstLevelTaxonomyChanged);

  this->setWindowModality(Qt::ApplicationModal);
  //this->setSizeGripEnabled(true);

  #ifdef Q_OS_MAC
    setWindowFlags(Qt::FramelessWindowHint);
  #else
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
  #endif
}


} // openstudio
