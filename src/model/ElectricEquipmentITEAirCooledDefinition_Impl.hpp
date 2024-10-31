/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

      virtual ~ElectricEquipmentITEAirCooledDefinition_Impl() override = default;

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

      boost::optional<double> designLevel() const;

      boost::optional<double> wattsperSpaceFloorArea() const;

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

      bool setDesignLevel(boost::optional<double> designLevel);

      bool setWattsperSpaceFloorArea(boost::optional<double> wattsperSpaceFloorArea);

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

      double getDesignLevel(double floorArea) const;

      double getPowerPerFloorArea(double floorArea) const;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ElectricEquipmentITEAirCooledDefinition");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ELECTRICEQUIPMENTITEAIRCOOLEDDEFINITION_IMPL_HPP
