/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRTERMINALSINGLEDUCTVAVHEATANDCOOLNOREHEAT_IMPL_HPP
#define MODEL_AIRTERMINALSINGLEDUCTVAVHEATANDCOOLNOREHEAT_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class Node;

  namespace detail {

    /** AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl is a StraightComponent_Impl that is the implementation class for AirTerminalSingleDuctVAVHeatAndCoolNoReheat.*/
    class MODEL_API AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl(const AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl& other, Model_Impl* model,
                                                       bool keepHandle);

      virtual ~AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl() override = default;

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

      std::string airOutlet() const;

      std::string airInlet() const;

      boost::optional<double> maximumAirFlowRate() const;

      bool isMaximumAirFlowRateAutosized() const;

      double zoneMinimumAirFlowFraction() const;

      boost::optional<double> autosizedMaximumAirFlowRate() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      boost::optional<Schedule> minimumAirFlowTurndownSchedule() const;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      void resetAvailabilitySchedule();

      // bool setAirOutlet(const std::string& airOutlet);
      // bool setAirInlet(const std::string& airInlet);

      bool setMaximumAirFlowRate(boost::optional<double> maximumAirFlowRate);

      void autosizeMaximumAirFlowRate();

      bool setZoneMinimumAirFlowFraction(double zoneMinimumAirFlowFraction);

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

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.AirTerminalSingleDuctVAVHeatAndCoolNoReheat");

      boost::optional<ModelObject> minimumAirFlowTurndownScheduleAsModelObject() const;

      bool setMinimumAirFlowTurndownScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRTERMINALSINGLEDUCTVAVHEATANDCOOLNOREHEAT_IMPL_HPP
