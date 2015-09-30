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

#include "OSComboBox.hpp"

#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/ModelObject.hpp"
#include "../model/ModelObject_Impl.hpp"

#include "../utilities/idf/WorkspaceObject.hpp"
#include "../utilities/idf/WorkspaceObject_Impl.hpp"
#include "../utilities/core/Assert.hpp"

#include <QCompleter>
#include <QEvent>

namespace openstudio {

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

  for( const auto & modelObject : modelObjects )
  {
    if( std::find(m_types.begin(),m_types.end(),modelObject.iddObjectType()) != m_types.end() )
    {
      m_workspaceObjects << modelObject;

      connect(modelObject.getImpl<model::detail::ModelObject_Impl>().get(), &model::detail::ModelObject_Impl::onChange, this, &OSObjectListCBDS::onObjectChanged);
    }
  }

  connect(m_model.getImpl<model::detail::Model_Impl>().get(),
    static_cast<void (model::detail::Model_Impl::*)(const WorkspaceObject &, const IddObjectType &, const UUID &) const>(&model::detail::Model_Impl::addWorkspaceObject), 
    this,
    &OSObjectListCBDS::onObjectAdded);

  connect(m_model.getImpl<model::detail::Model_Impl>().get(),
    static_cast<void (model::detail::Model_Impl::*)(const WorkspaceObject &, const IddObjectType &, const UUID &) const>(&model::detail::Model_Impl::removeWorkspaceObject), 
    this,
    &OSObjectListCBDS::onObjectWillBeRemoved);
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

    connect(workspaceObject.getImpl<model::detail::ModelObject_Impl>().get(), &model::detail::ModelObject_Impl::onChange, this, &OSObjectListCBDS::onObjectChanged);
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

OSComboBox2::OSComboBox2( QWidget * parent, bool editable )
  : QComboBox(parent)
{
  this->setAcceptDrops(false);
  auto completer = new QCompleter();
  this->setCompleter(completer);
  setEditable(editable);
  setEnabled(false);
  setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Fixed);
}

bool OSComboBox2::event( QEvent * e )
{
  if( e->type() == QEvent::Wheel )
  {
    return false;
  }
  else if (e->type() == QEvent::FocusIn && this->focusPolicy() == Qt::ClickFocus)
  {
    QString style("QComboBox { background: #ffc627; }");
    setStyleSheet(style);

    emit inFocus(true, hasData());

    return QComboBox::event(e);
  }
  else if (e->type() == QEvent::FocusOut && this->focusPolicy() == Qt::ClickFocus)
  {
    QString style("QComboBox { background: white; }");
    setStyleSheet(style);

    emit inFocus(false, false);

    return QComboBox::event(e);
  }
  else
  {
    return QComboBox::event(e);
  }
}

void OSComboBox2::bind(std::shared_ptr<OSComboBoxDataSource> dataSource)
{
  m_dataSource = dataSource;

  clear();
  completeBind();
}

void OSComboBox2::unbind() {
  if (m_modelObject){
    disconnect( m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get() );

    m_modelObject.reset();
    m_choiceConcept.reset();
  }

  if (m_dataSource) {
    disconnect(m_dataSource.get());

    m_dataSource.reset();
  }

  this->blockSignals(true);

  clear();

  this->blockSignals(false);

  setEnabled(false);
}

void OSComboBox2::onModelObjectChanged() {
  OS_ASSERT(m_modelObject);

  if( m_choiceConcept )
  {
    std::string value = m_choiceConcept->get();

    std::vector<std::string>::const_iterator it = std::find(m_values.begin(),
                                                            m_values.end(),
                                                            value);

    int i = int(it - m_values.begin());
    this->blockSignals(true);
    setCurrentIndex(i);
    this->blockSignals(false);
  }
}

void OSComboBox2::onModelObjectRemoved(Handle handle)
{
  unbind();
}

void OSComboBox2::onCurrentIndexChanged(const QString & text)
{
  emit inFocus(true, hasData());

  OS_ASSERT(m_modelObject);

  if( m_choiceConcept )
  {
    std::string value = text.toStdString();

    this->blockSignals(true);
    m_choiceConcept->set(value);
    onModelObjectChanged(); // will be sure to display actual value
    this->blockSignals(false);
  }
}

void OSComboBox2::onEditTextChanged(const QString & text)
{
  OS_ASSERT(m_modelObject);

  if( m_choiceConcept )
  {
    std::string value = text.toStdString();

    this->blockSignals(true);
    m_choiceConcept->set(value);
    onModelObjectChanged(); // will be sure to display actual value
    this->blockSignals(false);
  }
}

void OSComboBox2::onChoicesRefreshTrigger() {
  if( m_choiceConcept )
  {
    m_values = m_choiceConcept->choices();
    this->blockSignals(true);

    clear();
    for( const auto & value : m_values )
    {
      addItem(QString::fromStdString(value));
    }

    // re-initialize
    onModelObjectChanged();

    this->blockSignals(false);
    setEnabled(true);
  }
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
  if (m_modelObject) {
    // connections
    connect(m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get(), &openstudio::model::detail::ModelObject_Impl::onChange, this, &OSComboBox2::onModelObjectChanged);

    connect(m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get(), &openstudio::model::detail::ModelObject_Impl::onRemoveFromWorkspace, this, &OSComboBox2::onModelObjectRemoved);

    connect(this, static_cast<void (OSComboBox2::*)(const QString &)>(&OSComboBox2::currentIndexChanged), this, &OSComboBox2::onCurrentIndexChanged);

    bool isConnected = false;

    if (isEditable())
    {
      isConnected = connect(this, SIGNAL(editTextChanged(const QString&)), this, SLOT(onEditTextChanged(const QString&)));
      OS_ASSERT(isConnected);
    }

    // isConnected = connect( m_modelObject->model().getImpl<openstudio::model::detail::Model_Impl>().get(),
    //                        SIGNAL(addWorkspaceObject(const WorkspaceObject&, const openstudio::IddObjectType&, const openstudio::UUID&)),
    //                        this,
    //                        SLOT(onChoicesRefreshTrigger()) );
    // OS_ASSERT(isConnected);

    // isConnected = connect( m_modelObject->model().getImpl<openstudio::model::detail::Model_Impl>().get(),
    //                        SIGNAL(removeWorkspaceObject(const WorkspaceObject&, const openstudio::IddObjectType&, const openstudio::UUID&)),
    //                        this,
    //                        SLOT(onChoicesRefreshTrigger()) );
    // OS_ASSERT(isConnected);

    // if this is too burdensome, implement Workspace_Impl onNameChange() signal and uncomment the above two connections.
    // (IdfObject_Impl already has onNameChange(); Workspace_Impl::onChange() includes object addition and removal.)
    connect(m_modelObject->model().getImpl<openstudio::model::detail::Model_Impl>().get(), &openstudio::model::detail::Model_Impl::onChange, this, &OSComboBox2::onChoicesRefreshTrigger);

    // populate choices
    // ETH@20140228 - With extension of this class to choices of ModelObjects, and beyond,
    // do we need to figure out some way to signal when the choices have changed? Or maybe
    // controllers will be able to sense that and trigger an unbind(), (re-)bind?
    m_values = m_choiceConcept->choices();
    this->blockSignals(true);

    for( const auto & value : m_values )
    {
      addItem(QString::fromStdString(value));
    }

    // initialize
    onModelObjectChanged();
  }
  else if (m_dataSource) {

    // connections
    connect(m_dataSource.get(), &OSComboBoxDataSource::itemChanged, this, &OSComboBox2::onDataSourceChange);
    connect(m_dataSource.get(), &OSComboBoxDataSource::itemAdded, this, &OSComboBox2::onDataSourceAdd);
    connect(m_dataSource.get(), &OSComboBoxDataSource::itemRemoved, this, &OSComboBox2::onDataSourceRemove);

    this->blockSignals(true);

    // populate choices
    for( int i = 0;
         i < m_dataSource->numberOfItems();
         i++ )
    {
      this->addItem(m_dataSource->valueAt(i));
    }

    // initialize
    setCurrentIndex(-1);
  }
  else {
    OS_ASSERT(false);
  }

  this->blockSignals(false);
  setEnabled(true);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

  connect(m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get(), &openstudio::model::detail::ModelObject_Impl::onChange, this, &OSComboBox::onModelObjectChanged);

  connect(m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get(), &openstudio::model::detail::ModelObject_Impl::onRemoveFromWorkspace, this, &OSComboBox::onModelObjectRemoved);

  connect(this, static_cast<void (OSComboBox::*)(const QString &)>(&OSComboBox::currentIndexChanged), this, &OSComboBox::onCurrentIndexChanged);

  // Populate choices

  std::string valuesPropertyName = m_property;
  valuesPropertyName.append("Values");

  QVariant variant = m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>()->property(valuesPropertyName.c_str());

  OS_ASSERT( variant.canConvert<std::vector<std::string> >() );

  m_values = variant.value<std::vector<std::string> >();

  this->blockSignals(true);

  for( const auto & value : m_values )
  {
    addItem(QString::fromStdString(value));
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
  for( const auto & v : m_values )
  {
    if( istringEqual(v,value) )
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

  QVariant variant = m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>()->property(m_property.c_str());
  QVariant textString;
  if (variant.canConvert<QString>()) {
    textString = QVariant::fromValue(text); 
  } else if (variant.canConvert<std::string>()) {
    textString = QVariant::fromValue(toString(text));
  }
  m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>()->setProperty(m_property.c_str(), textString);

  // test if property changed
  variant = m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>()->property(m_property.c_str());
  std::string value;
  if (variant.canConvert<QString>()) {
    value = variant.toString().toStdString();
  } else if (variant.canConvert<std::string>()) {
    value = variant.value<std::string>();
  }

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

void OSComboBox::setDataSource(std::shared_ptr<OSComboBoxDataSource> dataSource)
{
  unbind();

  if( m_dataSource )
  {
    disconnect(m_dataSource.get(), &OSComboBoxDataSource::itemChanged, this, &OSComboBox::onDataSourceChange);
    disconnect(m_dataSource.get(), &OSComboBoxDataSource::itemAdded, this, &OSComboBox::onDataSourceAdd);
    disconnect(m_dataSource.get(), &OSComboBoxDataSource::itemRemoved, this, &OSComboBox::onDataSourceRemove);
  }

  m_dataSource = dataSource;

  connect(m_dataSource.get(), &OSComboBoxDataSource::itemChanged, this, &OSComboBox::onDataSourceChange);
  connect(m_dataSource.get(), &OSComboBoxDataSource::itemAdded, this, &OSComboBox::onDataSourceAdd);
  connect(m_dataSource.get(), &OSComboBoxDataSource::itemRemoved, this, &OSComboBox::onDataSourceRemove);
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

} // openstudio

