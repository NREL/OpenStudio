/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRTERMINALDUALDUCTVAV_IMPL_HPP
#define MODEL_AIRTERMINALDUALDUCTVAV_IMPL_HPP

#include "ModelAPI.hpp"
#include "Mixer_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class DesignSpecificationOutdoorAir;
  class Node;

  namespace detail {

    /** AirTerminalDualDuctVAV_Impl is a Mixer_Impl that is the implementation class for AirTerminalDualDuctVAV.*/
    class MODEL_API AirTerminalDualDuctVAV_Impl : public Mixer_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      AirTerminalDualDuctVAV_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AirTerminalDualDuctVAV_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AirTerminalDualDuctVAV_Impl(const AirTerminalDualDuctVAV_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AirTerminalDualDuctVAV_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<Schedule> availabilitySchedule() const;

      boost::optional<double> maximumDamperAirFlowRate() const;

      bool isMaximumDamperAirFlowRateAutosized() const;

      double zoneMinimumAirFlowFraction() const;

      boost::optional<DesignSpecificationOutdoorAir> designSpecificationOutdoorAirObject() const;

      boost::optional<double> autosizedMaximumDamperAirFlowRate() const;

      boost::optional<Schedule> minimumAirFlowTurndownSchedule() const;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      void resetAvailabilitySchedule();

      bool setMaximumDamperAirFlowRate(boost::optional<double> maximumDamperAirFlowRate);

      void autosizeMaximumDamperAirFlowRate();

      bool setZoneMinimumAirFlowFraction(double zoneMinimumAirFlowFraction);

      bool setDesignSpecificationOutdoorAirObject(const boost::optional<DesignSpecificationOutdoorAir>& designSpecificationOutdoorAir);

      void resetDesignSpecificationOutdoorAirObject();

      bool setMinimumAirFlowTurndownSchedule(Schedule& schedule);

      void resetMinimumAirFlowTurndownSchedule();

      //@}
      /** @name Other */
      //@{

      virtual unsigned outletPort() const override;

      virtual unsigned inletPort(unsigned branchIndex) const override;

      virtual unsigned nextInletPort() const override;

      unsigned newInletPortAfterBranch(unsigned branchIndex) override;

      void removePortForBranch(unsigned branchIndex) override;

      boost::optional<Node> hotAirInletNode() const;

      boost::optional<Node> coldAirInletNode() const;

      bool addToNode(Node& node) override;

      std::vector<IdfObject> remove() override;

      virtual ModelObject clone(Model model) const override;

      bool isRemovable() const override;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.AirTerminalDualDuctVAV");

      boost::optional<ModelObject> minimumAirFlowTurndownScheduleAsModelObject() const;

      bool setMinimumAirFlowTurndownScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRTERMINALDUALDUCTVAV_IMPL_HPP
