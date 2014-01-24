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

#include "OSComboBox.hpp"
#include <model/Model.hpp>
#include <model/Model_Impl.hpp>
#include <model/ModelObject.hpp>
#include <model/ModelObject_Impl.hpp>
#include <utilities/idf/WorkspaceObject.hpp>
#include <utilities/idf/WorkspaceObject_Impl.hpp>
#include <utilities/core/Assert.hpp>
#include <QEvent>

namespace openstudio {

OSComboBox2::OSComboBox2( QWidget * parent )
  : QComboBox(parent)
{
  this->setAcceptDrops(false);
  setEnabled(false);
}

bool OSComboBox2::event( QEvent * e )
{
  if( e->type() == QEvent::Wheel )
  {
    return false;
  }
  else
  {
    return QComboBox::event(e);
  }
}

void OSComboBox2::bind(model::ModelObject& modelObject,
                       ChoicesGetter choices, 
                       StringGetter get,
                       boost::optional<StringSetter> set,
                       boost::optional<NoFailAction> reset,
                       boost::optional<BasicQuery> isDefaulted)
{
  m_modelObject = modelObject;
  m_choices = choices;
  m_get = get;
  m_set = set;
  m_reset = reset;
  m_isDefaulted = isDefaulted;

  m_dataSource.reset();
  clear();
  completeBind();
}

void OSComboBox2::bind(model::ModelObject& modelObject,
                       ChoicesGetter choices, 
                       OptionalStringGetter get,
                       boost::optional<StringSetter> set,
                       boost::optional<NoFailAction> reset,
                       boost::optional<BasicQuery> isDefaulted)
{
  m_modelObject = modelObject;
  m_choices = choices;
  m_getOptional = get;
  m_set = set;
  m_reset = reset;
  m_isDefaulted = isDefaulted;

  m_dataSource.reset();
  clear();
  completeBind();
}

void OSComboBox2::unbind() {
  if (m_modelObject){
    disconnect( m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get() );
  }
  m_modelObject.reset();
  m_choices.reset();
  m_get.reset();
  m_getOptional.reset();
  m_set.reset();
  m_reset.reset();
  m_isDefaulted.reset();
  m_dataSource.reset();

  this->blockSignals(true);

  clear();

  this->blockSignals(false);

  setEnabled(false);
}

void OSComboBox2::onModelObjectChanged() {
  OS_ASSERT(m_modelObject);

  OptionalString oValue;
  if (m_get) {
    oValue = (*m_get)();
  }
  else {
    OS_ASSERT(m_getOptional);
    oValue = (*m_getOptional)();
  }

  std::string value;
  if (oValue) {
    value = *oValue;
  }

  int i = 0;
  for( std::vector<std::string>::iterator it = m_values.begin();
       it < m_values.end();
       ++it )
  {
    if( istringEqual(*it,value) )
    {
      this->blockSignals(true);
      setCurrentIndex(i);
      this->blockSignals(false);
      break;
    }

    i++;
  }
}

void OSComboBox2::onModelObjectRemoved(Handle handle)
{
  unbind();
}

void OSComboBox2::onCurrentIndexChanged(const QString & text)
{
  OS_ASSERT(m_modelObject);
  OS_ASSERT(m_set); // should only be enabled if there is a setter

  std::string value = text.toStdString();
  (*m_set)(value);

  // test if property changed
  OptionalString oValue;
  if (m_get) {
    oValue = (*m_get)();
  }
  else {
    OS_ASSERT(m_getOptional);
    oValue = (*m_getOptional)();
  }
  std::string actualValue;
  if (oValue) {
    actualValue = *oValue;
  }

  if (!istringEqual(actualValue, value)) {
    // failed, reset combo box
    onModelObjectChanged();
  }
}

void OSComboBox2::setDataSource(boost::shared_ptr<OSComboBoxDataSource> dataSource)
{
  unbind();

  if( m_dataSource )
  {
    disconnect(m_dataSource.get(),SIGNAL(itemChanged(int)),this,SLOT(onDataSourceChange(int)));
    disconnect(m_dataSource.get(),SIGNAL(itemAdded(int)),this,SLOT(onDataSourceAdd(int)));
    disconnect(m_dataSource.get(),SIGNAL(itemRemoved(int)),this,SLOT(onDataSourceRemove(int)));
  }

  m_dataSource = dataSource;

  connect(m_dataSource.get(),SIGNAL(itemChanged(int)),this,SLOT(onDataSourceChange(int)));
  connect(m_dataSource.get(),SIGNAL(itemAdded(int)),this,SLOT(onDataSourceAdd(int)));
  connect(m_dataSource.get(),SIGNAL(itemRemoved(int)),this,SLOT(onDataSourceRemove(int)));

  this->clear();

  for( int i = 0;
       i < m_dataSource->numberOfItems();
       i++ )
  {
    this->addItem(m_dataSource->valueAt(i));
  }

  setCurrentIndex(-1);

  setEnabled(true);
}

void OSComboBox2::onDataSourceChange(int i)
{
  this->setItemText(i,m_dataSource->valueAt(i));
}

void OSComboBox2::onDataSourceAdd(int i)
{
  int oldIndex = this->currentIndex();

  this->insertItem(i,m_dataSource->valueAt(i));

  if( oldIndex == -1 )
  {
    setCurrentIndex(oldIndex);
  }
}

void OSComboBox2::onDataSourceRemove(int i)
{
  if( currentIndex() == i )
  {
    this->setCurrentIndex(-1);
  }

  this->removeItem(i);
}

void OSComboBox2::completeBind() {
  OS_ASSERT(m_modelObject);
  OS_ASSERT(m_choices);

  // Connections

  bool isConnected = false;
  isConnected = connect( m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get(),SIGNAL(onChange()),
                         this,SLOT(onModelObjectChanged()) );
  OS_ASSERT(isConnected);

  isConnected = connect( m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get(),SIGNAL(onRemoveFromWorkspace(Handle)),
                         this,SLOT(onModelObjectRemoved(Handle)) );
  OS_ASSERT(isConnected);

  isConnected = connect( this, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(onCurrentIndexChanged(const QString&)) );
  OS_ASSERT(isConnected);

  // Populate choices
  m_values = (*m_choices)();
  if (m_getOptional) {
    // can be blank
    m_values.insert(m_values.begin(),std::string());
  }

  this->blockSignals(true);

  for( std::vector<std::string>::iterator it = m_values.begin();
       it < m_values.end();
       ++it )
  {
    addItem(QString::fromStdString(*it));
  }

  // Initialize

  onModelObjectChanged();

  this->blockSignals(false);

  setEnabled(true);
}

OSComboBox::OSComboBox( QWidget * parent )
  : QComboBox(parent)
{
  this->setAcceptDrops(false);
  setEnabled(false);
}

bool OSComboBox::event( QEvent * e )
{
  if( e->type() == QEvent::Wheel )
  {
    return false;
  }
  else
  {
    return QComboBox::event(e);
  }
}

void OSComboBox::bind(model::ModelObject & modelObject, const char * property)
{
  m_modelObject = modelObject;

  m_property = property;

  m_dataSource.reset();

  clear();

  // Connections

  bool isConnected = false;
  isConnected = connect( m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get(),SIGNAL(onChange()),
                              this,SLOT(onModelObjectChanged()) );
  OS_ASSERT(isConnected);

  isConnected = connect( m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get(),SIGNAL(onRemoveFromWorkspace(Handle)),
                         this,SLOT(onModelObjectRemoved(Handle)) );
  OS_ASSERT(isConnected);

  isConnected = connect( this, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(onCurrentIndexChanged(const QString&)) );
  OS_ASSERT(isConnected);

  // Populate choices

  std::string valuesPropertyName = m_property;
  valuesPropertyName.append("Values");

  QVariant variant = m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>()->property(valuesPropertyName.c_str());

  OS_ASSERT( variant.canConvert<std::vector<std::string> >() );

  m_values = variant.value<std::vector<std::string> >();

  this->blockSignals(true);

  for( std::vector<std::string>::iterator it = m_values.begin();
       it < m_values.end();
       ++it )
  {
    addItem(QString::fromStdString(*it));
  }

  // Initialize

  onModelObjectChanged();

  this->blockSignals(false);

  setEnabled(true);
}

void OSComboBox::onModelObjectChanged()
{
  OS_ASSERT(m_modelObject);

  QVariant variant = m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>()->property(m_property.c_str());

  OS_ASSERT( variant.canConvert<std::string>() );

  std::string value = variant.value<std::string>();

  int i = 0;
  for( std::vector<std::string>::iterator it = m_values.begin();
       it < m_values.end();
       ++it )
  {
    if( istringEqual(*it,value) )
    {
      this->blockSignals(true);
      setCurrentIndex(i);
      this->blockSignals(false);
      break;
    }

    i++;
  }
}

void OSComboBox::onModelObjectRemoved(Handle handle)
{
  unbind();
}

void OSComboBox::onCurrentIndexChanged(const QString & text)
{
  OS_ASSERT(m_modelObject);

  // does this version ever work?
  bool test = m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>()->setProperty(m_property.c_str(),text);

  if (!test){
    // try a std::string
    QVariant textString = QVariant::fromValue(toString(text));
    test = m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>()->setProperty(m_property.c_str(),textString);
  }

  // test if property changed
  QVariant variant = m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>()->property(m_property.c_str());
  OS_ASSERT( variant.canConvert<std::string>() );
  std::string value = variant.value<std::string>();

  if (!istringEqual(value, toString(text))){
    // failed, reset combo box
    onModelObjectChanged();
  }
}

void OSComboBox::unbind()
{
  if (m_modelObject){
    disconnect( m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get() );
  }
  m_modelObject = boost::none;

  m_property = "";

  m_dataSource.reset();

  this->blockSignals(true);

  clear();

  this->blockSignals(false);

  setEnabled(false);
}

void OSComboBox::onDataSourceChange(int i)
{
  this->setItemText(i,m_dataSource->valueAt(i));
}

void OSComboBox::onDataSourceAdd(int i)
{
  int oldIndex = this->currentIndex();

  this->insertItem(i,m_dataSource->valueAt(i));

  if( oldIndex == -1 )
  {
    setCurrentIndex(oldIndex);
  }
}

void OSComboBox::onDataSourceRemove(int i)
{
  if( currentIndex() == i )
  {
    this->setCurrentIndex(-1);
  }

  this->removeItem(i);
}

void OSComboBox::setDataSource(boost::shared_ptr<OSComboBoxDataSource> dataSource)
{
  unbind();

  if( m_dataSource )
  {
    disconnect(m_dataSource.get(),SIGNAL(itemChanged(int)),this,SLOT(onDataSourceChange(int)));
    disconnect(m_dataSource.get(),SIGNAL(itemAdded(int)),this,SLOT(onDataSourceAdd(int)));
    disconnect(m_dataSource.get(),SIGNAL(itemRemoved(int)),this,SLOT(onDataSourceRemove(int)));
  }

  m_dataSource = dataSource;

  connect(m_dataSource.get(),SIGNAL(itemChanged(int)),this,SLOT(onDataSourceChange(int)));
  connect(m_dataSource.get(),SIGNAL(itemAdded(int)),this,SLOT(onDataSourceAdd(int)));
  connect(m_dataSource.get(),SIGNAL(itemRemoved(int)),this,SLOT(onDataSourceRemove(int)));

  this->clear();

  for( int i = 0;
       i < m_dataSource->numberOfItems();
       i++ )
  {
    this->addItem(m_dataSource->valueAt(i));
  }

  setCurrentIndex(-1);

  setEnabled(true);
}

OSObjectListCBDS::OSObjectListCBDS(const IddObjectType & type, const model::Model & model)
  :  OSComboBoxDataSource(),
     m_model(model)
{
  m_types.push_back(type);

  initialize();
}

OSObjectListCBDS::OSObjectListCBDS(const std::vector<IddObjectType> & types, const model::Model & model)
  : OSComboBoxDataSource(),
    m_allowEmptySelection(true),
    m_types(types),
    m_model(model)
{
  initialize();
}

void OSObjectListCBDS::initialize()
{
  std::vector<model::ModelObject> modelObjects = m_model.getModelObjects<model::ModelObject>();

  for( std::vector<model::ModelObject>::iterator it = modelObjects.begin();
       it < modelObjects.end();
       ++it )
  {
    if( std::find(m_types.begin(),m_types.end(),it->iddObjectType()) != m_types.end() )
    {
      m_workspaceObjects << *it;

      connect( it->getImpl<openstudio::model::detail::ModelObject_Impl>().get(),
               SIGNAL(onChange()),
               this,
               SLOT(onObjectChanged()) );
    }
  }

  connect( m_model.getImpl<openstudio::model::detail::Model_Impl>().get(),
           SIGNAL(addWorkspaceObject(const WorkspaceObject&, const openstudio::IddObjectType&, const openstudio::UUID&)),
           this,
           SLOT(onObjectAdded(const WorkspaceObject&)) );

  connect( m_model.getImpl<openstudio::model::detail::Model_Impl>().get(),
           SIGNAL(removeWorkspaceObject(const WorkspaceObject&, const openstudio::IddObjectType&, const openstudio::UUID&)),
           this,
           SLOT(onObjectWillBeRemoved(const WorkspaceObject&)) );
}

int OSObjectListCBDS::numberOfItems()
{
  if( m_allowEmptySelection )
  {
    return m_workspaceObjects.count() + 1;
  }
  else
  {
    return m_workspaceObjects.count();
  }
}

QString OSObjectListCBDS::valueAt(int i)
{
  if( m_allowEmptySelection )
  {
    if( i > 0 )
    {
      return QString::fromStdString(m_workspaceObjects[i - 1].name().get());
    }
    else
    {
      return QString("");
    }
  }
  else
  {
    return QString::fromStdString(m_workspaceObjects[i].name().get());
  }
}

void OSObjectListCBDS::onObjectAdded(const WorkspaceObject & workspaceObject)
{
  if(std::find(m_types.begin(),m_types.end(),workspaceObject.cast<model::ModelObject>().iddObjectType()) != m_types.end())
  {
    m_workspaceObjects << workspaceObject;

    connect( workspaceObject.getImpl<openstudio::model::detail::ModelObject_Impl>().get(),
             SIGNAL(onChange()),
             this,
             SLOT(onObjectChanged()) );

    if( m_allowEmptySelection )
    {
      emit itemAdded(m_workspaceObjects.size());
    }
    else
    {
      emit itemAdded(m_workspaceObjects.size() - 1);
    }
  }
}

void OSObjectListCBDS::onObjectWillBeRemoved(const WorkspaceObject & workspaceObject)
{
  if(std::find(m_types.begin(),m_types.end(),workspaceObject.cast<model::ModelObject>().iddObjectType()) != m_types.end())
  {
    int i = m_workspaceObjects.indexOf(workspaceObject);

    m_workspaceObjects.removeAt(i);

    if( m_allowEmptySelection )
    {
      emit itemRemoved(i + 1);
    }
    else
    {
      emit itemRemoved(i);
    }
  }
}

void OSObjectListCBDS::onObjectChanged()
{
  WorkspaceObject workspaceObject = qobject_cast<detail::WorkspaceObject_Impl *>(sender())->getObject<WorkspaceObject>();

  if(std::find(m_types.begin(),m_types.end(),workspaceObject.cast<model::ModelObject>().iddObjectType()) != m_types.end())
  {

    int i = m_workspaceObjects.indexOf(workspaceObject);

    if( m_allowEmptySelection )
    {
      emit itemChanged(i + 1);
    }
    else
    {
      emit itemChanged(i);
    }
  }
}

} // openstudio

