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

#ifndef MODEL_HVACCOMPONENT_IMPL_HPP
#define MODEL_HVACCOMPONENT_IMPL_HPP

#include "ParentObject_Impl.hpp"
#include "AirLoopHVAC.hpp"
#include "AirLoopHVACOutdoorAirSystem.hpp"
#include "PlantLoop.hpp"

namespace openstudio {

class AppGFuelType;
class ComponentType;
class FuelType;

namespace model {

  class Node;

  class Splitter;

  class ZoneHVACComponent;

  namespace detail {

    class MODEL_API HVACComponent_Impl : public ParentObject_Impl
    {
     public:
      HVACComponent_Impl(IddObjectType type, Model_Impl* model);

      HVACComponent_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      HVACComponent_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      HVACComponent_Impl(const HVACComponent_Impl& other, Model_Impl* model, bool keepHandles);

      virtual ~HVACComponent_Impl() = default;

      virtual boost::optional<AirLoopHVAC> airLoopHVAC() const;

      virtual boost::optional<PlantLoop> plantLoop() const;

      virtual boost::optional<Loop> loop() const;

      virtual boost::optional<AirLoopHVACOutdoorAirSystem> airLoopHVACOutdoorAirSystem() const;

      virtual bool addToNode(Node& node);

      virtual bool addToSplitter(Splitter& splitter);

      virtual boost::optional<ParentObject> parent() const override;

      virtual std::vector<ModelObject> children() const override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual void disconnect();

      virtual std::vector<IdfObject> remove() override;

      virtual bool isRemovable() const;

      // children controls what we see in the Inspector in OS App, and it's not necessarilly what we want to clone
      // So the parent/children link is broken on purpose here as it will forward to ModelObject_Impl::clone and **not** ParentObject_Impl
      virtual ModelObject clone(Model model) const override;

      /** This function returns a vector of HVACComponent that are directly downstream
   *  from this object on an AirLoopHVAC or PlantLoop.
   *  @param[in] previous is used to indicate which path of edges to return
  **/
      virtual std::vector<HVACComponent> edges(const boost::optional<HVACComponent>& previous);

      virtual boost::optional<HVACComponent> containingHVACComponent() const;

      virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const;

      virtual boost::optional<StraightComponent> containingStraightComponent() const;

      bool addToNode(Node& node, const HVACComponent& systemStartComponent, const HVACComponent& systemEndComponent, unsigned componentInletPort,
                     unsigned componentOutletPort);

      bool removeFromLoop(const HVACComponent& systemStartComponent, const HVACComponent& systemEndComponent, unsigned componentInletPort,
                          unsigned componentOutletPort);

      virtual void autosize();

      virtual void applySizingValues();

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

      virtual ComponentType componentType() const = 0;
      virtual std::vector<FuelType> coolingFuelTypes() const = 0;
      virtual std::vector<FuelType> heatingFuelTypes() const = 0;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const = 0;

     protected:
      friend class Model_Impl;
      friend class AirLoopHVAC_Impl;

      mutable boost::optional<AirLoopHVAC> m_airLoopHVAC;
      mutable boost::optional<PlantLoop> m_plantLoop;
      mutable boost::optional<AirLoopHVACOutdoorAirSystem> m_airLoopHVACOutdoorAirSystem;

     private:
      REGISTER_LOGGER("openstudio.model.HVACComponent");

      boost::optional<ModelObject> airLoopHVACAsModelObject() const;
      boost::optional<ModelObject> plantLoopAsModelObject() const;
      boost::optional<ModelObject> airLoopHVACOutdoorAirSystemAsModelObject() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif
