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

#ifndef SHAREDGUICOMPONENTS_OSGRIDCONTROLLER_HPP
#define SHAREDGUICOMPONENTS_OSGRIDCONTROLLER_HPP

#include "OSConcepts.hpp"

#include "../openstudio_lib/OSItem.hpp"
#include "../openstudio_lib/OSVectorController.hpp"

#include "../model/Model.hpp"
#include "../model/ModelObject.hpp"

#include "../utilities/idd/IddObject.hpp"

#include <boost/function.hpp>

#include <QObject>
#include <QSharedPointer>
#include <QWidget>

class QButtonGroup;
class QCheckBox;
class QColor;
class QLabel;

namespace openstudio {

class OSComboBox;

class DataSource
{
  public:
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
        return std::vector<ConceptProxy>(result.begin(), result.end());
      };
    }


    std::vector<ConceptProxy> items(const ConceptProxy &t_proxy) const
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
    std::function<std::vector<ConceptProxy> (const ConceptProxy &)> m_sourceFunc;
    bool m_wantsPlaceholder;
    QSharedPointer<BaseConcept> m_dropZoneConcept;
};

class DataSourceAdapter : public BaseConcept
{
  public:
    DataSourceAdapter(DataSource t_source, QSharedPointer<BaseConcept> t_inner)
      : BaseConcept(t_inner->headingLabel()), m_source(t_source), m_inner(t_inner)
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


  template<typename DataSourceType>
  void addCheckBoxColumn(QString headingLabel,
                         std::function<bool (DataSourceType *)>  t_getter,
                         std::function<void (DataSourceType *, bool)> t_setter,
                         const boost::optional<DataSource> &t_source = boost::none)
  {
    m_baseConcepts.push_back(makeDataSourceAdapter(QSharedPointer<CheckBoxConcept>(new CheckBoxConceptImpl<DataSourceType>(headingLabel,t_getter,t_setter)), t_source));
  }

  template<typename ChoiceType, typename DataSourceType>
  void addComboBoxColumn(QString headingLabel,
                         std::function<std::string (const ChoiceType &)> toString,
                         std::function<std::vector<ChoiceType> ()> choices,
                         std::function<ChoiceType (DataSourceType*)> getter,
                         std::function<bool (DataSourceType*, const ChoiceType &)> setter,
                         const boost::optional<DataSource> &t_source = boost::none)
  {
    m_baseConcepts.push_back(makeDataSourceAdapter(QSharedPointer<ComboBoxConcept>(
        new ComboBoxRequiredChoiceImpl<ChoiceType,DataSourceType>(headingLabel,
                                                                  toString,
                                                                  choices,
                                                                  getter,
                                                                  setter)), t_source));
  }


  template<typename ChoiceType, typename DataSourceType>
  void addComboBoxColumn(QString headingLabel,
                         std::function<std::string (ChoiceType)> toString,
                         std::function<std::vector<ChoiceType> ()> choices,
                         std::function<ChoiceType (DataSourceType*)> getter,
                         std::function<bool (DataSourceType*, ChoiceType)> setter,
                         const boost::optional<DataSource> &t_source = boost::none)
  {
    m_baseConcepts.push_back(makeDataSourceAdapter(QSharedPointer<ComboBoxConcept>(
        new ComboBoxRequiredChoiceImpl<ChoiceType,DataSourceType>(headingLabel,
                                                                  toString,
                                                                  choices,
                                                                  getter,
                                                                  setter)), t_source));
  }

  template<typename ChoiceType, typename DataSourceType>
  void addComboBoxColumn(QString headingLabel,
                         std::function<std::string (ChoiceType)> toString,
                         std::function<std::vector<ChoiceType> ()> choices,
                         std::function<boost::optional<ChoiceType> (DataSourceType*)> getter,
                         std::function<bool (DataSourceType*, ChoiceType)> setter,
                         boost::optional<std::function<void (DataSourceType*)> > reset=boost::none,
                         const boost::optional<DataSource> &t_source = boost::none)
  {
    m_baseConcepts.push_back(makeDataSourceAdapter(QSharedPointer<ComboBoxConcept>(
      new ComboBoxOptionalChoiceImpl<ChoiceType,DataSourceType>(headingLabel,
                                                                toString,
                                                                choices,
                                                                getter,
                                                                setter,
                                                                reset)), t_source));
  }

  template<typename ValueType, typename DataSourceType>
  void addValueEditColumn(QString headingLabel,
                          std::function<ValueType (DataSourceType *)>  getter,
                          std::function<bool (DataSourceType *, ValueType)> setter,
                          const boost::optional<DataSource> &t_source = boost::none)
  {
    m_baseConcepts.push_back(makeDataSourceAdapter(QSharedPointer<ValueEditConcept<ValueType> >(new ValueEditConceptImpl<ValueType, DataSourceType>(headingLabel,getter,setter)), t_source));
  }

  template<typename ValueType, typename DataSourceType>
  void addValueEditColumn(QString headingLabel,
                          std::function<boost::optional<ValueType> (DataSourceType *)>  getter,
                          std::function<bool (DataSourceType *, ValueType)> setter,
                          const boost::optional<DataSource> &t_source = boost::none)
  {
    m_baseConcepts.push_back(makeDataSourceAdapter(QSharedPointer<OptionalValueEditConcept<ValueType> >(new OptionalValueEditConceptImpl<ValueType, DataSourceType>(headingLabel,getter,setter)), t_source));
  }

  template<typename ValueType, typename DataSourceType>
  void addValueEditColumn(QString headingLabel,
                          std::function<ValueType (DataSourceType *)>  getter,
                          std::function<void (DataSourceType *, ValueType)> setter,
                          const boost::optional<DataSource> &t_source = boost::none)
  {
    m_baseConcepts.push_back(makeDataSourceAdapter(QSharedPointer<ValueEditVoidReturnConcept<ValueType> >(new ValueEditVoidReturnConceptImpl<ValueType, DataSourceType>(headingLabel,getter,setter)), t_source));
  }

  template<typename ValueType, typename DataSourceType>
  void addValueEditColumn(QString headingLabel,
                          std::function<boost::optional<ValueType> (DataSourceType *)>  getter,
                          std::function<void (DataSourceType *, double)> setter,
                          const boost::optional<DataSource> &t_source = boost::none)
  {
    m_baseConcepts.push_back(makeDataSourceAdapter(QSharedPointer<OptionalValueEditVoidReturnConcept<ValueType> >(new OptionalValueEditVoidReturnConceptImpl<ValueType, DataSourceType>(headingLabel,getter,setter)), t_source));
  }

  template<typename DataSourceType>
  void addNameLineEditColumn(QString headingLabel,
                             const std::function<boost::optional<std::string> (DataSourceType *, bool)>  &getter,
                             const std::function<boost::optional<std::string> (DataSourceType *, const std::string &)> &setter,
                             const boost::optional<DataSource> &t_source = boost::none)
  {
    m_baseConcepts.push_back(makeDataSourceAdapter(QSharedPointer<NameLineEditConcept>(new NameLineEditConceptImpl<DataSourceType>(headingLabel,getter,setter)), t_source));
  }

  template<typename ValueType, typename DataSourceType>
  void addQuantityEditColumn(QString headingLabel,
                             QString modelUnits,
                             QString siUnits,
                             QString ipUnits,
                             bool isIP,
                             std::function<ValueType (DataSourceType *)>  getter,
                             std::function<bool (DataSourceType *, ValueType)> setter,
                             const boost::optional<DataSource> &t_source = boost::none)
  {
    m_baseConcepts.push_back(makeDataSourceAdapter(QSharedPointer<QuantityEditConcept<ValueType> >(new QuantityEditConceptImpl<ValueType, DataSourceType>(headingLabel, modelUnits, siUnits, ipUnits, isIP, getter, setter)), t_source));
  }

  template<typename ValueType, typename DataSourceType>
  void addQuantityEditColumn(QString headingLabel,
                             QString modelUnits,
                             QString siUnits,
                             QString ipUnits,
                             bool isIP,
                             std::function<boost::optional<ValueType> (DataSourceType *)>  getter,
                             std::function<bool (DataSourceType *, ValueType)> setter,
                             const boost::optional<DataSource> &t_source = boost::none)
  {
    m_baseConcepts.push_back(makeDataSourceAdapter(QSharedPointer<OptionalQuantityEditConcept<ValueType> >(new OptionalQuantityEditConceptImpl<ValueType, DataSourceType>(headingLabel, modelUnits, siUnits, ipUnits, isIP, getter, setter)), t_source));
  }

  template<typename ValueType, typename DataSourceType>
  void addQuantityEditColumn(QString headingLabel,
                             QString modelUnits,
                             QString siUnits,
                             QString ipUnits,
                             bool isIP,
                             std::function<ValueType (DataSourceType *)>  getter,
                             std::function<void (DataSourceType *, ValueType)> setter,
                             const boost::optional<DataSource> &t_source = boost::none)
  {
    m_baseConcepts.push_back(makeDataSourceAdapter(QSharedPointer<QuantityEditVoidReturnConcept<ValueType> >(new QuantityEditVoidReturnConceptImpl<ValueType, DataSourceType>(headingLabel, modelUnits, siUnits, ipUnits, isIP, getter, setter)), t_source));
  }

  template<typename ValueType, typename DataSourceType>
  void addQuantityEditColumn(QString headingLabel,
                             QString modelUnits,
                             QString siUnits,
                             QString ipUnits,
                             bool isIP,
                             std::function<boost::optional<ValueType> (DataSourceType *)>  getter,
                             std::function<void (DataSourceType *, ValueType)> setter,
                             const boost::optional<DataSource> &t_source = boost::none)
  {
    m_baseConcepts.push_back(makeDataSourceAdapter(QSharedPointer<OptionalQuantityEditVoidReturnConcept<ValueType> >(new OptionalQuantityEditVoidReturnConceptImpl<ValueType, DataSourceType>(headingLabel, modelUnits, siUnits, ipUnits, isIP, getter, setter)), t_source));
  }

  template<typename ValueType, typename DataSourceType>
  void addDropZoneColumn(QString headingLabel,
                         std::function<ValueType (DataSourceType *)>  getter,
                         std::function<bool (DataSourceType *, const ValueType &)> setter,
                         const boost::optional<DataSource> &t_source = boost::none)
  {
    m_baseConcepts.push_back(makeDataSourceAdapter(makeDropZoneConcept(headingLabel, getter, setter), t_source));
  }

  template<typename ValueType, typename DataSourceType>
  QSharedPointer<DropZoneConcept<ValueType> > makeDropZoneConcept(QString headingLabel,
                         std::function<ValueType (DataSourceType *)>  getter,
                         std::function<bool (DataSourceType *, const ValueType &)> setter)
  {
    return QSharedPointer<DropZoneConcept<ValueType> >(new DropZoneConceptImpl<ValueType, DataSourceType>(headingLabel,getter,setter));
  }

  std::vector<QString> categories();

  std::vector<std::pair<QString,std::vector<QString> > > categoriesAndFields();

  void categorySelected(int index);

  virtual int rowCount() const;

  virtual int columnCount() const;

  virtual std::vector<QWidget *> row(int i);

  QWidget * widgetAt(int row, int column);

  // Call this function on a model update
  virtual void refreshModelObjects() = 0;

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
  virtual void addColumns(std::vector<QString> & fields) = 0;

  // Call this function to get the color for the cell color
  virtual QString getColor(const model:: ModelObject & modelObject) = 0;

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

private:

  QWidget * makeWidget(model::ModelObject t_mo, const QSharedPointer<BaseConcept> &t_baseConcept);

  void loadQSettings();

  void saveQSettings() const;

  void setCustomCategoryAndFields();

  QButtonGroup * m_horizontalHeaderBtnGrp;

  QString m_headerText;

signals:

  // These signals will be used by the GridView to update

  // Emit this signal when an item has been added to the underlying model or data structure.
  void itemInserted(int row, int column);

  // Emit this signal when an item has been removed from the underlying model or data structure.
  void itemRemoved(int row, int column);

  // If the model or data structure beneath a single item has changed,
  // emit this signal to update the view attached to the item at this index.
  void itemChanged(int row, int column);

  // Nuclear reset of everything
  void modelReset();

  void toggleUnitsClicked(bool displayIP);

public slots:

  virtual void onItemDropped(const OSItemId& itemId) = 0;

  void toggleUnits(bool displayIP);

  virtual void onComboBoxIndexChanged(int index);

protected slots:

  void reset();

private slots:

  void horizontalHeaderChecked(int index);

};

class HorizontalHeaderWidget : public QWidget
{
  Q_OBJECT

public:

  HorizontalHeaderWidget(const QString & fieldName, QWidget * parent = nullptr);

  virtual ~HorizontalHeaderWidget() {}

  QLabel * m_label;

  QCheckBox * m_checkBox;

};

// TODO Currently, this is a dead class
class BulkSelectionWidget : public QWidget
{
  Q_OBJECT

public:

  BulkSelectionWidget(QWidget * parent = nullptr);

  virtual ~BulkSelectionWidget() {}

  OSComboBox * m_comboBox;

  QCheckBox * m_checkBox;

};

class GridViewDropZoneVectorController : public OSVectorController
{
  protected:

  virtual std::vector<OSItemId> makeVector() { return std::vector<OSItemId>(); }
};

} // openstudio

#endif // SHAREDGUICOMPONENTS_OSGRIDCONTROLLER_HPP

