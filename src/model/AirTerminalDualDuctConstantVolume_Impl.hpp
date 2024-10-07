/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRTERMINALDUALDUCTCONSTANTVOLUME_IMPL_HPP
#define MODEL_AIRTERMINALDUALDUCTCONSTANTVOLUME_IMPL_HPP

#include "ModelAPI.hpp"
#include "Mixer_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class Node;

  namespace detail {

    /** AirTerminalDualDuctConstantVolume_Impl is a Mixer_Impl that is the implementation class for AirTerminalDualDuctConstantVolume.*/
    class MODEL_API AirTerminalDualDuctConstantVolume_Impl : public Mixer_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      AirTerminalDualDuctConstantVolume_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AirTerminalDualDuctConstantVolume_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AirTerminalDualDuctConstantVolume_Impl(const AirTerminalDualDuctConstantVolume_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AirTerminalDualDuctConstantVolume_Impl() override = default;

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

      Schedule availabilitySchedule() const;

      boost::optional<double> maximumAirFlowRate() const;

      bool isMaximumAirFlowRateAutosized() const;

      //@}
      /** @name Setters */
      //@{

      // Note Schedules are passed by reference, not const reference.
      bool setAvailabilitySchedule(Schedule& schedule);

      bool setMaximumAirFlowRate(double maximumAirFlowRate);

      void autosizeMaximumAirFlowRate();

      //@}
      /** @name Other */
      //@{

      virtual unsigned outletPort() const override;

      virtual unsigned inletPort(unsigned branchIndex) const override;

      virtual unsigned nextInletPort() const override;

      virtual unsigned newInletPortAfterBranch(unsigned branchIndex) override;

      void removePortForBranch(unsigned branchIndex) override;

      boost::optional<Node> hotAirInletNode() const;

      boost::optional<Node> coldAirInletNode() const;

      bool addToNode(Node& node) override;

      std::vector<IdfObject> remove() override;

      virtual ModelObject clone(Model model) const override;

      bool isRemovable() const override;

      boost::optional<double> autosizedMaximumAirFlowRate() const;

      //@}

     protected:
     private:
      REGISTER_LOGGER("openstudio.model.AirTerminalDualDuctConstantVolume");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRTERMINALDUALDUCTCONSTANTVOLUME_IMPL_HPP
