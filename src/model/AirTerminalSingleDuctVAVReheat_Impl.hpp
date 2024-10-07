/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRTERMINALSINGLEDUCTVAVREHEAT_IMPL_HPP
#define MODEL_AIRTERMINALSINGLEDUCTVAVREHEAT_IMPL_HPP

#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class MODEL_API AirTerminalSingleDuctVAVReheat_Impl : public StraightComponent_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      AirTerminalSingleDuctVAVReheat_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AirTerminalSingleDuctVAVReheat_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AirTerminalSingleDuctVAVReheat_Impl(const AirTerminalSingleDuctVAVReheat_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AirTerminalSingleDuctVAVReheat_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual std::vector<openstudio::IdfObject> remove() override;

      virtual bool isRemovable() const override;

      virtual ModelObject clone(Model model) const override;

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

      HVACComponent reheatCoil() const;

      bool setReheatCoil(HVACComponent& coil);

      Schedule availabilitySchedule() const;

      bool setAvailabilitySchedule(Schedule& schedule);

      boost::optional<double> maximumAirFlowRate() const;

      bool setMaximumAirFlowRate(double value);

      void autosizeMaximumAirFlowRate();

      bool isMaximumAirFlowRateAutosized() const;

      std::string zoneMinimumAirFlowInputMethod();

      bool setZoneMinimumAirFlowInputMethod(const std::string& value);

      boost::optional<double> constantMinimumAirFlowFraction() const;
      bool isConstantMinimumAirFlowFractionAutosized() const;
      bool setConstantMinimumAirFlowFraction(double value);
      void autosizeConstantMinimumAirFlowFraction();

      boost::optional<double> fixedMinimumAirFlowRate() const;
      bool isFixedMinimumAirFlowRateAutosized() const;
      bool setFixedMinimumAirFlowRate(double value);
      void autosizeFixedMinimumAirFlowRate();

      boost::optional<Schedule> minimumAirFlowFractionSchedule() const;

      bool setMinimumAirFlowFractionSchedule(Schedule& schedule);

      void resetMinimumAirFlowFractionSchedule();

      boost::optional<Schedule> minimumAirFlowTurndownSchedule() const;

      bool setMinimumAirFlowTurndownSchedule(Schedule& schedule);

      void resetMinimumAirFlowTurndownSchedule();

      boost::optional<double> maximumHotWaterOrSteamFlowRate();

      bool setMaximumHotWaterOrSteamFlowRate(double value);

      void autosizeMaximumHotWaterOrSteamFlowRate();

      bool isMaximumHotWaterOrSteamFlowRateAutosized() const;

      double minimumHotWaterOrSteamFlowRate();

      bool setMinimumHotWaterOrStreamFlowRate(double value);

      double convergenceTolerance();

      bool setConvergenceTolerance(double value);

      std::string damperHeatingAction();

      bool setDamperHeatingAction(const std::string& value);

      boost::optional<double> maximumFlowPerZoneFloorAreaDuringReheat();

      void resetMaximumFlowPerZoneFloorAreaDuringReheat();

      bool setMaximumFlowPerZoneFloorAreaDuringReheat(double value);

      void autosizeMaximumFlowPerZoneFloorAreaDuringReheat();

      bool isMaximumFlowPerZoneFloorAreaDuringReheatAutosized() const;

      boost::optional<double> maximumFlowFractionDuringReheat();

      bool setMaximumFlowFractionDuringReheat(double value);

      void autosizeMaximumFlowFractionDuringReheat();

      bool isMaximumFlowFractionDuringReheatAutosized() const;

      void resetMaximumFlowFractionDuringReheat();

      double maximumReheatAirTemperature();

      bool setMaximumReheatAirTemperature(double value);

      bool controlForOutdoorAir() const;

      bool setControlForOutdoorAir(bool controlForOutdoorAir);

      boost::optional<double> autosizedMaximumAirFlowRate() const;

      boost::optional<double> autosizedConstantMinimumAirFlowFraction() const;

      boost::optional<double> autosizedFixedMinimumAirFlowRate() const;

      boost::optional<double> autosizedMaximumHotWaterOrSteamFlowRate() const;

      boost::optional<double> autosizedMaximumFlowPerZoneFloorAreaDuringReheat() const;

      boost::optional<double> autosizedMaximumFlowFractionDuringReheat() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      AirflowNetworkEquivalentDuct getAirflowNetworkEquivalentDuct(double length, double diameter);

      boost::optional<AirflowNetworkEquivalentDuct> airflowNetworkEquivalentDuct() const;

     private:
      REGISTER_LOGGER("openstudio.model.AirTerminalSingleDuctVAVReheat");

      // optional getters for children so can remove() if constructor fails
      boost::optional<HVACComponent> optionalReheatCoil() const;
      boost::optional<Schedule> optionalAvailabilitySchedule() const;

      boost::optional<ModelObject> availabilityScheduleAsModelObject() const;
      boost::optional<ModelObject> minimumAirFlowFractionScheduleAsModelObject() const;
      boost::optional<ModelObject> minimumAirFlowTurndownScheduleAsModelObject() const;

      bool setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setMinimumAirFlowFractionScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setMinimumAirFlowTurndownScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model

}  // namespace openstudio

#endif  // MODEL_AIRTERMINALSINGLEDUCTVAVREHEAT_IMPL_HPP
