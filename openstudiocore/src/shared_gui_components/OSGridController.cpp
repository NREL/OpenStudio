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
  emit inFocus(hasFocus, hasData, row, column);
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

  m_cellCheckBoxBtnGrp = new QButtonGroup();
  m_cellCheckBoxBtnGrp->setExclusive(false);

  bool isConnected = false;
  isConnected = connect(m_cellCheckBoxBtnGrp, SIGNAL(buttonClicked(int)), this, SLOT(cellChecked(int)));
  OS_ASSERT(isConnected);

  m_cellPushButtonBoxBtnGrp = new QButtonGroup();
  m_cellPushButtonBoxBtnGrp->setExclusive(false);

  isConnected = connect(m_cellPushButtonBoxBtnGrp, SIGNAL(buttonClicked(int)), this, SLOT(cellChecked(int)));
  OS_ASSERT(isConnected);

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

    auto checkBox = new OSCheckBox3(); // OSCheckBox3 is derived from QCheckBox, whereas OSCheckBox2 is derived from QPushButton
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

    auto comboBox = new OSComboBox2(nullptr, choiceConcept->editable());

    comboBox->bind(t_mo, choiceConcept);

    widget = comboBox;

    isConnected = connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxIndexChanged(int)));
    OS_ASSERT(isConnected);

  } else if(QSharedPointer<ValueEditConcept<double> > doubleEditConcept = t_baseConcept.dynamicCast<ValueEditConcept<double> >()) {

    auto doubleEdit = new OSDoubleEdit2();

    doubleEdit->bind(t_mo,
                     DoubleGetter(std::bind(&ValueEditConcept<double>::get,doubleEditConcept.data(),t_mo)),
                     boost::optional<DoubleSetter>(std::bind(&ValueEditConcept<double>::set,doubleEditConcept.data(),t_mo,std::placeholders::_1)),
                     boost::optional<NoFailAction>(std::bind(&ValueEditConcept<double>::reset,doubleEditConcept.data(),t_mo)),
                     boost::optional<NoFailAction>(),
                     boost::optional<NoFailAction>(),
                     boost::optional<BasicQuery>(std::bind(&ValueEditConcept<double>::isDefaulted,doubleEditConcept.data(),t_mo)));

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
                               DoubleSetterVoidReturn(std::bind(&ValueEditVoidReturnConcept<double>::set,doubleEditVoidReturnConcept.data(),t_mo,std::placeholders::_1)),
                     boost::optional<NoFailAction>(std::bind(&ValueEditVoidReturnConcept<double>::reset,doubleEditVoidReturnConcept.data(),t_mo)),
                     boost::optional<NoFailAction>(),
                     boost::optional<NoFailAction>(),
                     boost::optional<BasicQuery>(std::bind(&ValueEditVoidReturnConcept<double>::isDefaulted,doubleEditVoidReturnConcept.data(),t_mo)));

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
                      boost::optional<IntSetter>(std::bind(&ValueEditConcept<int>::set,integerEditConcept.data(),t_mo,std::placeholders::_1)),
                      boost::optional<NoFailAction>(std::bind(&ValueEditConcept<int>::reset,integerEditConcept.data(),t_mo)),
                      boost::optional<NoFailAction>(),
                      boost::optional<NoFailAction>(),
                      boost::optional<BasicQuery>(std::bind(&ValueEditConcept<int>::isDefaulted,integerEditConcept.data(),t_mo)));

    widget = integerEdit;

  } else if(QSharedPointer<ValueEditConcept<std::string> > lineEditConcept = t_baseConcept.dynamicCast<ValueEditConcept<std::string> >()) {

    auto lineEdit = new OSLineEdit2();

    lineEdit->bind(t_mo,
                   StringGetter(std::bind(&ValueEditConcept<std::string>::get,lineEditConcept.data(),t_mo)),
                   boost::optional<StringSetter>(std::bind(&ValueEditConcept<std::string>::set,lineEditConcept.data(),t_mo,std::placeholders::_1)),
                   boost::optional<NoFailAction>(std::bind(&ValueEditConcept<std::string>::reset,lineEditConcept.data(),t_mo)),
                   boost::optional<BasicQuery>(std::bind(&ValueEditConcept<std::string>::isDefaulted,lineEditConcept.data(),t_mo)));

    isConnected = connect(lineEdit, SIGNAL(objectRemoved(boost::optional<model::ParentObject>)), this, SLOT(onObjectRemoved(boost::optional<model::ParentObject>)));
    OS_ASSERT(isConnected);

    widget = lineEdit;

  } else if(QSharedPointer<LoadNameConcept> loadNameConcept = t_baseConcept.dynamicCast<LoadNameConcept>()) {

    auto loadName = new OSLoadNamePixmapLineEdit();

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

    auto nameLineEdit = new OSLineEdit2();

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
          quantityEditConcept->isIP());

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
          optionalQuantityEditConcept->isIP());

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
          quantityEditVoidReturnConcept->isIP());

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
          optionalQuantityEditVoidReturnConcept->isIP());

    optionalQuantityEditVoidReturn->bind(m_isIP,
                                         t_mo,
                                         OptionalDoubleGetter(std::bind(&OptionalQuantityEditVoidReturnConcept<double>::get,optionalQuantityEditVoidReturnConcept.data(),t_mo)),
                                         DoubleSetterVoidReturn(std::bind(&OptionalQuantityEditVoidReturnConcept<double>::set,optionalQuantityEditVoidReturnConcept.data(),t_mo,std::placeholders::_1)));

    isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), optionalQuantityEditVoidReturn, SLOT(onUnitSystemChange(bool)));
    OS_ASSERT(isConnected);

    widget = optionalQuantityEditVoidReturn;

  } else if(QSharedPointer<ValueEditConcept<unsigned> > unsignedEditConcept = t_baseConcept.dynamicCast<ValueEditConcept<unsigned> >()) {

    auto unsignedEdit = new OSUnsignedEdit2();

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
    auto * renderingColorWidget = new RenderingColorWidget2();

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

OSGridView * OSGridController::gridView(){
  auto gridView = qobject_cast<OSGridView *>(this->parent());
  OS_ASSERT(gridView);
  return gridView;
}

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
  if (columnIndex == 0){
    style.append("                      border-left: 1px solid black;");
  }
  style.append("                        border-right: 1px solid black;");
  style.append("                        border-bottom: 1px solid black;");
  style.append("}");

  style.append("QWidget#InnerCell { border: none;");
  style.append("                        background-color: " + cellColor + ";");
  if (columnIndex == 0){
    style.append("                      border-left: 1px solid black;");
  }
  style.append("                        border-right: 1px solid black;");
  style.append("}");

  style.append("QWidget#InnerCellBottom { border: none;");
  style.append("                        background-color: " + cellColor + ";");
  if (columnIndex == 0){
    style.append("                      border-left: 1px solid black;");
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

  auto layout = new QGridLayout();
  const int widgetHeight = 35;
  int numWidgets = 0;

  // start with a default sane value
  QSize recommendedSize(100, 20);
  bool hasSubRows = false;

  // wrapper
  auto wrapper = new QWidget();

  wrapper->setObjectName("TableCell");

  wrapper->setStyleSheet(this->cellStyle(row,column, false, true));

  layout->setSpacing(0);
  layout->setContentsMargins(0,0,0,0);
  wrapper->setLayout(layout);
  // end wrapper

  std::vector<Holder *> holders;
  auto addWidget = [&](QWidget *t_widget, const boost::optional<model::ModelObject> &t_obj, const bool t_selector)
  {
    auto expand=[](QSize &t_s, const QSize &t_newS) {
      if (t_newS.height() < 400 && t_newS.width() < 600)
      {
        t_s = t_s.expandedTo(t_newS);
      } else if (t_newS.height() < 400) {
        t_s = t_s.expandedTo(QSize(std::min(t_newS.width(), 600), 0));
      }
    };

    expand(recommendedSize, t_widget->size());
    expand(recommendedSize, t_widget->minimumSize());
    expand(recommendedSize, t_widget->minimumSizeHint());

    auto holder = new Holder();
    holder->setMinimumHeight(widgetHeight);
    auto l = new QVBoxLayout();
    l->setSpacing(0);
    if(row == 0){
      l->setContentsMargins(0,0,0,0);
    } else {
      l->setContentsMargins(5,0,5,0);
    }
    l->addWidget(t_widget, 0, Qt::AlignVCenter | Qt::AlignLeft);
    holder->setLayout(l);
    holder->setContentsMargins(0,0,0,0);
    layout->addWidget(holder, numWidgets, 0, 0);

    if (hasSubRows) {
      holder->setObjectName("InnerCell");
    }
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

    m_objectSelector->addWidget(t_obj, holder, row, column, hasSubRows?numWidgets:boost::optional<int>(), t_selector);

    ++numWidgets;
    expand(recommendedSize, t_widget->size());
  };

  if(m_hasHorizontalHeader && row == 0){
    if(column == 0){
      setHorizontalHeader();
      // Each concept should have its own column
      OS_ASSERT(m_horizontalHeader.size() == m_baseConcepts.size());
    }
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
          addWidget(new QWidget(), boost::none, false);
        }
      }

      if (dataSource->source().wantsPlaceholder())
      {
        // use this space to put in a blank placeholder of some kind to make sure the 
        // widget is evenly laid out relative to its friends in the adjacent columns
        addWidget(new QWidget(), boost::none, false);
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

  for (auto &holder : holders)
  {
    if(row == 0){
      holder->setMinimumSize(QSize(recommendedSize.width(),recommendedSize.height()));
    } else {
      holder->setMinimumSize(QSize(recommendedSize.width() + 10,widgetHeight));
    }
  }

  if(row == 0){
    wrapper->setMinimumSize(QSize(recommendedSize.width(),recommendedSize.height()));
  }

  wrapper->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

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
  // Currently this is cruft code
  OS_ASSERT(false);

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
//  int columnIndex = 0;
  std::vector<QWidget *> row = this->row(rowIndex);
  for (auto widget : row){
    auto button = qobject_cast<QPushButton *>(widget);
    if (!button){
      return;
    }
    button->blockSignals(true);
    button->setChecked(select);
    button->blockSignals(false);
//    button->setStyleSheet(cellStyle(rowIndex, columnIndex++, select));
  }

  //OSItem * item = nullptr; // TODO reevaluate
  //OSAppBase::instance()->currentDocument()->mainRightColumnController()->inspectModelObjectByItem(item, true);
  //qApp->processEvents();
  //OSAppBase::instance()->currentDocument()->mainRightColumnController()->mainRightColumnView()->setCurrentId(MainRightColumnController::LIBRARY);
  //qApp->processEvents();
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
  // Currently this is cruft code
  OS_ASSERT(false);
}

void OSGridController::reset()
{
  // Currently this is cruft code
  OS_ASSERT(false);
}

void OSGridController::cellChecked(int index)
{
  // Currently this is cruft code
  OS_ASSERT(false);

  int tableRowIndex = index;
  if (m_hasHorizontalHeader){
    // The header is not considered part of the table row count
    tableRowIndex++;
  }

  if (tableRowIndex == m_oldIndex) {
    // Note: 1 row must always be checked
    QAbstractButton * button = nullptr;
    button = m_cellCheckBoxBtnGrp->button(index);
    OS_ASSERT(button);
    button->blockSignals(true);
    button->setChecked(true);
    button->blockSignals(false);
  }
  else {
    // Deselect the old row...
    if (m_oldIndex >= 0) selectRow(m_oldIndex, false);

    // ... select the new...
    selectRow(tableRowIndex, true);

    // ... tell the world...
    OSItemId itemId = modelObjectToItemId(modelObject(tableRowIndex), false);
    OSItem* item = OSItem::makeItem(itemId, OSItemType::ListItem);
    emit gridRowSelected(item);

    // ... and remember who's selected
    m_oldIndex = tableRowIndex;
  }
}

void OSGridController::selectItemId(const OSItemId& itemId)
{
  // Currently this is cruft code
  OS_ASSERT(false);
}

void OSGridController::onItemSelected(OSItem * item)
{
}

bool OSGridController::selectRowByItem(OSItem * item, bool isSelected)
{
  // Currently this is cruft code
  OS_ASSERT(false);

  auto success = false;
  int i = 0;

  for (auto modelObject : m_modelObjects){
    OSItemId itemId = modelObjectToItemId(modelObject, false);
    if (item->itemId() == itemId){
      selectRow(rowIndexFromModelIndex(i), isSelected);
      success = true;
      break;
    }
    i++;
  }
  return success;
}

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
  // Currently this is cruft code
  OS_ASSERT(false);

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

void OSGridController::onInFocus(bool inFocus, bool hasData, int row, int column)
{
  m_selectedCellLocation = std::make_pair(row, column);

  HorizontalHeaderWidget * horizontalHeaderWidget = qobject_cast<HorizontalHeaderWidget *>(m_horizontalHeader.at(column));
  OS_ASSERT(horizontalHeaderWidget);

  horizontalHeaderWidget->m_pushButton->setEnabled(inFocus);

  if (hasData){
    horizontalHeaderWidget->m_pushButton->setText("Apply to Selected");
  }
  else {
    horizontalHeaderWidget->m_pushButton->setText("Apply to Selected");
    //horizontalHeaderWidget->m_pushButton->setText("Clear Selected"); TODO
  }
}

Holder::Holder(QWidget * parent)
  : QWidget(parent)
{
}

Holder::~Holder()
{
}

HorizontalHeaderWidget::HorizontalHeaderWidget(const QString & fieldName, QWidget * parent)
  : QWidget(parent),
  m_label(new QLabel(fieldName)),
  m_checkBox(new QCheckBox()),
  m_pushButton(new QPushButton())
{
  auto mainLayout = new QVBoxLayout();
  setLayout(mainLayout);

  auto layout = new QHBoxLayout();

  m_label->setWordWrap(true);
  m_label->setAlignment(Qt::AlignCenter);
  layout->addWidget(m_label);

  m_checkBox->setToolTip("Check to add this column to \"Custom\"");
  layout->addWidget(m_checkBox);

  mainLayout->addLayout(layout);

  m_pushButton->setText("Apply to Selected");
  m_pushButton->setFixedWidth(100);
  m_pushButton->setEnabled(false);
  mainLayout->addWidget(m_pushButton, Qt::AlignBottom);
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
    layout()->addWidget(t_widget.data());
    t_widget->setVisible(true);
  }
}

} // openstudio
