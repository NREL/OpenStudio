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

#ifndef MODEL_NODE_IMPL_HPP
#define MODEL_NODE_IMPL_HPP

#include "StraightComponent_Impl.hpp"

namespace openstudio {

namespace model {

class Node;
class SetpointManagerSingleZoneReheat;
class SetpointManagerMixedAir;
class SetpointManagerScheduled;
class SetpointManagerFollowOutdoorAirTemperature;
class SetpointManagerOutdoorAirReset;
class SetpointManagerWarmest;
class SetpointManager;

namespace detail {

  class MODEL_API Node_Impl : public StraightComponent_Impl {
   public:

    // constructor
    Node_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    // construct from workspace
    Node_Impl(const openstudio::detail::WorkspaceObject_Impl& other, 
              Model_Impl* model, 
              bool keepHandle);

    // copy constructor
    Node_Impl(const Node_Impl& other, Model_Impl* model, bool keepHandle);

    // virtual destructor
    virtual ~Node_Impl();

    // Get all output variable names that could be associated with this object.
    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    /** This function returns a vector of HVACComponent that are directly downstream
     *  from this object on an AirLoopHVAC or PlantLoop. 
     *  @param[in]  isDemandComponent  Boolean passed in whether object is a demand or supply component
    **/
    virtual std::vector<HVACComponent> edges(bool isDemandComponent);

    unsigned inletPort();

    unsigned outletPort();

    std::vector<SetpointManager> setpointManagers() const;

    void removeSetpointManagers();

    void addSetpointManager(SetpointManagerSingleZoneReheat & singleZoneReheat);

    void removeSetpointManagerSingleZoneReheat();

    boost::optional<SetpointManagerSingleZoneReheat> getSetpointManagerSingleZoneReheat() const;

    void  addSetpointManager(SetpointManagerMixedAir & mixedAir);

    void removeSetpointManagerMixedAir();

    boost::optional<SetpointManagerMixedAir> getSetpointManagerMixedAir() const;

    boost::optional<SetpointManagerScheduled> setpointManagerScheduled() const;

    void addSetpointManager( SetpointManagerScheduled & setPointManager );

    void removeSetpointManagerScheduled();

    boost::optional<SetpointManagerFollowOutdoorAirTemperature> setpointManagerFollowOutdoorAirTemperature() const;

    void addSetpointManager( SetpointManagerFollowOutdoorAirTemperature & setPointManager );

    void removeSetpointManagerFollowOutdoorAirTemperature();

    void addSetpointManager( SetpointManagerOutdoorAirReset & setPointManager );

    void removeSetpointManagerOutdoorAirReset();

    boost::optional<SetpointManagerOutdoorAirReset> setpointManagerOutdoorAirReset() const;

    void addSetpointManagerWarmest( SetpointManagerWarmest & setPointManager );

    void removeSetpointManagerWarmest();

    boost::optional<SetpointManagerWarmest> setpointManagerWarmest() const;

    std::vector<ModelObject> children() const;

    bool isRemovable() const;

    std::vector<IdfObject> remove();

    bool addToNode(Node & node);

    ModelObject clone(Model model) const;

   private:

    REGISTER_LOGGER("openstudio.model.Node");

  };

} //close detail

} //close model
} //close OpenStudio

#endif // MODEL_NODE_IMPL_HPP
