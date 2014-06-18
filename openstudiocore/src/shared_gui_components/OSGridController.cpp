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

#include "OSGridController.hpp"

#include "OSCheckBox.hpp"
#include "OSComboBox.hpp"
#include "OSDoubleEdit.hpp"
#include "OSGridView.hpp"
#include "OSIntegerEdit.hpp"
#include "OSLineEdit.hpp"
#include "OSQuantityEdit.hpp"
#include "OSUnsignedEdit.hpp"

#include "../openstudio_lib/OSDropZone.hpp"
#include "../openstudio_lib/SchedulesView.hpp"

#include "../model/Model_Impl.hpp"
#include "../model/ModelObject_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <QBoxLayout>
#include <QButtonGroup>
#include <QCheckBox>
#include <QColor>
#include <QSettings>
#include <QWidget>

namespace openstudio {

const std::vector<QColor> OSGridController::m_colors = SchedulesView::initializeColors();

OSGridController::OSGridController()
  : QObject()
{
}

OSGridController::OSGridController(bool isIP,
                                   const QString & headerText,
                                   model::Model model,
                                   std::vector<model::ModelObject> modelObjects)
  : QObject(),
    m_categoriesAndFields(std::vector<std::pair<QString,std::vector<QString> > >()),
    m_baseConcepts(std::vector<QSharedPointer<BaseConcept> >()),
    m_horizontalHeader(std::vector<QWidget *>()),
    m_hasHorizontalHeader(true),
    m_currentCategory(QString()),
    m_currentCategoryIndex(0),
    m_currentFields(std::vector<QString> ()),
    m_customFields(std::vector<QString>()),
    m_model(model),
    m_isIP(isIP),
    m_modelObjects(modelObjects),
    m_horizontalHeaderBtnGrp(nullptr),
    m_headerText(headerText)
{
  loadQSettings();
}

OSGridController::~OSGridController()
{
  saveQSettings();
}

void OSGridController::loadQSettings()
{
  QSettings settings("OpenStudio", m_headerText);
  m_customFields = settings.value("customFields").toStringList().toVector().toStdVector();
}

void OSGridController::saveQSettings() const
{
  QSettings settings("OpenStudio", m_headerText);
  QVector<QVariant> vector;
  for(unsigned i = 0; i < m_customFields.size(); i++){
    QVariant variant = m_customFields.at(i);
    vector.push_back(variant);
  }
  QList<QVariant> list = vector.toList();
  settings.setValue("customCategories", list);
}

void OSGridController::setCategoriesAndFields()
{
  std::vector<QString> fields;
  std::pair<QString,std::vector<QString> > categoryAndFields = std::make_pair(QString("Custom"),fields);
  m_categoriesAndFields.push_back(categoryAndFields);

  setCustomCategoryAndFields();
}

std::vector<QString> OSGridController::categories()
{
  std::vector<QString> categories;

  for(unsigned i = 0; i < m_categoriesAndFields.size(); i++){
    categories.push_back(m_categoriesAndFields.at(i).first);
  }

  return categories;
}

std::vector<std::pair<QString,std::vector<QString> > > OSGridController::categoriesAndFields()
{
  return m_categoriesAndFields;
}

void OSGridController::categorySelected(int index)
{
  m_currentCategoryIndex = index;

  m_currentCategory = m_categoriesAndFields.at(index).first;

  m_currentFields = m_categoriesAndFields.at(index).second;

  addColumns(m_currentFields);

}

void OSGridController::setHorizontalHeader()
{
  m_horizontalHeader.clear();

  if(m_horizontalHeaderBtnGrp == nullptr){
    m_horizontalHeaderBtnGrp = new QButtonGroup();
    m_horizontalHeaderBtnGrp->setExclusive(false);

    bool isConnected = false;
    isConnected = connect(m_horizontalHeaderBtnGrp, SIGNAL(buttonClicked(int)),
      this, SLOT(horizontalHeaderChecked(int)));
    OS_ASSERT(isConnected);

  } else {
    QList<QAbstractButton *> buttons = m_horizontalHeaderBtnGrp->buttons();
    for(int i = 0; i < buttons.size(); i++){
      m_horizontalHeaderBtnGrp->removeButton(buttons.at(i));
      OS_ASSERT(buttons.at(i));
      delete buttons.at(i);
    }
  }

  QList<QAbstractButton *> buttons = m_horizontalHeaderBtnGrp->buttons();
  OS_ASSERT(buttons.size() == 0);

  for (const QString& field : m_currentFields) {
    auto horizontalHeaderWidget = new HorizontalHeaderWidget(field);
    m_horizontalHeaderBtnGrp->addButton(horizontalHeaderWidget->m_checkBox,m_horizontalHeaderBtnGrp->buttons().size());
    m_horizontalHeader.push_back(horizontalHeaderWidget);
  }

  checkSelectedFields();
}

QWidget * OSGridController::widgetAt(int row, int column)
{
  OS_ASSERT(row >= 0);
  OS_ASSERT(column >= 0);
  
  // Note: If there is a horizontal header row,  m_modelObjects[0] starts on gridLayout[1]
  int modelObjectRow = m_hasHorizontalHeader ? row - 1 : row; 
  
  OS_ASSERT(static_cast<int>(m_modelObjects.size()) > modelObjectRow);
  OS_ASSERT(static_cast<int>(m_baseConcepts.size()) > column);

  QWidget * widget = nullptr;

  bool isConnected = false;

  QString cellColor("#FFFFFF"); // white

  if(m_hasHorizontalHeader && row == 0){
    if(column == 0){
      setHorizontalHeader();
      // Each concept should have its own column
      OS_ASSERT(m_horizontalHeader.size() == m_baseConcepts.size());
    }
    widget = m_horizontalHeader.at(column);
  } else {

    model::ModelObject mo = m_modelObjects[modelObjectRow];

    cellColor = getColor(mo);

    QSharedPointer<BaseConcept> baseConcept = m_baseConcepts[column];

    if(QSharedPointer<CheckBoxConcept> checkBoxConcept = baseConcept.dynamicCast<CheckBoxConcept>()){

      auto checkBox = new OSCheckBox2();

      checkBox->bind(mo,
                     std::bind(&CheckBoxConcept::get,checkBoxConcept.data(),mo),
                     boost::optional<BoolSetter>(std::bind(&CheckBoxConcept::set,checkBoxConcept.data(),mo,std::placeholders::_1)));

      widget = checkBox;

    } else if(QSharedPointer<ComboBoxConcept> comboBoxConcept = baseConcept.dynamicCast<ComboBoxConcept>()) {

        auto comboBox = new OSComboBox2();

        comboBox->bind(mo,
                       comboBoxConcept->choiceConcept(mo));

        widget = comboBox;

        isConnected = connect(comboBox, SIGNAL(currentIndexChanged(int)),
          this, SLOT(onComboBoxIndexChanged(int)));
        OS_ASSERT(isConnected);

    } else if(QSharedPointer<ValueEditConcept<double> > doubleEditConcept = baseConcept.dynamicCast<ValueEditConcept<double> >()) {

        auto doubleEdit = new OSDoubleEdit2();

        doubleEdit->bind(mo,
                         DoubleGetter(std::bind(&ValueEditConcept<double>::get,doubleEditConcept.data(),mo)),
                         boost::optional<DoubleSetter>(std::bind(&ValueEditConcept<double>::set,doubleEditConcept.data(),mo,std::placeholders::_1)));

        widget = doubleEdit;

    } else if(QSharedPointer<OptionalValueEditConcept<double> > optionalDoubleEditConcept = baseConcept.dynamicCast<OptionalValueEditConcept<double> >()) {

        auto optionalDoubleEdit = new OSDoubleEdit2();

        optionalDoubleEdit->bind(mo,
                                 OptionalDoubleGetter(std::bind(&OptionalValueEditConcept<double>::get,optionalDoubleEditConcept.data(),mo)),
                                 boost::optional<DoubleSetter>(std::bind(&OptionalValueEditConcept<double>::set,optionalDoubleEditConcept.data(),mo,std::placeholders::_1)));

        widget = optionalDoubleEdit;

    } else if(QSharedPointer<ValueEditVoidReturnConcept<double> > doubleEditVoidReturnConcept = baseConcept.dynamicCast<ValueEditVoidReturnConcept<double> >()) {

        auto doubleEditVoidReturn = new OSDoubleEdit2();

        doubleEditVoidReturn->bind(mo,
                                   DoubleGetter(std::bind(&ValueEditVoidReturnConcept<double>::get,doubleEditVoidReturnConcept.data(),mo)),
                                   DoubleSetterVoidReturn(std::bind(&ValueEditVoidReturnConcept<double>::set,doubleEditVoidReturnConcept.data(),mo,std::placeholders::_1)));

        widget = doubleEditVoidReturn;

    } else if(QSharedPointer<OptionalValueEditVoidReturnConcept<double> > optionalDoubleEditVoidReturnConcept = baseConcept.dynamicCast<OptionalValueEditVoidReturnConcept<double> >()) {

        auto optionalDoubleEditVoidReturn = new OSDoubleEdit2();

        optionalDoubleEditVoidReturn->bind(mo,
                                           OptionalDoubleGetter(std::bind(&OptionalValueEditVoidReturnConcept<double>::get,optionalDoubleEditVoidReturnConcept.data(),mo)),
                                           DoubleSetterVoidReturn(std::bind(&OptionalValueEditVoidReturnConcept<double>::set,optionalDoubleEditVoidReturnConcept.data(),mo,std::placeholders::_1)));

        widget = optionalDoubleEditVoidReturn;

    } else if(QSharedPointer<ValueEditConcept<int> > integerEditConcept = baseConcept.dynamicCast<ValueEditConcept<int> >()) {

        auto integerEdit = new OSIntegerEdit2();

        integerEdit->bind(mo,
                          IntGetter(std::bind(&ValueEditConcept<int>::get,integerEditConcept.data(),mo)),
                          boost::optional<IntSetter>(std::bind(&ValueEditConcept<int>::set,integerEditConcept.data(),mo,std::placeholders::_1)));

        widget = integerEdit;

    } else if(QSharedPointer<ValueEditConcept<std::string> > lineEditConcept = baseConcept.dynamicCast<ValueEditConcept<std::string> >()) {

        auto lineEdit = new OSLineEdit2();

        lineEdit->bind(mo,
                       StringGetter(std::bind(&ValueEditConcept<std::string>::get,lineEditConcept.data(),mo)),
                       boost::optional<StringSetter>(std::bind(&ValueEditConcept<std::string>::set,lineEditConcept.data(),mo,std::placeholders::_1)));

        widget = lineEdit;

    } else if(QSharedPointer<NameLineEditConcept> nameLineEditConcept = baseConcept.dynamicCast<NameLineEditConcept>()) {

        auto nameLineEdit = new OSLineEdit2();

        nameLineEdit->bind(mo,
                           OptionalStringGetter(std::bind(&NameLineEditConcept::get,nameLineEditConcept.data(),mo,true)),
                           boost::optional<StringSetter>(std::bind(&NameLineEditConcept::set,nameLineEditConcept.data(),mo,std::placeholders::_1)));

        widget = nameLineEdit;

    } else if(QSharedPointer<QuantityEditConcept<double> > quantityEditConcept = baseConcept.dynamicCast<QuantityEditConcept<double> >()) {

        OSQuantityEdit2 * quantityEdit = new OSQuantityEdit2(
              quantityEditConcept->modelUnits().toStdString().c_str(),
              quantityEditConcept->siUnits().toStdString().c_str(),
              quantityEditConcept->ipUnits().toStdString().c_str(),
              quantityEditConcept->isIP());

        quantityEdit->bind(m_isIP,
                           mo,
                           DoubleGetter(std::bind(&QuantityEditConcept<double>::get,quantityEditConcept.data(),mo)),
                           boost::optional<DoubleSetter>(std::bind(&QuantityEditConcept<double>::set,quantityEditConcept.data(),mo,std::placeholders::_1)));

        isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
          quantityEdit, SLOT(onUnitSystemChange(bool)));
        OS_ASSERT(isConnected);

        widget = quantityEdit;

    } else if(QSharedPointer<OptionalQuantityEditConcept<double> > optionalQuantityEditConcept = baseConcept.dynamicCast<OptionalQuantityEditConcept<double> >()) {

        OSQuantityEdit2 * optionalQuantityEdit = new OSQuantityEdit2(
              optionalQuantityEditConcept->modelUnits().toStdString().c_str(),
              optionalQuantityEditConcept->siUnits().toStdString().c_str(),
              optionalQuantityEditConcept->ipUnits().toStdString().c_str(),
              optionalQuantityEditConcept->isIP());

        optionalQuantityEdit->bind(m_isIP,
                                   mo,
                                   OptionalDoubleGetter(std::bind(&OptionalQuantityEditConcept<double>::get,optionalQuantityEditConcept.data(),mo)),
                                   boost::optional<DoubleSetter>(std::bind(&OptionalQuantityEditConcept<double>::set,optionalQuantityEditConcept.data(),mo,std::placeholders::_1)));

        isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
          optionalQuantityEdit, SLOT(onUnitSystemChange(bool)));
        OS_ASSERT(isConnected);

        widget = optionalQuantityEdit;

    } else if(QSharedPointer<QuantityEditVoidReturnConcept<double> > quantityEditVoidReturnConcept = baseConcept.dynamicCast<QuantityEditVoidReturnConcept<double> >()) {

        OSQuantityEdit2 * quantityEditVoidReturn = new OSQuantityEdit2(
              quantityEditVoidReturnConcept->modelUnits().toStdString().c_str(),
              quantityEditVoidReturnConcept->siUnits().toStdString().c_str(),
              quantityEditVoidReturnConcept->ipUnits().toStdString().c_str(),
              quantityEditVoidReturnConcept->isIP());

        quantityEditVoidReturn->bind(m_isIP,
                                     mo,
                                     DoubleGetter(std::bind(&QuantityEditVoidReturnConcept<double>::get,quantityEditVoidReturnConcept.data(),mo)),
                                     DoubleSetterVoidReturn(std::bind(&QuantityEditVoidReturnConcept<double>::set,quantityEditVoidReturnConcept.data(),mo,std::placeholders::_1)));

        isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
          quantityEditVoidReturn, SLOT(onUnitSystemChange(bool)));
        OS_ASSERT(isConnected);

        widget = quantityEditVoidReturn;

    } else if(QSharedPointer<OptionalQuantityEditVoidReturnConcept<double> > optionalQuantityEditVoidReturnConcept = baseConcept.dynamicCast<OptionalQuantityEditVoidReturnConcept<double> >()) {

        OSQuantityEdit2 * optionalQuantityEditVoidReturn = new OSQuantityEdit2(
              optionalQuantityEditVoidReturnConcept->modelUnits().toStdString().c_str(),
              optionalQuantityEditVoidReturnConcept->siUnits().toStdString().c_str(),
              optionalQuantityEditVoidReturnConcept->ipUnits().toStdString().c_str(),
              optionalQuantityEditVoidReturnConcept->isIP());

        optionalQuantityEditVoidReturn->bind(m_isIP,
                                             mo,
                                             OptionalDoubleGetter(std::bind(&OptionalQuantityEditVoidReturnConcept<double>::get,optionalQuantityEditVoidReturnConcept.data(),mo)),
                                             DoubleSetterVoidReturn(std::bind(&OptionalQuantityEditVoidReturnConcept<double>::set,optionalQuantityEditVoidReturnConcept.data(),mo,std::placeholders::_1)));

        isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
          optionalQuantityEditVoidReturn, SLOT(onUnitSystemChange(bool)));
        OS_ASSERT(isConnected);

        widget = optionalQuantityEditVoidReturn;

    } else if(QSharedPointer<ValueEditConcept<unsigned> > unsignedEditConcept = baseConcept.dynamicCast<ValueEditConcept<unsigned> >()) {

        auto unsignedEdit = new OSUnsignedEdit2();

        unsignedEdit->bind(mo,
                           UnsignedGetter(std::bind(&ValueEditConcept<unsigned>::get,unsignedEditConcept.data(),mo)),
                           boost::optional<UnsignedSetter>(std::bind(&ValueEditConcept<unsigned>::set,unsignedEditConcept.data(),mo,std::placeholders::_1)));

        widget = unsignedEdit;

    } else if(QSharedPointer<DropZoneConcept> dropZoneConcept = baseConcept.dynamicCast<DropZoneConcept>()) {
        // Note: OSDropZone2 bind not fully implemented
        OS_ASSERT(false);
        //GridViewDropZoneVectorController * vectorController = new GridViewDropZoneVectorController();
        //OSDropZone2 * dropZone = new OSDropZone2(vectorController);

        //dropZone->bind(mo,
        //          std::bind(&DropZoneConcept::get,dropZoneConcept.data(),mo),
        //          boost::optional<ModelObjectSetter>(std::bind(&DropZoneConcept::set,dropZoneConcept.data(),mo,std::placeholders::_1)),
        //          boost::none,
        //          boost::none);

        //widget = dropZone;
    } else {
      // Unknown type
      OS_ASSERT(false);
    }
  }

  auto wrapper = new QWidget();
  wrapper->setObjectName("TableCell");
  if(row == 0){
    wrapper->setMinimumSize(QSize(140,50));
  } else {
    wrapper->setMinimumSize(QSize(140,34));
  }

  QString style;
  style.append("QWidget#TableCell {");
  style.append("  background-color: ");
  style.append(cellColor);
  style.append(";");
  if(row == 0){
    style.append("  border-top: 1px solid black;");
  }
  if(column == 0){
    style.append("  border-left: 1px solid black;");
  }
  style.append("  border-right: 1px solid black;");
  style.append("  border-bottom: 1px solid black;");
  style.append("}");
  wrapper->setStyleSheet(style);

  auto layout = new QVBoxLayout();
  layout->setSpacing(0);
  if(row == 0){
    layout->setContentsMargins(0,0,0,0);
  } else {
    layout->setContentsMargins(5,5,5,5);
  }
  layout->addWidget(widget,0,Qt::AlignTop | Qt::AlignCenter);
  wrapper->setLayout(layout);

  return wrapper;
}

void OSGridController::checkSelectedFields()
{
  if(!this->m_hasHorizontalHeader) return;

  std::vector<QString>::iterator it;
  for(unsigned j = 0; j < m_customFields.size(); j++){
    it = std::find(m_currentFields.begin(), m_currentFields.end() ,m_customFields.at(j));
    if( it != m_currentFields.end() ){
      int index = std::distance(m_currentFields.begin(), it);
      HorizontalHeaderWidget * horizontalHeaderWidget = qobject_cast<HorizontalHeaderWidget *>(m_horizontalHeader.at(index));
      OS_ASSERT(horizontalHeaderWidget);
      horizontalHeaderWidget->m_checkBox->blockSignals(true);
      horizontalHeaderWidget->m_checkBox->setChecked(true);
      horizontalHeaderWidget->m_checkBox->blockSignals(false);
    }
  }

}

void OSGridController::setCustomCategoryAndFields()
{
  std::vector<QString> categories = this->categories();
  std::vector<QString>::iterator it;
  it = std::find(categories.begin(), categories.end() , QString("Custom"));
  if( it != categories.end() ){
    int index = std::distance(categories.begin(), it);
    m_categoriesAndFields.erase(m_categoriesAndFields.begin() + index);
  }

  std::pair<QString,std::vector<QString> > categoryAndFields = std::make_pair(QString("Custom"),m_customFields);
  m_categoriesAndFields.push_back(categoryAndFields);
}

int OSGridController::rowCount() const
{
  if(m_hasHorizontalHeader){
    return m_modelObjects.size() + 1;
  } else {
    return m_modelObjects.size();
  }
}

int OSGridController::columnCount() const
{
  return m_baseConcepts.size();
}

std::vector<QWidget *> OSGridController::row(int i)
{
  return std::vector<QWidget *>();
}

void OSGridController::horizontalHeaderChecked(int index)
{
  QCheckBox * checkBox = qobject_cast<QCheckBox *>(m_horizontalHeaderBtnGrp->button(index));
  OS_ASSERT(checkBox);
  if(checkBox->isChecked()){
    m_customFields.push_back(m_currentFields.at(index));
  } else {
    std::vector<QString>::iterator it;
    it = std::find(m_customFields.begin(), m_customFields.end(), m_currentFields.at(index));
    if( it != m_customFields.end() ){
      m_customFields.erase(it);
    }
  }
  setCustomCategoryAndFields();
}

void OSGridController::toggleUnits(bool displayIP)
{
  m_isIP = displayIP;
}

void OSGridController::onComboBoxIndexChanged(int index)
{
}

void OSGridController::reset()
{
  emit modelReset();
}

HorizontalHeaderWidget::HorizontalHeaderWidget(const QString & fieldName, QWidget * parent)
  : QWidget(parent),
  m_label(new QLabel(fieldName)),
  m_checkBox(new QCheckBox())
{
  auto layout = new QHBoxLayout();
  setLayout(layout);

  m_label->setWordWrap(true);
  layout->addWidget(m_label);

  layout->addWidget(m_checkBox);

}

BulkSelectionWidget::BulkSelectionWidget(QWidget * parent)
  : QWidget(parent),
  m_comboBox(new OSComboBox()),
  m_checkBox(new QCheckBox())
{
  auto layout = new QHBoxLayout();
  setLayout(layout);

  layout->addWidget(m_checkBox);

  layout->addWidget(m_comboBox);

}

} // openstudio

