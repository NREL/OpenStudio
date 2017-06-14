/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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

      modelObject.getImpl<model::detail::ModelObject_Impl>().get()->onChange.connect<OSObjectListCBDS, &OSObjectListCBDS::onObjectChanged>(this);
    }
  }

  m_model.getImpl<model::detail::Model_Impl>().get()->addWorkspaceObject.connect<OSObjectListCBDS, &OSObjectListCBDS::onObjectAdded>(this);

  m_model.getImpl<model::detail::Model_Impl>().get()->removeWorkspaceObject.connect<OSObjectListCBDS, &OSObjectListCBDS::onObjectWillBeRemoved>(this);
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

void OSObjectListCBDS::onObjectAdded(const WorkspaceObject & workspaceObject, const openstudio::IddObjectType& type, const openstudio::UUID& uuid)
{
  if(std::find(m_types.begin(),m_types.end(),workspaceObject.cast<model::ModelObject>().iddObjectType()) != m_types.end())
  {
    m_workspaceObjects << workspaceObject;

    workspaceObject.getImpl<model::detail::ModelObject_Impl>().get()->onChange.connect<OSObjectListCBDS, &OSObjectListCBDS::onObjectChanged>(this);
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

void OSObjectListCBDS::onObjectWillBeRemoved(const WorkspaceObject & workspaceObject, const openstudio::IddObjectType& type, const openstudio::UUID& uuid)
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
  // WorkspaceObject workspaceObject = qobject_cast<detail::WorkspaceObject_Impl *>(sender())->getObject<WorkspaceObject>();

  // if(std::find(m_types.begin(),m_types.end(),workspaceObject.cast<model::ModelObject>().iddObjectType()) != m_types.end())
  // {

  //   int i = m_workspaceObjects.indexOf(workspaceObject);

  //   if( m_allowEmptySelection )
  //   {
  //     emit itemChanged(i + 1);
  //   }
  //   else
  //   {
  //     emit itemChanged(i);
  //   }
  // }
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

OSComboBox2::~OSComboBox2()
{
  unbind();
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
    // disconnect( m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get() );

    m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get()->onChange.disconnect<OSComboBox2, &OSComboBox2::onModelObjectChanged>(this);
    m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get()->onRemoveFromWorkspace.disconnect<OSComboBox2, &OSComboBox2::onModelObjectRemoved>(this);
    // m_modelObject->model().getImpl<openstudio::model::detail::Model_Impl>().get()->onChange.disconnect<OSComboBox2, &OSComboBox2::onChoicesRefreshTrigger>(this);


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

void OSComboBox2::onModelObjectRemoved(const Handle& handle)
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
    m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get()->onChange.connect<OSComboBox2, &OSComboBox2::onModelObjectChanged>(this);
    m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get()->onRemoveFromWorkspace.connect<OSComboBox2, &OSComboBox2::onModelObjectRemoved>(this);

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
    // m_modelObject->model().getImpl<openstudio::model::detail::Model_Impl>().get()->onChange.connect<OSComboBox2, &OSComboBox2::onChoicesRefreshTrigger>(this);

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

// OSComboBox::OSComboBox( QWidget * parent )
//   : QComboBox(parent)
// {
//   this->setAcceptDrops(false);
//   setEnabled(false);
// }

// bool OSComboBox::event( QEvent * e )
// {
//   if( e->type() == QEvent::Wheel )
//   {
//     return false;
//   }
//   else
//   {
//     return QComboBox::event(e);
//   }
// }

// void OSComboBox::bind(model::ModelObject & modelObject, const char * property)
// {
//   m_modelObject = modelObject;

//   m_property = property;

//   m_dataSource.reset();

//   clear();

//   // Connections

//   m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get()->onChange.connect<OSComboBox, &OSComboBox::onModelObjectChanged>(this);

//   m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get()->onRemoveFromWorkspace.connect<OSComboBox, &OSComboBox::onModelObjectRemoved>(this);

//   connect(this, static_cast<void (OSComboBox::*)(const QString &)>(&OSComboBox::currentIndexChanged), this, &OSComboBox::onCurrentIndexChanged);

//   // Populate choices

//   std::string valuesPropertyName = m_property;
//   valuesPropertyName.append("Values");

//   QVariant variant = m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>()->property(valuesPropertyName.c_str());

//   OS_ASSERT( variant.canConvert<std::vector<std::string> >() );

//   m_values = variant.value<std::vector<std::string> >();

//   this->blockSignals(true);

//   for( const auto & value : m_values )
//   {
//     addItem(QString::fromStdString(value));
//   }

//   // Initialize

//   onModelObjectChanged();

//   this->blockSignals(false);

//   setEnabled(true);
// }

// void OSComboBox::onModelObjectChanged()
// {
//   OS_ASSERT(m_modelObject);

//   QVariant variant = m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>()->property(m_property.c_str());

//   OS_ASSERT( variant.canConvert<std::string>() );

//   std::string value = variant.value<std::string>();

//   int i = 0;
//   for( const auto & v : m_values )
//   {
//     if( istringEqual(v,value) )
//     {
//       this->blockSignals(true);
//       setCurrentIndex(i);
//       this->blockSignals(false);
//       break;
//     }

//     i++;
//   }
// }

// void OSComboBox::onModelObjectRemoved(const Handle& handle)
// {
//   unbind();
// }

// void OSComboBox::onCurrentIndexChanged(const QString & text)
// {
//   OS_ASSERT(m_modelObject);

//   QVariant variant = m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>()->property(m_property.c_str());
//   QVariant textString;
//   if (variant.canConvert<QString>()) {
//     textString = QVariant::fromValue(text);
//   } else if (variant.canConvert<std::string>()) {
//     textString = QVariant::fromValue(toString(text));
//   }
//   m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>()->setProperty(m_property.c_str(), textString);

//   // test if property changed
//   variant = m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>()->property(m_property.c_str());
//   std::string value;
//   if (variant.canConvert<QString>()) {
//     value = variant.toString().toStdString();
//   } else if (variant.canConvert<std::string>()) {
//     value = variant.value<std::string>();
//   }

//   if (!istringEqual(value, toString(text))){
//     // failed, reset combo box
//     onModelObjectChanged();
//   }
// }

// void OSComboBox::unbind()
// {
//   if (m_modelObject){
//     disconnect( m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get() );
//   }
//   m_modelObject = boost::none;

//   m_property = "";

//   m_dataSource.reset();

//   this->blockSignals(true);

//   clear();

//   this->blockSignals(false);

//   setEnabled(false);
// }

// void OSComboBox::onDataSourceChange(int i)
// {
//   this->setItemText(i,m_dataSource->valueAt(i));
// }

// void OSComboBox::onDataSourceAdd(int i)
// {
//   int oldIndex = this->currentIndex();

//   this->insertItem(i,m_dataSource->valueAt(i));

//   if( oldIndex == -1 )
//   {
//     setCurrentIndex(oldIndex);
//   }
// }

// void OSComboBox::onDataSourceRemove(int i)
// {
//   if( currentIndex() == i )
//   {
//     this->setCurrentIndex(-1);
//   }

//   this->removeItem(i);
// }

// void OSComboBox::setDataSource(std::shared_ptr<OSComboBoxDataSource> dataSource)
// {
//   unbind();

//   if( m_dataSource )
//   {
//     disconnect(m_dataSource.get(), &OSComboBoxDataSource::itemChanged, this, &OSComboBox::onDataSourceChange);
//     disconnect(m_dataSource.get(), &OSComboBoxDataSource::itemAdded, this, &OSComboBox::onDataSourceAdd);
//     disconnect(m_dataSource.get(), &OSComboBoxDataSource::itemRemoved, this, &OSComboBox::onDataSourceRemove);
//   }

//   m_dataSource = dataSource;

//   connect(m_dataSource.get(), &OSComboBoxDataSource::itemChanged, this, &OSComboBox::onDataSourceChange);
//   connect(m_dataSource.get(), &OSComboBoxDataSource::itemAdded, this, &OSComboBox::onDataSourceAdd);
//   connect(m_dataSource.get(), &OSComboBoxDataSource::itemRemoved, this, &OSComboBox::onDataSourceRemove);
//   this->clear();

//   for( int i = 0;
//        i < m_dataSource->numberOfItems();
//        i++ )
//   {
//     this->addItem(m_dataSource->valueAt(i));
//   }

//   setCurrentIndex(-1);

//   setEnabled(true);
// }

} // openstudio

