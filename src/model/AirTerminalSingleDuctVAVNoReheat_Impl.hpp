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

      virtual ~AirTerminalSingleDuctVAVNoReheat_Impl() = default;

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
