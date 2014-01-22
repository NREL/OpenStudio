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

#ifndef OPENSTUDIO_OSGRIDCONTROLLER_H
#define OPENSTUDIO_OSGRIDCONTROLLER_H

#include <openstudio_lib/OSDropZone.hpp>

#include <shared_gui_components/FieldMethodTypedefs.hpp>
#include <shared_gui_components/OSCheckBox.hpp>
#include <shared_gui_components/OSComboBox.hpp>
#include <shared_gui_components/OSDoubleEdit.hpp>
#include <shared_gui_components/OSIntegerEdit.hpp>
#include <shared_gui_components/OSLineEdit.hpp>
#include <shared_gui_components/OSQuantityEdit.hpp>
#include <shared_gui_components/OSUnsignedEdit.hpp>

#include <model/Model.hpp>
#include <model/ModelObject.hpp>

#include <utilities/idd/IddObject.hpp>

#include <QObject>
#include <QSharedPointer>

class QWidget;

namespace openstudio {

// TODO Move these ComboBoxConcept classes to another file.
class ComboBoxConcept
{
  public:

  virtual std::vector<std::string> choices() = 0;
  virtual std::string get(const model::ModelObject & obj) = 0;
  virtual bool set(const model::ModelObject & obj, std::string) = 0;
}; 

template<typename DataSourceType>
class ComboBoxConceptImpl : public ComboBoxConcept
{
  public:

  ComboBoxConceptImpl(QString t_headingLabel, 
    boost::function<std::vector<std::string> (void)> t_choices, 
    boost::function<std::string (DataSourceType *)>  t_getter, 
    boost::function<bool (DataSourceType *, std::string)> t_setter)

    : m_headingLabel(t_headingLabel),
      m_choices(t_choices),
      m_getter(t_getter),
      m_setter(t_setter)

  {
  }

  QString headingLabel() const { return m_headingLabel; }

  virtual std::vector<std::string> choices()
  {
    return m_choices();
  }

  virtual std::string get(const model::ModelObject & t_obj)
  {
    DataSourceType obj = t_obj.cast<DataSourceType>();
    return m_getter(&obj);
  }

  virtual bool set(const model::ModelObject & t_obj, std::string value)
  {
    DataSourceType obj = t_obj.cast<DataSourceType>();
    return m_setter(&obj,value);
  }

  private:

  QString m_headingLabel;
  boost::function<std::vector<std::string> (void)> m_choices;
  boost::function<std::string (DataSourceType *)>  m_getter;
  boost::function<bool (DataSourceType *, std::string)> m_setter;
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
  OSGridController(IddObjectType iddObjectType, model::Model model);

  virtual ~OSGridController();
  
  template<typename DataSourceType>
  void addComboBoxColumn(QString headingLabel, 
                         std::vector<std::string> (* choices)(void), 
                         std::string (DataSourceType::* getter)(void) const, 
                         bool (DataSourceType::* setter)(std::string))
  {
    m_comboBoxConcepts.push_back(QSharedPointer<ComboBoxConcept>(new ComboBoxConceptImpl<DataSourceType>(headingLabel,choices,getter,setter)));
  }

  virtual int rowCount() const;

  virtual int columnCount() const;

  virtual std::vector<QWidget *> row(int i);

  QWidget * widgetAt(int i, int j);

  private:

  enum ColumnType
  {
    CHECKBOX,
    COMBOBOX,
    DOUBLE,
    DROPZONE,
    INTEGER,
    LINEEDIT,
    QUANTITY,
    UNSIGNED,
    NOTVALID
  };

  //bool addCheckBoxColumn(std::string property);  
  
  bool addDoubleEditColumn(QString property);

  bool addDropZoneColumn(QString property);

  bool addIntegerEditColumn(std::string property);

  bool addLineEditColumn(std::string property);

  bool addQuantityEditColumn(std::string property);

  bool addUnsignedEditColumn(std::string property);

  void setVerticalHeader(bool visible, QString title);

  void setHorizontalHeader(std::vector<QWidget> widgets);

  void setHorizontalHeader(std::vector<QString> names);

  model::Model m_model;

  IddObjectType m_iddObjectType;

  std::vector<ColumnType> m_columnTypes; 

  std::vector<QSharedPointer<ComboBoxConcept> > m_comboBoxConcepts;

  std::vector<model::ModelObject> m_modelObjects;

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
};

} // openstudio

#endif // OPENSTUDIO_OSGRIDCONTROLLER_H

