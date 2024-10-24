/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRTERMINALSINGLEDUCTPARALLELPIUREHEAT_IMPL_HPP
#define MODEL_AIRTERMINALSINGLEDUCTPARALLELPIUREHEAT_IMPL_HPP

#include "StraightComponent_Impl.hpp"

namespace openstudio {

namespace model {

  class Schedule;

  namespace detail {

    /** AirTerminalSingleDuctParallelPIUReheat_Impl is a StraightComponent_Impl that is the implementation class for AirTerminalSingleDuctParallelPIUReheat.*/
    class MODEL_API AirTerminalSingleDuctParallelPIUReheat_Impl : public StraightComponent_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      AirTerminalSingleDuctParallelPIUReheat_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AirTerminalSingleDuctParallelPIUReheat_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AirTerminalSingleDuctParallelPIUReheat_Impl(const AirTerminalSingleDuctParallelPIUReheat_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AirTerminalSingleDuctParallelPIUReheat_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual ModelObject clone(Model model) const override;

      virtual std::vector<openstudio::IdfObject> remove() override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual std::vector<ModelObject> children() const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual bool addToNode(Node& node) override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      Schedule availabilitySchedule() const;

      boost::optional<double> maximumPrimaryAirFlowRate() const;

      bool isMaximumPrimaryAirFlowRateAutosized() const;

      boost::optional<double> maximumSecondaryAirFlowRate() const;

      bool isMaximumSecondaryAirFlowRateAutosized() const;

      boost::optional<double> minimumPrimaryAirFlowFraction() const;

      bool isMinimumPrimaryAirFlowFractionAutosized() const;

      boost::optional<double> fanOnFlowFraction() const;

      bool isFanOnFlowFractionAutosized() const;

      boost::optional<Node> secondaryAirInletNode() const;

      unsigned secondaryAirInletPort() const;

      HVACComponent fan() const;

      HVACComponent reheatCoil() const;

      boost::optional<double> maximumHotWaterorSteamFlowRate() const;

      bool isMaximumHotWaterorSteamFlowRateAutosized() const;

      double minimumHotWaterorSteamFlowRate() const;

      bool isMinimumHotWaterorSteamFlowRateDefaulted() const;

      double convergenceTolerance() const;

      std::string fanControlType() const;

      double minimumFanTurnDownRatio() const;

      std::string heatingControlType() const;

      double designHeatingDischargeAirTemperature() const;

      double highLimitHeatingDischargeAirTemperature() const;

      bool isConvergenceToleranceDefaulted() const;

      boost::optional<double> autosizedMaximumPrimaryAirFlowRate() const;

      boost::optional<double> autosizedMaximumSecondaryAirFlowRate() const;

      boost::optional<double> autosizedMinimumPrimaryAirFlowFraction() const;

      boost::optional<double> autosizedFanOnFlowFraction() const;

      boost::optional<double> autosizedMaximumHotWaterorSteamFlowRate() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      //@}
      /** @name Setters */
      //@{

      // TODO: @macumber all of these should have a return type of bool, and shouldn't accept boost::optional double (or at least be overloaded to accept a
      // double too), see https://github.com/NREL/OpenStudio/issues/2620 and https://github.com/jmarrec/OpenStudio/commit/5d295638aea240becc14a45641ea72a413e1c360
      // and https://github.com/NREL/OpenStudio/pull/2589
      bool setAvailabilitySchedule(Schedule& schedule);

      bool setMaximumPrimaryAirFlowRate(boost::optional<double> maximumPrimaryAirFlowRate);

      void autosizeMaximumPrimaryAirFlowRate();

      bool setMaximumSecondaryAirFlowRate(boost::optional<double> maximumSecondaryAirFlowRate);

      void autosizeMaximumSecondaryAirFlowRate();

      bool setMinimumPrimaryAirFlowFraction(boost::optional<double> minimumPrimaryAirFlowFraction);

      void autosizeMinimumPrimaryAirFlowFraction();

      bool setFanOnFlowFraction(boost::optional<double> fanOnFlowFraction);

      void autosizeFanOnFlowFraction();

      bool setFan(HVACComponent& hvacComponent);

      bool setReheatCoil(HVACComponent& hvacComponent);

      bool setMaximumHotWaterorSteamFlowRate(boost::optional<double> maximumHotWaterorSteamFlowRate);

      void resetMaximumHotWaterorSteamFlowRate();

      void autosizeMaximumHotWaterorSteamFlowRate();

      bool setMinimumHotWaterorSteamFlowRate(double minimumHotWaterorSteamFlowRate);

      void resetMinimumHotWaterorSteamFlowRate();

      bool setConvergenceTolerance(double convergenceTolerance);

      void resetConvergenceTolerance();

      bool setFanControlType(const std::string& fanControlType);

      bool setMinimumFanTurnDownRatio(double minimumFanTurnDownRatio);

      bool setHeatingControlType(const std::string& heatingControlType);

      bool setDesignHeatingDischargeAirTemperature(double designHeatingDischargeAirTemperature);

      bool setHighLimitHeatingDischargeAirTemperature(double highLimitHeatingDischargeAirTemperature);

      // This is a non publlic method to draw recirculation air from a plenum
      // Use carefully.  Removing (or moving) plenum will leave model in invalid state
      bool setInducedAirPlenumZone(ThermalZone& thermalZone);

      bool setFanAvailabilitySchedule(Schedule& schedule);

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.AirTerminalSingleDuctParallelPIUReheat");

      // optional getters for children() (in case want to remove() before fully constructed).
      boost::optional<Schedule> optionalAvailabilitySchedule() const;
      boost::optional<HVACComponent> optionalFan() const;
      boost::optional<HVACComponent> optionalReheatCoil() const;

      boost::optional<ModelObject> availabilityScheduleAsModelObject() const;

      bool setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRTERMINALSINGLEDUCTPARALLELPIUREHEAT_IMPL_HPP
