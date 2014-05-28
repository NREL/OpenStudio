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

#ifndef SHAREDGUICOMPONENTS_OSCOMBOBOX_HPP
#define SHAREDGUICOMPONENTS_OSCOMBOBOX_HPP

#include "FieldMethodTypedefs.hpp"
#include "OSConcepts.hpp"

#include "../model/Model.hpp"
#include "../model/ModelObject.hpp"

#include "../utilities/idf/WorkspaceObject.hpp"

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

class OSComboBox2 : public QComboBox {
  Q_OBJECT
 public:
  
  OSComboBox2( QWidget * parent = nullptr );

  virtual ~OSComboBox2() {}

  // interface for direct bind
  template<typename ChoiceType>
  void bind(model::ModelObject& modelObject,
            std::function<std::string (ChoiceType)> toString,
            std::function<std::vector<ChoiceType> ()> choices,
            std::function<ChoiceType ()> getter,
            std::function<bool (ChoiceType)> setter,
            boost::optional<NoFailAction> reset=boost::none,
            boost::optional<BasicQuery> isDefaulted=boost::none)
  {
    m_modelObject = modelObject;
    m_choiceConcept = std::shared_ptr<ChoiceConcept>(
          new RequiredChoiceConceptImpl<ChoiceType>(toString,
                                                    choices,
                                                    getter,
                                                    setter,
                                                    reset,
                                                    isDefaulted));
    clear();
    completeBind();
  }

  // interface for direct bind
  template<typename ChoiceType>
  void bind(model::ModelObject& modelObject,
            std::function<std::string (ChoiceType)> toString,
            std::function<std::vector<ChoiceType> ()> choices,
            std::function<boost::optional<ChoiceType> ()> getter,
            std::function<bool (ChoiceType)> setter,
            boost::optional<NoFailAction> reset=boost::none)
  {
    m_modelObject = modelObject;
    m_choiceConcept = std::shared_ptr<ChoiceConcept>(
          new OptionalChoiceConceptImpl<ChoiceType>(toString,
                                                    choices,
                                                    getter,
                                                    setter,
                                                    reset));
    clear();
    completeBind();
  }

  // interface for OSGridController bind
  void bind(model::ModelObject& modelObject,
            std::shared_ptr<ChoiceConcept> choiceConcept)
  {
    m_modelObject = modelObject;
    m_choiceConcept = choiceConcept;
    clear();
    completeBind();
  }

  // Bind to an OSComboBoxDataSource
  void bind(std::shared_ptr<OSComboBoxDataSource> dataSource);

  void unbind();

 protected:

  bool event( QEvent * e );

 private slots:

  void onModelObjectChanged();

  void onModelObjectRemoved(Handle handle);

  void onCurrentIndexChanged(const QString & text);

  void onChoicesRefreshTrigger();

  void onDataSourceChange(int);

  void onDataSourceAdd(int);

  void onDataSourceRemove(int);

 private:
  std::shared_ptr<OSComboBoxDataSource> m_dataSource;

  boost::optional<model::ModelObject> m_modelObject;
  std::shared_ptr<ChoiceConcept> m_choiceConcept;
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

  OSComboBox( QWidget * parent = nullptr );

  virtual ~OSComboBox() {}

  void bind(model::ModelObject & modelObject, const char * property);

  void unbind();

  void setDataSource(std::shared_ptr<OSComboBoxDataSource> dataSource);

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

  std::shared_ptr<OSComboBoxDataSource> m_dataSource;

  boost::optional<model::ModelObject> m_modelObject;

  std::string m_property;

  std::vector<std::string> m_values;
};

} // openstudio

#endif // SHAREDGUICOMPONENTS_OSCOMBOBOX_HPP

