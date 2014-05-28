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

#ifndef SHAREDGUICOMPONENTS_OSCONCEPTS_HPP
#define SHAREDGUICOMPONENTS_OSCONCEPTS_HPP

#include "FieldMethodTypedefs.hpp"

#include "../model/ModelObject.hpp"

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

/** Concept of being able to get and set std::string choices. */
class ChoiceConcept {
 public:
  virtual ~ChoiceConcept() {}

  virtual std::vector<std::string> choices() = 0;
  virtual std::string get() = 0;
  virtual bool set(std::string value) = 0;
  virtual void clear() {}
  virtual bool isDefaulted() { return false; }
};

/** Concept of a required choice, that is, one in which a non-empty choice,
 *  convertible to string, can always be returned. If it has a default, it can
 *  be cleared and state whether it is defaulted. Otherwise, it only has a getter
 *  and a setter. */
template<typename ChoiceType>
class RequiredChoiceConceptImpl : public ChoiceConcept {
 public:
  RequiredChoiceConceptImpl(
      boost::function<std::string (ChoiceType)> toString,
      boost::function<std::vector<ChoiceType> ()> choices,
      boost::function<ChoiceType ()> getter,
      boost::function<bool (ChoiceType)> setter,
      boost::optional<NoFailAction> reset=boost::none,
      boost::optional<BasicQuery> isDefaulted=boost::none)
    : m_toString(toString),
      m_choices(choices),
      m_getter(getter),
      m_setter(setter),
      m_reset(reset),
      m_isDefaulted(isDefaulted)
  {}

  virtual ~RequiredChoiceConceptImpl() {}

  virtual std::vector<std::string> choices() {
    m_choicesMap.clear();
    std::vector<std::string> result;
    std::vector<ChoiceType> typedChoices = m_choices();
    for (typename std::vector<ChoiceType>::const_iterator typedChoice = typedChoices.begin();
         typedChoice != typedChoices.end(); ++typedChoice)
    {
      std::string choice = m_toString(*typedChoice);
      result.push_back(choice);
      m_choicesMap.insert(typename std::map<std::string,ChoiceType>::value_type(choice,*typedChoice));
    }
    return result;
  }

  virtual std::string get() {
    ChoiceType typedValue = m_getter();
    std::string result = m_toString(typedValue);
    OS_ASSERT(m_choicesMap.find(result) != m_choicesMap.end());
    return result;
  }

  virtual bool set(std::string value) {
    typename std::map<std::string,ChoiceType>::const_iterator valuePair = m_choicesMap.find(value);
    OS_ASSERT(valuePair != m_choicesMap.end());
    return m_setter(valuePair->second);
  }

  virtual void clear() {
    if (m_reset) {
      (*m_reset)();
    }
  }

  virtual bool isDefaulted() {
    if (m_isDefaulted) {
      return (*m_isDefaulted)();
    }
    return false;
  }

 private:
  boost::function<std::string (ChoiceType)> m_toString;
  boost::function<std::vector<ChoiceType> ()> m_choices;
  boost::function<ChoiceType ()> m_getter;
  boost::function<bool (ChoiceType)> m_setter;
  boost::optional<NoFailAction> m_reset;
  boost::optional<BasicQuery> m_isDefaulted;

  std::map<std::string,ChoiceType> m_choicesMap;
};

template<typename ChoiceType, typename DataSourceType>
class RequiredChoiceSaveDataSourceConceptImpl : public RequiredChoiceConceptImpl<ChoiceType> {
 public:
  RequiredChoiceSaveDataSourceConceptImpl(
      std::shared_ptr<DataSourceType> dataSource,
      boost::function<std::string (ChoiceType)> toString,
      boost::function<std::vector<ChoiceType> ()> choices,
      boost::function<ChoiceType ()> getter,
      boost::function<bool (ChoiceType)> setter,
      boost::optional<NoFailAction> reset=boost::none,
      boost::optional<BasicQuery> isDefaulted=boost::none)
    : RequiredChoiceConceptImpl<ChoiceType>(toString,
                                            choices,
                                            getter,
                                            setter,
                                            reset,
                                            isDefaulted),
      m_dataSource(dataSource)
  {}

  virtual ~RequiredChoiceSaveDataSourceConceptImpl() {}
 private:
  std::shared_ptr<DataSourceType> m_dataSource;
};

/** Concept of an optional choice, that is, one in which an empty choice (converted
 *  to an empty string) is possible. There is no default, but the current value
 *  can be cleared. */
template<typename ChoiceType>
class OptionalChoiceConceptImpl : public ChoiceConcept {
 public:
  OptionalChoiceConceptImpl(
      boost::function<std::string (ChoiceType)> toString,
      boost::function<std::vector<ChoiceType> ()> choices,
      boost::function<boost::optional<ChoiceType> ()> getter,
      boost::function<bool (ChoiceType)> setter,
      boost::optional<NoFailAction> reset=boost::none)
    : m_toString(toString),
      m_choices(choices),
      m_getter(getter),
      m_setter(setter),
      m_reset(reset)
  {}

  virtual ~OptionalChoiceConceptImpl() {}

  virtual std::vector<std::string> choices() {
    m_choicesMap.clear();
    std::vector<std::string> result;
    // optional, so blank string is always a choice
    result.push_back(std::string());
    std::vector<ChoiceType> typedChoices = m_choices();
    for (typename std::vector<ChoiceType>::const_iterator typedChoice = typedChoices.begin();
         typedChoice != typedChoices.end(); ++typedChoice)
    {
      std::string choice = m_toString(*typedChoice);
      result.push_back(choice);
      m_choicesMap.insert(typename std::map<std::string,ChoiceType>::value_type(choice,*typedChoice));
    }
    return result;
  }

  virtual std::string get() {
    std::string result;
    boost::optional<ChoiceType> typedValue = m_getter();
    if (typedValue.is_initialized()) {
      result = m_toString(typedValue.get());
      if (!result.empty()) {
        OS_ASSERT(m_choicesMap.find(result) != m_choicesMap.end());
      }
    }
    return result;
  }

  virtual bool set(std::string value) {
    if (value.empty()) {
      clear();
      return true;
    }

    typename std::map<std::string,ChoiceType>::const_iterator valuePair = m_choicesMap.find(value);
    OS_ASSERT(valuePair != m_choicesMap.end());
    return m_setter(valuePair->second);
  }

  virtual void clear() {
    if (m_reset) {
      (*m_reset)();
    }
  }

 private:
  boost::function<std::string (ChoiceType)> m_toString;
  boost::function<std::vector<ChoiceType> ()> m_choices;
  boost::function<boost::optional<ChoiceType> ()> m_getter;
  boost::function<bool (ChoiceType)> m_setter;
  boost::optional<NoFailAction> m_reset;

  std::map<std::string,ChoiceType> m_choicesMap;
};

template<typename ChoiceType, typename DataSourceType>
class OptionalChoiceSaveDataSourceConceptImpl : public OptionalChoiceConceptImpl<ChoiceType> {
 public:
  OptionalChoiceSaveDataSourceConceptImpl(
      std::shared_ptr<DataSourceType> dataSource,
      boost::function<std::string (ChoiceType)> toString,
      boost::function<std::vector<ChoiceType> ()> choices,
      boost::function<boost::optional<ChoiceType> ()> getter,
      boost::function<bool (ChoiceType)> setter,
      boost::optional<NoFailAction> reset=boost::none)
    : OptionalChoiceConceptImpl<ChoiceType>(toString,
                                            choices,
                                            getter,
                                            setter,
                                            reset),
      m_dataSource(dataSource)
  {}

  virtual ~OptionalChoiceSaveDataSourceConceptImpl() {}

 private:
  std::shared_ptr<DataSourceType> m_dataSource;
};


class ComboBoxConcept : public BaseConcept
{
  public:

  ComboBoxConcept(QString t_headingLabel)
    : BaseConcept(t_headingLabel)
  {
  }

  virtual ~ComboBoxConcept() {}

  virtual std::shared_ptr<ChoiceConcept> choiceConcept(const model::ModelObject& obj) = 0;
};

template<typename ChoiceType, typename DataSourceType>
class ComboBoxRequiredChoiceImpl : public ComboBoxConcept
{
  public:

  ComboBoxRequiredChoiceImpl(
    QString t_headingLabel,
    boost::function<std::string (ChoiceType)> t_toString,
    boost::function<std::vector<ChoiceType> (void)> t_choices,
    boost::function<ChoiceType (DataSourceType*)>  t_getter,
    boost::function<bool (DataSourceType*, ChoiceType)> t_setter)
    : ComboBoxConcept(t_headingLabel),
      m_toString(t_toString),
      m_choices(t_choices),
      m_getter(t_getter),
      m_setter(t_setter)
  {
  }

  virtual ~ComboBoxRequiredChoiceImpl() {}

  virtual std::shared_ptr<ChoiceConcept> choiceConcept(const model::ModelObject& obj) {
    std::shared_ptr<DataSourceType> dataSource = std::shared_ptr<DataSourceType>(
        new DataSourceType(obj.cast<DataSourceType>()));
    return std::shared_ptr<ChoiceConcept>(
        new RequiredChoiceSaveDataSourceConceptImpl<ChoiceType,DataSourceType>(
            dataSource,
            m_toString,
            m_choices,
            std::bind(m_getter,dataSource.get()),
            std::bind(m_setter,dataSource.get(),std::placeholders::_1)));
  }

 private:
  boost::function<std::string (ChoiceType)> m_toString;
  boost::function<std::vector<ChoiceType> (void)> m_choices;
  boost::function<std::string (DataSourceType *)>  m_getter;
  boost::function<bool (DataSourceType *, ChoiceType)> m_setter;
};

template<typename ChoiceType, typename DataSourceType>
class ComboBoxOptionalChoiceImpl : public ComboBoxConcept
{
  public:

  ComboBoxOptionalChoiceImpl(
    QString t_headingLabel,
    boost::function<std::string (ChoiceType)> t_toString,
    boost::function<std::vector<ChoiceType> (void)> t_choices,
    boost::function<boost::optional<ChoiceType> (DataSourceType*)>  t_getter,
    boost::function<bool (DataSourceType*, ChoiceType)> t_setter,
    boost::optional<boost::function<void (DataSourceType*)> > t_reset=boost::none)
    : ComboBoxConcept(t_headingLabel),
      m_toString(t_toString),
      m_choices(t_choices),
      m_getter(t_getter),
      m_setter(t_setter),
      m_reset(t_reset)
  {}

  virtual ~ComboBoxOptionalChoiceImpl() {}

  virtual std::shared_ptr<ChoiceConcept> choiceConcept(const model::ModelObject& obj) {
    std::shared_ptr<DataSourceType> dataSource = std::shared_ptr<DataSourceType>(
        new DataSourceType(obj.cast<DataSourceType>()));
    std::shared_ptr<ChoiceConcept> result;
    if (m_reset) {
      result = std::shared_ptr<ChoiceConcept>(
          new OptionalChoiceSaveDataSourceConceptImpl<ChoiceType,DataSourceType>(
              dataSource,
              m_toString,
              m_choices,
              std::bind(m_getter,dataSource.get()),
              std::bind(m_setter,dataSource.get(),std::placeholders::_1),
              boost::optional<NoFailAction>(std::bind(m_reset.get(),dataSource.get()))));
    }
    else {
      result = std::shared_ptr<ChoiceConcept>(
        new OptionalChoiceSaveDataSourceConceptImpl<ChoiceType,DataSourceType>(
            dataSource,
            m_toString,
            m_choices,
            std::bind(m_getter,dataSource.get()),
            std::bind(m_setter,dataSource.get(),std::placeholders::_1)));
    }
    return result;
  }

 private:
  boost::function<std::string (ChoiceType)> m_toString;
  boost::function<std::vector<ChoiceType> (void)> m_choices;
  boost::function<boost::optional<ChoiceType> (DataSourceType *)>  m_getter;
  boost::function<bool (DataSourceType *, ChoiceType)> m_setter;
  boost::optional<boost::function<void (DataSourceType*)> > m_reset;
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

  virtual ~ValueEditVoidReturnConcept() {}

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


class NameLineEditConcept : public BaseConcept
{
  public:

  NameLineEditConcept(QString t_headingLabel)
    : BaseConcept(t_headingLabel)
  {
  }

  virtual ~NameLineEditConcept() {}

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

  virtual ~NameLineEditConceptImpl() {}

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


template<typename ValueType>
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

  virtual ~QuantityEditConcept() {}

  virtual ValueType get(const model::ModelObject & obj) = 0;
  virtual bool set(const model::ModelObject & obj, ValueType) = 0;

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

template<typename ValueType, typename DataSourceType>
class QuantityEditConceptImpl : public QuantityEditConcept<ValueType>
{
  public:

  QuantityEditConceptImpl(QString t_headingLabel,
    QString t_modelUnits,
    QString t_siUnits,
    QString t_ipUnits,
    bool t_isIP,
    boost::function<ValueType (DataSourceType *)>  t_getter,
    boost::function<bool (DataSourceType *, ValueType)> t_setter)
    : QuantityEditConcept<ValueType>(t_headingLabel,
      t_modelUnits,
      t_siUnits,
      t_ipUnits,
      t_isIP),
      m_getter(t_getter),
      m_setter(t_setter)
  {
  }

  virtual ~QuantityEditConceptImpl() {}

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
class OptionalQuantityEditConcept : public BaseConcept
{
  public:

  OptionalQuantityEditConcept(QString t_headingLabel, QString t_modelUnits, QString t_siUnits, QString t_ipUnits, bool t_isIP)
    : BaseConcept(t_headingLabel),
      m_modelUnits(t_modelUnits),
      m_siUnits(t_siUnits),
      m_ipUnits(t_ipUnits),
      m_isIP(t_isIP)
  {
  }

  virtual ~OptionalQuantityEditConcept() {}

  virtual boost::optional<ValueType> get(const model::ModelObject & obj) = 0;
  virtual bool set(const model::ModelObject & obj, ValueType) = 0;

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

template<typename ValueType, typename DataSourceType>
class OptionalQuantityEditConceptImpl : public OptionalQuantityEditConcept<ValueType>
{
  public:

  OptionalQuantityEditConceptImpl(QString t_headingLabel,
    QString t_modelUnits,
    QString t_siUnits,
    QString t_ipUnits,
    bool t_isIP,
    boost::function<boost::optional<ValueType> (DataSourceType *)>  t_getter,
    boost::function<bool (DataSourceType *, ValueType)> t_setter)
    : OptionalQuantityEditConcept<ValueType>(t_headingLabel,
      t_modelUnits,
      t_siUnits,
      t_ipUnits,
      t_isIP),
      m_getter(t_getter),
      m_setter(t_setter)
  {
  }

  virtual ~OptionalQuantityEditConceptImpl() {}

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
class QuantityEditVoidReturnConcept : public BaseConcept
{
  public:

  QuantityEditVoidReturnConcept(QString t_headingLabel, QString t_modelUnits, QString t_siUnits, QString t_ipUnits, bool t_isIP)
    : BaseConcept(t_headingLabel),
      m_modelUnits(t_modelUnits),
      m_siUnits(t_siUnits),
      m_ipUnits(t_ipUnits),
      m_isIP(t_isIP)
  {
  }

  virtual ~QuantityEditVoidReturnConcept() {}

  virtual ValueType get(const model::ModelObject & obj) = 0;
  virtual void set(const model::ModelObject & obj, ValueType) = 0;

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

template<typename ValueType, typename DataSourceType>
class QuantityEditVoidReturnConceptImpl : public QuantityEditVoidReturnConcept<ValueType>
{
  public:

  QuantityEditVoidReturnConceptImpl(QString t_headingLabel,
    QString t_modelUnits,
    QString t_siUnits,
    QString t_ipUnits,
    bool t_isIP,
    boost::function<ValueType (DataSourceType *)>  t_getter,
    boost::function<void (DataSourceType *, ValueType)> t_setter)
    : QuantityEditVoidReturnConcept<ValueType>(t_headingLabel,
      t_modelUnits,
      t_siUnits,
      t_ipUnits,
      t_isIP),
      m_getter(t_getter),
      m_setter(t_setter)
  {
  }

  virtual ~QuantityEditVoidReturnConceptImpl() {}

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
class OptionalQuantityEditVoidReturnConcept : public BaseConcept
{
  public:

  OptionalQuantityEditVoidReturnConcept(QString t_headingLabel, QString t_modelUnits, QString t_siUnits, QString t_ipUnits, bool t_isIP)
    : BaseConcept(t_headingLabel),
      m_modelUnits(t_modelUnits),
      m_siUnits(t_siUnits),
      m_ipUnits(t_ipUnits),
      m_isIP(t_isIP)
  {
  }

  virtual ~OptionalQuantityEditVoidReturnConcept() {}

  virtual boost::optional<ValueType> get(const model::ModelObject & obj) = 0;
  virtual void set(const model::ModelObject & obj, ValueType) = 0;

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

template<typename ValueType, typename DataSourceType>
class OptionalQuantityEditVoidReturnConceptImpl : public OptionalQuantityEditVoidReturnConcept<ValueType>
{
  public:

  OptionalQuantityEditVoidReturnConceptImpl(QString t_headingLabel,
    QString t_modelUnits,
    QString t_siUnits,
    QString t_ipUnits,
    bool t_isIP,
    boost::function<boost::optional<ValueType> (DataSourceType *)>  t_getter,
    boost::function<void (DataSourceType *, ValueType)> t_setter)
    : OptionalQuantityEditVoidReturnConcept<ValueType>(t_headingLabel,
      t_modelUnits,
      t_siUnits,
      t_ipUnits,
      t_isIP),
      m_getter(t_getter),
      m_setter(t_setter)
  {
  }

  virtual ~OptionalQuantityEditVoidReturnConceptImpl() {}

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


class DropZoneConcept : public BaseConcept
{
  public:

  DropZoneConcept(QString t_headingLabel)
    : BaseConcept(t_headingLabel)
  {
  }

  virtual ~DropZoneConcept() {}

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

  virtual ~DropZoneConceptImpl() {}

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

#endif // SHAREDGUICOMPONENTS_OSCONCEPTS_HPP

