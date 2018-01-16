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

#ifndef MODEL_SETPOINTMANAGER_IMPL_HPP
#define MODEL_SETPOINTMANAGER_IMPL_HPP

#include "HVACComponent_Impl.hpp"
#include <boost/optional.hpp>

namespace openstudio {
namespace model {

namespace detail {

  class MODEL_API SetpointManager_Impl : public HVACComponent_Impl {
   public:

    SetpointManager_Impl(IddObjectType type, Model_Impl* model);

    SetpointManager_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    SetpointManager_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                 Model_Impl* model,
                                 bool keepHandle);

    SetpointManager_Impl(const SetpointManager_Impl& other, Model_Impl* model,bool keepHandles);

    virtual ~SetpointManager_Impl();

    virtual bool addToNode(Node & node) override;

    virtual std::vector<openstudio::IdfObject> remove() override;

    virtual ModelObject clone(Model model) const override;

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual boost::optional<ParentObject> parent() const override;

    virtual std::vector<ModelObject> children() const override;

    virtual std::string controlVariable() const = 0;

    virtual bool setControlVariable( const std::string & value ) = 0;

    virtual boost::optional<Node> setpointNode() const = 0;

    virtual boost::optional<AirLoopHVAC> airLoopHVAC() const override;

    virtual boost::optional<PlantLoop> plantLoop() const override;

    virtual boost::optional<Loop> loop() const override;

    virtual boost::optional<AirLoopHVACOutdoorAirSystem> airLoopHVACOutdoorAirSystem() const override;

   private:

    /** This is used to set the field SetpointNodeName.
      * Use addToNode to add a setpoint manager to a node. */
    virtual bool setSetpointNode( const Node & node ) = 0;

    /** This is used to reset the field SetpointNodeName. */
    virtual void resetSetpointNode() = 0;

    REGISTER_LOGGER("openstudio.model.SetpointManager");
  };

} // detail

} // model

} // openstudio

#endif // MODEL_SETPOINTMANAGER_IMPL_HPP

