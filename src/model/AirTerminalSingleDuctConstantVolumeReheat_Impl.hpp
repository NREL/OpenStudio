/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRTERMINALSINGLEDUCTCONSTANTVOLUMEREHEAT_IMPL_HPP
#define MODEL_AIRTERMINALSINGLEDUCTCONSTANTVOLUMEREHEAT_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  // TODO: Check the following class names against object getters and setters.
  class Schedule;
  class HVACComponent;

  namespace detail {

    /** AirTerminalSingleDuctConstantVolumeReheat_Impl is a StraightComponent_Impl that is the implementation class for AirTerminalSingleDuctConstantVolumeReheat.*/
    class MODEL_API AirTerminalSingleDuctConstantVolumeReheat_Impl : public StraightComponent_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      AirTerminalSingleDuctConstantVolumeReheat_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AirTerminalSingleDuctConstantVolumeReheat_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AirTerminalSingleDuctConstantVolumeReheat_Impl(const AirTerminalSingleDuctConstantVolumeReheat_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AirTerminalSingleDuctConstantVolumeReheat_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual bool addToNode(Node& node) override;

      virtual std::vector<ModelObject> children() const override;

      virtual std::vector<openstudio::IdfObject> remove() override;

      virtual bool isRemovable() const override;

      virtual ModelObject clone(Model model) const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      // TODO: Check return type. From object lists, some candidates are: Schedule.
      Schedule availabilitySchedule() const;

      boost::optional<double> maximumAirFlowRate() const;

      bool isMaximumAirFlowRateAutosized() const;

      // TODO: Check return type. From object lists, some candidates are: HeatingCoilName.
      HVACComponent reheatCoil() const;

      boost::optional<double> maximumHotWaterorSteamFlowRate() const;

      bool isMaximumHotWaterorSteamFlowRateAutosized() const;

      double minimumHotWaterorSteamFlowRate() const;

      bool isMinimumHotWaterorSteamFlowRateDefaulted() const;

      double convergenceTolerance() const;

      bool isConvergenceToleranceDefaulted() const;

      double maximumReheatAirTemperature() const;

      bool isMaximumReheatAirTemperatureDefaulted() const;

      boost::optional<double> autosizedMaximumAirFlowRate() const;

      boost::optional<double> autosizedMaximumHotWaterorSteamFlowRate() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      //@}
      /** @name Setters */
      //@{

      // TODO: Check argument type. From object lists, some candidates are: Schedule.
      bool setAvailabilitySchedule(Schedule& schedule);

      bool setMaximumAirFlowRate(boost::optional<double> maximumAirFlowRate);

      void resetMaximumAirFlowRate();

      void autosizeMaximumAirFlowRate();

      // TODO: Check argument type. From object lists, some candidates are: HeatingCoilName.
      bool setReheatCoil(const HVACComponent& coil);

      void resetReheatCoil();

      bool setMaximumHotWaterorSteamFlowRate(boost::optional<double> maximumHotWaterorSteamFlowRate);

      void resetMaximumHotWaterorSteamFlowRate();

      void autosizeMaximumHotWaterorSteamFlowRate();

      bool setMinimumHotWaterorSteamFlowRate(double minimumHotWaterorSteamFlowRate);

      void resetMinimumHotWaterorSteamFlowRate();

      bool setConvergenceTolerance(double convergenceTolerance);

      void resetConvergenceTolerance();

      bool setMaximumReheatAirTemperature(double maximumReheatAirTemperature);

      void resetMaximumReheatAirTemperature();

      //@}
      /** @name Other */
      //@{

      AirflowNetworkEquivalentDuct getAirflowNetworkEquivalentDuct(double length, double diameter);
      boost::optional<AirflowNetworkEquivalentDuct> airflowNetworkEquivalentDuct() const;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.AirTerminalSingleDuctConstantVolumeReheat");

      // TODO: Check the return types of these methods.
      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<HVACComponent> optionalReheatCoil() const;
      boost::optional<Schedule> optionalAvailabilitySchedule() const;
      boost::optional<ModelObject> availabilityScheduleAsModelObject() const;
      boost::optional<ModelObject> reheatCoilAsModelObject() const;

      bool setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setReheatCoilAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRTERMINALSINGLEDUCTCONSTANTVOLUMEREHEAT_IMPL_HPP
