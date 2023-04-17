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

#ifndef MODEL_COILCOOLINGWATERPANELRADIANT_IMPL_HPP
#define MODEL_COILCOOLINGWATERPANELRADIANT_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"
#include "ZoneHVACComponent.hpp"
#include "ZoneHVACComponent_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** CoilCoolingWaterPanelRadiant_Impl is a StraightComponent_Impl that is the implementation class for CoilCoolingWaterPanelRadiant.*/
    class MODEL_API CoilCoolingWaterPanelRadiant_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilCoolingWaterPanelRadiant_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilCoolingWaterPanelRadiant_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilCoolingWaterPanelRadiant_Impl(const CoilCoolingWaterPanelRadiant_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilCoolingWaterPanelRadiant_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const override;

      virtual bool addToNode(Node& node) override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      double ratedInletWaterTemperature() const;

      double ratedInletSpaceTemperature() const;

      double ratedWaterMassFlowRate() const;

      std::string coolingDesignCapacityMethod() const;

      boost::optional<double> coolingDesignCapacity() const;

      bool isCoolingDesignCapacityAutosized() const;

      double coolingDesignCapacityPerFloorArea() const;

      double fractionofAutosizedCoolingDesignCapacity() const;

      boost::optional<double> maximumChilledWaterFlowRate() const;

      bool isMaximumChilledWaterFlowRateAutosized() const;

      std::string controlType() const;

      double coolingControlThrottlingRange() const;

      boost::optional<Schedule> coolingControlTemperatureSchedule() const;

      std::string condensationControlType() const;

      double condensationControlDewpointOffset() const;

      boost::optional<double> autosizedCoolingDesignCapacity() const;

      boost::optional<double> autosizedMaximumChilledWaterFlowRate() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      //@}
      /** @name Setters */
      //@{

      bool setRatedInletWaterTemperature(double ratedInletWaterTemperature);

      bool setRatedInletSpaceTemperature(double ratedInletSpaceTemperature);

      bool setRatedWaterMassFlowRate(double ratedWaterMassFlowRate);

      bool setCoolingDesignCapacityMethod(const std::string& coolingDesignCapacityMethod);

      bool setCoolingDesignCapacity(boost::optional<double> coolingDesignCapacity);

      void autosizeCoolingDesignCapacity();

      bool setCoolingDesignCapacityPerFloorArea(double coolingDesignCapacityPerFloorArea);

      bool setFractionofAutosizedCoolingDesignCapacity(double fractionofAutosizedCoolingDesignCapacity);

      bool setMaximumChilledWaterFlowRate(boost::optional<double> maximumChilledWaterFlowRate);

      void autosizeMaximumChilledWaterFlowRate();

      bool setControlType(const std::string& controlType);

      bool setCoolingControlThrottlingRange(double coolingControlThrottlingRange);

      bool setCoolingControlTemperatureSchedule(Schedule& coolingControlTemperatureSchedule);

      void resetCoolingControlTemperatureSchedule();

      bool setCondensationControlType(const std::string& condensationControlType);

      bool setCondensationControlDewpointOffset(double condensationControlDewpointOffset);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilCoolingWaterPanelRadiant");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILCOOLINGWATERPANELRADIANT_IMPL_HPP
