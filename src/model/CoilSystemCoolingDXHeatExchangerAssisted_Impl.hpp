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

#ifndef MODEL_COILSYSTEMCOOLINGDXHEATEXCHANGERASSISTED_IMPL_HPP
#define MODEL_COILSYSTEMCOOLINGDXHEATEXCHANGERASSISTED_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class AirToAirComponent;

  namespace detail {

    /** CoilSystemCoolingDXHeatExchangerAssisted_Impl is a StraightComponent_Impl that is the implementation class for CoilSystemCoolingDXHeatExchangerAssisted.*/
    class MODEL_API CoilSystemCoolingDXHeatExchangerAssisted_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilSystemCoolingDXHeatExchangerAssisted_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilSystemCoolingDXHeatExchangerAssisted_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilSystemCoolingDXHeatExchangerAssisted_Impl(const CoilSystemCoolingDXHeatExchangerAssisted_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilSystemCoolingDXHeatExchangerAssisted_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual unsigned inletPort() const override;
      virtual unsigned outletPort() const override;

      /**
     * Note JM 2019-03-13: At this point in time
     * CoilSystemCoolingDXHeatExchangerAssisted is **NOT** allowed on a Branch directly and should be placed inside one of the Unitary systems
     * cf https://github.com/NREL/EnergyPlus/issues/7222
     * This method returns false and does nothing as a result
     */
      virtual bool addToNode(Node& node) override;

      // will return the coolingCoil and heatExchanger
      virtual std::vector<ModelObject> children() const override;

      // Will also clone the coolingCoil and heatExchanger
      virtual ModelObject clone(Model model) const override;

      virtual boost::optional<HVACComponent> containingHVACComponent() const override;

      virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      AirToAirComponent heatExchanger() const;

      StraightComponent coolingCoil() const;

      //@}
      /** @name Setters */
      //@{

      bool setHeatExchanger(const AirToAirComponent& heatExchanger);

      bool setCoolingCoil(const StraightComponent& coolingCoil);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilSystemCoolingDXHeatExchangerAssisted");

      boost::optional<AirToAirComponent> optionalHeatExchanger() const;
      boost::optional<StraightComponent> optionalCoolingCoil() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILSYSTEMCOOLINGDXHEATEXCHANGERASSISTED_IMPL_HPP
