/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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
    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<HVACComponent> edges(const boost::optional<HVACComponent> & prev) override;

    unsigned inletPort() override;

    unsigned outletPort() override;

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

    std::vector<ModelObject> children() const override;

    bool isRemovable() const override;

    std::vector<IdfObject> remove() override;

    bool addToNode(Node & node) override;

    ModelObject clone(Model model) const override;

    bool isConnected(const ModelObject & modelObject);

   private:

    REGISTER_LOGGER("openstudio.model.Node");

  };

} //close detail

} //close model
} //close OpenStudio

#endif // MODEL_NODE_IMPL_HPP
