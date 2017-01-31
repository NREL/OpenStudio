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

#ifndef MODEL_LOOP_IMPL_HPP
#define MODEL_LOOP_IMPL_HPP

#include "ParentObject_Impl.hpp"

namespace openstudio {

namespace model {

class Node;
class HVACComponent;
class Splitter;
class Mixer;

namespace detail {

  class Model_Impl;

  class MODEL_API Loop_Impl : public ParentObject_Impl {

    
    
    
    
    

  public:

    Loop_Impl(IddObjectType type, Model_Impl* model);

    Loop_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    Loop_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
              Model_Impl* model,
              bool keepHandle);

    Loop_Impl(const Loop_Impl& other, Model_Impl* model, bool keepHandles);

    virtual ~Loop_Impl() {}

    virtual Node supplyInletNode() const = 0;

    virtual Node supplyOutletNode() const = 0;

    virtual std::vector<Node> supplyOutletNodes() const = 0;

    virtual Node demandInletNode() const = 0;

    virtual std::vector<Node> demandInletNodes() const = 0;

    virtual Node demandOutletNode() const = 0;

    virtual std::vector<ModelObject> children() const override;

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual std::vector<ModelObject> supplyComponents( HVACComponent inletComp,
                                                       HVACComponent outletComp,
                                                       openstudio::IddObjectType type = openstudio::IddObjectType("Catchall")) const;

    virtual std::vector<ModelObject> demandComponents( HVACComponent inletComp,
                                                       HVACComponent outletComp,
                                                       openstudio::IddObjectType type = openstudio::IddObjectType("Catchall")) const;

    virtual std::vector<ModelObject> supplyComponents(openstudio::IddObjectType type=openstudio::IddObjectType("Catchall")) const;

    virtual std::vector<ModelObject> demandComponents(openstudio::IddObjectType type=openstudio::IddObjectType("Catchall")) const;

    virtual std::vector<ModelObject> components(openstudio::IddObjectType type = openstudio::IddObjectType("Catchall"));

    std::vector<ModelObject> components(HVACComponent inletComp,
                                        HVACComponent outletComp,
                                        openstudio::IddObjectType type = openstudio::IddObjectType("Catchall"));

    virtual boost::optional<ModelObject> component(openstudio::Handle handle);

    virtual boost::optional<ModelObject> demandComponent(openstudio::Handle handle) const;

    virtual boost::optional<ModelObject> supplyComponent(openstudio::Handle handle) const;

    virtual std::vector<ModelObject> supplyComponents(std::vector<HVACComponent> inletComps,
                                                      std::vector<HVACComponent> outletComps,
                                                      openstudio::IddObjectType type
                                                      ) const;

    virtual std::vector<ModelObject> demandComponents(std::vector<HVACComponent> inletComps,
                                                      std::vector<HVACComponent> outletComps,
                                                      openstudio::IddObjectType type
                                                      ) const;

    virtual std::vector<openstudio::IdfObject> remove() override;

    virtual ModelObject clone(Model model) const override;

    virtual Splitter demandSplitter() = 0;

    virtual Mixer demandMixer() = 0;

  private:

    REGISTER_LOGGER("openstudio.model.Loop");

    // TODO: Make these const.
    boost::optional<ModelObject> supplyInletNodeAsModelObject();
    boost::optional<ModelObject> supplyOutletNodeAsModelObject();
    boost::optional<ModelObject> demandInletNodeAsModelObject();
    boost::optional<ModelObject> demandOutletNodeAsModelObject();

  };

} // detail

} // model

} // openstudio

#endif

