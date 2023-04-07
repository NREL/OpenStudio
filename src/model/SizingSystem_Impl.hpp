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

#ifndef MODEL_SIZINGSYSTEM_IMPL_HPP
#define MODEL_SIZINGSYSTEM_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {

namespace model {

  class AirLoopHVAC;

  namespace detail {

    /** SizingSystem_Impl is a ModelObject_Impl that is the implementation class for SizingSystem.*/
    class MODEL_API SizingSystem_Impl : public ModelObject_Impl
    {

     public:
      SizingSystem_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SizingSystem_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      SizingSystem_Impl(const SizingSystem_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~SizingSystem_Impl() = default;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual bool setParent(ParentObject& newParent) override;

      std::string typeofLoadtoSizeOn() const;

      bool isTypeofLoadtoSizeOnDefaulted() const;

      boost::optional<double> designOutdoorAirFlowRate() const;

      bool isDesignOutdoorAirFlowRateDefaulted() const;

      bool isDesignOutdoorAirFlowRateAutosized() const;

      boost::optional<double> centralHeatingMaximumSystemAirFlowRatio() const;

      bool isCentralHeatingMaximumSystemAirFlowRatioDefaulted() const;

      bool isCentralHeatingMaximumSystemAirFlowRatioAutosized() const;

      double preheatDesignTemperature() const;

      double preheatDesignHumidityRatio() const;

      double precoolDesignTemperature() const;

      double precoolDesignHumidityRatio() const;

      double centralCoolingDesignSupplyAirTemperature() const;

      double centralHeatingDesignSupplyAirTemperature() const;

      std::string sizingOption() const;

      bool isSizingOptionDefaulted() const;

      bool allOutdoorAirinCooling() const;

      bool isAllOutdoorAirinCoolingDefaulted() const;

      bool allOutdoorAirinHeating() const;

      bool isAllOutdoorAirinHeatingDefaulted() const;

      double centralCoolingDesignSupplyAirHumidityRatio() const;

      bool isCentralCoolingDesignSupplyAirHumidityRatioDefaulted() const;

      double centralHeatingDesignSupplyAirHumidityRatio() const;

      bool isCentralHeatingDesignSupplyAirHumidityRatioDefaulted() const;

      std::string coolingDesignAirFlowMethod() const;

      bool isCoolingDesignAirFlowMethodDefaulted() const;

      double coolingDesignAirFlowRate() const;

      bool isCoolingDesignAirFlowRateDefaulted() const;

      std::string heatingDesignAirFlowMethod() const;

      bool isHeatingDesignAirFlowMethodDefaulted() const;

      double heatingDesignAirFlowRate() const;

      bool isHeatingDesignAirFlowRateDefaulted() const;

      std::string systemOutdoorAirMethod() const;

      bool isSystemOutdoorAirMethodDefaulted() const;

      double zoneMaximumOutdoorAirFraction() const;

      double coolingSupplyAirFlowRatePerFloorArea() const;

      double coolingFractionofAutosizedCoolingSupplyAirFlowRate() const;

      double coolingSupplyAirFlowRatePerUnitCoolingCapacity() const;

      double heatingSupplyAirFlowRatePerFloorArea() const;

      double heatingFractionofAutosizedHeatingSupplyAirFlowRate() const;

      double heatingFractionofAutosizedCoolingSupplyAirFlowRate() const;

      double heatingSupplyAirFlowRatePerUnitHeatingCapacity() const;

      std::string coolingDesignCapacityMethod() const;

      boost::optional<double> coolingDesignCapacity() const;

      bool isCoolingDesignCapacityAutosized() const;

      double coolingDesignCapacityPerFloorArea() const;

      double fractionofAutosizedCoolingDesignCapacity() const;

      std::string heatingDesignCapacityMethod() const;

      boost::optional<double> heatingDesignCapacity() const;

      bool isHeatingDesignCapacityAutosized() const;

      double heatingDesignCapacityPerFloorArea() const;

      double fractionofAutosizedHeatingDesignCapacity() const;

      std::string centralCoolingCapacityControlMethod() const;

      boost::optional<double> occupantDiversity() const;

      bool isOccupantDiversityAutosized() const;

      bool setTypeofLoadtoSizeOn(const std::string& typeofLoadtoSizeOn);

      void resetTypeofLoadtoSizeOn();

      bool setDesignOutdoorAirFlowRate(boost::optional<double> designOutdoorAirFlowRate);

      void resetDesignOutdoorAirFlowRate();

      void autosizeDesignOutdoorAirFlowRate();

      bool setCentralHeatingMaximumSystemAirFlowRatio(boost::optional<double> centralHeatingMaximumSystemAirFlowRatio);

      void resetCentralHeatingMaximumSystemAirFlowRatio();

      void autosizeCentralHeatingMaximumSystemAirFlowRatio();

      bool setPreheatDesignTemperature(double preheatDesignTemperature);

      bool setPreheatDesignHumidityRatio(double preheatDesignHumidityRatio);

      bool setPrecoolDesignTemperature(double precoolDesignTemperature);

      bool setPrecoolDesignHumidityRatio(double precoolDesignHumidityRatio);

      bool setCentralCoolingDesignSupplyAirTemperature(double centralCoolingDesignSupplyAirTemperature);

      bool setCentralHeatingDesignSupplyAirTemperature(double centralHeatingDesignSupplyAirTemperature);

      bool setSizingOption(const std::string& sizingOption);

      void resetSizingOption();

      bool setAllOutdoorAirinCooling(bool allOutdoorAirinCooling);

      void resetAllOutdoorAirinCooling();

      bool setAllOutdoorAirinHeating(bool allOutdoorAirinHeating);

      void resetAllOutdoorAirinHeating();

      bool setCentralCoolingDesignSupplyAirHumidityRatio(double centralCoolingDesignSupplyAirHumidityRatio);

      void resetCentralCoolingDesignSupplyAirHumidityRatio();

      bool setCentralHeatingDesignSupplyAirHumidityRatio(double centralHeatingDesignSupplyAirHumidityRatio);

      void resetCentralHeatingDesignSupplyAirHumidityRatio();

      bool setCoolingDesignAirFlowMethod(const std::string& coolingDesignAirFlowMethod);

      void resetCoolingDesignAirFlowMethod();

      bool setCoolingDesignAirFlowRate(double coolingDesignAirFlowRate);

      void resetCoolingDesignAirFlowRate();

      bool setHeatingDesignAirFlowMethod(const std::string& heatingDesignAirFlowMethod);

      void resetHeatingDesignAirFlowMethod();

      bool setHeatingDesignAirFlowRate(double heatingDesignAirFlowRate);

      void resetHeatingDesignAirFlowRate();

      bool setSystemOutdoorAirMethod(const std::string& systemOutdoorAirMethod);

      void resetSystemOutdoorAirMethod();

      bool setZoneMaximumOutdoorAirFraction(double zoneMaximumOutdoorAirFraction);

      bool setCoolingSupplyAirFlowRatePerFloorArea(double coolingSupplyAirFlowRatePerFloorArea);

      bool setCoolingFractionofAutosizedCoolingSupplyAirFlowRate(double coolingFractionofAutosizedCoolingSupplyAirFlowRate);

      bool setCoolingSupplyAirFlowRatePerUnitCoolingCapacity(double coolingSupplyAirFlowRatePerUnitCoolingCapacity);

      bool setHeatingSupplyAirFlowRatePerFloorArea(double heatingSupplyAirFlowRatePerFloorArea);

      bool setHeatingFractionofAutosizedHeatingSupplyAirFlowRate(double heatingFractionofAutosizedHeatingSupplyAirFlowRate);

      bool setHeatingFractionofAutosizedCoolingSupplyAirFlowRate(double heatingFractionofAutosizedCoolingSupplyAirFlowRate);

      bool setHeatingSupplyAirFlowRatePerUnitHeatingCapacity(double heatingSupplyAirFlowRatePerUnitHeatingCapacity);

      bool setCoolingDesignCapacityMethod(const std::string& coolingDesignCapacityMethod);

      bool setCoolingDesignCapacity(boost::optional<double> coolingDesignCapacity);

      void autosizeCoolingDesignCapacity();

      bool setCoolingDesignCapacityPerFloorArea(double coolingDesignCapacityPerFloorArea);

      bool setFractionofAutosizedCoolingDesignCapacity(double fractionofAutosizedCoolingDesignCapacity);

      bool setHeatingDesignCapacityMethod(const std::string& heatingDesignCapacityMethod);

      bool setHeatingDesignCapacity(boost::optional<double> heatingDesignCapacity);

      void autosizeHeatingDesignCapacity();

      bool setHeatingDesignCapacityPerFloorArea(double heatingDesignCapacityPerFloorArea);

      bool setFractionofAutosizedHeatingDesignCapacity(double fractionofAutosizedHeatingDesignCapacity);

      bool setCentralCoolingCapacityControlMethod(const std::string& centralCoolingCapacityControlMethod);

      bool setOccupantDiversity(double occupantDiversity);

      void autosizeOccupantDiversity();

      AirLoopHVAC airLoopHVAC() const;

      bool setAirLoopHVAC(const AirLoopHVAC& airLoopHVAC);

      boost::optional<double> autosizedDesignOutdoorAirFlowRate() const;

      boost::optional<double> autosizedCentralHeatingMaximumSystemAirFlowRatio() const;

      boost::optional<double> autosizedCoolingDesignCapacity() const;

      boost::optional<double> autosizedHeatingDesignCapacity() const;

      boost::optional<double> autosizedOccupantDiversity() const;

      void autosize();

      void applySizingValues();

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

     protected:
     private:
      REGISTER_LOGGER("openstudio.model.SizingSystem");
    };

  }  // namespace detail

}  // namespace model

}  // namespace openstudio

#endif  // MODEL_SIZINGSYSTEM_IMPL_HPP
