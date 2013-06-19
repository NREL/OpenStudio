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

#ifndef OPENSTUDIO_EXTERNALMODELINTERFACE_HPP
#define OPENSTUDIO_EXTERNALMODELINTERFACE_HPP

#include <plugin/PluginAPI.hpp>

#include <utilities/idf/Handle.hpp>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/core/Logger.hpp>

#include <string>

#include <QObject>

namespace openstudio {
  namespace detail {
    class WorkspaceObject_Impl;
  }
namespace model {
  namespace detail {
    class Model_Impl;
    class ModelObject_Impl;
  }
}
namespace plugin {

/** ModelHolder simply holds a pointer to the OpenStudio model,
*   it is used to construct an ExternalModelInterface.
**/
class PLUGIN_API ModelHolder{
protected:
  friend class ExternalPlugin;
  friend class ExternalModelInterface;

  ModelHolder(openstudio::model::detail::Model_Impl* ptr);

  openstudio::model::detail::Model_Impl* ptr() const;

private:

  openstudio::model::detail::Model_Impl* m_ptr;

};

class ModelObjectHolder;

class ExternalPlugin;

/** ExternalModelInterface interfaces an OpenStudio model in an ExternalPlugin.
*   All communication between the ExternalModelInterface and the OpenStudio model
*   is done over queued signals and slots.
**/
class PLUGIN_API ExternalModelInterface : public QObject
{

  Q_OBJECT

  public:

    /// Create an ExternalModelInterface to the model wrapped in
    static ExternalModelInterface* create(const std::string& key, const ModelHolder& modelHolder);

    /// Constructor
    ExternalModelInterface(const std::string& key, const ModelHolder& modelHolder);

    /// Virtual destructor
    virtual ~ExternalModelInterface();

    /// Returns the key for this interface
    std::string key() const;

    /// Called for each model object already in the model when this interface was constructed.
    virtual void onInitialModelObject(const ModelObjectHolder& modelHolder, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle);

    /// Called after each initial model object has been reported.
    virtual void onInitialize();

    /// Called when a new model object is added.
    virtual void onNewModelObject(const ModelObjectHolder& modelHolder, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle);

  protected slots:

    /// Called for each model object already in the model when this interface was constructed.
    void initialModelObject(openstudio::model::detail::ModelObject_Impl* modelObject, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle);

    /// Called after each initial model object has been reported.
    void initialize();

    /// Called when a new model object is added.
    void newWorkspaceObject(boost::shared_ptr<openstudio::detail::WorkspaceObject_Impl> workspaceObject, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle);

  signals:

    /// Signal to request all the initial model objects.
    void requestInitialModelObjects();

  protected:

  private:

    REGISTER_LOGGER("ExternalModelInterface");

    std::string m_key;

};

} // plugin
} // openstudio

#endif // OPENSTUDIO_EXTERNALMODELINTERFACE_HPP

