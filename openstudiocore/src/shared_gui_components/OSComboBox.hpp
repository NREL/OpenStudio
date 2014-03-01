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

#ifndef OPENSTUDIO_OSCOMBOBOX_H
#define OPENSTUDIO_OSCOMBOBOX_H

#include <shared_gui_components/FieldMethodTypedefs.hpp>

#include <model/Model.hpp>
#include <model/ModelObject.hpp>

#include <utilities/idf/WorkspaceObject.hpp>

#include <QComboBox>
#include <QList>

#include <vector>

namespace openstudio {

class OSComboBoxDataSource : public QObject
{
  Q_OBJECT

  public:

  virtual ~OSComboBoxDataSource() {}

  virtual int numberOfItems() = 0;

  virtual QString valueAt(int i) = 0;

  signals:

  void itemChanged(int);

  void itemAdded(int);

  void itemRemoved(int);
};

class OSObjectListCBDS : public OSComboBoxDataSource
{
  Q_OBJECT

  public:

  OSObjectListCBDS(const IddObjectType & type, const model::Model & model);

  OSObjectListCBDS(const std::vector<IddObjectType> & types, const model::Model & model);

  virtual ~OSObjectListCBDS() {}

  int numberOfItems();

  QString valueAt(int i);

  protected:

  bool m_allowEmptySelection;

  private slots:

  void onObjectAdded(const WorkspaceObject&);

  void onObjectWillBeRemoved(const WorkspaceObject&);

  void onObjectChanged();

  private:

  void initialize();

  std::vector<IddObjectType> m_types;

  model::Model m_model;

  QList<WorkspaceObject> m_workspaceObjects;
};

class ChoiceConcept {
 public:
  virtual ~ChoiceConcept() {}

  virtual std::vector<std::string> choices() = 0;
  virtual std::string get() = 0;
  virtual bool set(std::string value) = 0;
  virtual void clear() {}
  virtual bool isDefaulted() { return false; }
};

// ValueType get()
// bool set(ValueType)
// void clear()
// bool isDefaulted()

// ValueType get()
// bool set(ValueType)

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

// boost::optional<ValueType> get()
// bool set(ValueType)
// void clear()

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
    if (typedValue) {
      std::string result = m_toString(*typedValue);
      OS_ASSERT(m_choicesMap.find(result) != m_choicesMap.end());
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


class OSComboBox2 : public QComboBox {
  Q_OBJECT
 public:
  
  OSComboBox2( QWidget * parent = 0 );

  virtual ~OSComboBox2() {}

  template<typename ChoiceType>
  void bind(model::ModelObject& modelObject,
            boost::function<std::string (ChoiceType)> toString,
            boost::function<std::vector<ChoiceType> ()> choices,
            boost::function<ChoiceType ()> getter,
            boost::function<bool (ChoiceType)> setter,
            boost::optional<NoFailAction> reset=boost::none,
            boost::optional<BasicQuery> isDefaulted=boost::none)
  {
    m_modelObject = modelObject;
    m_choiceConcept = boost::shared_ptr<ChoiceConcept>(
          new RequiredChoiceConceptImpl<ChoiceType>(toString,
                                                    choices,
                                                    getter,
                                                    setter,
                                                    reset,
                                                    isDefaulted));
    clear();
    completeBind();
  }

  template<typename ChoiceType>
  void bind(model::ModelObject& modelObject,
            boost::function<std::string (ChoiceType)> toString,
            boost::function<std::vector<ChoiceType> ()> choices,
            boost::function<boost::optional<ChoiceType> ()> getter,
            boost::function<bool (ChoiceType)> setter,
            boost::optional<NoFailAction> reset=boost::none)
  {
    m_modelObject = modelObject;
    m_choiceConcept = boost::shared_ptr<ChoiceConcept>(
          new OptionalChoiceConceptImpl<ChoiceType>(toString,
                                                    choices,
                                                    getter,
                                                    setter,
                                                    reset));
    clear();
    completeBind();
  }

  // Bind to an OSComboBoxDataSource
  void bind(boost::shared_ptr<OSComboBoxDataSource> dataSource);

  void unbind();

 protected:

  bool event( QEvent * e );

 private slots:

  void onModelObjectChanged();

  void onModelObjectRemoved(Handle handle);

  void onCurrentIndexChanged(const QString & text);

  void onDataSourceChange(int);

  void onDataSourceAdd(int);

  void onDataSourceRemove(int);

 private:
  boost::shared_ptr<OSComboBoxDataSource> m_dataSource;

  boost::optional<model::ModelObject> m_modelObject;
  boost::shared_ptr<ChoiceConcept> m_choiceConcept;
  std::vector<std::string> m_values;

  void completeBind();
};

/**
 * OSComboBox is a derived class of QComboBox that is made to easily bind to an OpenStudio
 * model choice field.
 *
 * Alternatively, a OSComboBoxDataSource can be set to provide data to OSComoboBox.
 **/
class OSComboBox : public QComboBox {
  Q_OBJECT

 public:

  OSComboBox( QWidget * parent = 0 );

  virtual ~OSComboBox() {}

  void bind(model::ModelObject & modelObject, const char * property);

  void unbind();

  void setDataSource(boost::shared_ptr<OSComboBoxDataSource> dataSource);

 protected:

  bool event( QEvent * e );

 private slots:

  void onDataSourceChange(int);

  void onDataSourceAdd(int);
  
  void onDataSourceRemove(int);

  void onModelObjectChanged();

  void onModelObjectRemoved(Handle handle);

  void onCurrentIndexChanged(const QString & text);

 private:

  boost::shared_ptr<OSComboBoxDataSource> m_dataSource;

  boost::optional<model::ModelObject> m_modelObject;

  std::string m_property;

  std::vector<std::string> m_values;
};

} // openstudio

#endif // OPENSTUDIO_OSCOMBOBOX_H

