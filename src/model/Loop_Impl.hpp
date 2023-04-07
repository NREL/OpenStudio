/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#ifndef MODEL_LOOP_IMPL_HPP
#define MODEL_LOOP_IMPL_HPP

#include "ParentObject_Impl.hpp"

namespace openstudio {

class AppGFuelType;
class ComponentType;
class FuelType;

namespace model {

  class Node;
  class HVACComponent;
  class Splitter;
  class Mixer;
  class AvailabilityManagerAssignmentList;

  namespace detail {

    class Model_Impl;

    class MODEL_API Loop_Impl : public ParentObject_Impl
    {

     public:
      Loop_Impl(IddObjectType type, Model_Impl* model);

      Loop_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      Loop_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      Loop_Impl(const Loop_Impl& other, Model_Impl* model, bool keepHandles);

      virtual ~Loop_Impl() = default;

      /** This pure virtual method is intended to be overriden by child classes (namely PlantLoop and AirLoopHVAC) to create the basic topology of the
     * loop, that is to create the supply/demand inlet/outlet nodes, splitters and mixers as appropriate */
      virtual void createTopology() = 0;

      virtual Node supplyInletNode() const = 0;

      virtual Node supplyOutletNode() const = 0;

      virtual std::vector<Node> supplyOutletNodes() const = 0;

      virtual Node demandInletNode() const = 0;

      virtual std::vector<Node> demandInletNodes() const = 0;

      virtual Node demandOutletNode() const = 0;

      virtual std::vector<ModelObject> children() const override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual std::vector<ModelObject> supplyComponents(const HVACComponent& inletComp, const HVACComponent& outletComp,
                                                        openstudio::IddObjectType type = openstudio::IddObjectType("Catchall")) const;

      virtual std::vector<ModelObject> demandComponents(const HVACComponent& inletComp, const HVACComponent& outletComp,
                                                        openstudio::IddObjectType type = openstudio::IddObjectType("Catchall")) const;

      virtual std::vector<ModelObject> supplyComponents(openstudio::IddObjectType type = openstudio::IddObjectType("Catchall")) const;

      virtual std::vector<ModelObject> demandComponents(openstudio::IddObjectType type = openstudio::IddObjectType("Catchall")) const;

      virtual std::vector<ModelObject> components(openstudio::IddObjectType type = openstudio::IddObjectType("Catchall")) const;

      std::vector<ModelObject> components(const HVACComponent& inletComp, const HVACComponent& outletComp,
                                          openstudio::IddObjectType type = openstudio::IddObjectType("Catchall")) const;

      virtual boost::optional<ModelObject> component(openstudio::Handle handle) const;

      virtual boost::optional<ModelObject> demandComponent(openstudio::Handle handle) const;

      virtual boost::optional<ModelObject> supplyComponent(openstudio::Handle handle) const;

      // virtual std::vector<ModelObject> supplyComponents(std::vector<HVACComponent> inletComps, std::vector<HVACComponent> outletComps,
      //                                                   openstudio::IddObjectType type) const;

      // virtual std::vector<ModelObject> demandComponents(std::vector<HVACComponent> inletComps, std::vector<HVACComponent> outletComps,
      //                                                   openstudio::IddObjectType type) const;

      virtual std::vector<openstudio::IdfObject> remove() override;

      virtual ModelObject clone(Model model) const override;

      virtual Splitter demandSplitter() const = 0;

      virtual Mixer demandMixer() const = 0;

      virtual void autosize();

      virtual void applySizingValues();

      virtual AvailabilityManagerAssignmentList availabilityManagerAssignmentList() const = 0;

      virtual ComponentType componentType() const;
      virtual std::vector<FuelType> coolingFuelTypes() const;
      virtual std::vector<FuelType> heatingFuelTypes() const;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const;

     private:
      REGISTER_LOGGER("openstudio.model.Loop");

      boost::optional<ModelObject> supplyInletNodeAsModelObject() const;
      boost::optional<ModelObject> supplyOutletNodeAsModelObject() const;
      boost::optional<ModelObject> demandInletNodeAsModelObject() const;
      boost::optional<ModelObject> demandOutletNodeAsModelObject() const;
    };

  }  // namespace detail

}  // namespace model

}  // namespace openstudio

#endif
