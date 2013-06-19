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

#ifndef OPENSTUDIO_EXTERNALOBJECTINTERFACE_HPP
#define OPENSTUDIO_EXTERNALOBJECTINTERFACE_HPP

#include <plugin/PluginAPI.hpp>

#include <utilities/idf/Handle.hpp>
#include <utilities/core/Logger.hpp>

#include <QObject>
#include <QStringList>
#include <QVariant>

namespace openstudio {
namespace model {
  namespace detail {
    class ModelObject_Impl;
  }
}

class IddObjectType;

namespace plugin {

/** ModelObjectHolder simply holds a pointer to an OpenStudio model object,
*   it is used to construct an ExternalObjectInterface.
**/
class PLUGIN_API ModelObjectHolder{
protected:
  friend class ExternalModelInterface;
  friend class ExternalObjectInterface;

  ModelObjectHolder(openstudio::model::detail::ModelObject_Impl* ptr);

  openstudio::model::detail::ModelObject_Impl* ptr() const;

private:

  openstudio::model::detail::ModelObject_Impl* m_ptr;

};

class ModelHolder;

/** ExternalObjectInterface interfaces an OpenStudio model object in an ExternalModelInterface.
*   All communication between the ExternalObjectInterface and the OpenStudio model object
*   is done over queued signals and slots.
**/
class PLUGIN_API ExternalObjectInterface : public QObject
{

  Q_OBJECT

  public:

    /// Create a new ExternalObjectInterface
    ExternalObjectInterface* create(const ModelObjectHolder& modelObjectHolder);

    /// Constructor
    ExternalObjectInterface(const ModelObjectHolder& modelObjectHolder);

    /// Virtual destructor
    virtual ~ExternalObjectInterface();

    /// Returns true if this interface has recieved properties from the model object
    bool initialized() const;

    /// Get the IddObjectType of the object, returns empty type if not initialized
    openstudio::IddObjectType iddObjectType() const;

    /// Get the handle of the object, returns null handle if not initialized
    openstudio::Handle handle() const;

    /// Get the name of the object, returns empty string if not initialized
    std::string name();

    /// Add a name of a property to request from the model object
    void addPropertyName(const std::string& name);

    /// Get the local properties
    QVariantMap localProperties() const;

    /// Get the local property by name
    QVariant getLocalProperty(const std::string& name) const;

    /// Called when properties are first recieved from the model object
    virtual void onLocalPropertiesInitialized();

    /// Called when new properties are recieved from the model object
    virtual void onLocalPropertiesChanged(const QVariantMap& changedProperties);

  public slots:

    /// Request that the model object send it's properties
    void requestModelObjectProperties();

  protected slots:

    /// Update local properties with current values of all properties from the model object
    void updateLocalPropertiesFromModelObjectProperties(const QVariantMap& properties);
  
  signals:

    void requestProperties(const QStringList& names);

    void setProperties(const QVariantMap& properties);

  private:

    REGISTER_LOGGER("ExternalObjectInterface");

    // Updates local properties 
    void updateLocalProperties(const QVariantMap& properties, bool callOnLocalPropertiesChanged);

    bool m_initialized;
    QStringList m_propertyNames;
    QVariantMap m_localProperties;

};

} // plugin
} // openstudio

#endif // OPENSTUDIO_EXTERNALOBJECTINTERFACE_HPP

