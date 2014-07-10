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

#include "Component.hpp"

#include "../utilities/bcl/BCLMeasure.hpp"
#include "../utilities/bcl/LocalBCL.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Compare.hpp"
#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/Unit.hpp"

#include <OpenStudio.hxx>

#include <QBoxLayout>
#include <QCheckBox>
#include <QHeaderView>
#include <QLabel>
#include <QPainter>
#include <QTableWidget>
#include <QTableWidgetItem>

#define OPENSTUDIO_TYPE "OpenStudio Type"

namespace openstudio {

Component::Component(const BCLMeasure & bclMeasure,
  bool showAbridgedView,
  bool showCheckBox,
  QWidget * parent)
  : QAbstractButton(parent),
  m_name(QString()),
  m_uid(QString()),
  m_versionId(QString()),
  m_description(QString()),
  m_fidelityLevel(QString()),
  m_attributes(std::vector<Attribute>()),
  m_files(std::vector<BCLFile>()),
  m_fileReferences(std::vector<BCLFileReference>()),
  m_provenances(std::vector<BCLProvenance>()),
  m_tags(std::vector<std::string>()),
  m_showAbridgedView(showAbridgedView),
  m_showCheckBox(showCheckBox),
  m_checkBox(NULL),
  m_msg(NULL)
{
  setCheckable(true);
  parseBCLMeasure(bclMeasure);
  if(m_showAbridgedView){
    createAbridgedLayout();
  }
  else{
    createCompleteLayout();
  }

  // This measure was created with a newer version of OpenStudio
  if (!m_available){
    if(m_checkBox){
      m_checkBox->setChecked(false);
      m_checkBox->setEnabled(false);
      m_updateAvailable = false;
      if (m_msg){
        m_msg->setText("This measure requires a newer version of OpenStudio");
        m_msg->setVisible(true);
      }
    }
  }else{
    // This measures has been pre-filtered and is known to require an update
    m_checkBox->setChecked(false);
    m_checkBox->setEnabled(true);
    m_updateAvailable = true;
    if (m_msg){
      m_msg->setText("An update is available for this measure");
      m_msg->setVisible(true);
    }
  }
}

Component::Component(const BCLSearchResult & bclSearchResult,
  bool showAbridgedView,
  bool showCheckBox,
  QWidget * parent)
  : QAbstractButton(parent),
  m_name(QString()),
  m_uid(QString()),
  m_versionId(QString()),
  m_description(QString()),
  m_fidelityLevel(QString()),
  m_attributes(std::vector<Attribute>()),
  m_files(std::vector<BCLFile>()),
  m_provenances(std::vector<BCLProvenance>()),
  m_tags(std::vector<std::string>()),
  m_showAbridgedView(showAbridgedView),
  m_showCheckBox(showCheckBox),
  m_checkBox(nullptr),
  m_msg(nullptr)
{
  setCheckable(true);
  parseBCLSearchResult(bclSearchResult);
  if(m_showAbridgedView){
    createAbridgedLayout();
  }
  else{
    createCompleteLayout();
  }

  if (bclSearchResult.componentType() == "component"){
    // This component has already been downloaded
    if (LocalBCL::instance().getComponent(this->uid(), this->versionId())){
      if(m_checkBox){
        m_checkBox->setChecked(true);
        m_checkBox->setEnabled(false);
        m_updateAvailable = false;
      }
    // This component has an update
    }else if (LocalBCL::instance().getComponent(this->uid())){
      if(m_checkBox){
        m_checkBox->setChecked(false);
        m_checkBox->setEnabled(true);
        m_updateAvailable = true;
        if (m_msg){
          m_msg->setText("An update is available for this component");
          m_msg->setVisible(true);
        }
      }
    // This component has not yet been downloaded
    }else{
      if(m_checkBox){
        m_checkBox->setChecked(false);
        m_checkBox->setEnabled(true);
        m_updateAvailable = false;
      }
    }
  }
  else if (bclSearchResult.componentType() == "measure"){
    // This measure was created with a newer version of OpenStudio
    if (!m_available){
      if(m_checkBox){
        m_checkBox->setChecked(false);
        m_checkBox->setEnabled(false);
        m_updateAvailable = false;
        if (m_msg){
          m_msg->setText("This measure requires a newer version of OpenStudio");
          m_msg->setVisible(true);
        }
      }
    }else{
      // This measure has already been downloaded
      if (LocalBCL::instance().getMeasure(this->uid(), this->versionId())){
        if(m_checkBox){
          m_checkBox->setChecked(true);
          m_checkBox->setEnabled(false);
          m_updateAvailable = false;
        }
      // This measure has an update
      }else if (LocalBCL::instance().getMeasure(this->uid())){
        if(m_checkBox){
          m_checkBox->setChecked(false);
          m_checkBox->setEnabled(true);
          m_updateAvailable = true;
          if (m_msg){
            m_msg->setText("An update is available for this measure");
            m_msg->setVisible(true);
          }
        }
      // This measure has not yet been downloaded
      }else{
        if(m_checkBox){
          m_checkBox->setChecked(false);
          m_checkBox->setEnabled(true);
          m_updateAvailable = false;
        }
      }
    }
  }
}

Component::Component(bool showAbridgedView,
  bool showCheckBox,
  QWidget * parent)
  : QAbstractButton(parent),
  m_name(QString()),
  m_uid(QString()),
  m_versionId(QString()),
  m_description(QString()),
  m_fidelityLevel(QString()),
  m_attributes(std::vector<Attribute>()),
  m_files(std::vector<BCLFile>()),
  m_fileReferences(std::vector<BCLFileReference>()),
  m_provenances(std::vector<BCLProvenance>()),
  m_tags(std::vector<std::string>()),
  m_showAbridgedView(showAbridgedView),
  m_showCheckBox(showCheckBox),
  m_checkBox(nullptr),
  m_msg(nullptr)
{
  setCheckable(true);
  if(m_showAbridgedView){
    createAbridgedLayout();
  }
  else{
    createCompleteLayout();
  }
}

Component::Component(const Component & other)
{
  ///! no self-assignment
  if(this != &other){
    m_name = other.m_name;
    m_uid = other.m_uid;
    m_versionId = other.m_versionId;
    m_description = other.m_description;
    m_fidelityLevel = other.m_fidelityLevel;
    m_attributes = other.m_attributes;
    m_files = other.m_files;
    m_fileReferences = other.m_fileReferences;
    m_provenances = other.m_provenances;
    m_tags = other.m_tags;
    m_showAbridgedView = other.m_showAbridgedView;
    m_showCheckBox = other.m_showCheckBox;

    m_showAbridgedView = false;
    m_showCheckBox = false;

    setCheckable(true);
    if(m_showAbridgedView){
      createAbridgedLayout();
    }
    else{
      createCompleteLayout();
    }
  }
}

Component::~Component()
{
}

// TODO The copy code for both operator= and copy CTOR should be in a common function
Component & Component::operator=(const Component & other)
{
  ///! no self-assignment
  if(this != &other){
    m_name = other.m_name;
    m_uid = other.m_uid;
    m_versionId = other.m_versionId;
    m_description = other.m_description;
    m_fidelityLevel = other.m_fidelityLevel;
    m_attributes = other.m_attributes;
    m_files = other.m_files;
    m_fileReferences = other.m_fileReferences;
    m_provenances = other.m_provenances;
    m_tags = other.m_tags;
    m_showAbridgedView = other.m_showAbridgedView;
    m_showCheckBox = other.m_showCheckBox;

    m_showAbridgedView = false;
    m_showCheckBox = false;

    setCheckable(true);
    if(m_showAbridgedView){
      createAbridgedLayout();
    }
    else{
      createCompleteLayout();
    }
  }

  return *this;
}

std::string Component::uid() const
{
  return toString(m_uid);
}

std::string Component::versionId() const
{
  return toString(m_versionId);
}

QCheckBox * Component::checkBox()
{
  return m_checkBox;
}

QLabel * Component::msg()
{
  return m_msg;
}

void Component::setChecked(bool checked)
{
  QAbstractButton::setChecked(checked);
  emit clicked(checked);
}

void Component::parseBCLMeasure(const BCLMeasure & bclMeasure)
{
  m_componentType = bclMeasure.measureType().enumName().c_str();
  m_name = bclMeasure.name().c_str();
  m_uid = bclMeasure.uid().c_str();
  m_versionId = bclMeasure.versionId().c_str();
  m_description = bclMeasure.description().c_str();

  m_attributes = bclMeasure.attributes();
  m_fileReferences = bclMeasure.files();
  m_tags = bclMeasure.tags();

  std::string softwareProgramVersion;
  Q_FOREACH(const BCLFileReference & fileReference, m_fileReferences){
    if (fileReference.usageType() == "script" && fileReference.softwareProgram() == "OpenStudio"){
      softwareProgramVersion = fileReference.softwareProgramVersion();
      break;
    }
  }
  if (!softwareProgramVersion.empty() && VersionString(softwareProgramVersion) > VersionString(openStudioVersion())){
    m_available = false;
  }else{
    m_available = true;
  }
}

void Component::parseBCLSearchResult(const BCLSearchResult & bclSearchResult)
{
  m_componentType = bclSearchResult.componentType().c_str();
  m_name = bclSearchResult.name().c_str();
  m_uid = bclSearchResult.uid().c_str();
  m_versionId = bclSearchResult.versionId().c_str();
  m_description = bclSearchResult.description().c_str();
  m_fidelityLevel = bclSearchResult.fidelityLevel().c_str();

  m_attributes = bclSearchResult.attributes();
  m_files = bclSearchResult.files();
  m_provenances = bclSearchResult.provenances();
  m_tags = bclSearchResult.tags();

  std::string componentVersion;
  for (const BCLFile & file : m_files) {
    if (file.usageType() == "script" && file.softwareProgram() == "OpenStudio"){
      componentVersion = file.identifier();
      break;
    }
  }
  if (!componentVersion.empty() && VersionString(componentVersion) > VersionString(openStudioVersion())){
    m_available = false;
  }else{
    m_available = true;
  }
}

void Component::createAbridgedLayout()
{
  m_checkBox = new QCheckBox(this);

  if(!m_showCheckBox){
    m_checkBox->hide();
  }

  QLabel * label = nullptr;

  QString string;

  auto leftLayout = new QVBoxLayout();

  string = "Name: ";
  string += m_name;
  label = new QLabel(string);
  leftLayout->addWidget(label);

  for (const Attribute & attribute : m_attributes) {
    string = attribute.name().c_str();
    if(m_componentType == "component"){
      if(string.toStdString() == OPENSTUDIO_TYPE){
        openstudio::AttributeValueType type = attribute.valueType();
        if(type == AttributeValueType::String){
          string = attribute.valueAsString().c_str();
          QString temp("Type: ");
          temp += string;
          label = new QLabel(temp);
          leftLayout->addWidget(label);
          break;
        }
      }
    }
    else if(m_componentType == "measure" || m_componentType == "MeasureType"){
      if(string.toStdString() == "Measure Type"){
        openstudio::AttributeValueType type = attribute.valueType();
        if(type == AttributeValueType::String){
          string = attribute.valueAsString().c_str();
          QString temp("Measure Type: ");
          temp += string;
          label = new QLabel(temp);
          leftLayout->addWidget(label);
        }
      }
      else if(string.toStdString() == "Measure Function"){
        openstudio::AttributeValueType type = attribute.valueType();
        if(type == AttributeValueType::String){
          string = attribute.valueAsString().c_str();
          QString temp("Measure Function: ");
          temp += string;
          label = new QLabel(temp);
          leftLayout->addWidget(label);
        }
      }
    }
  }
  
  m_msg = new QLabel(this);
  m_msg->setStyleSheet("color:#888;font-style:italic;");
  m_msg->setHidden(true);
  leftLayout->addWidget(m_msg);
  

  auto mainLayout = new QHBoxLayout();
  mainLayout->addLayout(leftLayout);
  mainLayout->addStretch();
  mainLayout->addWidget(m_checkBox);
  setLayout(mainLayout);
}

void Component::createCompleteLayout()
{
  QLabel * label = nullptr;

  QString string;

  auto mainLayout = new QVBoxLayout();

  QTableWidget * tableWidget = nullptr;

  ///! Attributes
  ///! Class BCL only stores double (optional units),
  ///! int (optional units), and string, with their names.
  label = new QLabel("Attributes");
  label->setObjectName("H1");
  mainLayout->addWidget(label);

  tableWidget = new QTableWidget(0,2,this);
  // really don't want the user to select or give focus to any table cells
  tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); 
  tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
  tableWidget->setAlternatingRowColors(true);
  tableWidget->verticalHeader()->hide();
  tableWidget->horizontalHeader()->hide();
  tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  tableWidget->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
  tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
  tableWidget->horizontalHeader()->setStretchLastSection(true);

  mainLayout->addWidget(tableWidget);

  for (const Attribute & attribute : m_attributes) {
    tableWidget->insertRow(tableWidget->rowCount());

    QTableWidgetItem * item = new QTableWidgetItem(attribute.name().c_str());
    tableWidget->setItem(tableWidget->rowCount() - 1, 0, item);
    
    boost::optional<std::string> optionalUnits = attribute.units();

    openstudio::AttributeValueType type = attribute.valueType();

    if(type == AttributeValueType::Boolean){
      bool success = attribute.valueAsBoolean();
      if(success){
        string = "true";
      } else {
        string = "false";
      }
    }
    else if(type == AttributeValueType::Double){
      string = string.setNum(attribute.valueAsDouble());
    }
    else if(type == AttributeValueType::Quantity){
      Quantity quantity = attribute.valueAsQuantity();
      string = string.setNum(quantity.value());
      string += " ";
      string += quantity.prettyUnitsString().c_str();
    }
    else if(type == AttributeValueType::Unit){
      Unit unit = attribute.valueAsUnit();
      string = unit.prettyString().c_str();
    }
    else if(type == AttributeValueType::Integer){
      string = string.setNum(attribute.valueAsInteger());
    }
    else if(type == AttributeValueType::Unsigned){
      string = string.setNum(attribute.valueAsUnsigned());
    } 
    else if(type == AttributeValueType::String){
      string = attribute.valueAsString().c_str();
    }
    else if(type == AttributeValueType::AttributeVector){
      AttributeVector attributeVector = attribute.valueAsAttributeVector();
      // TODO handle this case
    }
    else{
      // should never get here
    }
    if(optionalUnits){
      string += " ";
      std::string temp = optionalUnits.get();
      string += temp.c_str();
    }
    item = new QTableWidgetItem(string);
    tableWidget->setItem(tableWidget->rowCount() - 1, 1, item);
  }

  // make the table show completely
  int rowHeight = tableWidget->rowHeight(0);
  int rowCount =  tableWidget->rowCount();
  int tableHeight = rowHeight * rowCount;
  tableWidget->setFixedHeight(tableHeight);

  label = new QLabel();
  mainLayout->addWidget(label);

  /////! Files
  label = new QLabel("Files");
  label->setObjectName("H1");
  mainLayout->addWidget(label);
  for (const BCLFile & file : m_files) {
    label = new QLabel(file.filename().c_str());
    mainLayout->addWidget(label);

    //label = new QLabel(file.filetype().c_str());
    //mainLayout->addWidget(label);

    //label = new QLabel(file.softwareProgram().c_str());
    //mainLayout->addWidget(label);

    //label = new QLabel(file.identifier().c_str());
    //mainLayout->addWidget(label);
  }
  for (const BCLFileReference & fileReference : m_fileReferences){
    label = new QLabel(fileReference.fileName().c_str());
    mainLayout->addWidget(label);

    //label = new QLabel(fileReference.fileType().c_str());
    //mainLayout->addWidget(label);

    //label = new QLabel(fileReference.softwareProgram().c_str());
    //mainLayout->addWidget(label);

    //label = new QLabel(fileReference.softwareProgramVersion().c_str());
    //mainLayout->addWidget(label);
  }

  label = new QLabel();
  mainLayout->addWidget(label);

  ///! Provenances
  label = new QLabel("Sources");
  label->setObjectName("H1");
  mainLayout->addWidget(label);
  for (const BCLProvenance & provenance : m_provenances) {
    string = "Author: ";
    string += provenance.author().c_str();
    label = new QLabel(string);
    mainLayout->addWidget(label);

    string = "Comment: ";
    string += provenance.comment().c_str();
    label = new QLabel(string);
    mainLayout->addWidget(label);

    string = "Date & time: ";
    string += provenance.datetime().c_str();
    label = new QLabel(string);
    mainLayout->addWidget(label);

    label = new QLabel();
    mainLayout->addWidget(label);
  }

  if(m_provenances.size() == 0){
    label = new QLabel();
    mainLayout->addWidget(label);
  }

  ///! Tags
  label = new QLabel("Tags");
  label->setObjectName("H1");
  mainLayout->addWidget(label);
  for (const std::string & tag : m_tags) {
    label = new QLabel(tag.c_str());
    mainLayout->addWidget(label);

    label = new QLabel();
    mainLayout->addWidget(label);
  }

  setLayout(mainLayout);
}

void Component::paintEvent(QPaintEvent * event)
{
  QPainter p(this);

  if(isChecked()){
    QLinearGradient linearGrad(QPointF(0,0), QPointF(0,rect().height()));
    linearGrad.setColorAt(0, QColor(113,153,200,255));
    linearGrad.setColorAt(0.10, QColor(113,153,200,150));
    linearGrad.setColorAt(0.15, QColor(210,222,236,150));
    linearGrad.setColorAt(1.00, QColor(210,222,236,150));

    QBrush brush(linearGrad);
    p.setBrush(brush);
    p.setPen(Qt::NoPen);

    p.drawRect(0,0,rect().width(),rect().height() - 1);
  }

  p.setPen(QPen(Qt::black));
  p.drawLine(0,rect().height() - 1,rect().width(),rect().height() - 1);
}

///! SLOTS

} // namespace openstudio
