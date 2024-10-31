/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRTERMINALSINGLEDUCTSERIESPIUREHEAT_IMPL_HPP
#define MODEL_AIRTERMINALSINGLEDUCTSERIESPIUREHEAT_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class HVACComponent;

  namespace detail {

    /** AirTerminalSingleDuctSeriesPIUReheat_Impl is a StraightComponent_Impl that is the implementation class for AirTerminalSingleDuctSeriesPIUReheat.*/
    class MODEL_API AirTerminalSingleDuctSeriesPIUReheat_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      AirTerminalSingleDuctSeriesPIUReheat_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AirTerminalSingleDuctSeriesPIUReheat_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AirTerminalSingleDuctSeriesPIUReheat_Impl(const AirTerminalSingleDuctSeriesPIUReheat_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AirTerminalSingleDuctSeriesPIUReheat_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<Schedule> availabilitySchedule() const;

      boost::optional<double> maximumAirFlowRate() const;

      bool isMaximumAirFlowRateAutosized() const;

      boost::optional<double> maximumPrimaryAirFlowRate() const;

      bool isMaximumPrimaryAirFlowRateAutosized() const;

      boost::optional<double> minimumPrimaryAirFlowFraction() const;

      bool isMinimumPrimaryAirFlowFractionAutosized() const;

      HVACComponent fan() const;

      HVACComponent reheatCoil() const;

      boost::optional<double> maximumHotWaterorSteamFlowRate() const;

      bool isMaximumHotWaterorSteamFlowRateAutosized() const;

      double minimumHotWaterorSteamFlowRate() const;

      double convergenceTolerance() const;

      std::string fanControlType() const;

      double minimumFanTurnDownRatio() const;

      std::string heatingControlType() const;

      double designHeatingDischargeAirTemperature() const;

      double highLimitHeatingDischargeAirTemperature() const;

      boost::optional<double> autosizedMaximumAirFlowRate() const;

      boost::optional<double> autosizedMaximumPrimaryAirFlowRate() const;

      boost::optional<double> autosizedMinimumPrimaryAirFlowFraction() const;

      boost::optional<double> autosizedMaximumHotWaterorSteamFlowRate() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      void resetAvailabilitySchedule();

      bool setMaximumAirFlowRate(boost::optional<double> maximumAirFlowRate);

      void autosizeMaximumAirFlowRate();

      bool setMaximumPrimaryAirFlowRate(boost::optional<double> maximumPrimaryAirFlowRate);

      void autosizeMaximumPrimaryAirFlowRate();

      bool setMinimumPrimaryAirFlowFraction(boost::optional<double> minimumPrimaryAirFlowFraction);

      void autosizeMinimumPrimaryAirFlowFraction();

      bool setFan(const HVACComponent& fan);

      bool setReheatCoil(const HVACComponent& coil);

      bool setMaximumHotWaterorSteamFlowRate(boost::optional<double> maximumHotWaterorSteamFlowRate);

      void resetMaximumHotWaterorSteamFlowRate();

      void autosizeMaximumHotWaterorSteamFlowRate();

      bool setMinimumHotWaterorSteamFlowRate(double minimumHotWaterorSteamFlowRate);

      bool setConvergenceTolerance(double convergenceTolerance);

      bool setFanControlType(const std::string& fanControlType);

      bool setMinimumFanTurnDownRatio(double minimumFanTurnDownRatio);

      bool setHeatingControlType(const std::string& heatingControlType);

      bool setDesignHeatingDischargeAirTemperature(double designHeatingDischargeAirTemperature);

      bool setHighLimitHeatingDischargeAirTemperature(double highLimitHeatingDischargeAirTemperature);

      //@}
      /** @name Other */
      //@{

      virtual unsigned inletPort() const override;
      virtual unsigned outletPort() const override;
      unsigned secondaryAirInletPort() const;
      boost::optional<Node> secondaryAirInletNode() const;
      bool addToNode(Node& node) override;
      std::vector<ModelObject> children() const override;
      ModelObject clone(Model model) const override;
      bool setFanAvailabilitySchedule(Schedule& schedule);

      // This is a non publlic method to draw recirculation air from a plenum
      // Use carefully.  Removing (or moving) plenum will leave model in invalid state
      bool setInducedAirPlenumZone(ThermalZone& thermalZone);

      std::vector<IdfObject> remove() override;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.AirTerminalSingleDuctSeriesPIUReheat");

      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<HVACComponent> optionalFan() const;
      boost::optional<HVACComponent> optionalReheatCoil() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRTERMINALSINGLEDUCTSERIESPIUREHEAT_IMPL_HPP
