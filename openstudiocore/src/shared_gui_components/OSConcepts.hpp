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

#ifndef SHAREDGUICOMPONENTS_OSCONCEPTS_HPP
#define SHAREDGUICOMPONENTS_OSCONCEPTS_HPP

#include "FieldMethodTypedefs.hpp"

#include "../model/ModelObject.hpp"

#include <boost/any.hpp>
#include <QSharedPointer>
#include <QWidget>

namespace openstudio {

class Heading
{
  public:
    explicit Heading(const QString &t_headingLabel, const bool t_showColumnCheckbox = true, const bool t_showColumnButton = true,
        const QSharedPointer<QWidget> &t_widget = QSharedPointer<QWidget>())
        : m_label(t_headingLabel), m_showColumnCheckbox(t_showColumnCheckbox), m_showColumnButton(t_showColumnButton), m_widget(t_widget)
    {
    }

    QString label() const
    {
      return m_label;
    }

    bool showCheckbox() const
    {
      return m_showColumnCheckbox;
    }

    bool showButton() const
    {
      return m_showColumnButton;
    }

    QSharedPointer<QWidget> widget() const
    {
      return m_widget;
    }

  private:
    QString m_label;
    bool m_showColumnCheckbox;
    bool m_showColumnButton;
    QSharedPointer<QWidget> m_widget;
};

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
        const T* t = boost::any_cast<T>(&m_any);

        if (t) return *t;

        try {
          model::ModelObject obj(boost::any_cast<model::ModelObject>(m_any));

          try {
            return obj.cast<T>();
          } catch (...) {
            assert(false);
            throw;
          }
        } catch (...) {
          assert(false);
          throw;
        }
      }

  private:
    REGISTER_LOGGER("openstudio.ConceptProxy");
    boost::any m_any;
};

class BaseConcept
{
  public:

  virtual ~BaseConcept() {}

  BaseConcept(const Heading &t_heading, bool t_hasClickFocus = false)
    : m_heading(t_heading), m_selector(false), m_hasClickFocus(t_hasClickFocus)
  {
  }

  bool isSelector() const
  {
    return m_selector;
  }

  void setIsSelector(const bool t_selector)
  {
    m_selector = t_selector;
  }

  bool hasClickFocus() const
  {
    return m_hasClickFocus;
  }

  const Heading &heading() const { return m_heading; }

  private:

  Heading m_heading;
  bool m_selector;
  bool m_hasClickFocus;

};


///////////////////////////////////////////////////////////////////////////////////


class CheckBoxConcept : public BaseConcept
{
public:

  CheckBoxConcept(const Heading &t_heading,
    const std::string & t_tooltip)
    : BaseConcept(t_heading),
    m_tooltip(t_tooltip)
  {
  }

  virtual ~CheckBoxConcept() {}


  virtual bool get(const ConceptProxy & obj) = 0;
  virtual void set(const ConceptProxy & obj, bool) = 0;

  const std::string & tooltip() { return m_tooltip; }

  private:

  std::string m_tooltip;

};

template<typename DataSourceType>
class CheckBoxConceptImpl : public CheckBoxConcept
{
  public:

  CheckBoxConceptImpl(const Heading &t_heading,
    const std::string & t_tooltip,
    std::function<bool (DataSourceType *)> t_getter,
    std::function<void (DataSourceType *, bool)> t_setter)
    : CheckBoxConcept(t_heading, t_tooltip),
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
  virtual bool editable() { return false; }
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
    if (!m_choicesMap.size()) {
      // Oops, we forgot to update the choices
      this->choices();
    }
    OS_ASSERT(m_choicesMap.find(result) != m_choicesMap.end());
    return result;
  }

  virtual bool set(std::string value) {
    if (!m_choicesMap.size()) {
      // Oops, we forgot to update the choices
      this->choices();
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
      boost::optional<NoFailAction> reset,
      boost::optional<BasicQuery> isDefaulted)
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
      boost::optional<NoFailAction> reset=boost::none,
      bool editable = false)
    : m_toString(toString),
      m_choices(choices),
      m_getter(getter),
      m_setter(setter),
      m_reset(reset),
      m_editable(editable)
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
        if (!m_choicesMap.size()) {
          // Oops, we forgot to update the choices
          this->choices();
        }
        OS_ASSERT(m_choicesMap.find(result) != m_choicesMap.end());
      }
    }
    return result;
  }

  // overload specific for std::string setter, because we have the option of not doing
  // a lookup, but the call to t_setter can compile only in the case that we know
  // t_setter takes a string
  bool setImpl(const std::string &t_value, const std::function<bool (std::string)> &t_setter)
  {
    if (m_editable) {
      // no reason to do a lookup - it's editable
      return t_setter(t_value);
    } else {
      return setImplWithLookup(t_value, t_setter);
    }
  }

  template<typename T>
  bool setImpl(const std::string &t_value, const std::function<bool (T)> &t_setter)
  {
    return setImplWithLookup(t_value, t_setter);
  }

  template<typename T>
  bool setImplWithLookup(const std::string &t_value, const std::function<bool (T)> &t_setter)
  {
    if (!m_choicesMap.size()) {
      // Oops, we forgot to update the choices
      this->choices();
    }
    typename std::map<std::string,ChoiceType>::const_iterator valuePair = m_choicesMap.find(t_value);
    OS_ASSERT(valuePair != m_choicesMap.end());
    return t_setter(valuePair->second);
  }


  virtual bool set(std::string value) {
    if (value.empty()) {
      clear();
      return true;
    }

    return setImpl(value, m_setter);

  }

  virtual void clear() {
    if (m_reset) {
      (*m_reset)();
    }
  }

  virtual bool editable() {
    return m_editable;
  }

 private:
  std::function<std::string (ChoiceType)> m_toString;
  std::function<std::vector<ChoiceType> ()> m_choices;
  std::function<boost::optional<ChoiceType> ()> m_getter;
  std::function<bool (ChoiceType)> m_setter;
  boost::optional<NoFailAction> m_reset;
  bool m_editable;

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
      boost::optional<NoFailAction> reset=boost::none,
      bool editable = false)
    : OptionalChoiceConceptImpl<ChoiceType>(toString,
                                            choices,
                                            getter,
                                            setter,
                                            reset,
                                            editable),
      m_dataSource(dataSource)
  {}

  virtual ~OptionalChoiceSaveDataSourceConceptImpl() {}

 private:
  std::shared_ptr<DataSourceType> m_dataSource;
};


class ComboBoxConcept : public BaseConcept
{
  public:

    ComboBoxConcept(const Heading &t_heading, bool t_hasClickFocus = true)
      : BaseConcept(t_heading, t_hasClickFocus)
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
    const Heading &t_heading,
    std::function<std::string (ChoiceType)> t_toString,
    std::function<std::vector<ChoiceType> (DataSourceType*)> t_choices,
    std::function<ChoiceType (DataSourceType*)>  t_getter,
    std::function<bool (DataSourceType*, ChoiceType)> t_setter,
    boost::optional<std::function<void (DataSourceType*)> > t_reset = boost::none,
    boost::optional<std::function<bool (DataSourceType*)> > t_defaulted = boost::none)
    : ComboBoxConcept(t_heading),
      m_toString(t_toString),
      m_choices(t_choices),
      m_getter(t_getter),
      m_setter(t_setter),
      m_reset(t_reset),
      m_defaulted(t_defaulted)
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
            std::bind(m_choices, dataSource.get()),
            std::bind(m_getter,dataSource.get()),
            std::bind(m_setter,dataSource.get(),std::placeholders::_1),
            m_reset?boost::optional<std::function<void ()>>(std::bind(*m_reset, dataSource.get())):boost::none,
            m_defaulted?boost::optional<std::function<bool ()>>(std::bind(*m_defaulted, dataSource.get())):boost::none
            )
        );
  }

 private:
  std::function<std::string (ChoiceType)> m_toString;
  std::function<std::vector<ChoiceType> (DataSourceType *)> m_choices;
  std::function<ChoiceType (DataSourceType *)>  m_getter;
  std::function<bool (DataSourceType *, ChoiceType)> m_setter;
  boost::optional<std::function<void (DataSourceType*)> > m_reset;
  boost::optional<std::function<bool (DataSourceType*)> > m_defaulted;
};

template<typename ChoiceType, typename DataSourceType>
class ComboBoxOptionalChoiceImpl : public ComboBoxConcept
{
  public:

  ComboBoxOptionalChoiceImpl(
    const Heading &t_heading,
    std::function<std::string (ChoiceType)> t_toString,
    std::function<std::vector<ChoiceType> (DataSourceType *)> t_choices,
    std::function<boost::optional<ChoiceType> (DataSourceType*)>  t_getter,
    std::function<bool (DataSourceType*, ChoiceType)> t_setter,
    boost::optional<std::function<void (DataSourceType*)> > t_reset=boost::none,
    bool t_editable = false)
    : ComboBoxConcept(t_heading),
      m_toString(t_toString),
      m_choices(t_choices),
      m_getter(t_getter),
      m_setter(t_setter),
      m_reset(t_reset),
      m_editable(t_editable)
  {}

  virtual ~ComboBoxOptionalChoiceImpl() {}

  virtual std::shared_ptr<ChoiceConcept> choiceConcept(const ConceptProxy& obj) {
    std::shared_ptr<DataSourceType> dataSource = std::shared_ptr<DataSourceType>(
        new DataSourceType(obj.cast<DataSourceType>()));

    boost::optional<NoFailAction> resetAction;
    if (m_reset) {
      resetAction = std::bind(m_reset.get(), dataSource.get());
    }

    return std::make_shared<OptionalChoiceSaveDataSourceConceptImpl<ChoiceType,DataSourceType>>(
              dataSource,
              m_toString,
              std::bind(m_choices,dataSource.get()),
              std::bind(m_getter,dataSource.get()),
              std::bind(m_setter,dataSource.get(),std::placeholders::_1),
              resetAction,
              m_editable);

  }

 private:
  std::function<std::string (ChoiceType)> m_toString;
  std::function<std::vector<ChoiceType> (DataSourceType *)> m_choices;
  std::function<boost::optional<ChoiceType> (DataSourceType *)>  m_getter;
  std::function<bool (DataSourceType *, ChoiceType)> m_setter;
  boost::optional<std::function<void (DataSourceType*)> > m_reset;
  bool m_editable;
};

///////////////////////////////////////////////////////////////////////////////////


template<typename ValueType>
class ValueEditConcept : public BaseConcept
{
  public:

    ValueEditConcept(const Heading &t_heading, bool t_hasClickFocus = true)
      : BaseConcept(t_heading, t_hasClickFocus)
  {
  }

  virtual ~ValueEditConcept() {}

  virtual ValueType get(const ConceptProxy & obj) = 0;
  virtual bool set(const ConceptProxy & obj, ValueType) = 0;
  virtual void reset(const ConceptProxy & obj) = 0;
  virtual bool isDefaulted(const ConceptProxy &obj) = 0;
};

template<typename ValueType, typename DataSourceType>
class ValueEditConceptImpl : public ValueEditConcept<ValueType>
{
  public:

  ValueEditConceptImpl(const Heading &t_heading,
    std::function<ValueType (DataSourceType *)>  t_getter,
    std::function<bool (DataSourceType *, ValueType)> t_setter,
    boost::optional<std::function<void (DataSourceType *)>> t_reset,
    boost::optional<std::function<bool (DataSourceType *)>> t_isDefaulted
    )
    : ValueEditConcept<ValueType>(t_heading),
      m_getter(t_getter),
      m_setter(t_setter),
      m_reset(t_reset),
      m_isDefaulted(t_isDefaulted)
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

  virtual void reset(const ConceptProxy &t_obj)
  {
    if (m_reset)
    {
      DataSourceType obj = t_obj.cast<DataSourceType>();
      (*m_reset)(&obj);
    }
  }

  virtual bool isDefaulted(const ConceptProxy &t_obj)
  {
    if (m_isDefaulted)
    {
      DataSourceType obj = t_obj.cast<DataSourceType>();
      return (*m_isDefaulted)(&obj);
    } else {
      return false;
    }
  }

  private:

  std::function<ValueType (DataSourceType *)>  m_getter;
  std::function<bool (DataSourceType *, ValueType)> m_setter;
  boost::optional<std::function<void (DataSourceType *)>> m_reset;
  boost::optional<std::function<bool (DataSourceType *)>> m_isDefaulted;
};


///////////////////////////////////////////////////////////////////////////////////

template<typename ValueType>
class OptionalValueEditConcept : public BaseConcept
{
  public:

    OptionalValueEditConcept(const Heading &t_heading, bool t_hasClickFocus = true)
      : BaseConcept(t_heading, t_hasClickFocus)
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

  OptionalValueEditConceptImpl(const Heading &t_heading,
    std::function<boost::optional<ValueType> (DataSourceType *)>  t_getter,
    std::function<bool (DataSourceType *, ValueType)> t_setter)
    : OptionalValueEditConcept<ValueType>(t_heading),
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

    ValueEditVoidReturnConcept(const Heading &t_heading, bool t_hasClickFocus = true)
      : BaseConcept(t_heading, t_hasClickFocus)
  {
  }

  virtual ~ValueEditVoidReturnConcept() {}

  virtual ValueType get(const ConceptProxy & obj) = 0;
  virtual void set(const ConceptProxy & obj, ValueType) = 0;
  virtual void reset(const ConceptProxy &t_obj) = 0;
  virtual bool isDefaulted(const ConceptProxy &t_obj) = 0;
};

template<typename ValueType, typename DataSourceType>
class ValueEditVoidReturnConceptImpl : public ValueEditVoidReturnConcept<ValueType>
{
  public:

  ValueEditVoidReturnConceptImpl(const Heading &t_heading,
    std::function<ValueType (DataSourceType *)>  t_getter,
    std::function<void (DataSourceType *, ValueType)> t_setter,
    boost::optional<std::function<void (DataSourceType *)>> t_reset,
    boost::optional<std::function<bool (DataSourceType *)>> t_isDefaulted)
    : ValueEditVoidReturnConcept<ValueType>(t_heading),
      m_getter(t_getter),
      m_setter(t_setter),
      m_reset(t_reset),
      m_isDefaulted(t_isDefaulted)
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

  virtual void reset(const ConceptProxy &t_obj)
  {
    if (m_reset)
    {
      DataSourceType obj = t_obj.cast<DataSourceType>();
      (*m_reset)(&obj);
    }
  }

  virtual bool isDefaulted(const ConceptProxy &t_obj)
  {
    if (m_isDefaulted)
    {
      DataSourceType obj = t_obj.cast<DataSourceType>();
      return (*m_isDefaulted)(&obj);
    } else {
      return false;
    }
  }


  private:

  std::function<ValueType(DataSourceType *)>  m_getter;
  std::function<void (DataSourceType *, ValueType)> m_setter;
  boost::optional<std::function<void (DataSourceType *)>> m_reset;
  boost::optional<std::function<bool (DataSourceType *)>> m_isDefaulted;
};


///////////////////////////////////////////////////////////////////////////////////


template<typename ValueType>
class OptionalValueEditVoidReturnConcept : public BaseConcept
{
  public:

    OptionalValueEditVoidReturnConcept(const Heading &t_heading, bool t_hasClickFocus = true)
      : BaseConcept(t_heading, t_hasClickFocus)
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

  OptionalValueEditVoidReturnConceptImpl(const Heading &t_heading,
    std::function<boost::optional<ValueType> (DataSourceType *)>  t_getter,
    std::function<void (DataSourceType *, ValueType)> t_setter)
    : OptionalValueEditVoidReturnConcept<ValueType>(t_heading),
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

    NameLineEditConcept(const Heading &t_heading, 
      bool t_isInspectable,
      bool t_deleteObject,
      bool t_hasClickFocus = true)
      : BaseConcept(t_heading, t_hasClickFocus),
        m_isInspectable(t_isInspectable),
        m_deleteObject(t_deleteObject)
  {
  }

  virtual ~NameLineEditConcept() {}

  virtual boost::optional<std::string> get(const ConceptProxy & obj, bool) = 0;
  virtual boost::optional<std::string> set(const ConceptProxy & obj, const std::string &) = 0;
  virtual void reset(const ConceptProxy & obj) = 0;
  virtual bool readOnly() const = 0;
  bool isInspectable() { return m_isInspectable; }
  bool deleteObject() { return m_deleteObject; }

  private:

  bool m_isInspectable;
  bool m_deleteObject;

};

template<typename DataSourceType>
class NameLineEditConceptImpl : public NameLineEditConcept
{
  public:

  NameLineEditConceptImpl(const Heading &t_heading,
    bool t_isInspectable,
    bool t_deleteObject,
    std::function<boost::optional<std::string>(DataSourceType *, bool)>  t_getter,
    std::function<boost::optional<std::string> (DataSourceType *, const std::string &)> t_setter,
    boost::optional<std::function<void(DataSourceType*)> > t_reset = boost::none)
    : NameLineEditConcept(t_heading,
      t_isInspectable,
      t_deleteObject),
      m_getter(t_getter),
      m_setter(t_setter),
      m_reset(t_reset)
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

  virtual void reset(const ConceptProxy & t_obj)
  {
    if (m_reset) {
      DataSourceType obj = t_obj.cast<DataSourceType>();
      (*m_reset)(&obj);
    }
  }

  virtual bool readOnly() const
  {
    return m_setter ? false : true;
  }

  private:

  std::function<boost::optional<std::string> (DataSourceType *, bool)>  m_getter;
  std::function<boost::optional<std::string> (DataSourceType *, const std::string &)> m_setter;
  boost::optional<std::function<void(DataSourceType*)> > m_reset;

};


///////////////////////////////////////////////////////////////////////////////////


class LoadNameConcept : public BaseConcept
{
public:

  LoadNameConcept(const Heading &t_heading, bool t_hasClickFocus = true)
    : BaseConcept(t_heading, t_hasClickFocus)
  {
  }

  virtual ~LoadNameConcept() {}

  virtual boost::optional<std::string> get(const ConceptProxy & obj, bool) = 0;
  virtual boost::optional<std::string> set(const ConceptProxy & obj, const std::string &) = 0;
  virtual void reset(const ConceptProxy & obj) = 0;
  virtual bool readOnly() const = 0;
};

template<typename DataSourceType>
class LoadNameConceptImpl : public LoadNameConcept
{
public:

  LoadNameConceptImpl(const Heading &t_heading,
    std::function<boost::optional<std::string>(DataSourceType *, bool)>  t_getter,
    std::function<boost::optional<std::string>(DataSourceType *, const std::string &)> t_setter,
    boost::optional<std::function<void(DataSourceType*)> > t_reset = boost::none)
    : LoadNameConcept(t_heading),
    m_getter(t_getter),
    m_setter(t_setter),
    m_reset(t_reset)
  {
  }

  virtual ~LoadNameConceptImpl() {}

  virtual boost::optional<std::string> get(const ConceptProxy & t_obj, bool value)
  {
    DataSourceType obj = t_obj.cast<DataSourceType>();
    return m_getter(&obj, value);
  }

  virtual boost::optional<std::string> set(const ConceptProxy & t_obj, const std::string & value)
  {
    DataSourceType obj = t_obj.cast<DataSourceType>();
    if (m_setter)
    {
      return m_setter(&obj, value);
    }
    else {
      return boost::optional<std::string>();
    }
  }

  virtual void reset(const ConceptProxy & t_obj)
  {
    if (m_reset) {
      DataSourceType obj = t_obj.cast<DataSourceType>();
      (*m_reset)(&obj);
    }
  }

  virtual bool readOnly() const
  {
    return m_setter ? false : true;
  }

private:

  std::function<boost::optional<std::string>(DataSourceType *, bool)>  m_getter;
  std::function<boost::optional<std::string>(DataSourceType *, const std::string &)> m_setter;
  boost::optional<std::function<void(DataSourceType*)> > m_reset;

};


///////////////////////////////////////////////////////////////////////////////////


template<typename ValueType>
class QuantityEditConcept : public BaseConcept
{
  public:

    QuantityEditConcept(const Heading &t_heading, QString t_modelUnits, QString t_siUnits, QString t_ipUnits, bool t_isIP, bool t_hasClickFocus = true)
      : BaseConcept(t_heading, t_hasClickFocus),
      m_modelUnits(t_modelUnits),
      m_siUnits(t_siUnits),
      m_ipUnits(t_ipUnits),
      m_isIP(t_isIP)
  {
  }

  virtual ~QuantityEditConcept() {}

  virtual ValueType get(const ConceptProxy & obj) = 0;
  virtual bool set(const ConceptProxy & obj, ValueType) = 0;
  virtual void reset(const ConceptProxy & obj) = 0;
  virtual bool isDefaulted(const ConceptProxy &obj) = 0;

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

  QuantityEditConceptImpl(const Heading &t_heading,
    QString t_modelUnits,
    QString t_siUnits,
    QString t_ipUnits,
    bool t_isIP,
    std::function<ValueType (DataSourceType *)>  t_getter,
    std::function<bool (DataSourceType *, ValueType)> t_setter,
    boost::optional<std::function<void (DataSourceType *)>> t_reset,
    boost::optional<std::function<bool (DataSourceType *)>> t_isDefaulted)
    : QuantityEditConcept<ValueType>(t_heading,
      t_modelUnits,
      t_siUnits,
      t_ipUnits,
      t_isIP),
      m_getter(t_getter),
      m_setter(t_setter),
      m_reset(t_reset),
      m_isDefaulted(t_isDefaulted)

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

  virtual void reset(const ConceptProxy &t_obj)
  {
    if (m_reset)
    {
      DataSourceType obj = t_obj.cast<DataSourceType>();
      (*m_reset)(&obj);
    }
  }

  virtual bool isDefaulted(const ConceptProxy &t_obj)
  {
    if (m_isDefaulted)
    {
      DataSourceType obj = t_obj.cast<DataSourceType>();
      return (*m_isDefaulted)(&obj);
    } else {
      return false;
    }
  }


  private:

  std::function<ValueType (DataSourceType *)>  m_getter;
  std::function<bool (DataSourceType *, ValueType)> m_setter;
  boost::optional<std::function<void (DataSourceType *)>> m_reset;
  boost::optional<std::function<bool (DataSourceType *)>> m_isDefaulted;
};


///////////////////////////////////////////////////////////////////////////////////


template<typename ValueType>
class OptionalQuantityEditConcept : public BaseConcept
{
  public:

    OptionalQuantityEditConcept(const Heading &t_heading, QString t_modelUnits, QString t_siUnits, QString t_ipUnits, bool t_isIP, bool t_hasClickFocus = true)
      : BaseConcept(t_heading, t_hasClickFocus),
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

  OptionalQuantityEditConceptImpl(const Heading &t_heading,
    QString t_modelUnits,
    QString t_siUnits,
    QString t_ipUnits,
    bool t_isIP,
    std::function<boost::optional<ValueType> (DataSourceType *)>  t_getter,
    std::function<bool (DataSourceType *, ValueType)> t_setter)
    : OptionalQuantityEditConcept<ValueType>(t_heading,
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

    QuantityEditVoidReturnConcept(const Heading &t_heading, QString t_modelUnits, QString t_siUnits, QString t_ipUnits, bool t_isIP, bool t_hasClickFocus = true)
      : BaseConcept(t_heading, t_hasClickFocus),
      m_modelUnits(t_modelUnits),
      m_siUnits(t_siUnits),
      m_ipUnits(t_ipUnits),
      m_isIP(t_isIP)
  {
  }

  virtual ~QuantityEditVoidReturnConcept() {}

  virtual ValueType get(const ConceptProxy & obj) = 0;
  virtual void set(const ConceptProxy & obj, ValueType) = 0;
  virtual void reset(const ConceptProxy &t_obj) = 0;
  virtual bool isDefaulted(const ConceptProxy &t_obj) = 0;

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

  QuantityEditVoidReturnConceptImpl(const Heading &t_heading,
    QString t_modelUnits,
    QString t_siUnits,
    QString t_ipUnits,
    bool t_isIP,
    std::function<ValueType (DataSourceType *)>  t_getter,
    std::function<void (DataSourceType *, ValueType)> t_setter,
    boost::optional<std::function<void (DataSourceType *)>> t_reset,
    boost::optional<std::function<bool (DataSourceType *)>> t_isDefaulted
    )
    : QuantityEditVoidReturnConcept<ValueType>(t_heading,
      t_modelUnits,
      t_siUnits,
      t_ipUnits,
      t_isIP),
      m_getter(t_getter),
      m_setter(t_setter),
      m_reset(t_reset),
      m_isDefaulted(t_isDefaulted)
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

  virtual void reset(const ConceptProxy &t_obj)
  {
    if (m_reset)
    {
      DataSourceType obj = t_obj.cast<DataSourceType>();
      (*m_reset)(&obj);
    }
  }

  virtual bool isDefaulted(const ConceptProxy &t_obj)
  {
    if (m_isDefaulted)
    {
      DataSourceType obj = t_obj.cast<DataSourceType>();
      return (*m_isDefaulted)(&obj);
    } else {
      return false;
    }
  }


  private:

  std::function<ValueType(DataSourceType *)>  m_getter;
  std::function<void (DataSourceType *, ValueType)> m_setter;
  boost::optional<std::function<void (DataSourceType *)>> m_reset;
  boost::optional<std::function<bool (DataSourceType *)>> m_isDefaulted;
};


///////////////////////////////////////////////////////////////////////////////////


template<typename ValueType>
class OptionalQuantityEditVoidReturnConcept : public BaseConcept
{
  public:

    OptionalQuantityEditVoidReturnConcept(const Heading &t_heading, QString t_modelUnits, QString t_siUnits, QString t_ipUnits, bool t_isIP, bool t_hasClickFocus = true)
      : BaseConcept(t_heading, t_hasClickFocus),
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

  OptionalQuantityEditVoidReturnConceptImpl(const Heading &t_heading,
    QString t_modelUnits,
    QString t_siUnits,
    QString t_ipUnits,
    bool t_isIP,
    std::function<boost::optional<ValueType> (DataSourceType *)>  t_getter,
    std::function<void (DataSourceType *, ValueType)> t_setter)
    : OptionalQuantityEditVoidReturnConcept<ValueType>(t_heading,
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

    DropZoneConcept(const Heading &t_heading, bool t_hasClickFocus = true)
      : BaseConcept(t_heading, t_hasClickFocus)
  {
  }

  virtual ~DropZoneConcept() {}

  virtual boost::optional<model::ModelObject> get(const ConceptProxy & obj) = 0;
  virtual bool set(const ConceptProxy & obj, const model::ModelObject &) = 0;
  virtual void reset(const ConceptProxy & obj) = 0;
};

template<typename ValueType, typename DataSourceType>
class DropZoneConceptImpl : public DropZoneConcept 
{
  public:

  DropZoneConceptImpl(const Heading &t_heading,
    std::function<boost::optional<ValueType> (DataSourceType *)>  t_getter,
    std::function<bool(DataSourceType *, const ValueType &)> t_setter,
    boost::optional<std::function<void(DataSourceType*)> > t_reset = boost::none)
    : DropZoneConcept(t_heading),
      m_getter(t_getter),
      m_setter(t_setter),
      m_reset(t_reset)
  {
    //boost::optional<NoFailAction> resetAction;
    //if (m_reset) {
    //  resetAction = std::bind(m_reset.get(), dataSource.get());
    //}
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

  virtual void reset(const ConceptProxy & obj)
  {
    resetImpl(obj);
  }

  virtual boost::optional<ValueType> getImpl(const ConceptProxy & t_obj)
  {
    if (m_getter) {
      DataSourceType obj = t_obj.cast<DataSourceType>();
      return boost::optional<ValueType>(m_getter(&obj));
    } else {
      return boost::optional<ValueType>();
    }
  }

  virtual bool setImpl(const ConceptProxy & t_obj, const ValueType & t_value)
  {
    DataSourceType obj = t_obj.cast<DataSourceType>();
    return m_setter(&obj,t_value);
  }

  virtual void resetImpl(const ConceptProxy & t_obj)
  {
    if (m_reset) {
      DataSourceType obj = t_obj.cast<DataSourceType>();
      (*m_reset)(&obj);
    }
  }

  private:

  std::function<boost::optional<ValueType> (DataSourceType *)>  m_getter;
  std::function<bool(DataSourceType *, const ValueType &)> m_setter;
  boost::optional<std::function<void(DataSourceType*)> > m_reset;

};


///////////////////////////////////////////////////////////////////////////////////

class RenderingColorConcept : public BaseConcept
{
public:

  RenderingColorConcept(const Heading &t_heading)
    : BaseConcept(t_heading)
  {
  }

  virtual ~RenderingColorConcept() {}

  virtual boost::optional<model::ModelObject> get(const ConceptProxy & obj) = 0;
  virtual bool set(const ConceptProxy & obj, const model::ModelObject &) = 0;
};

template<typename ValueType, typename DataSourceType>
class RenderingColorConceptImpl : public RenderingColorConcept
{
public:

  RenderingColorConceptImpl(const Heading &t_heading,
    std::function<boost::optional<ValueType>(DataSourceType *)>  t_getter,
    std::function<bool(DataSourceType *, const ValueType &)> t_setter)
    : RenderingColorConcept(t_heading),
    m_getter(t_getter),
    m_setter(t_setter)
  {
  }

  virtual ~RenderingColorConceptImpl() {}

  virtual boost::optional<model::ModelObject> get(const ConceptProxy & obj)
  {
    if (boost::optional<ValueType> result = getImpl(obj)) {
      return result->template optionalCast<model::ModelObject>();
    }
    else {
      return boost::optional<model::ModelObject>();
    }
  }

  virtual bool set(const ConceptProxy & obj, const model::ModelObject & value)
  {
    if (boost::optional<ValueType> mo = value.optionalCast<ValueType>()) {
      return setImpl(obj, mo.get());
    }
    else {
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
    return m_setter(&obj, t_value);
  }

private:

  std::function<boost::optional<ValueType>(DataSourceType *)>  m_getter;
  std::function<bool(DataSourceType *, const ValueType &)> m_setter;
};

} // openstudio

#endif // SHAREDGUICOMPONENTS_OSCONCEPTS_HPP

