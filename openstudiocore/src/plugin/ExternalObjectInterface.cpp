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

#include <plugin/ExternalObjectInterface.hpp>

#include <model/ModelObject_Impl.hpp>

#include <utilities/core/Assert.hpp>

#include <QThread>

namespace openstudio {
namespace plugin {

ModelObjectHolder::ModelObjectHolder(openstudio::model::detail::ModelObject_Impl* ptr)
  : m_ptr(ptr)
{
}

openstudio::model::detail::ModelObject_Impl* ModelObjectHolder::ptr() const
{
  return m_ptr;
}

ExternalObjectInterface* ExternalObjectInterface::create(const ModelObjectHolder& modelObjectHolder)
{
  ExternalObjectInterface* result = new ExternalObjectInterface(modelObjectHolder);
  return result;
}

ExternalObjectInterface::ExternalObjectInterface(const ModelObjectHolder& modelObjectHolder)
  : QObject(), m_initialized(false)
{
  LOG(Trace, "ExternalObjectInterface");
  //LOG(Trace, "ExternalObjectInterface, modelObjectHolder.ptr() = " << modelObjectHolder.ptr());
  //QThread* ct = QThread::currentThread();
  //LOG(Trace, "ExternalObjectInterface, currentThread = " << ct);
  //QThread* thread = this->thread();
  //LOG(Trace, "ExternalModelInterface, this->thread() = " << thread);

  // connect signals
  bool test;
  test = connect(modelObjectHolder.ptr(), SIGNAL(onChange()), this, SLOT(requestModelObjectProperties()), Qt::QueuedConnection);
  BOOST_ASSERT(test);
  test = connect(modelObjectHolder.ptr(), SIGNAL(reportProperties(const QVariantMap&)), this, SLOT(updateLocalPropertiesFromModelObjectProperties(const QVariantMap&)), Qt::QueuedConnection);
  BOOST_ASSERT(test);

  test = connect(this, SIGNAL(requestProperties(const QStringList&)), modelObjectHolder.ptr(), SLOT(requestProperties(const QStringList&)), Qt::QueuedConnection);
  BOOST_ASSERT(test);
  test = connect(this, SIGNAL(setProperties(const QVariantMap&)), modelObjectHolder.ptr(), SLOT(setProperties(const QVariantMap&)), Qt::QueuedConnection);
  BOOST_ASSERT(test);

  m_propertyNames.push_back("iddObjectType");
  m_propertyNames.push_back("handle");
  m_propertyNames.push_back("name");
}

ExternalObjectInterface::~ExternalObjectInterface()
{
  LOG(Trace, "~ExternalObjectInterface");
}

bool ExternalObjectInterface::initialized() const
{
  return m_initialized;
}

IddObjectType ExternalObjectInterface::iddObjectType() const
{
  QVariant result = getLocalProperty("iddObjectType");
  if (!result.isNull() && result.isValid()){
    bool test = result.canConvert<openstudio::IddObjectType>();
    //BOOST_ASSERT(test);
    if (test){
      return result.value<openstudio::IddObjectType>();
    }
  }
  return IddObjectType();
}

Handle ExternalObjectInterface::handle() const
{
  QVariant result = getLocalProperty("handle");
  if (!result.isNull() && result.isValid()){
    bool test = result.canConvert<openstudio::Handle>();
    test = result.canConvert<openstudio::UUID>();
    //BOOST_ASSERT(test);
    if (test){
      return result.value<openstudio::UUID>();
    }
  }
  return Handle();
}

std::string ExternalObjectInterface::name()
{
  QVariant result = getLocalProperty("name");
  if (!result.isNull() && result.isValid()){
    bool test = result.canConvert<boost::optional<std::string> >();
    //BOOST_ASSERT(test);
    if (test){
      boost::optional<std::string> name = result.value<boost::optional<std::string> >();
      if (name){
        return name.get();
      }
    }
  }
  return "";
}

void ExternalObjectInterface::addPropertyName(const std::string& name)
{
  m_propertyNames.push_back(toQString(name));
}

QVariantMap ExternalObjectInterface::localProperties() const
{
  return m_localProperties;
}

QVariant ExternalObjectInterface::getLocalProperty(const std::string& name) const
{
  LOG(Trace, "getLocalProperty, name = " << name);

  QVariantMap::const_iterator it = m_localProperties.find(toQString(name));
  if (it != m_localProperties.end()){

    QVariant value = it.value();
    if (value.isNull() || !value.isValid()){
      LOG(Warn, "getLocalProperty, property found for name = " << name << " but it is null or invalid");
    }else{
      const char* typeName = it.value().typeName();
      LOG(Info, "getLocalProperty, found value of type = " << std::string(typeName) << " for name = " << name);
    }

    return value;
  }

  LOG(Warn, "getLocalProperty, property not found for name = " << name);

  return QVariant();
}

void ExternalObjectInterface::onLocalPropertiesInitialized()
{
  LOG(Trace, "onLocalPropertiesInitialized");
}

void ExternalObjectInterface::onLocalPropertiesChanged(const QVariantMap& changedProperties)
{
  LOG(Trace, "onLocalPropertiesChanged");
}

void ExternalObjectInterface::requestModelObjectProperties() 
{
  LOG(Trace, "requestModelObjectProperties");

  emit requestProperties(m_propertyNames);
}

void ExternalObjectInterface::updateLocalPropertiesFromModelObjectProperties(const QVariantMap& properties)
{
  LOG(Trace, "updateLocalPropertiesFromModelObjectProperties");

  if (!m_initialized){
    m_initialized = true;
    updateLocalProperties(properties, false);
    onLocalPropertiesInitialized();
  }else{
    updateLocalProperties(properties, true);
  }
}

void ExternalObjectInterface::updateLocalProperties(const QVariantMap& properties, bool callOnLocalPropertiesChanged)
{
  LOG(Trace, "updateLocalProperties");

  QVariantMap changedProperties;

  QVariantMap::const_iterator it = properties.begin();
  QVariantMap::const_iterator itend = properties.end();
  for (; it != itend; ++it){
    QVariantMap::const_iterator jt = m_localProperties.find(it.key());
    if (jt != m_localProperties.end()){
      if (jt.value() == it.value()){
        // no change
      }else{
        m_localProperties[it.key()] = it.value();
        changedProperties.insert(it.key(), it.value());
      }
    }else{
      // key not found
      m_localProperties[it.key()] = it.value();
      changedProperties.insert(it.key(), it.value());
    }
  }

  if (callOnLocalPropertiesChanged && !changedProperties.empty()){
    onLocalPropertiesChanged(changedProperties);
  }
}
  

} // plugin
} // openstudio
