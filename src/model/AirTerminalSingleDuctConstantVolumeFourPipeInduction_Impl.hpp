/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

      virtual ~AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl() = default;

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
