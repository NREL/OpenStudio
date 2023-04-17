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

      virtual ~AirTerminalDualDuctVAV_Impl() = default;

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
