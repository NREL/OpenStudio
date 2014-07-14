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

QWidget * OSGridController::makeWidget(model::ModelObject t_mo, const QSharedPointer<BaseConcept> &t_baseConcept)
{
  QWidget *widget = nullptr;
  bool isConnected = false;

  if(QSharedPointer<CheckBoxConcept> checkBoxConcept = t_baseConcept.dynamicCast<CheckBoxConcept>()){

    auto checkBox = new OSCheckBox2();

    checkBox->bind(t_mo,
                   BoolGetter(std::bind(&CheckBoxConcept::get,checkBoxConcept.data(),t_mo)),
                                       boost::optional<BoolSetter>(std::bind(&CheckBoxConcept::set, checkBoxConcept.data(), t_mo, std::placeholders::_1)));


    widget = checkBox;

  } else if(QSharedPointer<ComboBoxConcept> comboBoxConcept = t_baseConcept.dynamicCast<ComboBoxConcept>()) {

    auto comboBox = new OSComboBox2();

    comboBox->bind(t_mo,
                   comboBoxConcept->choiceConcept(t_mo));

    widget = comboBox;

    isConnected = connect(comboBox, SIGNAL(currentIndexChanged(int)),
      this, SLOT(onComboBoxIndexChanged(int)));
    OS_ASSERT(isConnected);

  } else if(QSharedPointer<ValueEditConcept<double> > doubleEditConcept = t_baseConcept.dynamicCast<ValueEditConcept<double> >()) {

    auto doubleEdit = new OSDoubleEdit2();

    doubleEdit->bind(t_mo,
                     DoubleGetter(std::bind(&ValueEditConcept<double>::get,doubleEditConcept.data(),t_mo)),
                     boost::optional<DoubleSetter>(std::bind(&ValueEditConcept<double>::set,doubleEditConcept.data(),t_mo,std::placeholders::_1)));

    widget = doubleEdit;

  } else if(QSharedPointer<OptionalValueEditConcept<double> > optionalDoubleEditConcept = t_baseConcept.dynamicCast<OptionalValueEditConcept<double> >()) {

    auto optionalDoubleEdit = new OSDoubleEdit2();

    optionalDoubleEdit->bind(t_mo,
                             OptionalDoubleGetter(std::bind(&OptionalValueEditConcept<double>::get,optionalDoubleEditConcept.data(),t_mo)),
                             boost::optional<DoubleSetter>(std::bind(&OptionalValueEditConcept<double>::set,optionalDoubleEditConcept.data(),t_mo,std::placeholders::_1)));

    widget = optionalDoubleEdit;

  } else if(QSharedPointer<ValueEditVoidReturnConcept<double> > doubleEditVoidReturnConcept = t_baseConcept.dynamicCast<ValueEditVoidReturnConcept<double> >()) {

    auto doubleEditVoidReturn = new OSDoubleEdit2();

    doubleEditVoidReturn->bind(t_mo,
                               DoubleGetter(std::bind(&ValueEditVoidReturnConcept<double>::get,doubleEditVoidReturnConcept.data(),t_mo)),
                               DoubleSetterVoidReturn(std::bind(&ValueEditVoidReturnConcept<double>::set,doubleEditVoidReturnConcept.data(),t_mo,std::placeholders::_1)));

    widget = doubleEditVoidReturn;

  } else if(QSharedPointer<OptionalValueEditVoidReturnConcept<double> > optionalDoubleEditVoidReturnConcept = t_baseConcept.dynamicCast<OptionalValueEditVoidReturnConcept<double> >()) {

    auto optionalDoubleEditVoidReturn = new OSDoubleEdit2();

    optionalDoubleEditVoidReturn->bind(t_mo,
                                       OptionalDoubleGetter(std::bind(&OptionalValueEditVoidReturnConcept<double>::get,optionalDoubleEditVoidReturnConcept.data(),t_mo)),
                                       DoubleSetterVoidReturn(std::bind(&OptionalValueEditVoidReturnConcept<double>::set,optionalDoubleEditVoidReturnConcept.data(),t_mo,std::placeholders::_1)));

    widget = optionalDoubleEditVoidReturn;

  } else if(QSharedPointer<ValueEditConcept<int> > integerEditConcept = t_baseConcept.dynamicCast<ValueEditConcept<int> >()) {

    auto integerEdit = new OSIntegerEdit2();

    integerEdit->bind(t_mo,
                      IntGetter(std::bind(&ValueEditConcept<int>::get,integerEditConcept.data(),t_mo)),
                      boost::optional<IntSetter>(std::bind(&ValueEditConcept<int>::set,integerEditConcept.data(),t_mo,std::placeholders::_1)));

    widget = integerEdit;

  } else if(QSharedPointer<ValueEditConcept<std::string> > lineEditConcept = t_baseConcept.dynamicCast<ValueEditConcept<std::string> >()) {

    auto lineEdit = new OSLineEdit2();

    lineEdit->bind(t_mo,
                   StringGetter(std::bind(&ValueEditConcept<std::string>::get,lineEditConcept.data(),t_mo)),
                   boost::optional<StringSetter>(std::bind(&ValueEditConcept<std::string>::set,lineEditConcept.data(),t_mo,std::placeholders::_1)));

    widget = lineEdit;

  } else if(QSharedPointer<NameLineEditConcept> nameLineEditConcept = t_baseConcept.dynamicCast<NameLineEditConcept>()) {

    auto nameLineEdit = new OSLineEdit2();

    nameLineEdit->bind(t_mo,
                       OptionalStringGetter(std::bind(&NameLineEditConcept::get,nameLineEditConcept.data(),t_mo,true)),
                       // If the concept is read only, pass an empty optional
                       nameLineEditConcept->readOnly() ? boost::none : boost::optional<StringSetter>(std::bind(&NameLineEditConcept::set,nameLineEditConcept.data(),t_mo,std::placeholders::_1)));

    widget = nameLineEdit;

  } else if(QSharedPointer<QuantityEditConcept<double> > quantityEditConcept = t_baseConcept.dynamicCast<QuantityEditConcept<double> >()) {

    OSQuantityEdit2 * quantityEdit = new OSQuantityEdit2(
          quantityEditConcept->modelUnits().toStdString().c_str(),
          quantityEditConcept->siUnits().toStdString().c_str(),
          quantityEditConcept->ipUnits().toStdString().c_str(),
          quantityEditConcept->isIP());

    quantityEdit->bind(m_isIP,
                       t_mo,
                       DoubleGetter(std::bind(&QuantityEditConcept<double>::get,quantityEditConcept.data(),t_mo)),
                       boost::optional<DoubleSetter>(std::bind(&QuantityEditConcept<double>::set,quantityEditConcept.data(),t_mo,std::placeholders::_1)));

    isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
      quantityEdit, SLOT(onUnitSystemChange(bool)));
    OS_ASSERT(isConnected);

    widget = quantityEdit;

  } else if(QSharedPointer<OptionalQuantityEditConcept<double> > optionalQuantityEditConcept = t_baseConcept.dynamicCast<OptionalQuantityEditConcept<double> >()) {

    OSQuantityEdit2 * optionalQuantityEdit = new OSQuantityEdit2(
          optionalQuantityEditConcept->modelUnits().toStdString().c_str(),
          optionalQuantityEditConcept->siUnits().toStdString().c_str(),
          optionalQuantityEditConcept->ipUnits().toStdString().c_str(),
          optionalQuantityEditConcept->isIP());

    optionalQuantityEdit->bind(m_isIP,
                               t_mo,
                               OptionalDoubleGetter(std::bind(&OptionalQuantityEditConcept<double>::get,optionalQuantityEditConcept.data(),t_mo)),
                               boost::optional<DoubleSetter>(std::bind(&OptionalQuantityEditConcept<double>::set,optionalQuantityEditConcept.data(),t_mo,std::placeholders::_1)));

    isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
      optionalQuantityEdit, SLOT(onUnitSystemChange(bool)));
    OS_ASSERT(isConnected);

    widget = optionalQuantityEdit;

  } else if(QSharedPointer<QuantityEditVoidReturnConcept<double> > quantityEditVoidReturnConcept = t_baseConcept.dynamicCast<QuantityEditVoidReturnConcept<double> >()) {

    OSQuantityEdit2 * quantityEditVoidReturn = new OSQuantityEdit2(
          quantityEditVoidReturnConcept->modelUnits().toStdString().c_str(),
          quantityEditVoidReturnConcept->siUnits().toStdString().c_str(),
          quantityEditVoidReturnConcept->ipUnits().toStdString().c_str(),
          quantityEditVoidReturnConcept->isIP());

    quantityEditVoidReturn->bind(m_isIP,
                                 t_mo,
                                 DoubleGetter(std::bind(&QuantityEditVoidReturnConcept<double>::get,quantityEditVoidReturnConcept.data(),t_mo)),
                                 DoubleSetterVoidReturn(std::bind(&QuantityEditVoidReturnConcept<double>::set,quantityEditVoidReturnConcept.data(),t_mo,std::placeholders::_1)));

    isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
      quantityEditVoidReturn, SLOT(onUnitSystemChange(bool)));
    OS_ASSERT(isConnected);

    widget = quantityEditVoidReturn;

  } else if(QSharedPointer<OptionalQuantityEditVoidReturnConcept<double> > optionalQuantityEditVoidReturnConcept = t_baseConcept.dynamicCast<OptionalQuantityEditVoidReturnConcept<double> >()) {

    OSQuantityEdit2 * optionalQuantityEditVoidReturn = new OSQuantityEdit2(
          optionalQuantityEditVoidReturnConcept->modelUnits().toStdString().c_str(),
          optionalQuantityEditVoidReturnConcept->siUnits().toStdString().c_str(),
          optionalQuantityEditVoidReturnConcept->ipUnits().toStdString().c_str(),
          optionalQuantityEditVoidReturnConcept->isIP());

    optionalQuantityEditVoidReturn->bind(m_isIP,
                                         t_mo,
                                         OptionalDoubleGetter(std::bind(&OptionalQuantityEditVoidReturnConcept<double>::get,optionalQuantityEditVoidReturnConcept.data(),t_mo)),
                                         DoubleSetterVoidReturn(std::bind(&OptionalQuantityEditVoidReturnConcept<double>::set,optionalQuantityEditVoidReturnConcept.data(),t_mo,std::placeholders::_1)));

    isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
      optionalQuantityEditVoidReturn, SLOT(onUnitSystemChange(bool)));
    OS_ASSERT(isConnected);

    widget = optionalQuantityEditVoidReturn;

  } else if(QSharedPointer<ValueEditConcept<unsigned> > unsignedEditConcept = t_baseConcept.dynamicCast<ValueEditConcept<unsigned> >()) {

    auto unsignedEdit = new OSUnsignedEdit2();

    unsignedEdit->bind(t_mo,
                       UnsignedGetter(std::bind(&ValueEditConcept<unsigned>::get,unsignedEditConcept.data(),t_mo)),
                       boost::optional<UnsignedSetter>(std::bind(&ValueEditConcept<unsigned>::set,unsignedEditConcept.data(),t_mo,std::placeholders::_1)));

    widget = unsignedEdit;

  } else if(QSharedPointer<DropZoneConcept<model::ModelObject> > dropZoneConcept = t_baseConcept.dynamicCast<DropZoneConcept<model::ModelObject> >()) {
    GridViewDropZoneVectorController * vectorController = new GridViewDropZoneVectorController();
    OSDropZone2 * dropZone = new OSDropZone2(vectorController);

    dropZone->bind(t_mo,
                   ModelObjectGetter(std::bind(&DropZoneConcept<model::ModelObject>::get,dropZoneConcept.data(),t_mo)),
                                       boost::optional<ModelObjectSetter>(std::bind(&DropZoneConcept<model::ModelObject>::set, dropZoneConcept.data(), t_mo, std::placeholders::_1)),
                   boost::none,
                   boost::none);

    widget = dropZone;
  } else {
    // Unknown type
    OS_ASSERT(false);
  }

  return widget;
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


  QString cellColor("#FFFFFF"); // white // TODO add alternate row colors here

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

    if (QSharedPointer<DataSourceAdapter> dataSource = baseConcept.dynamicCast<DataSourceAdapter>()) {
      // here we magically create a mutirow column of any type that was constructed

      auto layout = new QVBoxLayout();

      // we have a data source that provides multiple rows.
      for (auto &item : dataSource->source().items(mo))
      {
        layout->addWidget(makeWidget(item.cast<model::ModelObject>(), dataSource->innerConcept()));
      }

      if (dataSource->source().wantsDropZone())
      {
        // this needs to become an actual drop zone widge
        // and how this drop zone widget is hooked up is another question I guess too
        // probably some other data that needs to be attached to the DataSource object
        // that was passed in
        layout->addWidget(new QWidget());
      } else {
        // this needs to actually be a just a blank space holder
        // ... you can Modify DataSource class to have more bits of info
        // for how you need to lay this out
        layout->addWidget(new QWidget());
      }

      // right here you probably want some kind of container that's smart enough to know how to grow
      // and shrink as the contained items change. But I don't know enough about the model
      // to know how you'd want to do that. For now we make a fixed list that's got a VBoxLayout
      widget = new QWidget();
      widget->setLayout(layout);


    } else {
      // just the one
      widget = makeWidget(mo, baseConcept);
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
  // If there is a header row, investigate which columns were previously checked 
  // (and loaded into m_customFields by QSettings) and check the respective
  // header widgets
  if(!this->m_hasHorizontalHeader) return;

  std::vector<QString>::iterator it;
  for(unsigned j = 0; j < m_customFields.size(); j++){
    it = std::find(m_currentFields.begin(), m_currentFields.end(), m_customFields.at(j));
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
  // First, find and erase the old fields for custom
  std::vector<QString> categories = this->categories();
  std::vector<QString>::iterator it;
  it = std::find(categories.begin(), categories.end() , QString("Custom"));
  if( it != categories.end() ){
    int index = std::distance(categories.begin(), it);
    m_categoriesAndFields.erase(m_categoriesAndFields.begin() + index);
  }

  // Make a new set of fields for custom
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
  // Push_back or erase the field from the user-selected fields
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

  // Update the user-selected fields
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
  m_label->setAlignment(Qt::AlignCenter);
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

