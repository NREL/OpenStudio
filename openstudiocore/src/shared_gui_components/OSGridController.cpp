/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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

#include <shared_gui_components/OSGridController.hpp>

#include <shared_gui_components/OSCheckBox.hpp>
#include <shared_gui_components/OSComboBox.hpp>
#include <shared_gui_components/OSDoubleEdit.hpp>
#include <shared_gui_components/OSGridView.hpp>
#include <shared_gui_components/OSIntegerEdit.hpp>
#include <shared_gui_components/OSLineEdit.hpp>
#include <shared_gui_components/OSQuantityEdit.hpp>
#include <shared_gui_components/OSUnsignedEdit.hpp>

#include <openstudio_lib/OSDropZone.hpp>
#include <openstudio_lib/OSVectorController.hpp>
#include <openstudio_lib/SchedulesView.hpp>

#include <model/Model_Impl.hpp>
#include <model/ModelObject_Impl.hpp>

#include <utilities/core/Assert.hpp>

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

OSGridController::OSGridController(const QString & headerText,
  IddObjectType iddObjectType,
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
  m_model(model),
  m_modelObjects(modelObjects),
  m_iddObjectType(iddObjectType),
  m_horizontalHeaderBtnGrp(0),
  m_verticalHeaderBtnGrp(0),
  m_customCategories(std::vector<QString>()),
  m_vectorController(0),
  m_headerText(headerText)
{
  m_verticalHeaderBtnGrp = new QButtonGroup();
  m_verticalHeaderBtnGrp->setExclusive(false);

  loadQSettings();
}

OSGridController::~OSGridController()
{
  saveQSettings();
}

void OSGridController::loadQSettings()
{
  QSettings settings("OpenStudio", m_headerText);
  m_customCategories = settings.value("customCategories").toStringList().toVector().toStdVector();
}

void OSGridController::saveQSettings() const
{
  QSettings settings("OpenStudio", m_headerText);
  QVector<QVariant> vector;
  for(unsigned i = 0; i < m_customCategories.size(); i++){
    QVariant variant = m_customCategories.at(i);
    vector.push_back(variant);
  }
  QList<QVariant> list = vector.toList();
  settings.setValue("customCategories", list);
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

  if(m_horizontalHeaderBtnGrp == 0){
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

  HorizontalHeaderWidget * horizontalHeaderWidget = 0;
  Q_FOREACH(QString field, m_currentFields){
    horizontalHeaderWidget = new HorizontalHeaderWidget(field);
    m_horizontalHeaderBtnGrp->addButton(horizontalHeaderWidget->m_checkBox,m_horizontalHeaderBtnGrp->buttons().size());
    m_horizontalHeader.push_back(horizontalHeaderWidget);
  }

  checkSelectedFields();
}

QWidget * OSGridController::widgetAt(int row, int column)
{
  OS_ASSERT(row >= 0);
  OS_ASSERT(column >= 0);

  OS_ASSERT(m_modelObjects.size() > static_cast<unsigned>(row));
  OS_ASSERT(m_baseConcepts.size() > static_cast<unsigned>(column));    

  QWidget * widget = 0;

  // Note: If there is a header row,  modelObject[0] starts on gridLayout[1]
  int modelObjectRow = m_hasHorizontalHeader ? row - 1 : row;

  if(m_hasHorizontalHeader && row == 0){
    if(column == 0){
      setHorizontalHeader();
      // Each concept should have its own column
      //OS_ASSERT(m_horizontalHeader.size() == m_baseConcepts.size()); TODO uncomment this later, once all types are supported
    }
    widget = m_horizontalHeader.at(column);
  } else {

    model::ModelObject mo = m_modelObjects[modelObjectRow];

    QSharedPointer<BaseConcept> baseConcept = m_baseConcepts[column];

    if(QSharedPointer<CheckBoxConcept> checkBoxConcept = baseConcept.dynamicCast<CheckBoxConcept>()){

      OSCheckBox2 * checkBox = new OSCheckBox2();

      checkBox->bind(mo,
                boost::bind(&CheckBoxConcept::get,checkBoxConcept.data(),mo),
                boost::optional<BoolSetter>(boost::bind(&CheckBoxConcept::set,checkBoxConcept.data(),mo,_1)),
                boost::none,
                boost::none);

      widget = checkBox;

    } else if(QSharedPointer<ComboBoxConcept> comboBoxConcept = baseConcept.dynamicCast<ComboBoxConcept>()) {

        OSComboBox2 * comboBox = new OSComboBox2();

        comboBox->bindRequired(mo,
                  boost::bind(&ComboBoxConcept::choices,comboBoxConcept.data()),
                  boost::bind(&ComboBoxConcept::get,comboBoxConcept.data(),mo),
                  boost::optional<StringSetter>(boost::bind(&ComboBoxConcept::set,comboBoxConcept.data(),mo,_1)),
                  boost::none,
                  boost::none);

        widget = comboBox;

    } else if(QSharedPointer<DoubleEditConcept> doubleEditConcept = baseConcept.dynamicCast<DoubleEditConcept>()) {

        OSDoubleEdit2 * doubleEdit = new OSDoubleEdit2();

        doubleEdit->bindRequired(mo,
                  boost::bind(&DoubleEditConcept::get,doubleEditConcept.data(),mo),
                  boost::optional<DoubleSetter>(boost::bind(&DoubleEditConcept::set,doubleEditConcept.data(),mo,_1)),
                  boost::none,
                  boost::none,
                  boost::none,
                  boost::none,
                  boost::none,
                  boost::none);

        widget = doubleEdit;

    } else if(QSharedPointer<OptionalDoubleEditConcept> optionalDoubleEditConcept = baseConcept.dynamicCast<OptionalDoubleEditConcept>()) {

        OSDoubleEdit2 * optionalDoubleEdit = new OSDoubleEdit2();

        optionalDoubleEdit->bind(mo,
                  boost::bind(&OptionalDoubleEditConcept::get,optionalDoubleEditConcept.data(),mo),
                  boost::optional<DoubleSetter>(boost::bind(&OptionalDoubleEditConcept::set,optionalDoubleEditConcept.data(),mo,_1)),
                  boost::none,
                  boost::none,
                  boost::none,
                  boost::none,
                  boost::none,
                  boost::none);

        widget = optionalDoubleEdit;

    } else if(QSharedPointer<DoubleEditVoidReturnConcept> doubleEditVoidReturnConcept = baseConcept.dynamicCast<DoubleEditVoidReturnConcept>()) {

        OSDoubleEdit2 * doubleEditVoidReturn = new OSDoubleEdit2();

        doubleEditVoidReturn->bindRequiredVoidReturn(mo,
                  boost::bind(&DoubleEditVoidReturnConcept::get,doubleEditVoidReturnConcept.data(),mo),
                  boost::optional<DoubleSetterVoidReturn>(boost::bind(&DoubleEditVoidReturnConcept::set,doubleEditVoidReturnConcept.data(),mo,_1)),
                  boost::none,
                  boost::none,
                  boost::none,
                  boost::none,
                  boost::none,
                  boost::none);

        widget = doubleEditVoidReturn;

    } else if(QSharedPointer<OptionalDoubleEditVoidReturnConcept> optionalDoubleEditVoidReturnConcept = baseConcept.dynamicCast<OptionalDoubleEditVoidReturnConcept>()) {

        OSDoubleEdit2 * optionalDoubleEditVoidReturn = new OSDoubleEdit2();

        optionalDoubleEditVoidReturn->bindVoidReturn(mo,
                  boost::bind(&OptionalDoubleEditVoidReturnConcept::get,optionalDoubleEditVoidReturnConcept.data(),mo),
                  boost::optional<DoubleSetterVoidReturn>(boost::bind(&OptionalDoubleEditVoidReturnConcept::set,optionalDoubleEditVoidReturnConcept.data(),mo,_1)),
                  boost::none,
                  boost::none,
                  boost::none,
                  boost::none,
                  boost::none,
                  boost::none);

        widget = optionalDoubleEditVoidReturn;

    } else if(QSharedPointer<IntegerEditConcept> integerEditConcept = baseConcept.dynamicCast<IntegerEditConcept>()) {

        OSIntegerEdit2 * integerEdit = new OSIntegerEdit2();

        integerEdit->bindRequired(mo,
                  boost::bind(&IntegerEditConcept::get,integerEditConcept.data(),mo),
                  boost::optional<IntSetter>(boost::bind(&IntegerEditConcept::set,integerEditConcept.data(),mo,_1)),
                  boost::none,
                  boost::none,
                  boost::none,
                  boost::none,
                  boost::none,
                  boost::none);

        widget = integerEdit;

    } else if(QSharedPointer<LineEditConcept> lineEditConcept = baseConcept.dynamicCast<LineEditConcept>()) {

        OSLineEdit2 * lineEdit = new OSLineEdit2();

        lineEdit->bindRequired(mo,
                  boost::bind(&LineEditConcept::get,lineEditConcept.data(),mo),
                  boost::optional<StringSetter>(boost::bind(&LineEditConcept::set,lineEditConcept.data(),mo,_1)),
                  boost::none,
                  boost::none);

        widget = lineEdit;

    } else if(QSharedPointer<QuantityEditConcept> quantityEditConcept = baseConcept.dynamicCast<QuantityEditConcept>()) {

        OSQuantityEdit2 * quantityEdit = new OSQuantityEdit2("people/m^2", "people/m^2", "people/ft^2", true);

        quantityEdit->bindRequired(true,
                  mo,
                  boost::bind(&QuantityEditConcept::get,quantityEditConcept.data(),mo),
                  boost::optional<DoubleSetter>(boost::bind(&QuantityEditConcept::set,quantityEditConcept.data(),mo,_1)),
                  boost::none,
                  boost::none,
                  boost::none,
                  boost::none,
                  boost::none,
                  boost::none);

        widget = quantityEdit;

    } else if(QSharedPointer<UnsignedEditConcept> unsignedEditConcept = baseConcept.dynamicCast<UnsignedEditConcept>()) {

        OSUnsignedEdit2 * unsignedEdit = new OSUnsignedEdit2();

        unsignedEdit->bind(mo,
                  boost::bind(&UnsignedEditConcept::get,unsignedEditConcept.data(),mo),
                  boost::optional<UnsignedSetter>(boost::bind(&UnsignedEditConcept::set,unsignedEditConcept.data(),mo,_1)),
                  boost::none,
                  boost::none,
                  boost::none,
                  boost::none,
                  boost::none,
                  boost::none);

        widget = unsignedEdit;
    } else if(QSharedPointer<DropZoneConcept> dropZoneConcept = baseConcept.dynamicCast<DropZoneConcept>()) {

        OSDropZone2 * dropZone = new OSDropZone2(this->m_vectorController);

        dropZone->bind(mo,
                  boost::bind(&DropZoneConcept::get,dropZoneConcept.data(),mo),
                  boost::optional<ModelObjectSetter>(boost::bind(&DropZoneConcept::set,dropZoneConcept.data(),mo,_1)),
                  boost::none,
                  boost::none);

        widget = dropZone;
    } else {
      // Unknown type
      OS_ASSERT(false);
    }
  }

  QWidget * wrapper = new QWidget();
  wrapper->setFixedSize(QSize(200,70));
  wrapper->setObjectName("TableCell");

  QString color;
  if(modelObjectRow >= static_cast<int>(m_colors.size())) row = m_colors.size() - 1; // similar to scheduleView's approach
  if(column < 2 && row > 0){
    color = this->m_colors.at(modelObjectRow).name();
  } else {
    color = "#FFFFFF"; // white
  }

  QString style;
  style.append("QWidget#TableCell {");
  style.append("  background-color: ");
  style.append(color);
  style.append(";");
  style.append("  border-bottom: 1px solid black;");
  style.append("  border-right: 1px solid black;");
  style.append("}");
  wrapper->setStyleSheet(style);

  QVBoxLayout * layout = new QVBoxLayout();
  layout->setSpacing(0);
  if(row == 0){
    layout->setContentsMargins(0,0,0,0);
  } else {
    layout->setContentsMargins(20,20,20,20);
  }
  layout->addWidget(widget);
  wrapper->setLayout(layout);

  return wrapper;
}

void OSGridController::checkSelectedFields()
{
  if(!this->m_hasHorizontalHeader) return;

  std::vector<QString>::iterator it;
  this->m_currentFields;
  for(unsigned j = 0; j < m_customCategories.size(); j++){
    it = std::find(m_currentFields.begin(), m_currentFields.end() ,m_customCategories.at(j));
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

  std::pair<QString,std::vector<QString> > categoryAndFields = std::make_pair(QString("Custom"),m_customCategories);
  m_categoriesAndFields.push_back(categoryAndFields);
}

int OSGridController::rowCount() const
{
  return m_modelObjects.size();
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
    m_customCategories.push_back(m_currentFields.at(index));
  } else {
    std::vector<QString>::iterator it;
    it = std::find(m_customCategories.begin(), m_customCategories.end(), m_currentFields.at(index));
    if( it != m_customCategories.end() ){
      m_customCategories.erase(it);
    }
  }
  setCustomCategoryAndFields();
}

void OSGridController::verticalHeaderChecked(int index)
{
}

HorizontalHeaderWidget::HorizontalHeaderWidget(const QString & fieldName, QWidget * parent)
  : QWidget(parent),
  m_label(new QLabel(fieldName)),
  m_checkBox(new QCheckBox())
{
  QHBoxLayout * layout = new QHBoxLayout();
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
  QHBoxLayout * layout = new QHBoxLayout();
  setLayout(layout);

  layout->addWidget(m_checkBox);

  layout->addWidget(m_comboBox); 

}

} // openstudio

