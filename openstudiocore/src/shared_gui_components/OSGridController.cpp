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

#include "OSGridController.hpp"

#include "OSCheckBox.hpp"
#include "OSComboBox.hpp"
#include "OSDoubleEdit.hpp"
#include "OSGridView.hpp"
#include "OSIntegerEdit.hpp"
#include "OSLineEdit.hpp"
#include "OSLoadNamePixmapLineEdit.hpp"
#include "OSQuantityEdit.hpp"
#include "OSUnsignedEdit.hpp"

#include "../openstudio_lib/HorizontalTabWidget.hpp"
#include "../openstudio_lib/MainRightColumnController.hpp"
#include "../openstudio_lib/ModelObjectInspectorView.hpp"
#include "../openstudio_lib/ModelObjectItem.hpp"
#include "../openstudio_lib/ModelSubTabView.hpp"
#include "../openstudio_lib/OSAppBase.hpp"
#include "../openstudio_lib/OSDocument.hpp"
#include "../openstudio_lib/OSDropZone.hpp"
#include "../openstudio_lib/OSItemSelector.hpp"
#include "../openstudio_lib/RenderingColorWidget.hpp"
#include "../openstudio_lib/SchedulesView.hpp"

#include "../model/Model_Impl.hpp"
#include "../model/ModelObject_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <QApplication>
#include <QBoxLayout>
#include <QButtonGroup>
#include <QCheckBox>
#include <QColor>
#include <QPushButton>
#include <QSettings>
#include <QTimer>
#include <QWidget>

namespace openstudio {

const std::vector<QColor> OSGridController::m_colors = SchedulesView::initializeColors();

WidgetLocation::WidgetLocation(QWidget *t_widget, int t_row, int t_column, boost::optional<int> t_subrow)
  : widget(t_widget), row(t_row), column(t_column), subrow(std::move(t_subrow))
{
}

WidgetLocation::~WidgetLocation()
{
}

void WidgetLocation::onInFocus(bool hasFocus, bool hasData)
{
  emit inFocus(hasFocus, hasData, row, column, subrow);
}

ObjectSelector::ObjectSelector(OSGridController *t_grid)
  : m_grid(t_grid), m_objectFilter(getDefaultFilter())
{
}

void ObjectSelector::addWidget(const boost::optional<model::ModelObject> &t_obj, Holder *t_holder, int row, int column,
    const boost::optional<int> &t_subrow, const bool t_selector)
{
  WidgetLocation * widgetLoc = new WidgetLocation(t_holder, row, column, t_subrow);

  connect(t_holder, &QObject::destroyed, this, &ObjectSelector::widgetDestroyed);
  connect(t_holder, &Holder::inFocus, widgetLoc, &WidgetLocation::onInFocus);
  connect(widgetLoc, &WidgetLocation::inFocus, this, &ObjectSelector::inFocus);

  m_widgetMap.insert(std::make_pair(t_obj, widgetLoc));

  boost::optional<int> subRowToUpdate = t_subrow;

  // A subrow may have been added, but maybe this gridview is driven by rows instead of subrows
  bool rowSelected = false;
  for (auto &widget : m_widgetMap)
  {
    if (widget.first
      && widget.second->row == row
      && (!widget.second->subrow || (widget.second->subrow == t_subrow)))
    {
      if (m_selectedObjects.count(*widget.first) != 0)
      {
        // we found a selected row, not subrow
        if (!widget.second->subrow) {
          subRowToUpdate.reset();
        }
        rowSelected = true;
        break;
      }
    }
  }

  auto visible = true;

  // is this (sub)row visible?
  for (auto &widget : m_widgetMap)
  {
    if (widget.first
      && widget.second->row == row
      && (!widget.second->subrow || (widget.second->subrow == t_subrow)))
    {
      if (!m_objectFilter(*widget.first)) {
        // an object in this (sub)row is not visible, therefore I am not visible
        visible = false;
        break;
      }
    }
  }

  // we found something selected that overlaps (row or subrow) with the added widget
  // so let's go ahead and perform an update
  if (rowSelected || !visible) {
    updateWidgets(row, subRowToUpdate, rowSelected, visible);
  }

  if (t_selector && t_obj)
  {
    m_selectorObjects.insert(*t_obj);
  }
}

void ObjectSelector::clear()
{
  m_widgetMap.clear(); // TODO delete all QObjects, or set parent
  m_selectedObjects.clear();
  m_selectorObjects.clear();
  m_objectFilter = getDefaultFilter();
}

void ObjectSelector::objectRemoved(const openstudio::model::ModelObject &t_obj)
{
  std::cout << " Object removed\n";

  m_selectedObjects.erase(t_obj);
  m_selectorObjects.erase(t_obj);
  m_widgetMap.erase(boost::optional<model::ModelObject>(t_obj));
}

bool ObjectSelector::containsObject(const openstudio::model::ModelObject &t_obj) const
{
  return m_selectedObjects.count(t_obj) != 0
    || m_selectorObjects.count(t_obj) != 0
    || m_widgetMap.count(boost::optional<model::ModelObject>(t_obj));
}

void ObjectSelector::widgetDestroyed(QObject *t_obj)
{
  auto itr = m_widgetMap.begin();

  while (itr != m_widgetMap.end())
  {
    if (itr->second->widget == t_obj) {
      itr = m_widgetMap.erase(itr);
    } else {
      ++itr;
    }
  }
}

bool ObjectSelector::getObjectSelection(const model::ModelObject &t_obj) const
{
  return m_selectedObjects.count(t_obj) != 0;
}

void ObjectSelector::setObjectSelection(const model::ModelObject &t_obj, bool t_selected)
{
  auto changed = false;
  if (t_selected)
  {
    changed = m_selectedObjects.insert(t_obj).second;
  } else {
    changed = m_selectedObjects.erase(t_obj) != 0;
  }

  if (changed)
  {
    updateWidgets(t_obj);
  }
}

std::set<model::ModelObject> ObjectSelector::getSelectedObjects() const
{
  std::set<model::ModelObject> returned;

  std::copy_if(m_selectedObjects.begin(), m_selectedObjects.end(),
      std::inserter(returned, returned.begin()),
      m_objectFilter);

  return returned;
}

std::vector<QWidget *> ObjectSelector::getColumnsSelectedWidgets(int column)
{
  std::vector<QWidget *> results;

  for (auto selectedObject : m_selectedObjects) {
    for (auto &widgetLoc : m_widgetMap)
    {
      if (widgetLoc.first && selectedObject == widgetLoc.first.get())
      {
        results.push_back(getWidget(widgetLoc.second->row, column, widgetLoc.second->subrow));
        break;
      }
    }
  }
  return results;
}

void ObjectSelector::setObjectFilter(const std::function<bool (const model::ModelObject &)> &t_filter)
{
  m_objectFilter = t_filter;
  updateWidgets();
}

void ObjectSelector::resetObjectFilter()
{
  m_objectFilter = getDefaultFilter();
  updateWidgets();
}

std::function<bool (const model::ModelObject &)> ObjectSelector::getDefaultFilter()
{
  return [](const model::ModelObject &) { return true; };
}

void ObjectSelector::selectAll()
{
  m_selectedObjects = m_selectorObjects;
  m_grid->requestRefreshGrid();
}

void ObjectSelector::clearSelection()
{
  m_selectedObjects.clear();
  m_grid->requestRefreshGrid();
}

boost::optional<const model::ModelObject &> ObjectSelector::getObject(const int t_row, const int t_column, const boost::optional<int> &t_subrow)
{
  boost::optional<const model::ModelObject &> object;

  for (auto &widgetLoc : m_widgetMap)
  {
    if (widgetLoc.second->row == t_row && widgetLoc.second->column == t_column && (!t_subrow || t_subrow == widgetLoc.second->subrow))
    {
      object = widgetLoc.first;
      break;
    }
  }
  return object;
}

QWidget * ObjectSelector::getWidget(const int t_row, const int t_column, const boost::optional<int> &t_subrow)
{
  QWidget * widget = nullptr;

  for (auto &widgetLoc : m_widgetMap)
  {
    if (widgetLoc.second->row == t_row && widgetLoc.second->column == t_column && (!t_subrow || t_subrow == widgetLoc.second->subrow))
    {
      widget = qobject_cast<Holder *>(widgetLoc.second->widget)->widget;
      break;
    }
  }
  return widget;
}

void ObjectSelector::updateWidgets(const int t_row, const boost::optional<int> &t_subrow, bool t_objectSelected, bool t_objectVisible)
{
  std::set<std::pair<QWidget *,int>> widgetsToUpdate;
  bool isSubRow = t_subrow;

  // determine if we want to update the parent widget or the child widget
  for (auto &widgetLoc : m_widgetMap)
  {
    if (widgetLoc.second->row == t_row
      && (!t_subrow
      || (t_subrow == widgetLoc.second->subrow)))
    {
      if (!isSubRow)
      {
        widgetsToUpdate.insert(std::make_pair(widgetLoc.second->widget->parentWidget(), widgetLoc.second->column));
      } else {
        widgetsToUpdate.insert(std::make_pair(widgetLoc.second->widget, widgetLoc.second->column));
        widgetLoc.second->widget->setStyleSheet("");
      }
    }
  }

  // loop over list of determined widgets
  for (auto &widget : widgetsToUpdate)
  {
    widget.first->setVisible(t_objectVisible);
    widget.first->setStyleSheet(m_grid->cellStyle(t_row, widget.second, t_objectSelected, isSubRow));
  }
}

void ObjectSelector::updateWidgets()
{
  for (const auto &obj : m_selectorObjects)
  {
    updateWidgets(obj);
  }
}

void ObjectSelector::updateWidgets(const model::ModelObject &t_obj)
{
  auto range = m_widgetMap.equal_range(boost::optional<model::ModelObject>(t_obj));

  assert(range.first != range.second);

  // Find the row that contains this object
  auto row = std::make_tuple(range.first->second->row, range.first->second->subrow);

#if _DEBUG || (__GNUC__ && !NDEBUG)
  // Sanity check to make sure we don't have the same object in two different rows
  ++range.first;
  while (range.first != range.second)
  {
    assert(row == std::make_tuple(range.first->second->row, range.first->second->subrow));
    ++range.first;
  }
#endif

  const auto objectSelected = m_selectedObjects.count(t_obj) != 0;
  const auto objectVisible = m_objectFilter(t_obj);

  updateWidgets(std::get<0>(row), std::get<1>(row), objectSelected, objectVisible);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

OSGridController::OSGridController()
  : QObject(),
  m_objectSelector(std::make_shared<ObjectSelector>(this))
{
}

OSGridController::OSGridController(bool isIP,
                                   const QString & headerText,
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
    m_customFields(std::vector<QString>()),
    m_model(model),
    m_isIP(isIP),
    m_modelObjects(modelObjects),
    m_iddObjectType(iddObjectType),
    m_horizontalHeaderBtnGrp(nullptr),
    m_headerText(headerText),
    m_objectSelector(std::make_shared<ObjectSelector>(this))
{
  loadQSettings();

  connect(m_objectSelector.get(), &ObjectSelector::inFocus, this, &OSGridController::onInFocus);
}

OSGridController::~OSGridController()
{
  saveQSettings();
}

void OSGridController::requestRefreshGrid()
{
  gridView()->requestRefreshGrid();
}

void OSGridController::refreshGrid()
{
  // Never hit
  OS_ASSERT(false);
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
  m_objectSelector->clear();
  m_currentCategoryIndex = index;
  m_selectedCellLocation = std::make_tuple(-1,-1,-1);

  m_currentCategory = m_categoriesAndFields.at(index).first;

  m_currentFields = m_categoriesAndFields.at(index).second;

  addColumns(m_currentCategory, m_currentFields);
}

void OSGridController::setHorizontalHeader()
{
  m_horizontalHeader.clear();

  if(m_horizontalHeaderBtnGrp == nullptr){
    m_horizontalHeaderBtnGrp = new QButtonGroup();
    m_horizontalHeaderBtnGrp->setExclusive(false);

    connect(m_horizontalHeaderBtnGrp, static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked), this, &OSGridController::horizontalHeaderChecked);

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
    auto horizontalHeaderWidget = new HorizontalHeaderWidget(field, this->gridView());
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

    auto checkBox = new OSCheckBox3(this->gridView()); // OSCheckBox3 is derived from QCheckBox, whereas OSCheckBox2 is derived from QPushButton
    if (checkBoxConcept->tooltip().size()) {
      checkBox->setToolTip(checkBoxConcept->tooltip().c_str());
    }

    checkBox->bind(t_mo,
                   BoolGetter(std::bind(&CheckBoxConcept::get,checkBoxConcept.data(),t_mo)),
                                       boost::optional<BoolSetter>(std::bind(&CheckBoxConcept::set, checkBoxConcept.data(), t_mo, std::placeholders::_1)));

    isConnected = connect(checkBox, SIGNAL(stateChanged(int)), gridView(), SLOT(requestRefreshGrid())); // TODO this is not the most efficient, by single row would be better
    OS_ASSERT(isConnected);

    widget = checkBox;

  } else if(QSharedPointer<ComboBoxConcept> comboBoxConcept = t_baseConcept.dynamicCast<ComboBoxConcept>()) {

    auto choiceConcept = comboBoxConcept->choiceConcept(t_mo);

    auto comboBox = new OSComboBox2(this->gridView(), choiceConcept->editable());
    if (comboBoxConcept->hasClickFocus()) {
      comboBox->enableClickFocus();
    }

    comboBox->bind(t_mo, choiceConcept);

    widget = comboBox;

    isConnected = connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxIndexChanged(int)));
    OS_ASSERT(isConnected);

  } else if(QSharedPointer<ValueEditConcept<double> > doubleEditConcept = t_baseConcept.dynamicCast<ValueEditConcept<double> >()) {

    auto doubleEdit = new OSDoubleEdit2(this->gridView());
    if (doubleEditConcept->hasClickFocus()) {
      doubleEdit->enableClickFocus();
    }

    doubleEdit->bind(t_mo,
                     DoubleGetter(std::bind(&ValueEditConcept<double>::get,doubleEditConcept.data(),t_mo)),
                     boost::optional<DoubleSetter>(std::bind(&ValueEditConcept<double>::set,doubleEditConcept.data(),t_mo,std::placeholders::_1)),
                     boost::optional<NoFailAction>(std::bind(&ValueEditConcept<double>::reset,doubleEditConcept.data(),t_mo)),
                     boost::optional<NoFailAction>(),
                     boost::optional<NoFailAction>(),
                     boost::optional<BasicQuery>(std::bind(&ValueEditConcept<double>::isDefaulted,doubleEditConcept.data(),t_mo)));

    widget = doubleEdit;

  } else if(QSharedPointer<OptionalValueEditConcept<double> > optionalDoubleEditConcept = t_baseConcept.dynamicCast<OptionalValueEditConcept<double> >()) {

    auto optionalDoubleEdit = new OSDoubleEdit2(this->gridView());
    if (optionalDoubleEditConcept->hasClickFocus()) {
      optionalDoubleEdit->enableClickFocus();
    }

    optionalDoubleEdit->bind(t_mo,
                             OptionalDoubleGetter(std::bind(&OptionalValueEditConcept<double>::get,optionalDoubleEditConcept.data(),t_mo)),
                             boost::optional<DoubleSetter>(std::bind(&OptionalValueEditConcept<double>::set,optionalDoubleEditConcept.data(),t_mo,std::placeholders::_1)));

    widget = optionalDoubleEdit;

  } else if(QSharedPointer<ValueEditVoidReturnConcept<double> > doubleEditVoidReturnConcept = t_baseConcept.dynamicCast<ValueEditVoidReturnConcept<double> >()) {

    auto doubleEditVoidReturn = new OSDoubleEdit2(this->gridView());
    if (doubleEditVoidReturnConcept->hasClickFocus()) {
      doubleEditVoidReturn->enableClickFocus();
    }

    doubleEditVoidReturn->bind(t_mo,
                               DoubleGetter(std::bind(&ValueEditVoidReturnConcept<double>::get,doubleEditVoidReturnConcept.data(),t_mo)),
                               DoubleSetterVoidReturn(std::bind(&ValueEditVoidReturnConcept<double>::set,doubleEditVoidReturnConcept.data(),t_mo,std::placeholders::_1)),
                     boost::optional<NoFailAction>(std::bind(&ValueEditVoidReturnConcept<double>::reset,doubleEditVoidReturnConcept.data(),t_mo)),
                     boost::optional<NoFailAction>(),
                     boost::optional<NoFailAction>(),
                     boost::optional<BasicQuery>(std::bind(&ValueEditVoidReturnConcept<double>::isDefaulted,doubleEditVoidReturnConcept.data(),t_mo)));

    widget = doubleEditVoidReturn;

  } else if(QSharedPointer<OptionalValueEditVoidReturnConcept<double> > optionalDoubleEditVoidReturnConcept = t_baseConcept.dynamicCast<OptionalValueEditVoidReturnConcept<double> >()) {

    auto optionalDoubleEditVoidReturn = new OSDoubleEdit2(this->gridView());
    if (optionalDoubleEditVoidReturnConcept->hasClickFocus()) {
      optionalDoubleEditVoidReturn->enableClickFocus();
    }

    optionalDoubleEditVoidReturn->bind(t_mo,
                                       OptionalDoubleGetter(std::bind(&OptionalValueEditVoidReturnConcept<double>::get,optionalDoubleEditVoidReturnConcept.data(),t_mo)),
                                       DoubleSetterVoidReturn(std::bind(&OptionalValueEditVoidReturnConcept<double>::set,optionalDoubleEditVoidReturnConcept.data(),t_mo,std::placeholders::_1)));

    widget = optionalDoubleEditVoidReturn;

  } else if(QSharedPointer<ValueEditConcept<int> > integerEditConcept = t_baseConcept.dynamicCast<ValueEditConcept<int> >()) {

    auto integerEdit = new OSIntegerEdit2(this->gridView());
    if (integerEditConcept->hasClickFocus()) {
      integerEdit->enableClickFocus();
    }

    integerEdit->bind(t_mo,
                      IntGetter(std::bind(&ValueEditConcept<int>::get,integerEditConcept.data(),t_mo)),
                      boost::optional<IntSetter>(std::bind(&ValueEditConcept<int>::set,integerEditConcept.data(),t_mo,std::placeholders::_1)),
                      boost::optional<NoFailAction>(std::bind(&ValueEditConcept<int>::reset,integerEditConcept.data(),t_mo)),
                      boost::optional<NoFailAction>(),
                      boost::optional<NoFailAction>(),
                      boost::optional<BasicQuery>(std::bind(&ValueEditConcept<int>::isDefaulted,integerEditConcept.data(),t_mo)));

    widget = integerEdit;

  } else if(QSharedPointer<ValueEditConcept<std::string> > lineEditConcept = t_baseConcept.dynamicCast<ValueEditConcept<std::string> >()) {

    auto lineEdit = new OSLineEdit2(this->gridView());
    if (lineEditConcept->hasClickFocus()) {
      lineEdit->enableClickFocus();
    }

    lineEdit->bind(t_mo,
                   StringGetter(std::bind(&ValueEditConcept<std::string>::get,lineEditConcept.data(),t_mo)),
                   boost::optional<StringSetter>(std::bind(&ValueEditConcept<std::string>::set,lineEditConcept.data(),t_mo,std::placeholders::_1)),
                   boost::optional<NoFailAction>(std::bind(&ValueEditConcept<std::string>::reset,lineEditConcept.data(),t_mo)),
                   boost::optional<BasicQuery>(std::bind(&ValueEditConcept<std::string>::isDefaulted,lineEditConcept.data(),t_mo)));

    isConnected = connect(lineEdit, SIGNAL(objectRemoved(boost::optional<model::ParentObject>)), this, SLOT(onObjectRemoved(boost::optional<model::ParentObject>)));
    OS_ASSERT(isConnected);

    widget = lineEdit;

  } else if(QSharedPointer<LoadNameConcept> loadNameConcept = t_baseConcept.dynamicCast<LoadNameConcept>()) {

    auto loadName = new OSLoadNamePixmapLineEdit(this->gridView());
    if (loadNameConcept->hasClickFocus()) {
      loadName->enableClickFocus();
    }

    loadName->bind(t_mo,
                   OptionalStringGetter(std::bind(&LoadNameConcept::get,loadNameConcept.data(),t_mo,true)),
                   // If the concept is read only, pass an empty optional
                   loadNameConcept->readOnly() ? boost::none : boost::optional<StringSetter>(std::bind(&LoadNameConcept::set,loadNameConcept.data(),t_mo,std::placeholders::_1)),
                   boost::optional<NoFailAction>(std::bind(&LoadNameConcept::reset, loadNameConcept.data(), t_mo)));

    isConnected = connect(loadName, SIGNAL(itemClicked(OSItem*)), gridView(), SIGNAL(dropZoneItemClicked(OSItem*)));
    OS_ASSERT(isConnected);

    isConnected = connect(loadName, SIGNAL(itemClicked(OSItem*)), this, SLOT(onDropZoneItemClicked(OSItem*)));
    OS_ASSERT(isConnected);

    isConnected = connect(loadName, SIGNAL(objectRemoved(boost::optional<model::ParentObject>)), this, SLOT(onObjectRemoved(boost::optional<model::ParentObject>)));
    OS_ASSERT(isConnected);

    widget = loadName;

  } else if(QSharedPointer<NameLineEditConcept> nameLineEditConcept = t_baseConcept.dynamicCast<NameLineEditConcept>()) {

    auto nameLineEdit = new OSLineEdit2(this->gridView());
    if (nameLineEditConcept->hasClickFocus()) {
      nameLineEdit->enableClickFocus();
    }

    nameLineEdit->setDeleteObject(nameLineEditConcept->deleteObject());

    nameLineEdit->bind(t_mo,
                       OptionalStringGetter(std::bind(&NameLineEditConcept::get,nameLineEditConcept.data(),t_mo,true)),
                       // If the concept is read only, pass an empty optional
                       nameLineEditConcept->readOnly() ? boost::none : boost::optional<StringSetter>(std::bind(&NameLineEditConcept::set,nameLineEditConcept.data(),t_mo,std::placeholders::_1)),
                       boost::optional<NoFailAction>(std::bind(&NameLineEditConcept::reset, nameLineEditConcept.data(), t_mo)));

    if (nameLineEditConcept->isInspectable()) {
      isConnected = connect(nameLineEdit, SIGNAL(itemClicked(OSItem*)), gridView(), SIGNAL(dropZoneItemClicked(OSItem*)));
      OS_ASSERT(isConnected);

      isConnected = connect(nameLineEdit, SIGNAL(itemClicked(OSItem*)), this, SLOT(onDropZoneItemClicked(OSItem*)));
      OS_ASSERT(isConnected);

      isConnected = connect(nameLineEdit, SIGNAL(objectRemoved(boost::optional<model::ParentObject>)), this, SLOT(onObjectRemoved(boost::optional<model::ParentObject>)));
      OS_ASSERT(isConnected);
    }

    widget = nameLineEdit;

  } else if(QSharedPointer<QuantityEditConcept<double> > quantityEditConcept = t_baseConcept.dynamicCast<QuantityEditConcept<double> >()) {

    OSQuantityEdit2 * quantityEdit = new OSQuantityEdit2(
          quantityEditConcept->modelUnits().toStdString().c_str(),
          quantityEditConcept->siUnits().toStdString().c_str(),
          quantityEditConcept->ipUnits().toStdString().c_str(),
          quantityEditConcept->isIP(),
          this->gridView());
    if (quantityEditConcept->hasClickFocus()) {
      quantityEdit->enableClickFocus();
    }

    quantityEdit->bind(m_isIP,
                       t_mo,
                       DoubleGetter(std::bind(&QuantityEditConcept<double>::get,quantityEditConcept.data(),t_mo)),
                       boost::optional<DoubleSetter>(std::bind(&QuantityEditConcept<double>::set,quantityEditConcept.data(),t_mo,std::placeholders::_1)),
                   boost::optional<NoFailAction>(std::bind(&QuantityEditConcept<double>::reset,quantityEditConcept.data(),t_mo)),
                   boost::optional<NoFailAction>(),
                   boost::optional<NoFailAction>(),
                   boost::optional<BasicQuery>(std::bind(&QuantityEditConcept<double>::isDefaulted,quantityEditConcept.data(),t_mo)));

    isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), quantityEdit, SLOT(onUnitSystemChange(bool)));
    OS_ASSERT(isConnected);

    widget = quantityEdit;

  } else if(QSharedPointer<OptionalQuantityEditConcept<double> > optionalQuantityEditConcept = t_baseConcept.dynamicCast<OptionalQuantityEditConcept<double> >()) {

    OSQuantityEdit2 * optionalQuantityEdit = new OSQuantityEdit2(
          optionalQuantityEditConcept->modelUnits().toStdString().c_str(),
          optionalQuantityEditConcept->siUnits().toStdString().c_str(),
          optionalQuantityEditConcept->ipUnits().toStdString().c_str(),
          optionalQuantityEditConcept->isIP(),
          this->gridView());
    if (optionalQuantityEditConcept->hasClickFocus()) {
      optionalQuantityEdit->enableClickFocus();
    }

    optionalQuantityEdit->bind(m_isIP,
                               t_mo,
                               OptionalDoubleGetter(std::bind(&OptionalQuantityEditConcept<double>::get,optionalQuantityEditConcept.data(),t_mo)),
                               boost::optional<DoubleSetter>(std::bind(&OptionalQuantityEditConcept<double>::set,optionalQuantityEditConcept.data(),t_mo,std::placeholders::_1)));

    isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), optionalQuantityEdit, SLOT(onUnitSystemChange(bool)));
    OS_ASSERT(isConnected);

    widget = optionalQuantityEdit;

  } else if(QSharedPointer<QuantityEditVoidReturnConcept<double> > quantityEditVoidReturnConcept = t_baseConcept.dynamicCast<QuantityEditVoidReturnConcept<double> >()) {

    OSQuantityEdit2 * quantityEditVoidReturn = new OSQuantityEdit2(
          quantityEditVoidReturnConcept->modelUnits().toStdString().c_str(),
          quantityEditVoidReturnConcept->siUnits().toStdString().c_str(),
          quantityEditVoidReturnConcept->ipUnits().toStdString().c_str(),
          quantityEditVoidReturnConcept->isIP(),
          this->gridView());
    if (quantityEditVoidReturnConcept->hasClickFocus()) {
      quantityEditVoidReturn->enableClickFocus();
    }

    quantityEditVoidReturn->bind(m_isIP,
                                 t_mo,
                                 DoubleGetter(std::bind(&QuantityEditVoidReturnConcept<double>::get,quantityEditVoidReturnConcept.data(),t_mo)),
                                 DoubleSetterVoidReturn(std::bind(&QuantityEditVoidReturnConcept<double>::set,quantityEditVoidReturnConcept.data(),t_mo,std::placeholders::_1)),
                   boost::optional<NoFailAction>(std::bind(&QuantityEditVoidReturnConcept<double>::reset,quantityEditVoidReturnConcept.data(),t_mo)),
                   boost::optional<NoFailAction>(),
                   boost::optional<NoFailAction>(),
                   boost::optional<BasicQuery>(std::bind(&QuantityEditVoidReturnConcept<double>::isDefaulted,quantityEditVoidReturnConcept.data(),t_mo)));

    isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), quantityEditVoidReturn, SLOT(onUnitSystemChange(bool)));
    OS_ASSERT(isConnected);

    widget = quantityEditVoidReturn;

  } else if(QSharedPointer<OptionalQuantityEditVoidReturnConcept<double> > optionalQuantityEditVoidReturnConcept = t_baseConcept.dynamicCast<OptionalQuantityEditVoidReturnConcept<double> >()) {

    OSQuantityEdit2 * optionalQuantityEditVoidReturn = new OSQuantityEdit2(
          optionalQuantityEditVoidReturnConcept->modelUnits().toStdString().c_str(),
          optionalQuantityEditVoidReturnConcept->siUnits().toStdString().c_str(),
          optionalQuantityEditVoidReturnConcept->ipUnits().toStdString().c_str(),
          optionalQuantityEditVoidReturnConcept->isIP(),
          this->gridView());
    if (optionalQuantityEditVoidReturnConcept->hasClickFocus()) {
      optionalQuantityEditVoidReturn->enableClickFocus();
    }

    optionalQuantityEditVoidReturn->bind(m_isIP,
                                         t_mo,
                                         OptionalDoubleGetter(std::bind(&OptionalQuantityEditVoidReturnConcept<double>::get,optionalQuantityEditVoidReturnConcept.data(),t_mo)),
                                         DoubleSetterVoidReturn(std::bind(&OptionalQuantityEditVoidReturnConcept<double>::set,optionalQuantityEditVoidReturnConcept.data(),t_mo,std::placeholders::_1)));

    isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), optionalQuantityEditVoidReturn, SLOT(onUnitSystemChange(bool)));
    OS_ASSERT(isConnected);

    widget = optionalQuantityEditVoidReturn;

  } else if(QSharedPointer<ValueEditConcept<unsigned> > unsignedEditConcept = t_baseConcept.dynamicCast<ValueEditConcept<unsigned> >()) {

    auto unsignedEdit = new OSUnsignedEdit2(this->gridView());
    if (unsignedEditConcept->hasClickFocus()) {
      unsignedEdit->enableClickFocus();
    }

    unsignedEdit->bind(t_mo,
                       UnsignedGetter(std::bind(&ValueEditConcept<unsigned>::get,unsignedEditConcept.data(),t_mo)),
                       boost::optional<UnsignedSetter>(std::bind(&ValueEditConcept<unsigned>::set,unsignedEditConcept.data(),t_mo,std::placeholders::_1)),
                   boost::optional<NoFailAction>(std::bind(&ValueEditConcept<unsigned>::reset,unsignedEditConcept.data(),t_mo)),
                   boost::optional<NoFailAction>(),
                   boost::optional<NoFailAction>(),
                   boost::optional<BasicQuery>(std::bind(&ValueEditConcept<unsigned>::isDefaulted,unsignedEditConcept.data(),t_mo)));

    widget = unsignedEdit;

  } else if(QSharedPointer<DropZoneConcept> dropZoneConcept = t_baseConcept.dynamicCast<DropZoneConcept>()) {
    OSDropZone2 * dropZone = new OSDropZone2();
    if (dropZoneConcept->hasClickFocus()) {
      dropZone->enableClickFocus();
    }

    dropZone->bind(t_mo,
      OptionalModelObjectGetter(std::bind(&DropZoneConcept::get,dropZoneConcept.data(),t_mo)),
      ModelObjectSetter(std::bind(&DropZoneConcept::set, dropZoneConcept.data(), t_mo, std::placeholders::_1)),
      NoFailAction(std::bind(&DropZoneConcept::reset, dropZoneConcept.data(), t_mo)));

    isConnected = connect(dropZone, SIGNAL(itemClicked(OSItem*)), gridView(), SIGNAL(dropZoneItemClicked(OSItem*)));
    OS_ASSERT(isConnected);

    isConnected = connect(dropZone, SIGNAL(itemClicked(OSItem*)), this, SLOT(onDropZoneItemClicked(OSItem*)));
    OS_ASSERT(isConnected);

    isConnected = connect(dropZone, SIGNAL(objectRemoved(boost::optional<model::ParentObject>)), this, SLOT(onObjectRemoved(boost::optional<model::ParentObject>)));
    OS_ASSERT(isConnected);

    widget = dropZone;

  } else if (QSharedPointer<RenderingColorConcept> renderingColorConcept = t_baseConcept.dynamicCast<RenderingColorConcept>()) {
    auto * renderingColorWidget = new RenderingColorWidget2(this->gridView());

    renderingColorWidget->bind(t_mo,
      OptionalModelObjectGetter(std::bind(&RenderingColorConcept::get, renderingColorConcept.data(), t_mo)),
      ModelObjectSetter(std::bind(&RenderingColorConcept::set, renderingColorConcept.data(), t_mo, std::placeholders::_1)));

    widget = renderingColorWidget;

  } else {
    // Unknown type
    OS_ASSERT(false);
  }

  return widget;
}

void OSGridController::setConceptValue(model::ModelObject t_setterMO, model::ModelObject t_getterMO, const QSharedPointer<BaseConcept> &t_baseConcept)
{
  if (QSharedPointer<CheckBoxConcept> concept = t_baseConcept.dynamicCast<CheckBoxConcept>()){
    auto setter = std::bind(&CheckBoxConcept::set, concept.data(), t_setterMO, std::placeholders::_1);
    auto getter = std::bind(&CheckBoxConcept::get, concept.data(), t_getterMO);
    auto temp = getter();
    setter(temp);
  }
  else if (QSharedPointer<ComboBoxConcept> concept = t_baseConcept.dynamicCast<ComboBoxConcept>()) {
    auto getterChoiceConcept = concept->choiceConcept(t_getterMO);
    auto setterChoiceConcept = concept->choiceConcept(t_setterMO);
    auto getter = std::bind(&ChoiceConcept::get, getterChoiceConcept.get());
    auto setter = std::bind(&ChoiceConcept::set, setterChoiceConcept.get(), std::placeholders::_1);
    auto temp = getter();
    setter(temp);
  }
  else if (QSharedPointer<ValueEditConcept<double> > concept = t_baseConcept.dynamicCast<ValueEditConcept<double> >()) {
    auto setter = std::bind(&ValueEditConcept<double>::set, concept.data(), t_setterMO, std::placeholders::_1);
    auto getter = std::bind(&ValueEditConcept<double>::get, concept.data(), t_getterMO);
    auto temp = getter();
    setter(temp);
  }
  else if (QSharedPointer<OptionalValueEditConcept<double> > concept = t_baseConcept.dynamicCast<OptionalValueEditConcept<double> >()) {
    auto setter = std::bind(&OptionalValueEditConcept<double>::set, concept.data(), t_setterMO, std::placeholders::_1);
    auto getter = std::bind(&OptionalValueEditConcept<double>::get, concept.data(), t_getterMO);
    auto temp = getter();
    if (temp) setter(temp.get());
  }
  else if (QSharedPointer<ValueEditVoidReturnConcept<double> > concept = t_baseConcept.dynamicCast<ValueEditVoidReturnConcept<double> >()) {
    auto setter = std::bind(&ValueEditVoidReturnConcept<double>::set, concept.data(), t_setterMO, std::placeholders::_1);
    auto getter = std::bind(&ValueEditVoidReturnConcept<double>::get, concept.data(), t_getterMO);
    auto temp = getter();
    setter(temp);
  }
  else if (QSharedPointer<OptionalValueEditVoidReturnConcept<double> > concept = t_baseConcept.dynamicCast<OptionalValueEditVoidReturnConcept<double> >()) {
    auto setter = std::bind(&OptionalValueEditVoidReturnConcept<double>::set, concept.data(), t_setterMO, std::placeholders::_1);
    auto getter = std::bind(&OptionalValueEditVoidReturnConcept<double>::get, concept.data(), t_getterMO);
    auto temp = getter();
    if (temp) setter(temp.get());
  }
  else if (QSharedPointer<ValueEditConcept<int> > concept = t_baseConcept.dynamicCast<ValueEditConcept<int> >()) {
    auto setter = std::bind(&ValueEditConcept<int>::set, concept.data(), t_setterMO, std::placeholders::_1);
    auto getter = std::bind(&ValueEditConcept<int>::get, concept.data(), t_getterMO);
    auto temp = getter();
    setter(temp);
  }
  else if (QSharedPointer<ValueEditConcept<std::string> > concept = t_baseConcept.dynamicCast<ValueEditConcept<std::string> >()) {
    auto setter = std::bind(&ValueEditConcept<std::string>::set, concept.data(), t_setterMO, std::placeholders::_1);
    auto getter = std::bind(&ValueEditConcept<std::string>::get, concept.data(), t_getterMO);
    auto temp = getter();
    setter(temp);
  }
  else if (QSharedPointer<LoadNameConcept> concept = t_baseConcept.dynamicCast<LoadNameConcept>()) {
    auto setter = std::bind(&LoadNameConcept::set, concept.data(), t_setterMO, std::placeholders::_1);
    auto getter = std::bind(&LoadNameConcept::get, concept.data(), t_getterMO, true); // NOTE Evan: Do we always want true?
    auto temp = getter();
    if (temp) setter(temp.get());
  }
  else if (QSharedPointer<NameLineEditConcept> concept = t_baseConcept.dynamicCast<NameLineEditConcept>()) {
    auto setter = std::bind(&NameLineEditConcept::set, concept.data(), t_setterMO, std::placeholders::_1);
    auto getter = std::bind(&NameLineEditConcept::get, concept.data(), t_getterMO, true); // NOTE Evan: Do we always want true?
    auto temp = getter();
    if (temp) setter(temp.get());
  }
  else if (QSharedPointer<QuantityEditConcept<double> > concept = t_baseConcept.dynamicCast<QuantityEditConcept<double> >()) {
    auto setter = std::bind(&QuantityEditConcept<double>::set, concept.data(), t_setterMO, std::placeholders::_1);
    auto getter = std::bind(&QuantityEditConcept<double>::get, concept.data(), t_getterMO);
    auto temp = getter();
    setter(temp);
  }
  else if (QSharedPointer<OptionalQuantityEditConcept<double> > concept = t_baseConcept.dynamicCast<OptionalQuantityEditConcept<double> >()) {
    auto setter = std::bind(&OptionalQuantityEditConcept<double>::set, concept.data(), t_setterMO, std::placeholders::_1);
    auto getter = std::bind(&OptionalQuantityEditConcept<double>::get, concept.data(), t_getterMO);
    auto temp = getter();
    if (temp) setter(temp.get());
  }
  else if (QSharedPointer<QuantityEditVoidReturnConcept<double> > concept = t_baseConcept.dynamicCast<QuantityEditVoidReturnConcept<double> >()) {
    auto setter = std::bind(&QuantityEditVoidReturnConcept<double>::set, concept.data(), t_setterMO, std::placeholders::_1);
    auto getter = std::bind(&QuantityEditVoidReturnConcept<double>::get, concept.data(), t_getterMO);
    auto temp = getter();
    setter(temp);
  }
  else if (QSharedPointer<OptionalQuantityEditVoidReturnConcept<double> > concept = t_baseConcept.dynamicCast<OptionalQuantityEditVoidReturnConcept<double> >()) {
    auto setter = std::bind(&OptionalQuantityEditVoidReturnConcept<double>::set, concept.data(), t_setterMO, std::placeholders::_1);
    auto getter = std::bind(&OptionalQuantityEditVoidReturnConcept<double>::get, concept.data(), t_getterMO);
    auto temp = getter();
    if (temp) setter(temp.get());
  }
  else if (QSharedPointer<ValueEditConcept<unsigned> > concept = t_baseConcept.dynamicCast<ValueEditConcept<unsigned> >()) {
    auto setter = std::bind(&ValueEditConcept<unsigned>::set, concept.data(), t_setterMO, std::placeholders::_1);
    auto getter = std::bind(&ValueEditConcept<unsigned>::get, concept.data(), t_getterMO);
    auto temp = getter();
    setter(temp);
  }
  else if (QSharedPointer<DropZoneConcept> concept = t_baseConcept.dynamicCast<DropZoneConcept>()) {
    auto setter = std::bind(&DropZoneConcept::set, concept.data(), t_setterMO, std::placeholders::_1);
    auto getter = std::bind(&DropZoneConcept::get, concept.data(), t_getterMO);
    auto temp = getter();
    if (temp) setter(temp.get());
  }
  else if (QSharedPointer<RenderingColorConcept> concept = t_baseConcept.dynamicCast<RenderingColorConcept>()) {
    auto setter = std::bind(&RenderingColorConcept::set, concept.data(), t_setterMO, std::placeholders::_1);
    auto getter = std::bind(&RenderingColorConcept::get, concept.data(), t_getterMO);
    auto temp = getter();
    if (temp) setter(temp.get());
  }
  else {
    // Unknown type
    OS_ASSERT(false);
  }
}

void OSGridController::setConceptValue(model::ModelObject t_setterMO, model::ModelObject t_getterMO, const QSharedPointer<BaseConcept> &t_setterBaseConcept, const QSharedPointer<BaseConcept> &t_getterBaseConcept)
{
  if (QSharedPointer<NameLineEditConcept> getterConcept = t_getterBaseConcept.dynamicCast<NameLineEditConcept>()) {
    if (QSharedPointer<DropZoneConcept> setterConcept = t_setterBaseConcept.dynamicCast<DropZoneConcept>()) {
      auto mo = m_model.getModelObject<model::ModelObject>(t_getterMO.handle());
      OS_ASSERT(mo);
      auto setter = std::bind(&DropZoneConcept::set, setterConcept.data(), t_setterMO, std::placeholders::_1);
      setter(mo.get());
    }
  }
  else {
    // No other combination is currently in use
    // Should never get here
    OS_ASSERT(false);
  }
}

void OSGridController::resetConceptValue(model::ModelObject t_resetMO, const QSharedPointer<BaseConcept> &t_baseConcept)
{
  if (QSharedPointer<ValueEditConcept<double> > concept = t_baseConcept.dynamicCast<ValueEditConcept<double> >()) {
    auto reset = std::bind(&ValueEditConcept<double>::reset, concept.data(), t_resetMO);
    reset();
  }
  else if (QSharedPointer<ValueEditVoidReturnConcept<double> > concept = t_baseConcept.dynamicCast<ValueEditVoidReturnConcept<double> >()) {
    auto reset = std::bind(&ValueEditVoidReturnConcept<double>::reset, concept.data(), t_resetMO);
    reset();
  }
  else if (QSharedPointer<ValueEditConcept<int> > concept = t_baseConcept.dynamicCast<ValueEditConcept<int> >()) {
    auto reset = std::bind(&ValueEditConcept<int>::reset, concept.data(), t_resetMO);
    reset();
  }
  else if (QSharedPointer<ValueEditConcept<std::string> > concept = t_baseConcept.dynamicCast<ValueEditConcept<std::string> >()) {
    auto reset = std::bind(&ValueEditConcept<std::string>::reset, concept.data(), t_resetMO);
    reset();
  }
  else if (QSharedPointer<LoadNameConcept> concept = t_baseConcept.dynamicCast<LoadNameConcept>()) {
    auto reset = std::bind(&LoadNameConcept::reset, concept.data(), t_resetMO);
    reset();
  }
  else if (QSharedPointer<NameLineEditConcept> concept = t_baseConcept.dynamicCast<NameLineEditConcept>()) {
    auto reset = std::bind(&NameLineEditConcept::reset, concept.data(), t_resetMO);
    reset();
  }
  else if (QSharedPointer<QuantityEditConcept<double> > concept = t_baseConcept.dynamicCast<QuantityEditConcept<double> >()) {
    auto reset = std::bind(&QuantityEditConcept<double>::reset, concept.data(), t_resetMO);
    reset();
  }
  else if (QSharedPointer<QuantityEditVoidReturnConcept<double> > concept = t_baseConcept.dynamicCast<QuantityEditVoidReturnConcept<double> >()) {
    auto reset = std::bind(&QuantityEditVoidReturnConcept<double>::reset, concept.data(), t_resetMO);
    reset();
  }
  else if (QSharedPointer<ValueEditConcept<unsigned> > concept = t_baseConcept.dynamicCast<ValueEditConcept<unsigned> >()) {
    auto reset = std::bind(&ValueEditConcept<unsigned>::reset, concept.data(), t_resetMO);
    reset();
  }
  else if (QSharedPointer<DropZoneConcept> concept = t_baseConcept.dynamicCast<DropZoneConcept>()) {
    auto reset = std::bind(&DropZoneConcept::reset, concept.data(), t_resetMO);
    reset();
  }
  else {
    // Unknown type
    OS_ASSERT(false);
  }
}

OSGridView * OSGridController::gridView(){
  auto gridView = qobject_cast<OSGridView *>(this->parent());
  OS_ASSERT(gridView);
  return gridView;
}

// Evan: Required for Qt to respect style sheet commands
QString OSGridController::cellStyle(int rowIndex, int columnIndex, bool isSelected, bool isSubRow)
{
  /// \todo this is a lot of string concatenation to do for each cell update
  QString cellColor;
  if (isSelected) {
    // blue
    cellColor = "#94b3de";
  }
  else if (rowIndex % 2){
    // light gray
    cellColor = "#ededed";
  }
  else {
    // medium gray
    cellColor = "#cecece";
  }

  QString style;
  style.append("QWidget#TableCell { border: none;");
  style.append("                        background-color: " + cellColor + ";");
  if (rowIndex == 0){
    style.append("                      border-top: 1px solid black;");
  }
  style.append("                        border-right: 1px solid black;");
  style.append("                        border-bottom: 1px solid black;");
  style.append("}");

  return style;
}

QWidget * OSGridController::widgetAt(int row, int column)
{
  OS_ASSERT(row >= 0);
  OS_ASSERT(column >= 0);
  
  // Note: If there is a horizontal header row,  m_modelObjects[0] starts on gridLayout[1]
  int modelObjectRow = m_hasHorizontalHeader ? row - 1 : row; 
  
  OS_ASSERT(static_cast<int>(m_modelObjects.size()) > modelObjectRow);
  OS_ASSERT(static_cast<int>(m_baseConcepts.size()) > column);

  auto layout = new QGridLayout(this->gridView());
  const int widgetHeight = 30;
  int numWidgets = 0;

  // start with a default sane value
  QSize recommendedSize(100, 20);
  bool hasSubRows = false;

  // wrapper - this is the thing that will be returned by this method.  The outermost widget that forms a gridview cell.
  // May contain sub rows.
  auto wrapper = new QWidget(this->gridView());
  wrapper->setObjectName("TableCell");
  wrapper->setStyleSheet(this->cellStyle(row,column, false, true));
  layout->setSpacing(0);
  layout->setVerticalSpacing(0);
  layout->setHorizontalSpacing(0);
  layout->setContentsMargins(5,5,5,5);
  wrapper->setLayout(layout);
  // end wrapper

  // Holder is a Widget that corresponds to sub row cell, it is present even if the cell does not have sub rows
  // When no subrows wrapper -> holder -> bindable control provided by ::makeWidget
  std::vector<Holder *> holders;
  // addWidget lambda adds to holders (by first creating a new holder).
  // Also adds to layout, which is the layout of the main cell (wrapper).
  // holders and layout are accessible in the lamda through capture.
  // t_widget will be provided by ::makeWidget, it is the bindable control
  auto addWidget = [&](QWidget *t_widget, const boost::optional<model::ModelObject> &t_obj, const bool t_selector)
  {
    auto holder = new Holder(this->gridView());
    holder->setMinimumHeight(widgetHeight);
    auto l = new QVBoxLayout(this->gridView());
    l->setAlignment(Qt::AlignCenter);
    l->setSpacing(0);
    l->setContentsMargins(0,0,0,0);
    l->addWidget(t_widget);
    holder->widget = t_widget;
    holder->setLayout(l);
    // layout is defined outside the lambda and brought in through capture!
    layout->addWidget(holder, numWidgets, 0, 0);

    //if (hasSubRows) {
    //  holder->setObjectName("InnerCell");
    //}
    // holders is defined outside the lambda and brought in through capture!
    holders.push_back(holder);

    if (OSComboBox2 * comboBox = qobject_cast<OSComboBox2 *>(t_widget)) {
      connect(comboBox, &OSComboBox2::inFocus, holder, &Holder::inFocus);
    }
    else if (OSDoubleEdit2 * doubleEdit = qobject_cast<OSDoubleEdit2 *>(t_widget)) {
      connect(doubleEdit, &OSDoubleEdit2::inFocus, holder, &Holder::inFocus);
    }
    else if (OSIntegerEdit2 * integerEdit = qobject_cast<OSIntegerEdit2 *>(t_widget)) {
      connect(integerEdit, &OSIntegerEdit2::inFocus, holder, &Holder::inFocus);
    } 
    else if (OSQuantityEdit2 * quantityEdit = qobject_cast<OSQuantityEdit2 *>(t_widget)) {
      connect(quantityEdit, &OSQuantityEdit2::inFocus, holder, &Holder::inFocus);
    }
    else if (OSLineEdit2 * lineEdit = qobject_cast<OSLineEdit2 *>(t_widget)) {
      connect(lineEdit, &OSLineEdit2::inFocus, holder, &Holder::inFocus);
    }
    else if (OSUnsignedEdit2 * unsignedEdit = qobject_cast<OSUnsignedEdit2 *>(t_widget)) {
      connect(unsignedEdit, &OSUnsignedEdit2::inFocus, holder, &Holder::inFocus);
    }
    else if (OSDropZone2 * dropZone = qobject_cast<OSDropZone2 *>(t_widget)) {
      connect(dropZone, &OSDropZone2::inFocus, holder, &Holder::inFocus);
    }
    else if (HorizontalHeaderWidget * horizontalHeaderWidget = qobject_cast<HorizontalHeaderWidget *>(t_widget)) {
      connect(horizontalHeaderWidget, &HorizontalHeaderWidget::inFocus, holder, &Holder::inFocus);
    }

    m_objectSelector->addWidget(t_obj, holder, row, column, hasSubRows?numWidgets:boost::optional<int>(), t_selector);

    ++numWidgets;
  };

  if(m_hasHorizontalHeader && row == 0){
    if(column == 0){
      setHorizontalHeader();
      // Each concept should have its own column
      OS_ASSERT(m_horizontalHeader.size() == m_baseConcepts.size());
    }
    layout->setContentsMargins(0,1,1,0);
    addWidget(m_horizontalHeader.at(column), boost::none, false);
    QSharedPointer<BaseConcept> baseConcept = m_baseConcepts[column];
    const Heading &heading = baseConcept->heading();
    HorizontalHeaderWidget * horizontalHeaderWidget = qobject_cast<HorizontalHeaderWidget *>(m_horizontalHeader.at(column));
    OS_ASSERT(horizontalHeaderWidget);
    if (!heading.showCheckbox())
    {
      horizontalHeaderWidget->m_checkBox->hide();
    }
    if (!heading.showButton())
    {
      horizontalHeaderWidget->m_pushButton->hide();
    }

    horizontalHeaderWidget->addWidget(heading.widget());
  } else {

    model::ModelObject mo = m_modelObjects[modelObjectRow];

    //cellColor = getColor(mo);  TODO

    QSharedPointer<BaseConcept> baseConcept = m_baseConcepts[column];

    if (QSharedPointer<DataSourceAdapter> dataSource = baseConcept.dynamicCast<DataSourceAdapter>()) {
      hasSubRows = true;
      // here we magically create a multi-row column of any type that was constructed
      //
      // The details need to be fleshed out. The ideas all work, and it's rendering as expected,
      // however the placeHolder isn't doing its job, it might need to be a QSpacer of some kind.
      // The spacing around the list is a little awkward. The padding might need to be set to 0
      // all the way around.

      for (auto &item : dataSource->source().items(mo))
      {
        if (item)
        {
          addWidget(makeWidget(item->cast<model::ModelObject>(), dataSource->innerConcept()), item->cast<model::ModelObject>(), 
              baseConcept->isSelector() || dataSource->innerConcept()->isSelector());
        } else {
          addWidget(new QWidget(this->gridView()), boost::none, false);
        }
      }

      if (dataSource->source().wantsPlaceholder())
      {
        // use this space to put in a blank placeholder of some kind to make sure the 
        // widget is evenly laid out relative to its friends in the adjacent columns
        addWidget(new QWidget(this->gridView()), boost::none, false);
      }

      if (dataSource->source().dropZoneConcept())
      {
        // it makes sense to me that the drop zone would need a reference to the parent containing object
        // not an object the rest in the list was derived from
        // this should also be working and doing what you want
        addWidget(makeWidget(mo, dataSource->source().dropZoneConcept()), boost::none, false);
      }

      // right here you probably want some kind of container that's smart enough to know how to grow
      // and shrink as the contained items change. But I don't know enough about the model
      // to know how you'd want to do that. For now we make a fixed list that's got a VBoxLayout
      //
      // And think about this.
    } else {
      // This case is exactly what it used to do before the DataSource idea was added.

      // just the one
      addWidget(makeWidget(mo, baseConcept), mo, baseConcept->isSelector());
    }
  }

  if (hasSubRows) {
    holders.back()->setObjectName("InnerCellBottom");
  }

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

QWidget * OSGridController::cell(int rowIndex, int columnIndex)
{
  QWidget * widget = nullptr;

  QLayoutItem * child = gridView()->itemAtPosition(rowIndex, columnIndex);
  if (child) {
    widget = child->widget();
  }

  return widget;
}

model::ModelObject OSGridController::modelObject(int rowIndex)
{
  if (m_hasHorizontalHeader){
    OS_ASSERT(rowIndex > 0);
    return m_modelObjects.at(rowIndex - 1);
  }
  else {
    return m_modelObjects.at(rowIndex);
  }
}

int OSGridController::rowIndexFromModelIndex(int modelIndex)
{
  if (m_hasHorizontalHeader){
    return modelIndex + 1;
  }
  else {
    return modelIndex;
  }
}

std::vector<QWidget *> OSGridController::row(int rowIndex)
{
  std::vector<QWidget *> row;

  for (unsigned columnIndex = 0; columnIndex < m_currentFields.size(); columnIndex++){
    row.push_back(cell(rowIndex, columnIndex));
  }

  return row;
}

void OSGridController::selectRow(int rowIndex, bool select)
{
  std::vector<QWidget *> row = this->row(rowIndex);
  for (auto widget : row){
    auto button = qobject_cast<QPushButton *>(widget);
    if (!button){
      return;
    }
    button->blockSignals(true);
    button->setChecked(select);
    button->blockSignals(false);
  }
}

void OSGridController::horizontalHeaderChecked(int index)
{
  // Push_back or erase the field from the user-selected fields
  auto checkBox = qobject_cast<QAbstractButton *>(m_horizontalHeaderBtnGrp->button(index));
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
  // Pure virtual in base class
  OS_ASSERT(false);
}

void OSGridController::onItemSelected(OSItem * item)
{
}

//bool OSGridController::selectRowByItem(OSItem * item, bool isSelected)
//{
//  auto success = false;
//  int i = 0;
//
//  for (auto modelObject : m_modelObjects){
//    OSItemId itemId = modelObjectToItemId(modelObject, false);
//    if (item->itemId() == itemId){
//      selectRow(rowIndexFromModelIndex(i), isSelected);
//      success = true;
//      break;
//    }
//    i++;
//  }
//  return success;
//}

bool OSGridController::getRowIndexByItem(OSItem * item, int & rowIndex)
{
  auto success = false;
  rowIndex = -1;

  for (auto modelObject : m_modelObjects){
    rowIndex++;
    OSItemId itemId = modelObjectToItemId(modelObject, false);
    if (item->itemId() == itemId){
      success = true;
      break;
    }
  }

  if (!success) {
    // At this point, none of the itemIds exactly matched,
    // let's try to match a subset.
    rowIndex = -1;

    QString handle(""), handle2("");
    QStringList strings = item->itemId().otherData().split(",");
    if (strings.size() > 2){
      QString temp = strings[2];
      QStringList strings = temp.split(";");
      if (strings.size() > 0){
        handle = strings[0];
      }
    }

    for (auto modelObject : m_modelObjects){
      rowIndex++;
      OSItemId itemId = modelObjectToItemId(modelObject, false);
      QStringList strings = itemId.otherData().split(",");
      if (strings.size() > 2){
        QString temp = strings[2];
        QStringList strings = temp.split(";");
        if (strings.size() > 0){
          handle2 = strings[0];
        }
      }

      if (handle == handle2){
        success = true;
        break;
      }
    }
  }

  if (success) {
    // We found the model index and must convert it to the row index
    rowIndex = rowIndexFromModelIndex(rowIndex);
  }
  else {
    // We could never find a valid index
    rowIndex = -1;
  }

  return success;
}

OSItem * OSGridController::getSelectedItemFromModelSubTabView()
{
  OSItem * item = nullptr;

  auto modelSubTabView = gridView()->modelSubTabView();
  if (!modelSubTabView) return item;

  item = modelSubTabView->itemSelector()->selectedItem();

  return item;
}

void OSGridController::connectToModel()
{
  connect(m_model.getImpl<model::detail::Model_Impl>().get(),
    static_cast<void (model::detail::Model_Impl::*)(const WorkspaceObject &, const IddObjectType &, const UUID &) const>(&model::detail::Model_Impl::addWorkspaceObject),
    this,
    &OSGridController::onAddWorkspaceObject);

  connect(m_model.getImpl<model::detail::Model_Impl>().get(),
    static_cast<void (model::detail::Model_Impl::*)(const WorkspaceObject &, const IddObjectType &, const UUID &) const>(&model::detail::Model_Impl::removeWorkspaceObject),
    this,
    &OSGridController::onRemoveWorkspaceObject);
}

void OSGridController::disconnectFromModel()
{
  disconnect(m_model.getImpl<openstudio::model::detail::Model_Impl>().get());
}

void OSGridController::onSelectionCleared()
{
}

void OSGridController::onDropZoneItemClicked(OSItem* item)
{
}

void OSGridController::onRemoveWorkspaceObject(const WorkspaceObject& object, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle)
{
  auto modelObject = object.cast<model::ModelObject>();
  auto weHaveObject = false;

  if (m_objectSelector->containsObject(modelObject))
  {
    m_objectSelector->objectRemoved(object.cast<model::ModelObject>());
    weHaveObject = true;
  }

  //if (m_iddObjectType == iddObjectType) { TODO uncomment
    // Update model list
    std::vector<model::ModelObject>::iterator it;
    it = std::find(m_modelObjects.begin(), m_modelObjects.end(), object.cast<model::ModelObject>());
    if (it != m_modelObjects.end()) {
      int index = std::distance(m_modelObjects.begin(), it);
      OS_ASSERT(index >= 0);
      m_modelObjects.erase(m_modelObjects.begin() + index);

      // Update row
      gridView()->requestRemoveRow(rowIndexFromModelIndex(index));
    } else if (weHaveObject) {
      // we know we are tracking this object, but it's not one of the row-major ones...
      // must be a subrow... for now, not ideal, but let's queue a refresh of the grid
      requestRefreshGrid();
    }
  //}
}

void OSGridController::onAddWorkspaceObject(const WorkspaceObject& object, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle)
{
  //if (m_iddObjectType == iddObjectType) { TODO uncomment, currently used to update views with extensible dropzones, which need to issue their own signal to refresh
    // Update model list
    // m_modelObjects.push_back(object.cast<model::ModelObject>());
    refreshModelObjects();

    // Update row
    gridView()->requestAddRow(rowCount()-1);
  //}
}

void OSGridController::onObjectRemoved(boost::optional<model::ParentObject> parent)
{
  if (parent) {
    // We have a parent we can search for in our current list of modelObjects and just delete that 1 row
    this->requestRefreshGrid(); // TODO replace this with a by-row refresh only
  }
  else {
    // We don't know which row needs to be redrawn, so we have to do the whole grid
    this->requestRefreshGrid();
  }
}

void OSGridController::selectAllStateChanged(const int newState) const
{
  LOG(Debug, "Select all state changed: " << newState);

  auto objectSelector = getObjectSelector();
  if (newState == 0)
  {
    objectSelector->clearSelection();
  } else {
    objectSelector->selectAll();
  }
}

void OSGridController::onInFocus(bool inFocus, bool hasData, int row, int column, boost::optional<int> subrow)
{
  // First thing to do is to check if row is 0, because that means that the apply button was clicked
  if (row == 0 && this->m_hasHorizontalHeader) {
    // Do great things
    auto selectedRow = std::get<0>(m_selectedCellLocation);
    auto selectedColumn = std::get<1>(m_selectedCellLocation);
    auto selectedSubrow = std::get<2>(m_selectedCellLocation);
    OS_ASSERT(selectedColumn == column);

    m_selectedCellLocation = std::make_tuple(row, column, subrow);

    std::set<model::ModelObject> selectedObjects = this->m_objectSelector->getSelectedObjects();

    QSharedPointer<DataSourceAdapter> dataSource = m_baseConcepts[column].dynamicCast<DataSourceAdapter>();
    if (selectedSubrow && dataSource) {
      // Sub rows present, either in a widget, or in a row
      const DataSource &source = dataSource->source();
      QSharedPointer<BaseConcept> dropZoneConcept = source.dropZoneConcept();
      boost::optional<const model::ModelObject &> object = this->m_objectSelector->getObject(selectedRow, selectedColumn, selectedSubrow);
      if (object) {
        for (auto modelObject : selectedObjects) {
          // Don't set the chosen object when iterating through the selected objects
          if (modelObject != object.get()) {
            OS_ASSERT(dataSource.data()->innerConcept());
            if (dropZoneConcept) {
              // Widget has sub rows
              setConceptValue(modelObject, object.get(), dropZoneConcept, dataSource.data()->innerConcept());
            }
            else {
              // Row has sub rows
              setConceptValue(modelObject, object.get(), dataSource.data()->innerConcept());
            }
          }
        }
      }
    }
    else if (!selectedSubrow) {
      for (auto modelObject : selectedObjects) {
        // Don't set the chosen object when iterating through the selected objects
        if (modelObject != this->modelObject(selectedRow)) {
          setConceptValue(modelObject, this->modelObject(selectedRow), m_baseConcepts[column]);
        }
      }
    }
    else {
      // Should never get here
      OS_ASSERT(false);
    }

    // Now refresh
    gridView()->requestRefreshGrid(); // TODO this is heavy handed; each cell should update itself

  } else {
    HorizontalHeaderWidget * horizontalHeaderWidget = qobject_cast<HorizontalHeaderWidget *>(m_horizontalHeader.at(column));
    OS_ASSERT(horizontalHeaderWidget);
    auto button = horizontalHeaderWidget->m_pushButton;
    OS_ASSERT(button);

    bool hasData = true;

    if (inFocus) {
      m_selectedCellLocation = std::make_tuple(row, column, subrow);
      auto widget = m_objectSelector->getWidget(row, column, subrow);
      if (widget && qobject_cast<OSDropZone2 *>(widget)) {
        hasData = qobject_cast<OSDropZone2 *>(widget)->hasData();
      }

      if (hasData){
        button->setText("Apply to Selected");
      }
      else {
        //button->setText("Clear Selected");
        button->setText("Apply to Selected");
      }
    }
    else {
      button->setText("Apply to Selected");
    }

    m_applyToButtonStates.push_back(std::make_pair(column, inFocus && hasData));

    QTimer::singleShot(0, this, SLOT(setApplyButtonState()));
  }
}

void OSGridController::setApplyButtonState()
{
  for (auto pair : m_applyToButtonStates) {
    HorizontalHeaderWidget * horizontalHeaderWidget = qobject_cast<HorizontalHeaderWidget *>(m_horizontalHeader.at(pair.first));
    OS_ASSERT(horizontalHeaderWidget);
    auto button = horizontalHeaderWidget->m_pushButton;
    OS_ASSERT(button);
    button->setEnabled(pair.second);
  }

  m_applyToButtonStates.clear();
}

HorizontalHeaderPushButton::HorizontalHeaderPushButton(QWidget * parent)
  : QPushButton()
{
  QString style =
    "QPushButton {"
    "    font-size: 8pt;"
    "}";
  setStyleSheet(style);
  setFocusPolicy(Qt::StrongFocus);
}

HorizontalHeaderPushButton::~HorizontalHeaderPushButton()
{
}

void HorizontalHeaderPushButton::focusInEvent(QFocusEvent * e)
{
  if (e->reason() == Qt::MouseFocusReason)
  {
    emit inFocus(true, true); // TODO should hasData = true???
  }
  
  QPushButton::focusInEvent(e);
}

void HorizontalHeaderPushButton::focusOutEvent(QFocusEvent * e)
{
  if (e->reason() == Qt::MouseFocusReason)
  {
    emit inFocus(false, false);
  }
  
  QPushButton::focusOutEvent(e);
}

//ColumnSizer::mouseMoveEvent ( QMouseEvent * event )
//{
//  if( event->buttons == Qt::LeftButton ) {
//    
//  }
//}

Holder::Holder(QWidget * parent)
  : QWidget(parent)
{
}

Holder::~Holder()
{
}

void Holder::paintEvent(QPaintEvent *)
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

HorizontalHeaderWidget::HorizontalHeaderWidget(const QString & fieldName, QWidget * parent)
  : QWidget(parent),
  m_label(new QLabel(fieldName, this)),
  m_checkBox(new QPushButton(this)),
  m_pushButton(new HorizontalHeaderPushButton(this))
{
  auto mainLayout = new QVBoxLayout(this);
  mainLayout->setContentsMargins(0,0,0,5);
  mainLayout->setAlignment(Qt::AlignCenter);
  setLayout(mainLayout);

  mainLayout->addWidget(m_checkBox);
  m_checkBox->setToolTip("Check to add this column to \"Custom\"");
  m_checkBox->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
  QString style = "\
    QPushButton {\
      border: none;\
      background: #808080;\
      padding: 0px;\
    }\
    QPushButton:checked {\
      background: #94b3de;\
    }\
  ";

  m_checkBox->setStyleSheet(style);
  m_checkBox->setCheckable(true);

  m_label->setWordWrap(true);
  m_label->setStyleSheet("QLabel { padding: 5px; }");
  m_label->setAlignment(Qt::AlignHCenter);
  mainLayout->addWidget(m_label);

  mainLayout->addStretch();

  m_pushButton->setText("Apply to Selected");
  m_pushButton->setMaximumWidth(150);
  m_pushButton->setEnabled(false);
  connect(m_pushButton, &HorizontalHeaderPushButton::inFocus, this, &HorizontalHeaderWidget::inFocus);
  mainLayout->addWidget(m_pushButton,0,Qt::AlignCenter);
}

HorizontalHeaderWidget::~HorizontalHeaderWidget()
{
  for (auto &widget : m_addedWidgets)
  {
    layout()->removeWidget(widget.data());
    widget->setVisible(false);
    widget->setParent(nullptr);
  }
}

void HorizontalHeaderWidget::addWidget(const QSharedPointer<QWidget> &t_widget)
{
  if (!t_widget.isNull()) {
    m_addedWidgets.push_back(t_widget);
    auto hLayout = new QHBoxLayout();
    hLayout->setContentsMargins(5,0,5,0);
    qobject_cast<QVBoxLayout *>(layout())->addLayout(hLayout);
    hLayout->addWidget(t_widget.data());
    t_widget->setVisible(true);
  }
}

} // openstudio
