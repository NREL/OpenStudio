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

      virtual ~AirTerminalSingleDuctVAVReheat_Impl() = default;

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
