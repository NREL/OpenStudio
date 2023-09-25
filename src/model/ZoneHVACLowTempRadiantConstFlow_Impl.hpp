/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONEHVACLOWTEMPRADIANTCONSTFLOW_IMPL_HPP
#define MODEL_ZONEHVACLOWTEMPRADIANTCONSTFLOW_IMPL_HPP

#include "ModelAPI.hpp"
#include "Surface.hpp"
#include "Surface_Impl.hpp"
#include "ZoneHVACComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class HVACComponent;

  namespace detail {

    /** ZoneHVACLowTempRadiantConstFlow_Impl is a ZoneHVACComponent_Impl that is the implementation class for ZoneHVACLowTempRadiantConstFlow.*/
    class MODEL_API ZoneHVACLowTempRadiantConstFlow_Impl : public ZoneHVACComponent_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      ZoneHVACLowTempRadiantConstFlow_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ZoneHVACLowTempRadiantConstFlow_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ZoneHVACLowTempRadiantConstFlow_Impl(const ZoneHVACLowTempRadiantConstFlow_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ZoneHVACLowTempRadiantConstFlow_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{
      virtual ModelObject clone(Model model) const override;

      virtual std::vector<IdfObject> remove() override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual std::vector<ModelObject> children() const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      Schedule availabilitySchedule() const;

      boost::optional<std::string> radiantSurfaceType() const;

      std::vector<Surface> surfaces() const;

      std::string fluidtoRadiantSurfaceHeatTransferModel() const;

      bool isFluidtoRadiantSurfaceHeatTransferModelDefaulted() const;

      double hydronicTubingInsideDiameter() const;

      bool isHydronicTubingInsideDiameterDefaulted() const;

      double hydronicTubingOutsideDiameter() const;

      bool isHydronicTubingOutsideDiameterDefaulted() const;

      boost::optional<double> hydronicTubingLength() const;

      bool isHydronicTubingLengthAutosized() const;

      double hydronicTubingConductivity() const;

      bool isHydronicTubingConductivityDefaulted() const;

      std::string temperatureControlType() const;

      bool isTemperatureControlTypeDefaulted() const;

      double runningMeanOutdoorDryBulbTemperatureWeightingFactor() const;

      bool isRunningMeanOutdoorDryBulbTemperatureWeightingFactorDefaulted() const;

      HVACComponent heatingCoil() const;

      HVACComponent coolingCoil() const;

      boost::optional<double> ratedFlowRate() const;

      bool isRatedFlowRateAutosized() const;

      boost::optional<Schedule> pumpFlowRateSchedule() const;

      double ratedPumpHead() const;

      bool isRatedPumpHeadDefaulted() const;

      boost::optional<double> ratedPowerConsumption() const;

      double motorEfficiency() const;

      bool isMotorEfficiencyDefaulted() const;

      double fractionofMotorInefficienciestoFluidStream() const;

      bool isFractionofMotorInefficienciestoFluidStreamDefaulted() const;

      std::string numberofCircuits() const;

      double circuitLength() const;

      boost::optional<Schedule> changeoverDelayTimePeriodSchedule() const;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      bool setRadiantSurfaceType(boost::optional<std::string> radiantSurfaceType);

      void resetRadiantSurfaceType();

      bool setFluidtoRadiantSurfaceHeatTransferModel(const std::string& fluidtoRadiantSurfaceHeatTransferModel);

      void resetFluidtoRadiantSurfaceHeatTransferModel();

      bool setHydronicTubingInsideDiameter(double hydronicTubingInsideDiameter);

      void resetHydronicTubingInsideDiameter();

      bool setHydronicTubingOutsideDiameter(double hydronicTubingOutsideDiameter);

      void resetHydronicTubingOutsideDiameter();

      bool setHydronicTubingLength(boost::optional<double> hydronicTubingLength);

      void autosizeHydronicTubingLength();

      bool setHydronicTubingConductivity(double hydronicTubingConductivity);

      void resetHydronicTubingConductivity();

      bool setTemperatureControlType(const std::string& temperatureControlType);

      void resetTemperatureControlType();

      bool setRunningMeanOutdoorDryBulbTemperatureWeightingFactor(double runningMeanOutdoorDryBulbTemperatureWeightingFactor);

      void resetRunningMeanOutdoorDryBulbTemperatureWeightingFactor();

      bool setHeatingCoil(HVACComponent& heatingCoil);

      bool setCoolingCoil(HVACComponent& coolingCoil);

      bool setRatedFlowRate(boost::optional<double> ratedFlowRate);

      void autosizeRatedFlowRate();

      bool setPumpFlowRateSchedule(Schedule& schedule);

      void resetPumpFlowRateSchedule();

      bool setRatedPumpHead(double ratedPumpHead);

      void resetRatedPumpHead();

      bool setRatedPowerConsumption(boost::optional<double> ratedPowerConsumption);

      void resetRatedPowerConsumption();

      bool setMotorEfficiency(double motorEfficiency);

      void resetMotorEfficiency();

      bool setFractionofMotorInefficienciestoFluidStream(double fractionofMotorInefficienciestoFluidStream);

      void resetFractionofMotorInefficienciestoFluidStream();

      bool setNumberofCircuits(const std::string& numberofCircuits);

      bool setCircuitLength(double circuitLength);

      bool setChangeoverDelayTimePeriodSchedule(Schedule& schedule);

      void resetChangeoverDelayTimePeriodSchedule();

      boost::optional<ThermalZone> thermalZone() const override;

      bool addToThermalZone(ThermalZone& thermalZone) override;

      void removeFromThermalZone() override;

      //@}
      /** @name Other */
      //@{

      boost::optional<double> autosizedHydronicTubingLength() const;

      boost::optional<double> autosizedRatedFlowRate() const;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ZoneHVACLowTempRadiantConstFlow");

      // TODO: Check the return types of these methods.
      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<Schedule> optionalAvailabilitySchedule() const;
      boost::optional<HVACComponent> optionalHeatingCoil() const;
      boost::optional<HVACComponent> optionalCoolingCoil() const;

      std::vector<std::string> radiantSurfaceTypeValues() const;
      std::vector<std::string> temperatureControlTypeValues() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEHVACLOWTEMPRADIANTCONSTFLOW_IMPL_HPP
