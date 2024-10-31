/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRTERMINALSINGLEDUCTVAVHEATANDCOOLREHEAT_IMPL_HPP
#define MODEL_AIRTERMINALSINGLEDUCTVAVHEATANDCOOLREHEAT_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class HVACComponent;

  namespace detail {

    /** AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl is a StraightComponent_Impl that is the implementation class for AirTerminalSingleDuctVAVHeatAndCoolReheat.*/
    class MODEL_API AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl(const AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl() override = default;

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

      double zoneMinimumAirFlowFraction() const;

      HVACComponent reheatCoil() const;

      boost::optional<double> maximumHotWaterorSteamFlowRate() const;

      bool isMaximumHotWaterorSteamFlowRateAutosized() const;

      double minimumHotWaterorSteamFlowRate() const;

      double convergenceTolerance() const;

      double maximumReheatAirTemperature() const;

      boost::optional<double> autosizedMaximumAirFlowRate() const;

      boost::optional<double> autosizedMaximumHotWaterorSteamFlowRate() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      boost::optional<Schedule> minimumAirFlowTurndownSchedule() const;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      void resetAvailabilitySchedule();

      bool setMaximumAirFlowRate(boost::optional<double> maximumAirFlowRate);

      void autosizeMaximumAirFlowRate();

      bool setZoneMinimumAirFlowFraction(double zoneMinimumAirFlowFraction);

      bool setReheatCoil(const HVACComponent& heatingCoil);

      bool setMaximumHotWaterorSteamFlowRate(boost::optional<double> maximumHotWaterorSteamFlowRate);

      void autosizeMaximumHotWaterorSteamFlowRate();

      bool setMinimumHotWaterorSteamFlowRate(double minimumHotWaterorSteamFlowRate);

      bool setAirOutlet(const std::string& airOutlet);

      bool setConvergenceTolerance(double convergenceTolerance);

      bool setMaximumReheatAirTemperature(double maximumReheatAirTemperature);

      bool setMinimumAirFlowTurndownSchedule(Schedule& schedule);

      void resetMinimumAirFlowTurndownSchedule();

      //@}
      /** @name Other */
      //@{

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      bool addToNode(Node& node) override;

      std::vector<IdfObject> remove() override;

      bool isRemovable() const override;

      std::vector<ModelObject> children() const override;

      ModelObject clone(Model model) const override;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.AirTerminalSingleDuctVAVHeatAndCoolReheat");

      boost::optional<HVACComponent> optionalReheatCoil() const;

      boost::optional<ModelObject> minimumAirFlowTurndownScheduleAsModelObject() const;

      bool setMinimumAirFlowTurndownScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRTERMINALSINGLEDUCTVAVHEATANDCOOLREHEAT_IMPL_HPP
