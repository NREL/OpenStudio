/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PUMPVARIABLESPEEDCONDENSATE_IMPL_HPP
#define MODEL_PUMPVARIABLESPEEDCONDENSATE_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class Curve;

  namespace detail {

    /** PumpVariableSpeedCondensate_Impl is a StraightComponent_Impl that is the implementation class for
   *  PumpVariableSpeedCondensate.*/
    class MODEL_API PumpVariableSpeedCondensate_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      PumpVariableSpeedCondensate_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      PumpVariableSpeedCondensate_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      PumpVariableSpeedCondensate_Impl(const PumpVariableSpeedCondensate_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~PumpVariableSpeedCondensate_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual std::vector<ModelObject> children() const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual bool addToNode(Node& node) override;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<double> ratedSteamVolumeFlowRate() const;

      bool isRatedSteamVolumeFlowRateAutosized() const;

      double ratedPumpHead() const;

      boost::optional<double> ratedPowerConsumption() const;

      bool isRatedPowerConsumptionAutosized() const;

      double motorEfficiency() const;

      double fractionofMotorInefficienciestoFluidStream() const;

      double coefficient1ofthePartLoadPerformanceCurve() const;

      double coefficient2ofthePartLoadPerformanceCurve() const;

      double coefficient3ofthePartLoadPerformanceCurve() const;

      double coefficient4ofthePartLoadPerformanceCurve() const;

      boost::optional<Schedule> pumpFlowRateSchedule() const;

      boost::optional<ThermalZone> zone() const;

      double skinLossRadiativeFraction() const;

      std::string designPowerSizingMethod() const;

      double designElectricPowerPerUnitFlowRate() const;

      double designShaftPowerPerUnitFlowRatePerUnitHead() const;

      std::string endUseSubcategory() const;

      //@}
      /** @name Setters */
      //@{

      bool setRatedSteamVolumeFlowRate(boost::optional<double> ratedSteamVolumeFlowRate);

      void resetRatedSteamVolumeFlowRate();

      void autosizeRatedSteamVolumeFlowRate();

      bool setRatedPumpHead(double ratedPumpHead);

      bool setRatedPowerConsumption(boost::optional<double> ratedPowerConsumption);

      void resetRatedPowerConsumption();

      void autosizeRatedPowerConsumption();

      bool setMotorEfficiency(double motorEfficiency);

      bool setFractionofMotorInefficienciestoFluidStream(double fractionofMotorInefficienciestoFluidStream);

      bool setCoefficient1ofthePartLoadPerformanceCurve(double coefficient1ofthePartLoadPerformanceCurve);

      bool setCoefficient2ofthePartLoadPerformanceCurve(double coefficient2ofthePartLoadPerformanceCurve);

      bool setCoefficient3ofthePartLoadPerformanceCurve(double coefficient3ofthePartLoadPerformanceCurve);

      bool setCoefficient4ofthePartLoadPerformanceCurve(double coefficient4ofthePartLoadPerformanceCurve);

      bool setPumpFlowRateSchedule(Schedule& schedule);

      void resetPumpFlowRateSchedule();

      bool setDesignPowerSizingMethod(const std::string& designPowerSizingMethod);

      bool setDesignElectricPowerPerUnitFlowRate(double designElectricPowerPerUnitFlowRate);

      bool setDesignShaftPowerPerUnitFlowRatePerUnitHead(double designShaftPowerPerUnitFlowRatePerUnitHead);

      bool setZone(const ThermalZone& thermalZone);

      void resetZone();

      bool setSkinLossRadiativeFraction(double skinLossRadiativeFraction);

      bool setEndUseSubcategory(const std::string& endUseSubcategory);

      //@}
      /** @name Other */
      //@{

      boost::optional<double> autosizedRatedSteamVolumeFlowRate() const;

      boost::optional<double> autosizedRatedPowerConsumption() const;

      //@}
     private:
      REGISTER_LOGGER("openstudio.model.PumpVariableSpeedCondensate");

      std::vector<std::string> pumpControlTypeValues() const;
      std::vector<std::string> vfdControlTypeValues() const;

      boost::optional<ModelObject> pumpFlowRateScheduleAsModelObject() const;

      bool setPumpFlowRateScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PUMPVARIABLESPEEDCONDENSATE_IMPL_HPP
