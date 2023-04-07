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

#ifndef MODEL_ELECTRICEQUIPMENTITEAIRCOOLEDDEFINITION_IMPL_HPP
#define MODEL_ELECTRICEQUIPMENTITEAIRCOOLEDDEFINITION_IMPL_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadDefinition_Impl.hpp"

namespace openstudio {
namespace model {

  class Curve;
  class Connection;
  class Schedule;

  namespace detail {

    /** ElectricEquipmentITEAirCooledDefinition_Impl is a SpaceLoadDefinition_Impl that is the implementation class for ElectricEquipmentITEAirCooledDefinition.*/
    class MODEL_API ElectricEquipmentITEAirCooledDefinition_Impl : public SpaceLoadDefinition_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      ElectricEquipmentITEAirCooledDefinition_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ElectricEquipmentITEAirCooledDefinition_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ElectricEquipmentITEAirCooledDefinition_Impl(const ElectricEquipmentITEAirCooledDefinition_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ElectricEquipmentITEAirCooledDefinition_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      //@}
      /** @name Getters */
      //@{

      std::string airFlowCalculationMethod() const;

      std::string designPowerInputCalculationMethod() const;

      boost::optional<double> wattsperUnit() const;

      boost::optional<double> wattsperZoneFloorArea() const;

      Curve cPUPowerInputFunctionofLoadingandAirTemperatureCurve() const;
      boost::optional<Curve> optionalCPUPowerInputFunctionofLoadingandAirTemperatureCurve() const;

      double designFanPowerInputFraction() const;

      bool isDesignFanPowerInputFractionDefaulted() const;

      boost::optional<double> designFanAirFlowRateperPowerInput() const;
      Curve airFlowFunctionofLoadingandAirTemperatureCurve() const;
      boost::optional<Curve> optionalAirFlowFunctionofLoadingandAirTemperatureCurve() const;

      Curve fanPowerInputFunctionofFlowCurve() const;
      boost::optional<Curve> optionalFanPowerInputFunctionofFlowCurve() const;

      double designEnteringAirTemperature() const;

      bool isDesignEnteringAirTemperatureDefaulted() const;

      std::string environmentalClass() const;

      std::string airInletConnectionType() const;

      double designRecirculationFraction() const;

      bool isDesignRecirculationFractionDefaulted() const;

      boost::optional<Curve> recirculationFunctionofLoadingandSupplyTemperatureCurve() const;

      double designElectricPowerSupplyEfficiency() const;

      bool isDesignElectricPowerSupplyEfficiencyDefaulted() const;

      boost::optional<Curve> electricPowerSupplyEfficiencyFunctionofPartLoadRatioCurve() const;

      double fractionofElectricPowerSupplyLossestoZone() const;

      bool isFractionofElectricPowerSupplyLossestoZoneDefaulted() const;

      double supplyTemperatureDifference() const;

      bool isSupplyTemperatureDifferenceDefaulted() const;

      boost::optional<Schedule> supplyTemperatureDifferenceSchedule() const;

      double returnTemperatureDifference() const;

      bool isReturnTemperatureDifferenceDefaulted() const;

      boost::optional<Schedule> returnTemperatureDifferenceSchedule() const;

      //@}
      /** @name Setters */
      //@{

      bool setAirFlowCalculationMethod(const std::string& airFlowCalculationMethod);

      bool setDesignPowerInputCalculationMethod(const std::string& designPowerInputCalculationMethod, double floorArea);

      bool setWattsperUnit(boost::optional<double> wattsperUnit);

      bool setWattsperZoneFloorArea(boost::optional<double> wattsperZoneFloorArea);

      bool setCPUPowerInputFunctionofLoadingandAirTemperatureCurve(const Curve& curve);

      bool setDesignFanPowerInputFraction(double designFanPowerInputFraction);

      void resetDesignFanPowerInputFraction();

      bool setDesignFanAirFlowRateperPowerInput(double designFanAirFlowRateperPowerInput);

      bool setAirFlowFunctionofLoadingandAirTemperatureCurve(const Curve& curve);

      bool setFanPowerInputFunctionofFlowCurve(const Curve& curve);

      void setDesignEnteringAirTemperature(double designEnteringAirTemperature);

      void resetDesignEnteringAirTemperature();

      bool setEnvironmentalClass(const std::string& environmentalClass);

      bool setAirInletConnectionType(const std::string& airInletConnectionType);

      bool setDesignRecirculationFraction(double designRecirculationFraction);

      void resetDesignRecirculationFraction();

      bool setRecirculationFunctionofLoadingandSupplyTemperatureCurve(const Curve& curve);

      void resetRecirculationFunctionofLoadingandSupplyTemperatureCurve();

      bool setDesignElectricPowerSupplyEfficiency(double designElectricPowerSupplyEfficiency);

      void resetDesignElectricPowerSupplyEfficiency();

      bool setElectricPowerSupplyEfficiencyFunctionofPartLoadRatioCurve(const Curve& curve);

      void resetElectricPowerSupplyEfficiencyFunctionofPartLoadRatioCurve();

      bool setFractionofElectricPowerSupplyLossestoZone(double fractionofElectricPowerSupplyLossestoZone);

      void resetFractionofElectricPowerSupplyLossestoZone();

      bool setSupplyTemperatureDifference(double supplyTemperatureDifference);

      void resetSupplyTemperatureDifference();

      // Note Schedules are passed by reference, not const reference.
      bool setSupplyTemperatureDifferenceSchedule(Schedule& schedule);

      void resetSupplyTemperatureDifferenceSchedule();

      bool setReturnTemperatureDifference(double returnTemperatureDifference);

      void resetReturnTemperatureDifference();

      // Note Schedules are passed by reference, not const reference.
      bool setReturnTemperatureDifferenceSchedule(Schedule& schedule);

      void resetReturnTemperatureDifferenceSchedule();

      //@}
      /** @name Other */
      //@{

      double getWattsperUnit(double floorArea) const;

      double getWattsperZoneFloorArea(double floorArea) const;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ElectricEquipmentITEAirCooledDefinition");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ELECTRICEQUIPMENTITEAIRCOOLEDDEFINITION_IMPL_HPP
