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

      virtual ~AirTerminalSingleDuctConstantVolumeReheat_Impl() = default;

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
