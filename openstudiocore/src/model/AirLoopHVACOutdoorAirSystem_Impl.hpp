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
#ifndef MODEL_AIRLOOPHVACOUTDOORAIRSYSTEM_IMPL_HPP
#define MODEL_AIRLOOPHVACOUTDOORAIRSYSTEM_IMPL_HPP

#include "HVACComponent_Impl.hpp"

namespace openstudio {
namespace model {

class ModelObject;
class AirLoopHVAC;
class ControllerOutdoorAir;
class Node;

namespace detail {

  // derive AirLoopHVACOutdoorAirSystem_Impl from ModelObject_Impl to override virtual methods
  class MODEL_API AirLoopHVACOutdoorAirSystem_Impl : public HVACComponent_Impl{
    Q_OBJECT;

    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> controllerOutdoorAir READ controllerOutdoorAirAsModelObject WRITE setControllerOutdoorAirAsModelObject);
    Q_PROPERTY(std::vector<openstudio::model::ModelObject> oaComponents READ oaComponentsAsModelObjects);
    Q_PROPERTY(std::vector<openstudio::model::ModelObject> reliefComponents READ reliefComponentsAsModelObjects);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> outboardOANode READ outboardOANodeAsModelObject);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> outboardReliefNode READ outboardReliefNodeAsModelObject);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> airLoop READ airLoopAsModelObject);
    Q_PROPERTY(std::vector<openstudio::model::ModelObject> components READ componentsAsModelObjects);
   public:

    // constructor
    AirLoopHVACOutdoorAirSystem_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    // construct from workspace
    AirLoopHVACOutdoorAirSystem_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                     Model_Impl* model,
                                     bool keepHandle);

    // copy constructor
    AirLoopHVACOutdoorAirSystem_Impl(const AirLoopHVACOutdoorAirSystem_Impl& other,
                                     Model_Impl* model,
                                     bool keepHandle);

    // virtual destructor
    virtual ~AirLoopHVACOutdoorAirSystem_Impl();

    // return the parent object in the hierarchy
    virtual boost::optional<ParentObject> parent() const;

    // return any children objects in the hierarchy
    virtual std::vector<ModelObject> children() const;

    // Get all output variable names that could be associated with this object.
    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    unsigned returnAirPort() const;
    unsigned outdoorAirPort() const;
    unsigned reliefAirPort() const;
    unsigned mixedAirPort() const;

    boost::optional<ModelObject> returnAirModelObject() const;
    boost::optional<ModelObject> outdoorAirModelObject() const;
    boost::optional<ModelObject> reliefAirModelObject() const;
    boost::optional<ModelObject> mixedAirModelObject() const;

    ControllerOutdoorAir getControllerOutdoorAir() const;

    void setControllerOutdoorAir(const ControllerOutdoorAir& controllerOutdoorAir );

    virtual ModelObject clone(Model model) const;

    virtual std::vector<openstudio::IdfObject> remove();

    virtual bool addToNode(Node & node);

    /** This function returns a vector of HVACComponent that are directly downstream
     *  from this object on an AirLoopHVAC or PlantLoop. 
     *  @param[in]  isDemandComponent  Boolean passed in whether object is a demand or supply component
    **/
    virtual std::vector<HVACComponent> edges(bool isDemandComponent);

    std::vector<ModelObject> oaComponents() const;

    std::vector<ModelObject> reliefComponents() const;

    boost::optional<Node> outboardOANode() const;
    boost::optional<Node> outboardReliefNode() const;

    boost::optional<AirLoopHVAC> airLoop() const;
    std::vector<ModelObject> components() const;
    boost::optional<ModelObject> component(openstudio::Handle handle);
    boost::optional<ModelObject> oaComponent(openstudio::Handle handle);
    boost::optional<ModelObject> reliefComponent(openstudio::Handle handle);

   private:
    REGISTER_LOGGER("openstudio.model.AirLoopHVACOutdoorAirSystem");

    boost::optional<ModelObject> controllerOutdoorAirAsModelObject() const;
    std::vector<ModelObject> oaComponentsAsModelObjects() const;
    std::vector<ModelObject> reliefComponentsAsModelObjects() const;
    boost::optional<ModelObject> outboardOANodeAsModelObject() const;
    boost::optional<ModelObject> outboardReliefNodeAsModelObject() const;
    boost::optional<ModelObject> airLoopAsModelObject() const;
    std::vector<ModelObject> componentsAsModelObjects() const;

    bool setControllerOutdoorAirAsModelObject(const boost::optional<ModelObject>& modelObject);
  };
} // detail

} // model
} // openstudio

#endif
