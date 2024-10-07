/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_HEADEREDPUMPSCONSTANTSPEED_IMPL_HPP
#define MODEL_HEADEREDPUMPSCONSTANTSPEED_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class ThermalZone;

  namespace detail {

    /** HeaderedPumpsConstantSpeed_Impl is a StraightComponent_Impl that is the implementation class for HeaderedPumpsConstantSpeed.*/
    class MODEL_API HeaderedPumpsConstantSpeed_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      HeaderedPumpsConstantSpeed_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      HeaderedPumpsConstantSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      HeaderedPumpsConstantSpeed_Impl(const HeaderedPumpsConstantSpeed_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~HeaderedPumpsConstantSpeed_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

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

      boost::optional<double> totalRatedFlowRate() const;

      bool isTotalRatedFlowRateAutosized() const;

      int numberofPumpsinBank() const;

      std::string flowSequencingControlScheme() const;

      double ratedPumpHead() const;

      boost::optional<double> ratedPowerConsumption() const;

      bool isRatedPowerConsumptionAutosized() const;

      double motorEfficiency() const;

      double fractionofMotorInefficienciestoFluidStream() const;

      std::string pumpControlType() const;

      boost::optional<Schedule> pumpFlowRateSchedule() const;

      boost::optional<ThermalZone> thermalZone() const;

      double skinLossRadiativeFraction() const;

      std::string designPowerSizingMethod() const;

      double designElectricPowerPerUnitFlowRate() const;

      double designShaftPowerPerUnitFlowRatePerUnitHead() const;

      std::string endUseSubcategory() const;

      boost::optional<double> autosizedTotalRatedFlowRate() const;

      boost::optional<double> autosizedRatedPowerConsumption() const;

      //@}
      /** @name Setters */
      //@{

      bool setTotalRatedFlowRate(boost::optional<double> totalRatedFlowRate);

      void autosizeTotalRatedFlowRate();

      bool setNumberofPumpsinBank(int numberofPumpsinBank);

      bool setFlowSequencingControlScheme(const std::string& flowSequencingControlScheme);

      bool setRatedPumpHead(double ratedPumpHead);

      bool setRatedPowerConsumption(boost::optional<double> ratedPowerConsumption);

      void autosizeRatedPowerConsumption();

      bool setMotorEfficiency(double motorEfficiency);

      bool setFractionofMotorInefficienciestoFluidStream(double fractionofMotorInefficienciestoFluidStream);

      bool setPumpControlType(const std::string& pumpControlType);

      bool setPumpFlowRateSchedule(Schedule& schedule);

      void resetPumpFlowRateSchedule();

      bool setThermalZone(const boost::optional<ThermalZone>& thermalZone);

      void resetThermalZone();

      bool setSkinLossRadiativeFraction(double skinLossRadiativeFraction);

      bool setDesignPowerSizingMethod(const std::string& designPowerSizingMethod);

      bool setDesignElectricPowerPerUnitFlowRate(double designElectricPowerPerUnitFlowRate);

      bool setDesignShaftPowerPerUnitFlowRatePerUnitHead(double designShaftPowerPerUnitFlowRatePerUnitHead);

      bool setEndUseSubcategory(const std::string& endUseSubcategory);

      //@}
      /** @name Other */
      //@{

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      bool addToNode(Node& node) override;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.HeaderedPumpsConstantSpeed");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_HEADEREDPUMPSCONSTANTSPEED_IMPL_HPP
