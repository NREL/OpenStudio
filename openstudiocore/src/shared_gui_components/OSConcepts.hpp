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

#ifndef OPENSTUDIO_OSCONCEPT_H
#define OPENSTUDIO_OSCONCEPT_H

#include <shared_gui_components/FieldMethodTypedefs.hpp>

#include <model/ModelObject.hpp>

namespace openstudio {

class BaseConcept
{
  public:

  virtual ~BaseConcept() {}

  BaseConcept(QString t_headingLabel)
    : m_headingLabel(t_headingLabel)
  {
  }

  QString headingLabel() const { return m_headingLabel; }

  private:

  QString m_headingLabel;

}; 


///////////////////////////////////////////////////////////////////////////////////


class CheckBoxConcept : public BaseConcept
{
  public:

  CheckBoxConcept(QString t_headingLabel)
    : BaseConcept(t_headingLabel)
  {
  }

  virtual ~CheckBoxConcept() {}

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
    : CheckBoxConcept(t_headingLabel),
      m_getter(t_getter),
      m_setter(t_setter)
  {
  }

  virtual ~CheckBoxConceptImpl() {}


  virtual bool get(const model::ModelObject & t_obj)
  {
    DataSourceType obj = t_obj.cast<DataSourceType>();
    return m_getter(&obj);
  }

  virtual void set(const model::ModelObject & t_obj, bool value)
  {
    DataSourceType obj = t_obj.cast<DataSourceType>();
    return m_setter(&obj, value);
  }

  private:

  boost::function<bool (DataSourceType *)>  m_getter;
  boost::function<bool (DataSourceType *, bool)> m_setter;
};


///////////////////////////////////////////////////////////////////////////////////


class ComboBoxConcept : public BaseConcept
{
  public:

   ComboBoxConcept(QString t_headingLabel)
    : BaseConcept(t_headingLabel)
  {
  }

   virtual ~ComboBoxConcept() {}


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
    : ComboBoxConcept(t_headingLabel),
      m_choices(t_choices),
      m_getter(t_getter),
      m_setter(t_setter)
  {
  }

  virtual ~ComboBoxConceptImpl() {}

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

  boost::function<std::vector<std::string> (void)> m_choices;
  boost::function<std::string (DataSourceType *)>  m_getter;
  boost::function<bool (DataSourceType *, std::string)> m_setter;
};


///////////////////////////////////////////////////////////////////////////////////


template<typename ValueType>
class ValueEditConcept : public BaseConcept
{
  public:

  ValueEditConcept(QString t_headingLabel)
    : BaseConcept(t_headingLabel)
  {
  }

  virtual ~ValueEditConcept() {}

  virtual ValueType get(const model::ModelObject & obj) = 0;
  virtual bool set(const model::ModelObject & obj, ValueType) = 0;
}; 

template<typename ValueType, typename DataSourceType>
class ValueEditConceptImpl : public ValueEditConcept<ValueType>
{
  public:

  ValueEditConceptImpl(QString t_headingLabel, 
    boost::function<ValueType (DataSourceType *)>  t_getter, 
    boost::function<bool (DataSourceType *, ValueType)> t_setter)
    : ValueEditConcept<ValueType>(t_headingLabel),
      m_getter(t_getter),
      m_setter(t_setter)
  {
  }

  virtual ~ValueEditConceptImpl() {}

  virtual ValueType get(const model::ModelObject & t_obj)
  {
    DataSourceType obj = t_obj.cast<DataSourceType>();
    return m_getter(&obj);
  }

  virtual bool set(const model::ModelObject & t_obj, ValueType value)
  {
    DataSourceType obj = t_obj.cast<DataSourceType>();
    return m_setter(&obj,value);
  }

  private:

  boost::function<ValueType (DataSourceType *)>  m_getter;
  boost::function<bool (DataSourceType *, ValueType)> m_setter;
};


///////////////////////////////////////////////////////////////////////////////////

template<typename ValueType>
class OptionalValueEditConcept : public BaseConcept
{
  public:

   OptionalValueEditConcept(QString t_headingLabel)
     : BaseConcept(t_headingLabel)
  {
  }

   virtual ~OptionalValueEditConcept() {}

  virtual boost::optional<ValueType> get(const model::ModelObject & obj) = 0;
  virtual bool set(const model::ModelObject & obj, ValueType) = 0;
}; 

template<typename ValueType, typename DataSourceType>
class OptionalValueEditConceptImpl : public OptionalValueEditConcept<ValueType>
{
  public:

  OptionalValueEditConceptImpl(QString t_headingLabel, 
    boost::function<boost::optional<ValueType> (DataSourceType *)>  t_getter, 
    boost::function<bool (DataSourceType *, ValueType)> t_setter)
    : OptionalValueEditConcept<ValueType>(t_headingLabel),
      m_getter(t_getter),
      m_setter(t_setter)
  {
  }

  virtual ~OptionalValueEditConceptImpl() {}

  virtual boost::optional<ValueType> get(const model::ModelObject & t_obj)
  {
    DataSourceType obj = t_obj.cast<DataSourceType>();
    return m_getter(&obj);
  }

  virtual bool set(const model::ModelObject & t_obj, ValueType value)
  {
    DataSourceType obj = t_obj.cast<DataSourceType>();
    return m_setter(&obj,value);
  }

  private:

  boost::function<boost::optional<ValueType> (DataSourceType *)>  m_getter;
  boost::function<bool (DataSourceType *, ValueType)> m_setter;
};


///////////////////////////////////////////////////////////////////////////////////


template<typename ValueType>
class ValueEditVoidReturnConcept : public BaseConcept
{
  public:

   ValueEditVoidReturnConcept(QString t_headingLabel)
     : BaseConcept(t_headingLabel)
  {
  }

  virtual ValueType get(const model::ModelObject & obj) = 0;
  virtual void set(const model::ModelObject & obj, ValueType) = 0;
}; 

template<typename ValueType, typename DataSourceType>
class ValueEditVoidReturnConceptImpl : public ValueEditVoidReturnConcept<ValueType>
{
  public:

  ValueEditVoidReturnConceptImpl(QString t_headingLabel,
    boost::function<ValueType (DataSourceType *)>  t_getter,
    boost::function<void (DataSourceType *, ValueType)> t_setter)
    : ValueEditVoidReturnConcept<ValueType>(t_headingLabel),
      m_getter(t_getter),
      m_setter(t_setter)
  {
  }

  virtual ~ValueEditVoidReturnConceptImpl() {}

  virtual ValueType get(const model::ModelObject & t_obj)
  {
    DataSourceType obj = t_obj.cast<DataSourceType>();
    return m_getter(&obj);
  }

  virtual void set(const model::ModelObject & t_obj, ValueType value)
  {
    DataSourceType obj = t_obj.cast<DataSourceType>();
    return m_setter(&obj,value);
  }

  private:

  boost::function<ValueType(DataSourceType *)>  m_getter;
  boost::function<void (DataSourceType *, ValueType)> m_setter;
};


///////////////////////////////////////////////////////////////////////////////////


template<typename ValueType>
class OptionalValueEditVoidReturnConcept : public BaseConcept
{
  public:

  OptionalValueEditVoidReturnConcept(QString t_headingLabel)
    : BaseConcept(t_headingLabel)
  {
  }

  virtual ~OptionalValueEditVoidReturnConcept() {}

  virtual boost::optional<ValueType> get(const model::ModelObject & obj) = 0;
  virtual void set(const model::ModelObject & obj, ValueType) = 0;
}; 

template<typename ValueType, typename DataSourceType>
class OptionalValueEditVoidReturnConceptImpl : public OptionalValueEditVoidReturnConcept<ValueType>
{
  public:

  OptionalValueEditVoidReturnConceptImpl(QString t_headingLabel, 
    boost::function<boost::optional<ValueType> (DataSourceType *)>  t_getter, 
    boost::function<void (DataSourceType *, ValueType)> t_setter)
    : OptionalValueEditVoidReturnConcept<ValueType>(t_headingLabel),
      m_getter(t_getter),
      m_setter(t_setter)
  {
  }

  virtual ~OptionalValueEditVoidReturnConceptImpl() {}

  virtual boost::optional<ValueType> get(const model::ModelObject & t_obj)
  {
    DataSourceType obj = t_obj.cast<DataSourceType>();
    return m_getter(&obj);
  }

  virtual void set(const model::ModelObject & t_obj, ValueType value)
  {
    DataSourceType obj = t_obj.cast<DataSourceType>();
    return m_setter(&obj,value);
  }

  private:

  boost::function<boost::optional<ValueType> (DataSourceType *)>  m_getter;
  boost::function<void (DataSourceType *, ValueType)> m_setter;
};


///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
  

class NameLineEditConcept : public BaseConcept
{
  public:

  NameLineEditConcept(QString t_headingLabel)
    : BaseConcept(t_headingLabel)
  {
  }

  virtual boost::optional<std::string> get(const model::ModelObject & obj, bool) = 0;
  virtual boost::optional<std::string> set(const model::ModelObject & obj, const std::string &) = 0;
}; 

template<typename DataSourceType>
class NameLineEditConceptImpl : public NameLineEditConcept
{
  public:

  NameLineEditConceptImpl(QString t_headingLabel,
    boost::function<boost::optional<std::string> (DataSourceType *, bool)>  t_getter, 
    boost::function<boost::optional<std::string> (DataSourceType *, const std::string &)> t_setter)
    : NameLineEditConcept(t_headingLabel),
      m_getter(t_getter),
      m_setter(t_setter)
  {
  }

  virtual boost::optional<std::string> get(const model::ModelObject & t_obj, bool value)
  {
    DataSourceType obj = t_obj.cast<DataSourceType>();
    return m_getter(&obj,value);
  }

  virtual boost::optional<std::string> set(const model::ModelObject & t_obj, const std::string & value)
  {
    DataSourceType obj = t_obj.cast<DataSourceType>();
    return m_setter(&obj,value);
  }

  private:

  boost::function<boost::optional<std::string> (DataSourceType *, bool)>  m_getter;
  boost::function<boost::optional<std::string> (DataSourceType *, const std::string &)> m_setter;
};


///////////////////////////////////////////////////////////////////////////////////


class QuantityEditConcept : public BaseConcept
{
  public:

  QuantityEditConcept(QString t_headingLabel, QString t_modelUnits, QString t_siUnits, QString t_ipUnits, bool t_isIP)
    : BaseConcept(t_headingLabel),
      m_modelUnits(t_modelUnits),
      m_siUnits(t_siUnits),
      m_ipUnits(t_ipUnits),
      m_isIP(t_isIP)
  {
  }

  virtual double get(const model::ModelObject & obj) = 0;
  virtual bool set(const model::ModelObject & obj, double) = 0;

  QString modelUnits() const { return m_modelUnits; }
  QString siUnits() const { return m_siUnits; }
  QString ipUnits() const { return m_ipUnits; }
  bool isIP() const { return m_isIP; }

  private:

  QString m_modelUnits;
  QString m_siUnits;
  QString m_ipUnits;
  bool m_isIP;
}; 

template<typename DataSourceType>
class QuantityEditConceptImpl : public QuantityEditConcept
{
  public:

  QuantityEditConceptImpl(QString t_headingLabel,
    boost::function<double (DataSourceType *)>  t_getter,
    boost::function<bool (DataSourceType *, double)> t_setter)
    : QuantityEditConcept(t_headingLabel),
      m_getter(t_getter),
      m_setter(t_setter)
  {
  }

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

  boost::function<double (DataSourceType *)>  m_getter;
  boost::function<bool (DataSourceType *, double)> m_setter;

};



///////////////////////////////////////////////////////////////////////////////////
  

class DropZoneConcept : public BaseConcept
{
  public:

  DropZoneConcept(QString t_headingLabel)
    : BaseConcept(t_headingLabel)
  {
  }

  virtual boost::optional<model::ModelObject> get(const model::ModelObject & obj) = 0;
  virtual bool set(const model::ModelObject & obj, const model::ModelObject &) = 0;
}; 

template<typename ValueType, typename DataSourceType>
class DropZoneConceptImpl : public DropZoneConcept
{
  public:

  DropZoneConceptImpl(QString t_headingLabel, 
    boost::function<boost::optional<ValueType> (DataSourceType *)>  t_getter, 
    boost::function<bool (DataSourceType *, const ValueType &)> t_setter)
    : DropZoneConcept(t_headingLabel),
      m_getter(t_getter),
      m_setter(t_setter)
  {
  }

  virtual boost::optional<model::ModelObject> get(const model::ModelObject & t_obj)
  {
    DataSourceType obj = t_obj.cast<DataSourceType>();

    return boost::optional<model::ModelObject>(m_getter(&obj));
  }

  virtual bool set(const model::ModelObject & t_obj, const model::ModelObject & t_value)
  {
    DataSourceType obj = t_obj.cast<DataSourceType>();
    ValueType value = t_value.cast<ValueType>();
    return m_setter(&obj,value);
  }

  private:

  boost::function<boost::optional<ValueType> (DataSourceType *)>  m_getter;
  boost::function<bool (DataSourceType *, const ValueType &)> m_setter;
};


} // openstudio

#endif // OPENSTUDIO_OSCONCEPT_H

