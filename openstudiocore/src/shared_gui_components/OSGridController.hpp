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

#ifndef SHAREDGUICOMPONENTS_OSGRIDCONTROLLER_HPP
#define SHAREDGUICOMPONENTS_OSGRIDCONTROLLER_HPP

#include "OSConcepts.hpp"

#include "../openstudio_lib/OSItem.hpp"
#include "../openstudio_lib/OSVectorController.hpp"

#include "../model/Model.hpp"
#include "../model/ModelObject.hpp"

#include "../utilities/idd/IddObject.hpp"

#include <string>
#include <functional>
#include <vector>

#include <QObject>
#include <QPushButton>
#include <QSharedPointer>
#include <QWidget>
#include <QVBoxLayout>

class QButtonGroup;
class QCheckBox;
class QColor;
class QLabel;
class QPaintEvent;

namespace openstudio {

class OSComboBox;
class OSGridView;

// forward declaration
class Holder;

/// Provides a Concept with an alternative source of data.
///
/// Generally, Concepts take a ModelObject and display some field of it.
///
/// This has been extended with the ProxyAdapter, which inserts itself between the `getter`, `setter` 
/// and the Concept. This ProxyAdapter allows you to not just display data with the ModelObject
/// but to actually display data on something the ModelObject contains, for example, showing the name
/// of a SizingZone which is associated with a ThermalZone
///
/// DataSource takes the ProxyAdapter concept one step further, by allowing you to examine a `std::vector`
/// of items which are associated with the ModelObject that was passed in. The vector could be a list
/// of one, if you were so inclined, and all of the interactions with the ModelObject
/// are abstracted via std::function objects, so the data you display can truly come from anywhere.
///
///
/// Essentially, by using this class you are creating a stack of widgets in the grid cell:
///
/// \code
/// <widget for t_sourceFunc item 1>
/// <widget for t_sourceFunc item 2>
/// ...
/// <widget for t_sourceFunc item N>
/// <widget for t_dropZoneConcept (see notes below)>
/// <space holder widget if enabled>
/// \endcode
class DataSource
{
  public:

    /// Create a new DataSource object, which is optionally passed into the various
    /// add.*Column functions.
    ///
    ///
    /// \param[in] t_sourceFunc Takes an Input object (practically speaking, a ModelObject derivation) and returns a vector 
    ///                         of items (probably some derived class from ModelObject)
    /// \param[in] t_wantsPlaceholder Informs the system to add a placeholder gap at the bottom of the list when it is 
    ///                         rendering the list of widgets. This is to help non-dropzone enabled stacks of widgets
    ///                         line up with those stacks that do contain dropzones
    /// \param[in] t_dropZoneConcept The `BaseConcept` you want displayed at the bottom of the list of widgets.
    ///                         It will be displayed after the PlaceHolder, but it is probably illogical to enable both.
    ///                         It is named "t_dropZoneConcept" because it's intended for DropZone widgets. However,
    ///                         it's rendered with the function OSGridController::makeWidget function just like every
    ///                         other widget, so it can really be anything.
    template<typename ItemType, typename InputType>
    DataSource(
        const std::function<std::vector<ItemType> (InputType)> &t_sourceFunc,
        bool t_wantsPlaceholder = false,
        const QSharedPointer<BaseConcept> &t_dropZoneConcept = QSharedPointer<BaseConcept>()
        )
        : m_wantsPlaceholder(t_wantsPlaceholder),
        m_dropZoneConcept(t_dropZoneConcept)
    {
      typedef decltype(t_sourceFunc) IncommingFuncType;
      typedef typename std::remove_reference<typename std::remove_cv<IncommingFuncType>::type>::type FunctionType;
      typedef typename std::remove_reference<typename std::remove_cv<typename FunctionType::argument_type>::type>::type ParamType;

      m_sourceFunc = [t_sourceFunc](ConceptProxy t_proxy)  {
        auto result = t_sourceFunc(t_proxy.cast<ParamType>());
        return std::vector<boost::optional<ConceptProxy>>(result.begin(), result.end());
      };
    }

    std::vector<boost::optional<ConceptProxy>> items(const ConceptProxy &t_proxy) const
    {
      return m_sourceFunc(t_proxy);
    }

    QSharedPointer<BaseConcept> dropZoneConcept() const
    {
      return m_dropZoneConcept;
    }

    bool wantsPlaceholder() const
    {
      return m_wantsPlaceholder;
    }

  private:
    std::function<std::vector<boost::optional<ConceptProxy>> (const ConceptProxy &)> m_sourceFunc;
    bool m_wantsPlaceholder;
    QSharedPointer<BaseConcept> m_dropZoneConcept;
};

class DataSourceAdapter : public BaseConcept
{
  public:
    DataSourceAdapter(DataSource t_source, QSharedPointer<BaseConcept> t_inner)
      : BaseConcept(t_inner->heading()), m_source(t_source), m_inner(t_inner)
    {
    }

    const DataSource &source() const
    {
      return m_source;
    }

    QSharedPointer<BaseConcept> innerConcept() const
    {
      return m_inner;
    }

  private:
    DataSource m_source;
    QSharedPointer<BaseConcept> m_inner;
};

class OSGridController;

class WidgetLocation : public QObject
{
  Q_OBJECT;

public:

  WidgetLocation(QWidget *t_widget, int t_row, int t_column, boost::optional<int> t_subrow);

  virtual ~WidgetLocation();

  QWidget * widget;
  int row;
  int column;
  boost::optional<int> subrow;

signals:

  void inFocus(bool inFocus, bool hasData, int row, int column, boost::optional<int> subrow);

public slots:

  void onInFocus(bool hasFocus, bool hasData);

};

class ObjectSelector : public QObject
{
  Q_OBJECT;

  public:
    ObjectSelector(OSGridController *t_grid);

    void addWidget(const boost::optional<model::ModelObject> &t_obj, Holder *t_holder, int row, int column, 
        const boost::optional<int> &subrow, bool t_selector);
    void setObjectSelection(const model::ModelObject &t_obj, bool t_selected);
    bool getObjectSelection(const model::ModelObject &t_obj) const;
    boost::optional<const model::ModelObject &> getObject(const int t_row, const int t_column, const boost::optional<int> &t_subrow);
    QWidget * getWidget(const int t_row, const int t_column, const boost::optional<int> &t_subrow);
    std::set<model::ModelObject> getSelectedObjects() const;
    std::vector<QWidget *> getColumnsSelectedWidgets(int column);
    void clear();
    void objectRemoved(const openstudio::model::ModelObject &t_obj);
    void setObjectFilter(const std::function<bool (const model::ModelObject &)> &t_filter);
    void resetObjectFilter();
    bool containsObject(const openstudio::model::ModelObject &t_obj) const;
    void selectAll();
    void clearSelection();

  signals:
    void inFocus(bool inFocus, bool hasData, int row, int column, boost::optional<int> subrow);

  private slots:
    void widgetDestroyed(QObject *t_obj);

  private:
    void updateWidgets();
    void updateWidgets(const model::ModelObject &t_obj);
    void updateWidgets(const int t_row, const boost::optional<int> &t_subrow, bool t_selected, bool t_visible);
    static std::function<bool (const model::ModelObject &)> getDefaultFilter();

    OSGridController *m_grid;
    std::multimap<boost::optional<model::ModelObject>, WidgetLocation *> m_widgetMap;
    std::set<model::ModelObject> m_selectedObjects;
    std::set<model::ModelObject> m_selectorObjects;
    std::function<bool (const model::ModelObject &)> m_objectFilter;
};

class OSGridController : public QObject
{
  Q_OBJECT

public:
  // This form requires clients to subclass OSGridController and
  // reimplement rowCount(), and itemAt()
  OSGridController();

  // This form utilizes the default implementations of
  // rowCount() and itemAt(), showing one row for each object
  // in the model that is iddObjectType
  OSGridController(bool isIP,
    const QString & headerText,
    IddObjectType iddObjectType,
    model::Model model,
    std::vector<model::ModelObject> modelObjects);

  virtual ~OSGridController();

  static QSharedPointer<BaseConcept> makeDataSourceAdapter(const QSharedPointer<BaseConcept> &t_inner,
      const boost::optional<DataSource> &t_source)
  {
    if (t_source)
    {
      return QSharedPointer<BaseConcept>(new DataSourceAdapter(*t_source, t_inner));
    } else {
      // if there is no t_source passed in, we don't want to wrap, just pass through
      return t_inner;
    }
  }

  void addSelectColumn(const Heading &heading,
                       const std::string & tooltip,
                       const boost::optional<DataSource> &t_source = boost::none)
  {
    auto objectSelector = m_objectSelector;
    auto getter = std::function<bool (model::ModelObject *)>([objectSelector](model::ModelObject *t_obj) -> bool {
      assert(t_obj);
      return objectSelector->getObjectSelection(*t_obj);
    });

    auto setter = std::function<void (model::ModelObject *, bool)>([objectSelector](model::ModelObject *t_obj, bool t_set) {
      assert(t_obj);
      objectSelector->setObjectSelection(*t_obj, t_set);
    });

    addCheckBoxColumn(heading, tooltip, getter, setter, t_source);
    m_baseConcepts.back()->setIsSelector(true);
  }

  template<typename DataSourceType>
  void addCheckBoxColumn(const Heading &heading,
                         const std::string & tooltip,
                         std::function<bool (DataSourceType *)>  t_getter,
                         std::function<void (DataSourceType *, bool)> t_setter,
                         const boost::optional<DataSource> &t_source = boost::none)
  {
    m_baseConcepts.push_back(makeDataSourceAdapter(QSharedPointer<CheckBoxConcept>(new CheckBoxConceptImpl<DataSourceType>(heading,tooltip,t_getter,t_setter)), t_source));
  }

  template<typename ChoiceType, typename DataSourceType>
  void addComboBoxColumn(const Heading &heading,
                         std::function<std::string (const ChoiceType &)> toString,
                         std::function<std::vector<ChoiceType> ()> choices,
                         std::function<ChoiceType (DataSourceType*)> getter,
                         std::function<bool (DataSourceType*, ChoiceType)> setter,
                         const boost::optional<std::function<void (DataSourceType *)>> &reset = boost::none,
                         const boost::optional<std::function<bool (DataSourceType *)>> &isDefaulted = boost::none,
                         const boost::optional<DataSource> &t_source = boost::none)
  {
    addComboBoxColumn<ChoiceType, DataSourceType>(
        heading,
        toString,
        std::function<std::vector<ChoiceType> (DataSourceType*)>([choices](DataSourceType*) { return choices(); }),
        getter,
        setter,
        reset,
        isDefaulted,
        t_source);
  }

  template<typename ChoiceType, typename DataSourceType>
  void addComboBoxColumn(const Heading &heading,
                         std::function<std::string (const ChoiceType &)> toString,
                         std::function<std::vector<ChoiceType> ()> choices,
                         std::function<boost::optional<ChoiceType> (DataSourceType*)> getter,
                         std::function<bool (DataSourceType*, ChoiceType)> setter,
                         boost::optional<std::function<void (DataSourceType*)> > reset=boost::none,
                         const boost::optional<DataSource> &t_source = boost::none,
                         bool editable = true)
  {
    addComboBoxColumn<ChoiceType, DataSourceType>(
          heading,
          toString,
          std::function<std::vector<ChoiceType> (DataSourceType*)>([choices](DataSourceType*) { return choices(); }),
          getter,
          setter,
          reset,
          t_source,
          editable);
  }

  template<typename ChoiceType, typename DataSourceType>
  void addComboBoxColumn(const Heading &heading,
                         std::function<std::string (const ChoiceType &)> toString,
                         std::function<std::vector<ChoiceType> (DataSourceType *)> choices,
                         std::function<ChoiceType (DataSourceType*)> getter,
                         std::function<bool (DataSourceType*, ChoiceType)> setter,
                         const boost::optional<std::function<void (DataSourceType *)>> reset = boost::none,
                         const boost::optional<std::function<bool (DataSourceType *)>> isDefaulted = boost::none,
                         const boost::optional<DataSource> &t_source = boost::none)
  {
    m_baseConcepts.push_back(makeDataSourceAdapter(QSharedPointer<ComboBoxConcept>(
        new ComboBoxRequiredChoiceImpl<ChoiceType,DataSourceType>(heading,
                                                                  toString,
                                                                  choices,
                                                                  getter,
                                                                  setter,
                                                                  reset,
                                                                  isDefaulted)), t_source));
  }

  template<typename ChoiceType, typename DataSourceType>
  void addComboBoxColumn(const Heading &heading,
                         std::function<std::string (const ChoiceType &)> toString,
                         std::function<std::vector<ChoiceType> (DataSourceType *)> choices,
                         std::function<ChoiceType (DataSourceType*)> getter,
                         std::function<bool (DataSourceType*, ChoiceType)> setter,
                         const boost::optional<DataSource> &t_source = boost::none)
  {
    m_baseConcepts.push_back(makeDataSourceAdapter(QSharedPointer<ComboBoxConcept>(
        new ComboBoxRequiredChoiceImpl<ChoiceType,DataSourceType>(heading,
                                                                  toString,
                                                                  choices,
                                                                  getter,
                                                                  setter)), t_source));
  }

  template<typename ChoiceType, typename DataSourceType>
  void addComboBoxColumn(const Heading &heading,
                         std::function<std::string (const ChoiceType &)> toString,
                         std::function<std::vector<ChoiceType> (DataSourceType *)> choices,
                         std::function<boost::optional<ChoiceType> (DataSourceType*)> getter,
                         std::function<bool (DataSourceType*, ChoiceType)> setter,
                         boost::optional<std::function<void (DataSourceType*)> > reset=boost::none,
                         const boost::optional<DataSource> &t_source = boost::none,
                         bool editable = false)
  {
    m_baseConcepts.push_back(makeDataSourceAdapter(QSharedPointer<ComboBoxConcept>(
      new ComboBoxOptionalChoiceImpl<ChoiceType,DataSourceType>(heading,
                                                                toString,
                                                                choices,
                                                                getter,
                                                                setter,
                                                                reset,
                                                                editable)), t_source));
  }

  template<typename ValueType, typename DataSourceType>
  void addValueEditColumn(const Heading &heading,
                          std::function<ValueType (DataSourceType *)>  getter,
                          std::function<bool (DataSourceType *, ValueType)> setter,
                          const boost::optional<std::function<void (DataSourceType *)>> reset = boost::none,
                          const boost::optional<std::function<bool (DataSourceType *)>> isDefaulted = boost::none,
                          const boost::optional<DataSource> &t_source = boost::none)
  {
    m_baseConcepts.push_back(makeDataSourceAdapter(QSharedPointer<ValueEditConcept<ValueType> >(new ValueEditConceptImpl<ValueType, DataSourceType>(heading,getter,setter, reset, isDefaulted)), t_source));
  }

  template<typename ValueType, typename DataSourceType>
  void addValueEditColumn(const Heading &heading,
                          std::function<boost::optional<ValueType> (DataSourceType *)>  getter,
                          std::function<bool (DataSourceType *, ValueType)> setter,
                          const boost::optional<DataSource> &t_source = boost::none)
  {
    m_baseConcepts.push_back(makeDataSourceAdapter(QSharedPointer<OptionalValueEditConcept<ValueType> >(new OptionalValueEditConceptImpl<ValueType, DataSourceType>(heading,getter,setter)), t_source));
  }

  template<typename ValueType, typename DataSourceType>
  void addValueEditColumn(const Heading &heading,
                          std::function<ValueType (DataSourceType *)>  getter,
                          std::function<void (DataSourceType *, ValueType)> setter,
                          const boost::optional<std::function<void (DataSourceType *)>> reset = boost::none,
                          const boost::optional<std::function<bool (DataSourceType *)>> isDefaulted = boost::none,
                          const boost::optional<DataSource> &t_source = boost::none)
  {
    m_baseConcepts.push_back(makeDataSourceAdapter(QSharedPointer<ValueEditVoidReturnConcept<ValueType> >(new ValueEditVoidReturnConceptImpl<ValueType, DataSourceType>(heading,getter,setter, reset, isDefaulted)), t_source));
  }

  template<typename ValueType, typename DataSourceType>
  void addValueEditColumn(const Heading &heading,
                          std::function<boost::optional<ValueType> (DataSourceType *)>  getter,
                          std::function<void (DataSourceType *, double)> setter,
                          const boost::optional<DataSource> &t_source = boost::none)
  {
    m_baseConcepts.push_back(makeDataSourceAdapter(QSharedPointer<OptionalValueEditVoidReturnConcept<ValueType> >(new OptionalValueEditVoidReturnConceptImpl<ValueType, DataSourceType>(heading,getter,setter)), t_source));
  }

  template<typename DataSourceType>
  void addNameLineEditColumn(const Heading &heading,
                             bool isInspectable,
                             bool deleteObject,
                             const std::function<boost::optional<std::string> (DataSourceType *, bool)>  &getter,
                             const std::function<boost::optional<std::string> (DataSourceType *, const std::string &)> &setter,
                             const boost::optional<std::function<void (DataSourceType *)>> &resetter = boost::none,
                             const boost::optional<DataSource> &t_source = boost::none)
  {
    m_baseConcepts.push_back(makeDataSourceAdapter(QSharedPointer<NameLineEditConcept>(new NameLineEditConceptImpl<DataSourceType>(heading, isInspectable, deleteObject, getter, setter, resetter)), t_source));
  }

  template<typename DataSourceType>
  void addLoadNameColumn(const Heading &heading,
    const std::function<boost::optional<std::string>(DataSourceType *, bool)>  &getter,
    const std::function<boost::optional<std::string>(DataSourceType *, const std::string &)> &setter,
    const boost::optional<std::function<void(DataSourceType *)>> &resetter = boost::none,
    const boost::optional<DataSource> &t_source = boost::none)
  {
    m_baseConcepts.push_back(makeDataSourceAdapter(QSharedPointer<LoadNameConcept>(new LoadNameConceptImpl<DataSourceType>(heading, getter, setter, resetter)), t_source));
  }

  template<typename ValueType, typename DataSourceType>
  void addQuantityEditColumn(const Heading &heading,
                             QString modelUnits,
                             QString siUnits,
                             QString ipUnits,
                             bool isIP,
                             std::function<ValueType (DataSourceType *)>  getter,
                             std::function<bool (DataSourceType *, ValueType)> setter,
                             const boost::optional<std::function<void (DataSourceType *)>> reset = boost::none,
                             const boost::optional<std::function<bool (DataSourceType *)>> isDefaulted = boost::none,
                             const boost::optional<DataSource> &t_source = boost::none)
  {
    m_baseConcepts.push_back(makeDataSourceAdapter(QSharedPointer<QuantityEditConcept<ValueType> >(new QuantityEditConceptImpl<ValueType, DataSourceType>(heading, modelUnits, siUnits, ipUnits, isIP, getter, setter, reset, isDefaulted)), t_source));
  }

  template<typename ValueType, typename DataSourceType>
  void addQuantityEditColumn(const Heading &heading,
                             QString modelUnits,
                             QString siUnits,
                             QString ipUnits,
                             bool isIP,
                             std::function<boost::optional<ValueType> (DataSourceType *)>  getter,
                             std::function<bool (DataSourceType *, ValueType)> setter,
                             const boost::optional<DataSource> &t_source = boost::none)
  {
    m_baseConcepts.push_back(makeDataSourceAdapter(QSharedPointer<OptionalQuantityEditConcept<ValueType> >(new OptionalQuantityEditConceptImpl<ValueType, DataSourceType>(heading, modelUnits, siUnits, ipUnits, isIP, getter, setter)), t_source));
  }

  template<typename ValueType, typename DataSourceType>
  void addQuantityEditColumn(const Heading &heading,
                             QString modelUnits,
                             QString siUnits,
                             QString ipUnits,
                             bool isIP,
                             std::function<ValueType (DataSourceType *)>  getter,
                             std::function<void (DataSourceType *, ValueType)> setter,
                             const boost::optional<std::function<void (DataSourceType *)>> reset = boost::none,
                             const boost::optional<std::function<bool (DataSourceType *)>> isDefaulted = boost::none,
                             const boost::optional<DataSource> &t_source = boost::none)
  {
    m_baseConcepts.push_back(makeDataSourceAdapter(QSharedPointer<QuantityEditVoidReturnConcept<ValueType> >(new QuantityEditVoidReturnConceptImpl<ValueType, DataSourceType>(heading, modelUnits, siUnits, ipUnits, isIP, getter, setter, reset, isDefaulted)), t_source));
  }

  template<typename ValueType, typename DataSourceType>
  void addQuantityEditColumn(const Heading &heading,
                             QString modelUnits,
                             QString siUnits,
                             QString ipUnits,
                             bool isIP,
                             std::function<boost::optional<ValueType> (DataSourceType *)>  getter,
                             std::function<void (DataSourceType *, ValueType)> setter,
                             const boost::optional<DataSource> &t_source = boost::none)
  {
    m_baseConcepts.push_back(makeDataSourceAdapter(QSharedPointer<OptionalQuantityEditVoidReturnConcept<ValueType> >(new OptionalQuantityEditVoidReturnConceptImpl<ValueType, DataSourceType>(heading, modelUnits, siUnits, ipUnits, isIP, getter, setter)), t_source));
  }

  template<typename ValueType, typename DataSourceType>
  void addDropZoneColumn(const Heading &heading,
                         std::function<boost::optional<ValueType> (DataSourceType *)>  getter,
                         std::function<bool (DataSourceType *, const ValueType &)> setter,
                         boost::optional<std::function<void(DataSourceType*)> > reset = boost::none,
                         const boost::optional<DataSource> &t_source = boost::none)
  {
    m_baseConcepts.push_back(makeDataSourceAdapter(QSharedPointer<DropZoneConcept>(new DropZoneConceptImpl<ValueType, DataSourceType>(heading,getter,setter,reset)), t_source));
  }

  template<typename ValueType, typename DataSourceType>
  void addRenderingColorColumn(const Heading &heading,
    std::function<boost::optional<ValueType>(DataSourceType *)>  getter,
    std::function<bool(DataSourceType *, const ValueType &)> setter,
    const boost::optional<DataSource> &t_source = boost::none)
  {
    m_baseConcepts.push_back(QSharedPointer<RenderingColorConcept>(new RenderingColorConceptImpl<ValueType, DataSourceType>(heading, getter, setter)));
  }

  std::vector<QString> categories();

  std::vector<std::pair<QString,std::vector<QString> > > categoriesAndFields();

  virtual void categorySelected(int index);

  virtual int rowCount() const;

  virtual int columnCount() const;

  // Widget that exists at the given top level coordinates (may contain sub rows).
  // This will not create a new widget.
  QWidget * cell(int rowIndex, int columnIndex);

  model::ModelObject modelObject(int rowIndex);

  virtual std::vector<QWidget *> row(int rowIndex);

  void selectRow(int rowIndex, bool select);

  int rowIndexFromModelIndex(int modelIndex);

  // Return a new widget at a "top level" row and column specified by arguments.
  // There might be sub rows within the specified location.
  // In that case a QWidget with sub rows (innner grid layout) will be returned.
  QWidget * widgetAt(int row, int column);

  // Call this function on a model update
  virtual void refreshModelObjects() = 0;

  void connectToModel();

  void disconnectFromModel();

  std::shared_ptr<ObjectSelector> getObjectSelector() const { return m_objectSelector; }

protected:

  // This function determines the category for
  // each button, and the fields associated with
  // each category
  virtual void setCategoriesAndFields();

  // Call this function with the fields required,
  // and it adds the columns and does the binds.
  // This provides a mechanism to easily manage
  // a dynamic, user-preference column.
  // This function will be called from the slot
  // connected to the QButtonGroup signal
  virtual void addColumns(const QString &category, std::vector<QString> & fields) = 0;

  // Call this function to get the color for the cell color
  virtual QString getColor(const model::ModelObject & modelObject) = 0;

  // This function sets the column header caption
  virtual void setHorizontalHeader();

  // Call this function after the table is constructed
  // to appropriately check user-selected category fields
  // from QSettings and load them into a "Custom" button
  virtual void checkSelectedFields();

  void checkSelectedFields(int category);

  std::vector<std::pair<QString,std::vector<QString> > > m_categoriesAndFields;

  std::vector<QSharedPointer<BaseConcept> > m_baseConcepts;

  std::vector<QWidget *> m_horizontalHeader;

  bool m_hasHorizontalHeader;

  QString m_currentCategory;

  int m_currentCategoryIndex;

  std::vector<QString> m_currentFields;

  std::vector<QString> m_customFields;

  static const std::vector<QColor> m_colors;

  model::Model m_model;

  bool m_isIP;

  std::vector<model::ModelObject> m_modelObjects;

  IddObjectType m_iddObjectType;

  REGISTER_LOGGER("openstudio.OSGridController");

private:

  friend class OSGridView;
  friend class ObjectSelector;

  // Make the lowest level widgets that corresponds to concepts.
  // These will be put in container widgets to form the cell, regardless of the presence of sub rows.
  QWidget * makeWidget(model::ModelObject t_mo, const QSharedPointer<BaseConcept> &t_baseConcept);

  void loadQSettings();

  void saveQSettings() const;

  void setCustomCategoryAndFields();

  QString cellStyle(int rowIndex, int columnIndex, bool isSelected, bool isSubRow);

  OSGridView * gridView();

  OSItem * getSelectedItemFromModelSubTabView();

  bool getRowIndexByItem(OSItem * item, int & rowIndex);

  void setConceptValue(model::ModelObject t_setterMO, model::ModelObject t_getterMO, const QSharedPointer<BaseConcept> &t_baseConcept);

  void resetConceptValue(model::ModelObject t_resetMO, const QSharedPointer<BaseConcept> &t_baseConcept);

  void setConceptValue(model::ModelObject t_setterMO, model::ModelObject t_getterMO, const QSharedPointer<BaseConcept> &t_setterBaseConcept, const QSharedPointer<BaseConcept> &t_getterBaseConcept);

  QButtonGroup * m_horizontalHeaderBtnGrp;

  QString m_headerText;

  int m_oldIndex = -1;

  std::shared_ptr<ObjectSelector> m_objectSelector;

  std::tuple<int, int, boost::optional<int>> m_selectedCellLocation = std::make_tuple(-1, -1, -1);

  std::vector <std::pair<int, bool> > m_applyToButtonStates = std::vector < std::pair<int, bool> >();

signals:

  // Nuclear reset of everything
  void modelReset();

  void toggleUnitsClicked(bool displayIP);

  void gridRowSelected(OSItem*);

public slots:

  virtual void onItemDropped(const OSItemId& itemId) = 0;

  void toggleUnits(bool displayIP);

  virtual void onComboBoxIndexChanged(int index);

  void onItemSelected(OSItem * item);

  void onSelectionCleared();

  void refreshGrid();

  void requestRefreshGrid();

  void onInFocus(bool inFocus, bool hasData, int row, int column, boost::optional<int> subrow);

protected slots:

  void selectAllStateChanged(const int newState) const;

private slots:

  void horizontalHeaderChecked(int index);

  void onDropZoneItemClicked(OSItem* item);

  void onRemoveWorkspaceObject(const WorkspaceObject& object, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle);

  void onAddWorkspaceObject(const WorkspaceObject& object, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle);

  void onObjectRemoved(boost::optional<model::ParentObject> parent);

  void setApplyButtonState();

};

// Possible solution for user facing column resize 
// Hardst part is addressing persitance when grid redraws
//class ColumnSizer : public QWidget
//{
//  Q_OBJECT
//
//  void mouseMoveEvent ( QMouseEvent * event );
//}

class Holder : public QWidget
{
  Q_OBJECT

public:

  Holder(QWidget * parent = nullptr);

  virtual ~Holder();

  QWidget * widget = nullptr;

protected:

  void paintEvent(QPaintEvent * event);

signals:

  void inFocus(bool inFocus, bool hasData);

};

class HorizontalHeaderPushButton : public QPushButton
{
  Q_OBJECT

public:

  HorizontalHeaderPushButton(QWidget * parent = nullptr);

  virtual ~HorizontalHeaderPushButton();


protected:

  virtual void focusInEvent(QFocusEvent * e);

  virtual void focusOutEvent(QFocusEvent * e);

signals:

  void inFocus(bool inFocus, bool hasData);

};

class HorizontalHeaderWidget : public QWidget
{
  Q_OBJECT

public:

  HorizontalHeaderWidget(const QString & fieldName, QWidget * parent = nullptr);

  virtual ~HorizontalHeaderWidget();
  void addWidget(const QSharedPointer<QWidget> &t_widget);

  QLabel * m_label = nullptr;

  QPushButton * m_checkBox = nullptr;

  HorizontalHeaderPushButton * m_pushButton = nullptr;

  std::vector<QSharedPointer<QWidget>> m_addedWidgets;

signals:

  void inFocus(bool inFocus, bool hasData);

private:

  QVBoxLayout * m_innerLayout;
};

class GridViewDropZoneVectorController : public OSVectorController
{
  protected:

  virtual std::vector<OSItemId> makeVector() { return std::vector<OSItemId>(); }
};

} // openstudio

#endif // SHAREDGUICOMPONENTS_OSGRIDCONTROLLER_HPP
