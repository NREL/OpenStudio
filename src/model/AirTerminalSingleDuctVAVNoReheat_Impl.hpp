/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRTERMINALSINGLEDUCTVAVNOREHEAT_IMPL_HPP
#define MODEL_AIRTERMINALSINGLEDUCTVAVNOREHEAT_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;

  namespace detail {

    /** AirTerminalSingleDuctVAVNoReheat_Impl is a StraightComponent_Impl that is the implementation class for AirTerminalSingleDuctVAVNoReheat.*/
    class MODEL_API AirTerminalSingleDuctVAVNoReheat_Impl : public StraightComponent_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      AirTerminalSingleDuctVAVNoReheat_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AirTerminalSingleDuctVAVNoReheat_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AirTerminalSingleDuctVAVNoReheat_Impl(const AirTerminalSingleDuctVAVNoReheat_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AirTerminalSingleDuctVAVNoReheat_Impl() override = default;

      //@}

      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual bool addToNode(Node& node) override;

      virtual std::vector<openstudio::IdfObject> remove() override;

      virtual bool isRemovable() const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      Schedule availabilitySchedule() const;

      boost::optional<double> maximumAirFlowRate() const;

      bool isMaximumAirFlowRateAutosized() const;

      boost::optional<std::string> zoneMinimumAirFlowInputMethod() const;

      boost::optional<double> constantMinimumAirFlowFraction() const;
      bool isConstantMinimumAirFlowFractionAutosized() const;
      bool isConstantMinimumAirFlowFractionDefaulted() const;

      boost::optional<double> fixedMinimumAirFlowRate() const;
      bool isFixedMinimumAirFlowRateAutosized() const;
      bool isFixedMinimumAirFlowRateDefaulted() const;

      boost::optional<Schedule> minimumAirFlowFractionSchedule() const;
      boost::optional<Schedule> minimumAirFlowTurndownSchedule() const;

      boost::optional<double> autosizedMaximumAirFlowRate() const;
      boost::optional<double> autosizedConstantMinimumAirFlowFraction() const;
      boost::optional<double> autosizedFixedMinimumAirFlowRate() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      bool setMaximumAirFlowRate(boost::optional<double> maximumAirFlowRate);

      void resetMaximumAirFlowRate();

      void autosizeMaximumAirFlowRate();

      bool setZoneMinimumAirFlowInputMethod(boost::optional<std::string> zoneMinimumAirFlowInputMethod);

      void resetZoneMinimumAirFlowInputMethod();

      bool setConstantMinimumAirFlowFraction(double constantMinimumAirFlowFraction);
      void autosizeConstantMinimumAirFlowFraction();
      void resetConstantMinimumAirFlowFraction();

      bool setFixedMinimumAirFlowRate(double fixedMinimumAirFlowRate);
      void autosizeFixedMinimumAirFlowRate();
      void resetFixedMinimumAirFlowRate();

      bool setMinimumAirFlowFractionSchedule(Schedule& schedule);

      void resetMinimumAirFlowFractionSchedule();

      bool setMinimumAirFlowTurndownSchedule(Schedule& schedule);

      void resetMinimumAirFlowTurndownSchedule();

      bool controlForOutdoorAir() const;

      bool setControlForOutdoorAir(bool controlForOutdoorAir);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.AirTerminalSingleDuctVAVNoReheat");

      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<Schedule> optionalAvailabilitySchedule() const;

      std::vector<std::string> zoneMinimumAirFlowInputMethodValues() const;

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

#endif  // MODEL_AIRTERMINALSINGLEDUCTVAVNOREHEAT_IMPL_HPP
