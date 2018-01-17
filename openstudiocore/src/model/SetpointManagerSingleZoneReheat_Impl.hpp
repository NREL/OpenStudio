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

#ifndef MODEL_SETPOINTMANAGERSINGLEZONEREHEAT_IMPL_HPP
#define MODEL_SETPOINTMANAGERSINGLEZONEREHEAT_IMPL_HPP

#include "SetpointManager_Impl.hpp"

namespace openstudio {

namespace model {

class ZoneHVACEquipmentConnections;

namespace detail {

  class MODEL_API SetpointManagerSingleZoneReheat_Impl : public SetpointManager_Impl {

   public:

    SetpointManagerSingleZoneReheat_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    SetpointManagerSingleZoneReheat_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                         Model_Impl* model,
                                         bool keepHandle);

    SetpointManagerSingleZoneReheat_Impl(const SetpointManagerSingleZoneReheat_Impl& other,
                                         Model_Impl* model,
                                         bool keepHandles);

    virtual ~SetpointManagerSingleZoneReheat_Impl();

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual ModelObject clone(Model model) const override;

    virtual bool addToNode(Node & node) override;

    double minimumSupplyAirTemperature();

    bool setMinimumSupplyAirTemperature( double value );

    double maximumSupplyAirTemperature();

    bool setMaximumSupplyAirTemperature( double value );

    virtual boost::optional<Node> setpointNode() const override;

    virtual std::string controlVariable() const override;

    virtual bool setControlVariable( const std::string& controlVariable ) override;

    boost::optional<ThermalZone> controlZone();

    bool setControlZone(ThermalZone& thermalZone);

    void resetControlZone();

   private:
    virtual bool setSetpointNode( const Node & node ) override;

    virtual void resetSetpointNode() override;

    boost::optional<ModelObject> controlZoneAsModelObject();

    bool setControlZoneAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model

} // openstudio

#endif // MODEL_SETPOINTMANAGERSINGLEZONEREHEAT_IMPL_HPP
