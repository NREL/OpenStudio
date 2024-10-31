/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRTERMINALSINGLEDUCTCONSTANTVOLUMEFOURPIPEINDUCTION_IMPL_HPP
#define MODEL_AIRTERMINALSINGLEDUCTCONSTANTVOLUMEFOURPIPEINDUCTION_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;

  namespace detail {

    /** AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl is a StraightComponent_Impl that is the implementation class for AirTerminalSingleDuctConstantVolumeFourPipeInduction.*/
    class MODEL_API AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                bool keepHandle);

      AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl(const AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl& other,
                                                                Model_Impl* model, bool keepHandle);

      virtual ~AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual bool addToNode(Node& node) override;

      virtual std::vector<ModelObject> children() const override;

      virtual ModelObject clone(Model model) const override;

      virtual std::vector<IdfObject> remove() override;

      virtual bool isRemovable() const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      boost::optional<Node> inducedAirInletNode() const;

      unsigned inducedAirInletPort() const;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<Schedule> availabilitySchedule() const;

      boost::optional<double> maximumTotalAirFlowRate() const;

      bool isMaximumTotalAirFlowRateAutosized() const;

      double inductionRatio() const;

      bool isInductionRatioDefaulted() const;

      HVACComponent heatingCoil() const;

      boost::optional<double> maximumHotWaterFlowRate() const;

      bool isMaximumHotWaterFlowRateAutosized() const;

      double minimumHotWaterFlowRate() const;

      bool isMinimumHotWaterFlowRateDefaulted() const;

      double heatingConvergenceTolerance() const;

      bool isHeatingConvergenceToleranceDefaulted() const;

      boost::optional<HVACComponent> coolingCoil() const;

      boost::optional<double> maximumColdWaterFlowRate() const;

      bool isMaximumColdWaterFlowRateAutosized() const;

      double minimumColdWaterFlowRate() const;

      bool isMinimumColdWaterFlowRateDefaulted() const;

      double coolingConvergenceTolerance() const;

      bool isCoolingConvergenceToleranceDefaulted() const;

      boost::optional<double> autosizedMaximumTotalAirFlowRate() const;

      boost::optional<double> autosizedMaximumHotWaterFlowRate() const;

      boost::optional<double> autosizedMaximumColdWaterFlowRate() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      void resetAvailabilitySchedule();

      bool setMaximumTotalAirFlowRate(boost::optional<double> maximumTotalAirFlowRate);

      void autosizeMaximumTotalAirFlowRate();

      bool setInductionRatio(double inductionRatio);

      void resetInductionRatio();

      bool setHeatingCoil(const HVACComponent& heatingCoil);

      bool setMaximumHotWaterFlowRate(boost::optional<double> maximumHotWaterFlowRate);

      void resetMaximumHotWaterFlowRate();

      void autosizeMaximumHotWaterFlowRate();

      bool setMinimumHotWaterFlowRate(double minimumHotWaterFlowRate);

      void resetMinimumHotWaterFlowRate();

      bool setHeatingConvergenceTolerance(double heatingConvergenceTolerance);

      void resetHeatingConvergenceTolerance();

      bool setCoolingCoil(const boost::optional<HVACComponent>& coolingCoil);

      void resetCoolingCoil();

      bool setMaximumColdWaterFlowRate(boost::optional<double> maximumColdWaterFlowRate);

      void resetMaximumColdWaterFlowRate();

      void autosizeMaximumColdWaterFlowRate();

      bool setMinimumColdWaterFlowRate(double minimumColdWaterFlowRate);

      void resetMinimumColdWaterFlowRate();

      bool setCoolingConvergenceTolerance(double coolingConvergenceTolerance);

      void resetCoolingConvergenceTolerance();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.AirTerminalSingleDuctConstantVolumeFourPipeInduction");

      // TODO: Check the return types of these methods.
      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<HVACComponent> optionalHeatingCoil() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRTERMINALSINGLEDUCTCONSTANTVOLUMEFOURPIPEINDUCTION_IMPL_HPP
