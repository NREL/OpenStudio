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

// TODO Move these Concept classes to another file.

class CheckBoxConcept
{
  public:

  virtual bool get(const model::ModelObject & obj) = 0;
  virtual void set(const model::ModelObject & obj, bool) = 0;
}; 

template<typename DataSourceType>
class CheckBoxConceptImpl : public CheckBoxConcept
{
  public:

  CheckBoxConceptImpl(QString t_headingLabel, 
    boost::function<bool (DataSourceType *)>  t_getter, 
    boost::function<void (DataSourceType *, bool)> t_setter)

    : m_headingLabel(t_headingLabel),
      m_getter(t_getter),
      m_setter(t_setter)

  {
  }

  QString headingLabel() const { return m_headingLabel; }

  virtual bool get(const model::ModelObject & t_obj)
  {
    DataSourceType obj = t_obj.cast<DataSourceType>();
    return m_getter(&obj);
  }

  virtual void set(const model::ModelObject & t_obj, bool)
  {
    DataSourceType obj = t_obj.cast<DataSourceType>();
    return m_setter(&obj,value);
  }

  private:

  QString m_headingLabel;
  boost::function<bool (DataSourceType *)>  m_getter;
  boost::function<bool (DataSourceType *, bool)> m_setter;
};


///////////////////////////////////////////////////////////////////////////////////


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


///////////////////////////////////////////////////////////////////////////////////


class DoubleEditConcept
{
  public:

  virtual double get(const model::ModelObject & obj) = 0;
  virtual bool set(const model::ModelObject & obj, double) = 0;
}; 

template<typename DataSourceType>
class DoubleEditConceptImpl : public DoubleEditConcept
{
  public:

  DoubleEditConceptImpl(QString t_headingLabel, 
    boost::function<double (DataSourceType *)>  t_getter, 
    boost::function<bool (DataSourceType *, double)> t_setter)

    : m_headingLabel(t_headingLabel),
      m_getter(t_getter),
      m_setter(t_setter)

  {
  }

  QString headingLabel() const { return m_headingLabel; }

  virtual double get(const model::ModelObject & t_obj)
  {
    DataSourceType obj = t_obj.cast<DataSourceType>();
    return m_getter(&obj);
  }

  virtual bool set(const model::ModelObject & t_obj, double value)
  {
    DataSourceType obj = t_obj.cast<DataSourceType>();
    return m_setter(&obj,value);
  }

  private:

  QString m_headingLabel;
  boost::function<double (DataSourceType *)>  m_getter;
  boost::function<bool (DataSourceType *, double)> m_setter;
};


///////////////////////////////////////////////////////////////////////////////////


class IntegerEditConcept
{
  public:

  virtual int get(const model::ModelObject & obj) = 0;
  virtual bool set(const model::ModelObject & obj, int) = 0;
}; 

template<typename DataSourceType>
class IntegerEditConceptImpl : public IntegerEditConcept
{
  public:

  IntegerEditConceptImpl(QString t_headingLabel,
    boost::function<int (DataSourceType *)>  t_getter, 
    boost::function<bool (DataSourceType *, int)> t_setter)

    : m_headingLabel(t_headingLabel),
      m_getter(t_getter),
      m_setter(t_setter)

  {
  }

  QString headingLabel() const { return m_headingLabel; }

  virtual int get(const model::ModelObject & t_obj)
  {
    DataSourceType obj = t_obj.cast<DataSourceType>();
    return m_getter(&obj);
  }

  virtual bool set(const model::ModelObject & t_obj, int)
  {
    DataSourceType obj = t_obj.cast<DataSourceType>();
    return m_setter(&obj,value);
  }

  private:

  QString m_headingLabel;
  boost::function<int (DataSourceType *)>  m_getter;
  boost::function<bool (DataSourceType *, int)> m_setter;
};


///////////////////////////////////////////////////////////////////////////////////
  

class LineEditConcept
{
  public:

  virtual std::string get(const model::ModelObject & obj) = 0;
  virtual bool set(const model::ModelObject & obj, std::string) = 0;
}; 

template<typename DataSourceType>
class LineEditConceptImpl : public LineEditConcept
{
  public:

  LineEditConceptImpl(QString t_headingLabel, 
    boost::function<std::string (DataSourceType *)>  t_getter, 
    boost::function<bool (DataSourceType *, std::string)> t_setter)

    : m_headingLabel(t_headingLabel),
      m_getter(t_getter),
      m_setter(t_setter)

  {
  }

  QString headingLabel() const { return m_headingLabel; }

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
  boost::function<std::string (DataSourceType *)>  m_getter;
  boost::function<bool (DataSourceType *, std::string)> m_setter;
};


///////////////////////////////////////////////////////////////////////////////////
  

class QuantityEditConcept
{
  public:

  virtual double get(const model::ModelObject & obj) = 0;
  virtual bool set(const model::ModelObject & obj, double) = 0;
}; 

template<typename DataSourceType>
class QuantityEditConceptImpl : public QuantityEditConcept
{
  public:

  QuantityEditConceptImpl(QString t_headingLabel, 
    boost::function<double (DataSourceType *)>  t_getter, 
    boost::function<bool (DataSourceType *, double)> t_setter)

    : m_headingLabel(t_headingLabel),
      m_getter(t_getter),
      m_setter(t_setter)

  {
  }

  QString headingLabel() const { return m_headingLabel; }

  virtual double get(const model::ModelObject & t_obj)
  {
    DataSourceType obj = t_obj.cast<DataSourceType>();
    return m_getter(&obj);
  }

  virtual bool set(const model::ModelObject & t_obj, double value)
  {
    DataSourceType obj = t_obj.cast<DataSourceType>();
    return m_setter(&obj,value);
  }

  private:

  QString m_headingLabel;
  boost::function<double (DataSourceType *)>  m_getter;
  boost::function<bool (DataSourceType *, double)> m_setter;
};


///////////////////////////////////////////////////////////////////////////////////
  

class UnsignedEditConcept
{
  public:

  virtual unsigned get(const model::ModelObject & obj) = 0;
  virtual bool set(const model::ModelObject & obj, unsigned) = 0;
}; 

template<typename DataSourceType>
class UnsignedEditConceptImpl : public UnsignedEditConcept
{
  public:

  UnsignedEditConceptImpl(QString t_headingLabel, 
    boost::function<unsigned (DataSourceType *)>  t_getter, 
    boost::function<bool (DataSourceType *, unsigned)> t_setter)

    : m_headingLabel(t_headingLabel),
      m_getter(t_getter),
      m_setter(t_setter)

  {
  }

  QString headingLabel() const { return m_headingLabel; }

  virtual unsigned get(const model::ModelObject & t_obj)
  {
    DataSourceType obj = t_obj.cast<DataSourceType>();
    return m_getter(&obj);
  }

  virtual bool set(const model::ModelObject & t_obj, unsigned value)
  {
    DataSourceType obj = t_obj.cast<DataSourceType>();
    return m_setter(&obj,value);
  }

  private:

  QString m_headingLabel;
  boost::function<unsigned (DataSourceType *)>  m_getter;
  boost::function<bool (DataSourceType *, unsigned)> m_setter;
};


///////////////////////////////////////////////////////////////////////////////////


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
  void addCheckBoxColumn(QString headingLabel, 
                         bool (DataSourceType::* getter)(void) const, 
                         void (DataSourceType::* setter)(bool))
  {
    m_checkBoxConcepts.push_back(QSharedPointer<CheckBoxConcept>(new CheckBoxConceptImpl<DataSourceType>(headingLabel,getter,setter)));
  }

  template<typename DataSourceType>
  void addComboBoxColumn(QString headingLabel, 
                         std::vector<std::string> (* choices)(void), 
                         std::string (DataSourceType::* getter)(void) const, 
                         bool (DataSourceType::* setter)(std::string))
  {
    m_comboBoxConcepts.push_back(QSharedPointer<ComboBoxConcept>(new ComboBoxConceptImpl<DataSourceType>(headingLabel,choices,getter,setter)));
  }

  template<typename DataSourceType>
  void addDoubleEditColumn(QString headingLabel, 
                         double (DataSourceType::* getter)(void) const, 
                         bool (DataSourceType::* setter)(double))
  {
    m_doubleEditConcepts.push_back(QSharedPointer<DoubleEditConcept>(new DoubleEditConceptImpl<DataSourceType>(headingLabel,getter,setter)));
  }

  template<typename DataSourceType>
  void addIntegerEditColumn(QString headingLabel, 
                         int (DataSourceType::* getter)(void) const, 
                         bool (DataSourceType::* setter)(int))
  {
    m_integerEditConcepts.push_back(QSharedPointer<IntegerEditConcept>(new IntegerEditConceptImpl<DataSourceType>(headingLabel,getter,setter)));
  }

  template<typename DataSourceType>
  void addLineEditColumn(QString headingLabel, 
                         std::string (DataSourceType::* getter)(void) const, 
                         bool (DataSourceType::* setter)(std::string))
  {
    m_lineEditConcepts.push_back(QSharedPointer<LineEditConcept>(new LineEditConceptImpl<DataSourceType>(headingLabel,getter,setter)));
  }

  template<typename DataSourceType>
  void addQuantityEditColumn(QString headingLabel, 
                         double (DataSourceType::* getter)(void) const, 
                         bool (DataSourceType::* setter)(double))
  {
    m_quantityEditConcepts.push_back(QSharedPointer<QuantityEditConcept>(new QuantityEditConceptImpl<DataSourceType>(headingLabel,getter,setter)));
  }

  template<typename DataSourceType>
  void addUnsignedEditColumn(QString headingLabel, 
                         unsigned (DataSourceType::* getter)(void) const, 
                         bool (DataSourceType::* setter)(unsigned))
  {
    m_unsignedEditConcepts.push_back(QSharedPointer<UnsignedEditConcept>(new UnsignedEditConceptImpl<DataSourceType>(headingLabel,getter,setter)));
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

  void setVerticalHeader(bool visible, QString title);

  void setHorizontalHeader(std::vector<QWidget> widgets);

  void setHorizontalHeader(std::vector<QString> names);



  // Call this function with the fields required,
  // and it adds the columns and does the binds.
  // This provides a mechinism to easily manage
  // a dynamic, user-preference column.
  // This function will be called from the slot
  // connected to the QButtonGroup signal
  //virtual void addColumns(const std::vector<QString> & fields) = 0;
  void DisplayCaseColumns(const std::vector<QString> & fields); // TODO rename to "addColumns" and move to derived class
  void WalkInColumns(const std::vector<QString> & fields);      // TODO rename to "addColumns" and move to derived class



  std::vector<QSharedPointer<CheckBoxConcept> > m_checkBoxConcepts;

  std::vector<QSharedPointer<ComboBoxConcept> > m_comboBoxConcepts;

  std::vector<QSharedPointer<DoubleEditConcept> > m_doubleEditConcepts;

  std::vector<QSharedPointer<IntegerEditConcept> > m_integerEditConcepts;

  std::vector<QSharedPointer<LineEditConcept> > m_lineEditConcepts; 
  
  std::vector<QSharedPointer<QuantityEditConcept> > m_quantityEditConcepts;

  std::vector<QSharedPointer<UnsignedEditConcept> > m_unsignedEditConcepts;

  model::Model m_model;

  std::vector<model::ModelObject> m_modelObjects;

  IddObjectType m_iddObjectType;

  std::vector<ColumnType> m_columnTypes; 
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

