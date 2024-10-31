/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRTERMINALDUALDUCTVAVOUTDOORAIR_IMPL_HPP
#define MODEL_AIRTERMINALDUALDUCTVAVOUTDOORAIR_IMPL_HPP

#include "ModelAPI.hpp"
#include "Mixer_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class Node;
  class DesignSpecificationOutdoorAir;

  namespace detail {

    /** AirTerminalDualDuctVAVOutdoorAir_Impl is a Mixer_Impl that is the implementation class for AirTerminalDualDuctVAVOutdoorAir.*/
    class MODEL_API AirTerminalDualDuctVAVOutdoorAir_Impl : public Mixer_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      AirTerminalDualDuctVAVOutdoorAir_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AirTerminalDualDuctVAVOutdoorAir_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AirTerminalDualDuctVAVOutdoorAir_Impl(const AirTerminalDualDuctVAVOutdoorAir_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AirTerminalDualDuctVAVOutdoorAir_Impl() override = default;

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

      boost::optional<double> maximumTerminalAirFlowRate() const;

      bool isMaximumTerminalAirFlowRateAutosized() const;

      std::string perPersonVentilationRateMode() const;

      bool controlForOutdoorAir() const;

      //@}
      /** @name Setters */
      //@{

      // Note Schedules are passed by reference, not const reference.
      bool setAvailabilitySchedule(Schedule& schedule);

      bool setMaximumTerminalAirFlowRate(double maximumTerminalAirFlowRate);

      void autosizeMaximumTerminalAirFlowRate();

      bool setControlForOutdoorAir(bool controlForOutdoorAir);

      bool setPerPersonVentilationRateMode(const std::string& perPersonVentilationRateMode);

      //@}
      /** @name Other */
      //@{

      virtual unsigned outletPort() const override;

      virtual unsigned inletPort(unsigned branchIndex) const override;

      virtual unsigned nextInletPort() const override;

      unsigned newInletPortAfterBranch(unsigned branchIndex) override;

      void removePortForBranch(unsigned branchIndex) override;

      bool addToNode(Node& node) override;

      std::vector<IdfObject> remove() override;

      virtual ModelObject clone(Model model) const override;

      bool isRemovable() const override;

      boost::optional<Node> outdoorAirInletNode() const;

      boost::optional<Node> recirculatedAirInletNode() const;

      boost::optional<double> autosizedMaximumTerminalAirFlowRate() const;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.AirTerminalDualDuctVAVOutdoorAir");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRTERMINALDUALDUCTVAVOUTDOORAIR_IMPL_HPP
