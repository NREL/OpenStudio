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

#include <boost/any.hpp>

namespace openstudio {


template<typename DataType, typename RetType>
std::function<RetType(DataType *)> NullAdapter(RetType(DataType::*t_func)() )
{
  return std::function<RetType(DataType *)>([t_func](DataType *obj) { return (obj->*t_func)(); });
}

template<typename DataType, typename RetType>
std::function<RetType (DataType *)> NullAdapter(RetType (DataType::*t_func)() const)
{
  return std::function<RetType (DataType *)>([t_func] (DataType *obj) { return (obj->*t_func)(); } );
}

template<typename DataType, typename RetType, typename Param1>
std::function<RetType (DataType *, const Param1 &)> NullAdapter(RetType (DataType::*t_func)(const Param1 &) )
{
  return std::function<RetType (DataType *, const Param1 &)>(t_func);
}

template<typename DataType, typename RetType, typename Param1>
std::function<RetType (DataType *, Param1)> NullAdapter(RetType (DataType::*t_func)(Param1 &) )
{
  return std::function<RetType (DataType *, Param1)>( [t_func] (DataType *obj, Param1 p1) { return (obj->*t_func)(p1); }  );
}


template<typename DataType, typename RetType, typename Param1>
std::function<RetType (DataType *, Param1)> NullAdapter(RetType (DataType::*t_func)(Param1) )
{
  return std::function<RetType (DataType *, Param1)>([t_func](DataType *obj, Param1 p1) { return (obj->*t_func)(p1); });
}

template<typename DataType, typename RetType, typename Param1>
std::function<RetType (DataType *, const Param1 &)> NullAdapter2(RetType (DataType::*t_func)(Param1 &) )
{
  return std::function<RetType (DataType *, const Param1 &)>([t_func](DataType *obj, const Param1 & p1) { Param1 t_p1(p1); return (obj->*t_func)(t_p1); });
}


template<typename DataType, typename RetType, typename Param1>
std::function<RetType (DataType *, Param1)> NullAdapter(RetType (DataType::*t_func)(Param1) const)
{
  return std::function<RetType (DataType *, Param1)>([t_func] ( DataType *obj, Param1 p1) { return (obj->*t_func)(p1); } );
}


template<typename DataType, typename RetType, typename DataType2>
std::function<RetType(DataType *)> CastNullAdapter(RetType(DataType2::*t_func)() )
{
  return std::function<RetType(DataType *)>([t_func](DataType *obj) { return (obj->*t_func)(); });
}


template<typename DataType, typename RetType, typename DataType2>
std::function<RetType(DataType *)> CastNullAdapter(RetType(DataType2::*t_func)() const)
{
  return std::function<RetType(DataType *)>([t_func](DataType *obj) { return (obj->*t_func)(); });
}

template<typename DataType, typename RetType, typename Param1, typename DataType2>
std::function<RetType(DataType *, const Param1 &)> CastNullAdapter(RetType(DataType2::*t_func)(const Param1 &))
{
  return std::function<RetType(DataType *, const Param1 &)>([t_func](DataType *obj, Param1 p1) { return (obj->*t_func)(p1); });
}

template<typename DataType, typename RetType, typename Param1, typename DataType2>
std::function<RetType(DataType *, Param1)> CastNullAdapter(RetType(DataType2::*t_func)(Param1 &))
{
  return std::function<RetType(DataType *, Param1)>([t_func](DataType *obj, Param1 p1) { return (obj->*t_func)(p1); });
}


template<typename DataType, typename RetType, typename Param1, typename DataType2>
std::function<RetType(DataType *, Param1)> CastNullAdapter(RetType(DataType2::*t_func)(Param1))
{
  return std::function<RetType(DataType *, Param1)>([t_func](DataType *obj, Param1 p1) { return (obj->*t_func)(p1); });
}


template<typename DataType, typename RetType, typename Param1, typename DataType2>
std::function<RetType(DataType *, Param1)> CastNullAdapter(RetType(DataType2::*t_func)(Param1) const)
{
  return std::function<RetType(DataType *, Param1)>([t_func](DataType *obj, Param1 p1) { return (obj->*t_func)(p1); });
}


// Proxy with optional<> default values
template<typename RetType, typename FromDataType, typename ToDataType>
RetType ZeroParamOptionalProxy(FromDataType *t_from, const std::function<RetType (ToDataType *)> &t_outter, const std::function<boost::optional<ToDataType> (FromDataType *)> t_inner, const RetType &t_defaultValue)
{
  boost::optional<ToDataType> t(t_inner(t_from));
  return t ? t_outter(t.get_ptr()) : t_defaultValue;
}

template<typename RetType, typename FromDataType, typename Param1, typename ToDataType>
RetType OneParamOptionalProxy(FromDataType *t_from, Param1 t_param1, const std::function<RetType (ToDataType *, Param1)> &t_outter, const std::function<boost::optional<ToDataType> (FromDataType *)> t_inner, const RetType &t_defaultValue)
{
  boost::optional<ToDataType> t(t_inner(t_from));
  return t ? t_outter(t.get_ptr(), t_param1) : t_defaultValue;
}

template<typename RetType, typename FromDataType, typename Param1, typename ToDataType>
RetType OneConstParamOptionalProxy(FromDataType *t_from, const Param1 & t_param1, const std::function<RetType (ToDataType *, const Param1 &)> &t_outter, const std::function<boost::optional<ToDataType> (FromDataType *)> t_inner, const RetType &t_defaultValue)
{
  boost::optional<ToDataType> t(t_inner(t_from));
  Param1 param1Copy(t_param1);
  return t ? t_outter(t.get_ptr(), param1Copy) : t_defaultValue;
}

// Consider the example
// class B
// {
//  std::string foo()
// };
//
// class A 
// {
//  B b();
// };
//
// If you want to access foo from B via A
// A is FromDataType
// B is ToDataType
// std::string is RetType
// b is the t_proxyFunc
// foo is the t_func
//
// Proxy adapters effectively give A a foo method
//
template<typename RetType, typename FromDataType, typename ToDataType>
std::function<RetType (FromDataType *)> ProxyAdapter(RetType (ToDataType::*t_func)() const, boost::optional<ToDataType> (FromDataType:: *t_proxyFunc)() const, const RetType &t_defaultValue)
{
  std::function<RetType (ToDataType *)> outter(CastNullAdapter<ToDataType>(t_func));
  std::function<boost::optional<ToDataType> (FromDataType *)> inner(CastNullAdapter<FromDataType>(t_proxyFunc));
  return std::bind(&ZeroParamOptionalProxy<RetType,FromDataType,ToDataType>, std::placeholders::_1, outter, inner, t_defaultValue);
}

template<typename RetType, typename FromDataType, typename Param1, typename ToDataType>
std::function<RetType (FromDataType *, Param1)> ProxyAdapter(RetType (ToDataType::*t_func)(Param1), boost::optional<ToDataType> (FromDataType:: *t_proxyFunc)() const, const RetType &t_defaultValue)
{
  std::function<RetType (ToDataType *, Param1)> outter(CastNullAdapter<ToDataType>(t_func));
  std::function<boost::optional<ToDataType> (FromDataType *)> inner(CastNullAdapter<FromDataType>(t_proxyFunc));
  return std::bind(&OneParamOptionalProxy<RetType,FromDataType,Param1,ToDataType>, std::placeholders::_1, std::placeholders::_2, outter, inner, t_defaultValue);
}

template<typename RetType, typename FromDataType, typename Param1, typename ToDataType>
std::function<RetType (FromDataType *, const Param1 &)> ProxyAdapter(RetType (ToDataType::*t_func)(Param1 &), boost::optional<ToDataType> (FromDataType:: *t_proxyFunc)() const, const RetType &t_defaultValue)
{
  std::function<RetType (ToDataType *, Param1)> outter(CastNullAdapter<ToDataType>(t_func));
  std::function<boost::optional<ToDataType> (FromDataType *)> inner(CastNullAdapter<FromDataType>(t_proxyFunc));
  return std::bind(&OneConstParamOptionalProxy<RetType,FromDataType,Param1,ToDataType>, std::placeholders::_1, std::placeholders::_2, outter, inner, t_defaultValue);
}

template<typename RetType, typename FromDataType, typename Param1, typename ToDataType>
std::function<RetType (FromDataType *, Param1 )> ProxyAdapter(RetType (ToDataType::*t_func)(Param1) const, boost::optional<ToDataType> (FromDataType:: *t_proxyFunc)() const, const RetType &t_defaultValue)
{
  std::function<RetType (ToDataType *, Param1)> outter(CastNullAdapter<ToDataType>(t_func));
  std::function<boost::optional<ToDataType> (FromDataType *)> inner(CastNullAdapter<FromDataType>(t_proxyFunc));
  return std::bind(&OneParamOptionalProxy<RetType,FromDataType,Param1,ToDataType>, std::placeholders::_1, std::placeholders::_2, outter, inner, t_defaultValue);
}


// Proxies which do not deal with optionals
template<typename RetType, typename FromDataType, typename ToDataType>
RetType ZeroParamProxy(FromDataType *t_from, const std::function<RetType (ToDataType *)> &t_outter, const std::function<ToDataType (FromDataType *)> t_inner)
{
  ToDataType t(t_inner(t_from));
  return t_outter(&t);
}

template<typename RetType, typename FromDataType, typename Param1, typename ToDataType>
RetType OneParamProxy(FromDataType *t_from, Param1 param1, const std::function<RetType (ToDataType *, Param1)> &t_outter, const std::function<ToDataType (FromDataType *)> t_inner)
{
  ToDataType t(t_inner(t_from));
  return t_outter(&t, param1);
}

template<typename RetType, typename FromDataType, typename ToDataType>
std::function<RetType (FromDataType *)> ProxyAdapter(RetType (ToDataType::*t_func)() const, ToDataType (FromDataType:: *t_proxyFunc)() const)
{
  std::function<RetType (ToDataType *)> outter(CastNullAdapter<ToDataType>(t_func));
  std::function<ToDataType (FromDataType *)> inner(CastNullAdapter<FromDataType>(t_proxyFunc));
  return std::bind(&ZeroParamProxy<RetType, FromDataType, ToDataType>, std::placeholders::_1, outter, inner);
}

template<typename RetType, typename FromDataType, typename Param1, typename ToDataType>
std::function<RetType (FromDataType *, Param1)> ProxyAdapter(RetType (ToDataType::*t_func)(Param1), ToDataType (FromDataType:: *t_proxyFunc)() const)
{
  std::function<RetType (ToDataType *, Param1)> outter(CastNullAdapter<ToDataType>(t_func));
  std::function<ToDataType (FromDataType *)> inner(CastNullAdapter<FromDataType>(t_proxyFunc));
  return std::bind(&OneParamProxy<RetType, FromDataType, Param1, ToDataType>, std::placeholders::_1, std::placeholders::_2, outter, inner);
}

template<typename RetType, typename FromDataType, typename Param1, typename ToDataType>
std::function<RetType (FromDataType *, Param1)> ProxyAdapter(RetType (ToDataType::*t_func)(Param1) const, ToDataType (FromDataType:: *t_proxyFunc)() const)
{
  std::function<RetType (ToDataType *, Param1)> outter(CastNullAdapter<ToDataType>(t_func));
  std::function<ToDataType (FromDataType *)> inner(CastNullAdapter<FromDataType>(t_proxyFunc));
  return std::bind(&OneParamProxy<RetType, FromDataType, Param1, ToDataType>, std::placeholders::_1, std::placeholders::_2, outter, inner);
}


class ConceptProxy
{
  public:
    template<typename T>
      ConceptProxy(const T &t_obj)
        : m_any(t_obj)
      {
      }

    template<typename T>
      T cast() const
      {
        try {
          return boost::any_cast<T>(m_any);
        } catch (const boost::bad_any_cast &) {
          return boost::any_cast<model::ModelObject>(m_any).cast<T>();
        }
      }

  private:
    boost::any m_any;
};

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

  virtual bool get(const ConceptProxy & obj) = 0;
  virtual void set(const ConceptProxy & obj, bool) = 0;
};

template<typename DataSourceType>
class CheckBoxConceptImpl : public CheckBoxConcept
{
  public:

  CheckBoxConceptImpl(QString t_headingLabel,
    std::function<bool (DataSourceType *)> t_getter,
    std::function<void (DataSourceType *, bool)> t_setter)
    : CheckBoxConcept(t_headingLabel),
      m_getter(t_getter),
      m_setter(t_setter)
  {
  }

  virtual ~CheckBoxConceptImpl() {}


  virtual bool get(const ConceptProxy & t_obj)
  {
    DataSourceType obj = t_obj.cast<DataSourceType>();
    return m_getter(&obj);
  }

  virtual void set(const ConceptProxy & t_obj, bool value)
  {
    DataSourceType obj = t_obj.cast<DataSourceType>();
    return m_setter(&obj, value);
  }

  private:

  std::function<bool (DataSourceType *)>  m_getter;
  std::function<void (DataSourceType *, bool)> m_setter;
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
      std::function<std::string (ChoiceType)> toString,
      std::function<std::vector<ChoiceType> ()> choices,
      std::function<ChoiceType ()> getter,
      std::function<bool (ChoiceType)> setter,
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
  std::function<std::string (ChoiceType)> m_toString;
  std::function<std::vector<ChoiceType> ()> m_choices;
  std::function<ChoiceType ()> m_getter;
  std::function<bool (ChoiceType)> m_setter;
  boost::optional<NoFailAction> m_reset;
  boost::optional<BasicQuery> m_isDefaulted;

  std::map<std::string,ChoiceType> m_choicesMap;
};

template<typename ChoiceType, typename DataSourceType>
class RequiredChoiceSaveDataSourceConceptImpl : public RequiredChoiceConceptImpl<ChoiceType> {
 public:
  RequiredChoiceSaveDataSourceConceptImpl(
      std::shared_ptr<DataSourceType> dataSource,
      std::function<std::string (ChoiceType)> toString,
      std::function<std::vector<ChoiceType> ()> choices,
      std::function<ChoiceType ()> getter,
      std::function<bool (ChoiceType)> setter,
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
      std::function<std::string (ChoiceType)> toString,
      std::function<std::vector<ChoiceType> ()> choices,
      std::function<boost::optional<ChoiceType> ()> getter,
      std::function<bool (ChoiceType)> setter,
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
  std::function<std::string (ChoiceType)> m_toString;
  std::function<std::vector<ChoiceType> ()> m_choices;
  std::function<boost::optional<ChoiceType> ()> m_getter;
  std::function<bool (ChoiceType)> m_setter;
  boost::optional<NoFailAction> m_reset;

  std::map<std::string,ChoiceType> m_choicesMap;
};

template<typename ChoiceType, typename DataSourceType>
class OptionalChoiceSaveDataSourceConceptImpl : public OptionalChoiceConceptImpl<ChoiceType> {
 public:
  OptionalChoiceSaveDataSourceConceptImpl(
      std::shared_ptr<DataSourceType> dataSource,
      std::function<std::string (ChoiceType)> toString,
      std::function<std::vector<ChoiceType> ()> choices,
      std::function<boost::optional<ChoiceType> ()> getter,
      std::function<bool (ChoiceType)> setter,
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

  virtual std::shared_ptr<ChoiceConcept> choiceConcept(const ConceptProxy& obj) = 0;

};

template<typename ChoiceType, typename DataSourceType>
class ComboBoxRequiredChoiceImpl : public ComboBoxConcept
{
  public:

  ComboBoxRequiredChoiceImpl(
    QString t_headingLabel,
    std::function<std::string (ChoiceType)> t_toString,
    std::function<std::vector<ChoiceType> (void)> t_choices,
    std::function<ChoiceType (DataSourceType*)>  t_getter,
    std::function<bool (DataSourceType*, ChoiceType)> t_setter)
    : ComboBoxConcept(t_headingLabel),
      m_toString(t_toString),
      m_choices(t_choices),
      m_getter(t_getter),
      m_setter(t_setter)
  {
  }

  virtual ~ComboBoxRequiredChoiceImpl() {}


  virtual std::shared_ptr<ChoiceConcept> choiceConcept(const ConceptProxy& obj) {
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
  std::function<std::string (ChoiceType)> m_toString;
  std::function<std::vector<ChoiceType> (void)> m_choices;
  std::function<std::string (DataSourceType *)>  m_getter;
  std::function<bool (DataSourceType *, ChoiceType)> m_setter;
};

template<typename ChoiceType, typename DataSourceType>
class ComboBoxOptionalChoiceImpl : public ComboBoxConcept
{
  public:

  ComboBoxOptionalChoiceImpl(
    QString t_headingLabel,
    std::function<std::string (ChoiceType)> t_toString,
    std::function<std::vector<ChoiceType> (void)> t_choices,
    std::function<boost::optional<ChoiceType> (DataSourceType*)>  t_getter,
    std::function<bool (DataSourceType*, ChoiceType)> t_setter,
    boost::optional<std::function<void (DataSourceType*)> > t_reset=boost::none)
    : ComboBoxConcept(t_headingLabel),
      m_toString(t_toString),
      m_choices(t_choices),
      m_getter(t_getter),
      m_setter(t_setter),
      m_reset(t_reset)
  {}

  virtual ~ComboBoxOptionalChoiceImpl() {}

  virtual std::shared_ptr<ChoiceConcept> choiceConcept(const ConceptProxy& obj) {
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
  std::function<std::string (ChoiceType)> m_toString;
  std::function<std::vector<ChoiceType> (void)> m_choices;
  std::function<boost::optional<ChoiceType> (DataSourceType *)>  m_getter;
  std::function<bool (DataSourceType *, ChoiceType)> m_setter;
  boost::optional<std::function<void (DataSourceType*)> > m_reset;
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

  virtual ValueType get(const ConceptProxy & obj) = 0;
  virtual bool set(const ConceptProxy & obj, ValueType) = 0;
};

template<typename ValueType, typename DataSourceType>
class ValueEditConceptImpl : public ValueEditConcept<ValueType>
{
  public:

  ValueEditConceptImpl(QString t_headingLabel,
    std::function<ValueType (DataSourceType *)>  t_getter,
    std::function<bool (DataSourceType *, ValueType)> t_setter)
    : ValueEditConcept<ValueType>(t_headingLabel),
      m_getter(t_getter),
      m_setter(t_setter)
  {
  }

  virtual ~ValueEditConceptImpl() {}

  virtual ValueType get(const ConceptProxy & t_obj)
  {
    DataSourceType obj = t_obj.cast<DataSourceType>();
    return m_getter(&obj);
  }

  virtual bool set(const ConceptProxy & t_obj, ValueType value)
  {
    DataSourceType obj = t_obj.cast<DataSourceType>();
    return m_setter(&obj,value);
  }

  private:

  std::function<ValueType (DataSourceType *)>  m_getter;
  std::function<bool (DataSourceType *, ValueType)> m_setter;
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

  virtual boost::optional<ValueType> get(const ConceptProxy & obj) = 0;
  virtual bool set(const ConceptProxy & obj, ValueType) = 0;
};

template<typename ValueType, typename DataSourceType>
class OptionalValueEditConceptImpl : public OptionalValueEditConcept<ValueType>
{
  public:

  OptionalValueEditConceptImpl(QString t_headingLabel,
    std::function<boost::optional<ValueType> (DataSourceType *)>  t_getter,
    std::function<bool (DataSourceType *, ValueType)> t_setter)
    : OptionalValueEditConcept<ValueType>(t_headingLabel),
      m_getter(t_getter),
      m_setter(t_setter)
  {
  }

  virtual ~OptionalValueEditConceptImpl() {}

  virtual boost::optional<ValueType> get(const ConceptProxy & t_obj)
  {
    DataSourceType obj = t_obj.cast<DataSourceType>();
    return m_getter(&obj);
  }

  virtual bool set(const ConceptProxy & t_obj, ValueType value)
  {
    DataSourceType obj = t_obj.cast<DataSourceType>();
    return m_setter(&obj,value);
  }

  private:

  std::function<boost::optional<ValueType> (DataSourceType *)>  m_getter;
  std::function<bool (DataSourceType *, ValueType)> m_setter;
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

  virtual ValueType get(const ConceptProxy & obj) = 0;
  virtual void set(const ConceptProxy & obj, ValueType) = 0;
};

template<typename ValueType, typename DataSourceType>
class ValueEditVoidReturnConceptImpl : public ValueEditVoidReturnConcept<ValueType>
{
  public:

  ValueEditVoidReturnConceptImpl(QString t_headingLabel,
    std::function<ValueType (DataSourceType *)>  t_getter,
    std::function<void (DataSourceType *, ValueType)> t_setter)
    : ValueEditVoidReturnConcept<ValueType>(t_headingLabel),
      m_getter(t_getter),
      m_setter(t_setter)
  {
  }

  virtual ~ValueEditVoidReturnConceptImpl() {}

  virtual ValueType get(const ConceptProxy & t_obj)
  {
    DataSourceType obj = t_obj.cast<DataSourceType>();
    return m_getter(&obj);
  }

  virtual void set(const ConceptProxy & t_obj, ValueType value)
  {
    DataSourceType obj = t_obj.cast<DataSourceType>();
    return m_setter(&obj,value);
  }

  private:

  std::function<ValueType(DataSourceType *)>  m_getter;
  std::function<void (DataSourceType *, ValueType)> m_setter;
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

  virtual boost::optional<ValueType> get(const ConceptProxy & obj) = 0;
  virtual void set(const ConceptProxy & obj, ValueType) = 0;
};

template<typename ValueType, typename DataSourceType>
class OptionalValueEditVoidReturnConceptImpl : public OptionalValueEditVoidReturnConcept<ValueType>
{
  public:

  OptionalValueEditVoidReturnConceptImpl(QString t_headingLabel,
    std::function<boost::optional<ValueType> (DataSourceType *)>  t_getter,
    std::function<void (DataSourceType *, ValueType)> t_setter)
    : OptionalValueEditVoidReturnConcept<ValueType>(t_headingLabel),
      m_getter(t_getter),
      m_setter(t_setter)
  {
  }

  virtual ~OptionalValueEditVoidReturnConceptImpl() {}

  virtual boost::optional<ValueType> get(const ConceptProxy & t_obj)
  {
    DataSourceType obj = t_obj.cast<DataSourceType>();
    return m_getter(&obj);
  }

  virtual void set(const ConceptProxy & t_obj, ValueType value)
  {
    DataSourceType obj = t_obj.cast<DataSourceType>();
    return m_setter(&obj,value);
  }

  private:

  std::function<boost::optional<ValueType> (DataSourceType *)>  m_getter;
  std::function<void (DataSourceType *, ValueType)> m_setter;
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

  virtual boost::optional<std::string> get(const ConceptProxy & obj, bool) = 0;
  virtual boost::optional<std::string> set(const ConceptProxy & obj, const std::string &) = 0;
  virtual bool readOnly() const = 0;
};

template<typename DataSourceType>
class NameLineEditConceptImpl : public NameLineEditConcept
{
  public:

  NameLineEditConceptImpl(QString t_headingLabel,
    std::function<boost::optional<std::string> (DataSourceType *, bool)>  t_getter,
    std::function<boost::optional<std::string> (DataSourceType *, const std::string &)> t_setter)
    : NameLineEditConcept(t_headingLabel),
      m_getter(t_getter),
      m_setter(t_setter)
  {
  }

  virtual ~NameLineEditConceptImpl() {}

  virtual boost::optional<std::string> get(const ConceptProxy & t_obj, bool value)
  {
    DataSourceType obj = t_obj.cast<DataSourceType>();
    return m_getter(&obj,value);
  }

  virtual boost::optional<std::string> set(const ConceptProxy & t_obj, const std::string & value)
  {
    DataSourceType obj = t_obj.cast<DataSourceType>();
    if (m_setter)
    {
      return m_setter(&obj,value);
    } else {
      return boost::optional<std::string>();
    }
  }

  virtual bool readOnly() const
  {
    return m_setter ? false : true;
  }

  private:

  std::function<boost::optional<std::string> (DataSourceType *, bool)>  m_getter;
  std::function<boost::optional<std::string> (DataSourceType *, const std::string &)> m_setter;
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

  virtual ValueType get(const ConceptProxy & obj) = 0;
  virtual bool set(const ConceptProxy & obj, ValueType) = 0;

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
    std::function<ValueType (DataSourceType *)>  t_getter,
    std::function<bool (DataSourceType *, ValueType)> t_setter)
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

  virtual ValueType get(const ConceptProxy & t_obj)
  {
    DataSourceType obj = t_obj.cast<DataSourceType>();
    return m_getter(&obj);
  }

  virtual bool set(const ConceptProxy & t_obj, ValueType value)
  {
    DataSourceType obj = t_obj.cast<DataSourceType>();
    return m_setter(&obj,value);
  }

  private:

  std::function<ValueType (DataSourceType *)>  m_getter;
  std::function<bool (DataSourceType *, ValueType)> m_setter;
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

  virtual boost::optional<ValueType> get(const ConceptProxy & obj) = 0;
  virtual bool set(const ConceptProxy & obj, ValueType) = 0;

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
    std::function<boost::optional<ValueType> (DataSourceType *)>  t_getter,
    std::function<bool (DataSourceType *, ValueType)> t_setter)
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

  virtual boost::optional<ValueType> get(const ConceptProxy & t_obj)
  {
    DataSourceType obj = t_obj.cast<DataSourceType>();
    return m_getter(&obj);
  }

  virtual bool set(const ConceptProxy & t_obj, ValueType value)
  {
    DataSourceType obj = t_obj.cast<DataSourceType>();
    return m_setter(&obj,value);
  }

  private:

  std::function<boost::optional<ValueType> (DataSourceType *)>  m_getter;
  std::function<bool (DataSourceType *, ValueType)> m_setter;
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

  virtual ValueType get(const ConceptProxy & obj) = 0;
  virtual void set(const ConceptProxy & obj, ValueType) = 0;

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
    std::function<ValueType (DataSourceType *)>  t_getter,
    std::function<void (DataSourceType *, ValueType)> t_setter)
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

  virtual ValueType get(const ConceptProxy & t_obj)
  {
    DataSourceType obj = t_obj.cast<DataSourceType>();
    return m_getter(&obj);
  }

  virtual void set(const ConceptProxy & t_obj, ValueType value)
  {
    DataSourceType obj = t_obj.cast<DataSourceType>();
    return m_setter(&obj,value);
  }

  private:

  std::function<ValueType(DataSourceType *)>  m_getter;
  std::function<void (DataSourceType *, ValueType)> m_setter;
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

  virtual boost::optional<ValueType> get(const ConceptProxy & obj) = 0;
  virtual void set(const ConceptProxy & obj, ValueType) = 0;

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
    std::function<boost::optional<ValueType> (DataSourceType *)>  t_getter,
    std::function<void (DataSourceType *, ValueType)> t_setter)
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

  virtual boost::optional<ValueType> get(const ConceptProxy & t_obj)
  {
    DataSourceType obj = t_obj.cast<DataSourceType>();
    return m_getter(&obj);
  }

  virtual void set(const ConceptProxy & t_obj, ValueType value)
  {
    DataSourceType obj = t_obj.cast<DataSourceType>();
    return m_setter(&obj,value);
  }

  private:

  std::function<boost::optional<ValueType> (DataSourceType *)>  m_getter;
  std::function<void (DataSourceType *, ValueType)> m_setter;
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

  virtual boost::optional<model::ModelObject> get(const ConceptProxy & obj) = 0;
  virtual bool set(const ConceptProxy & obj, const model::ModelObject &) = 0;
};

template<typename ValueType, typename DataSourceType>
class DropZoneConceptImpl : public DropZoneConcept 
{
  public:

  DropZoneConceptImpl(QString t_headingLabel,
    std::function<boost::optional<ValueType> (DataSourceType *)>  t_getter,
    std::function<bool (DataSourceType *, const ValueType &)> t_setter)
    : DropZoneConcept(t_headingLabel),
      m_getter(t_getter),
      m_setter(t_setter)
  {
  }

  virtual ~DropZoneConceptImpl() {}

  virtual boost::optional<model::ModelObject> get(const ConceptProxy & obj)
  {
    if(boost::optional<ValueType> result = getImpl(obj)) {
      return result->template optionalCast<model::ModelObject>();
    } else {
      return boost::optional<model::ModelObject>();
    }
  }

  virtual bool set(const ConceptProxy & obj, const model::ModelObject & value)
  {
    if( boost::optional<ValueType> mo = value.optionalCast<ValueType>() ) {
      return setImpl(obj,mo.get());
    } else {
      return false;
    }
  }

  virtual boost::optional<ValueType> getImpl(const ConceptProxy & t_obj)
  {
    DataSourceType obj = t_obj.cast<DataSourceType>();
    return boost::optional<ValueType>(m_getter(&obj));
  }

  virtual bool setImpl(const ConceptProxy & t_obj, const ValueType & t_value)
  {
    DataSourceType obj = t_obj.cast<DataSourceType>();
    return m_setter(&obj,t_value);
  }

  private:

  std::function<boost::optional<ValueType> (DataSourceType *)>  m_getter;
  std::function<bool (DataSourceType *, const ValueType &)> m_setter;
};


} // openstudio

#endif // SHAREDGUICOMPONENTS_OSCONCEPTS_HPP

