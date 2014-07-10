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

  template<typename DataSourceType>
  void addCheckBoxColumn(QString headingLabel,
                         bool (DataSourceType::* getter)(void) const,
                         void (DataSourceType::* setter)(bool))
  {
    m_baseConcepts.push_back(QSharedPointer<CheckBoxConcept>(new CheckBoxConceptImpl<DataSourceType>(headingLabel,getter,setter)));
  }

  template<typename ChoiceType, typename DataSourceType>
  void addComboBoxColumn(QString headingLabel,
                         boost::function<std::string (ChoiceType)> toString,
                         boost::function<std::vector<ChoiceType> ()> choices,
                         boost::function<ChoiceType (DataSourceType*)> getter,
                         boost::function<bool (DataSourceType*, ChoiceType)> setter)
  {
    m_baseConcepts.push_back(QSharedPointer<ComboBoxConcept>(
        new ComboBoxRequiredChoiceImpl<ChoiceType,DataSourceType>(headingLabel,
                                                                  toString,
                                                                  choices,
                                                                  getter,
                                                                  setter)));
  }

  template<typename ChoiceType, typename DataSourceType>
  void addComboBoxColumn(QString headingLabel,
                         boost::function<std::string (ChoiceType)> toString,
                         boost::function<std::vector<ChoiceType> ()> choices,
                         boost::function<boost::optional<ChoiceType> (DataSourceType*)> getter,
                         boost::function<bool (DataSourceType*, ChoiceType)> setter,
                         boost::optional<boost::function<void (DataSourceType*)> > reset=boost::none)
  {
    m_baseConcepts.push_back(QSharedPointer<ComboBoxConcept>(
      new ComboBoxOptionalChoiceImpl<ChoiceType,DataSourceType>(headingLabel,
                                                                toString,
                                                                choices,
                                                                getter,
                                                                setter,
                                                                reset)));
  }

  template<typename ValueType, typename DataSourceType>
  void addValueEditColumn(QString headingLabel,
                         ValueType (DataSourceType::* getter)(void) const,
                         bool (DataSourceType::* setter)(ValueType))
  {
    m_baseConcepts.push_back(QSharedPointer<ValueEditConcept<ValueType> >(new ValueEditConceptImpl<ValueType, DataSourceType>(headingLabel,getter,setter)));
  }

  template<typename ValueType, typename DataSourceType>
  void addValueEditColumn(QString headingLabel,
                         boost::optional<ValueType> (DataSourceType::* getter)(void) const,
                         bool (DataSourceType::* setter)(ValueType))
  {
    m_baseConcepts.push_back(QSharedPointer<OptionalValueEditConcept<ValueType> >(new OptionalValueEditConceptImpl<ValueType, DataSourceType>(headingLabel,getter,setter)));
  }

  template<typename ValueType, typename DataSourceType>
  void addValueEditColumn(QString headingLabel,
                         ValueType (DataSourceType::* getter)(void) const,
                         void (DataSourceType::* setter)(ValueType))
  {
    m_baseConcepts.push_back(QSharedPointer<ValueEditVoidReturnConcept<ValueType> >(new ValueEditVoidReturnConceptImpl<ValueType, DataSourceType>(headingLabel,getter,setter)));
  }

  template<typename ValueType, typename DataSourceType>
  void addValueEditColumn(QString headingLabel,
                         boost::optional<ValueType> (DataSourceType::* getter)(void) const,
                         void (DataSourceType::* setter)(double))
  {
    m_baseConcepts.push_back(QSharedPointer<OptionalValueEditVoidReturnConcept<ValueType> >(new OptionalValueEditVoidReturnConceptImpl<ValueType, DataSourceType>(headingLabel,getter,setter)));
  }

  template<typename DataSourceType>
  void addNameLineEditColumn(QString headingLabel,
                         boost::optional<std::string> (DataSourceType::* getter)(bool) const,
                         boost::optional<std::string> (DataSourceType::* setter)(const std::string &))
  {
    m_baseConcepts.push_back(QSharedPointer<NameLineEditConcept>(new NameLineEditConceptImpl<DataSourceType>(headingLabel,getter,setter)));
  }

  template<typename ValueType, typename DataSourceType>
  void addQuantityEditColumn(QString headingLabel,
                             QString modelUnits,
                             QString siUnits,
                             QString ipUnits,
                             bool isIP,
                             ValueType (DataSourceType::* getter)(void) const,
                             bool (DataSourceType::* setter)(ValueType))
  {
    m_baseConcepts.push_back(QSharedPointer<QuantityEditConcept<ValueType> >(new QuantityEditConceptImpl<ValueType, DataSourceType>(headingLabel, modelUnits, siUnits, ipUnits, isIP, getter, setter)));
  }

  template<typename ValueType, typename DataSourceType>
  void addQuantityEditColumn(QString headingLabel,
                             QString modelUnits,
                             QString siUnits,
                             QString ipUnits,
                             bool isIP,
                             boost::optional<ValueType> (DataSourceType::* getter)(void) const,
                             bool (DataSourceType::* setter)(ValueType))
  {
    m_baseConcepts.push_back(QSharedPointer<OptionalQuantityEditConcept<ValueType> >(new OptionalQuantityEditConceptImpl<ValueType, DataSourceType>(headingLabel, modelUnits, siUnits, ipUnits, isIP, getter, setter)));
  }

  template<typename ValueType, typename DataSourceType>
  void addQuantityEditColumn(QString headingLabel,
                             QString modelUnits,
                             QString siUnits,
                             QString ipUnits,
                             bool isIP,
                             ValueType (DataSourceType::* getter)(void) const,
                             void (DataSourceType::* setter)(ValueType))
  {
    m_baseConcepts.push_back(QSharedPointer<QuantityEditVoidReturnConcept<ValueType> >(new QuantityEditVoidReturnConceptImpl<ValueType, DataSourceType>(headingLabel, modelUnits, siUnits, ipUnits, isIP, getter, setter)));
  }

  template<typename ValueType, typename DataSourceType>
  void addQuantityEditColumn(QString headingLabel,
                             QString modelUnits,
                             QString siUnits,
                             QString ipUnits,
                             bool isIP,
                             boost::optional<ValueType> (DataSourceType::* getter)(void) const,
                             void (DataSourceType::* setter)(ValueType))
  {
    m_baseConcepts.push_back(QSharedPointer<OptionalQuantityEditVoidReturnConcept<ValueType> >(new OptionalQuantityEditVoidReturnConceptImpl<ValueType, DataSourceType>(headingLabel, modelUnits, siUnits, ipUnits, isIP, getter, setter)));
  }

  template<typename ValueType, typename DataSourceType>
  void addDropZoneColumn(QString headingLabel,
                         boost::optional<ValueType> (DataSourceType::* getter)(void) const,
                         bool (DataSourceType::* setter)(const ValueType &))
  {
    m_baseConcepts.push_back(QSharedPointer<DropZoneConcept>(new DropZoneConceptImpl<ValueType, DataSourceType>(headingLabel,getter,setter)));
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

