/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef MODEL_AIRTERMINALSINGLEDUCTVAVHEATANDCOOLREHEAT_IMPL_HPP
#define MODEL_AIRTERMINALSINGLEDUCTVAVHEATANDCOOLREHEAT_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Schedule;
class HVACComponent;

namespace detail {

  /** AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl is a StraightComponent_Impl that is the implementation class for AirTerminalSingleDuctVAVHeatAndCoolReheat.*/
  class MODEL_API AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl : public StraightComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl(const IdfObject& idfObject,
                                                   Model_Impl* model,
                                                   bool keepHandle);

    AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                   Model_Impl* model,
                                                   bool keepHandle);

    AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl(const AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl& other,
                                                   Model_Impl* model,
                                                   bool keepHandle);

    virtual ~AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

    //@}
    /** @name Getters */
    //@{

    boost::optional<Schedule> availabilitySchedule() const;

    boost::optional<double> maximumAirFlowRate() const;

    bool isMaximumAirFlowRateAutosized() const;

    double zoneMinimumAirFlowFraction() const;

    HVACComponent reheatCoil() const;

    boost::optional<double> maximumHotWaterorSteamFlowRate() const;

    bool isMaximumHotWaterorSteamFlowRateAutosized() const;

    double minimumHotWaterorSteamFlowRate() const;

    double convergenceTolerance() const;

    double maximumReheatAirTemperature() const;

  boost::optional<double> autosizedMaximumAirFlowRate() const ;

  boost::optional<double> autosizedMaximumHotWaterorSteamFlowRate() const ;

  virtual void autosize() override;

  virtual void applySizingValues() override;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    bool setMaximumAirFlowRate(boost::optional<double> maximumAirFlowRate);

    void autosizeMaximumAirFlowRate();

    bool setZoneMinimumAirFlowFraction(double zoneMinimumAirFlowFraction);

    bool setReheatCoil(const HVACComponent& heatingCoil);

    bool setMaximumHotWaterorSteamFlowRate(boost::optional<double> maximumHotWaterorSteamFlowRate);

    void autosizeMaximumHotWaterorSteamFlowRate();

    bool setMinimumHotWaterorSteamFlowRate(double minimumHotWaterorSteamFlowRate);

    bool setAirOutlet(std::string airOutlet);

    bool setConvergenceTolerance(double convergenceTolerance);

    bool setMaximumReheatAirTemperature(double maximumReheatAirTemperature);

    //@}
    /** @name Other */
    //@{

    unsigned inletPort() override;

    unsigned outletPort() override;

    bool addToNode(Node & node) override;

    std::vector<IdfObject> remove() override;

    bool isRemovable() const override;

    std::vector<ModelObject> children() const override;

    ModelObject clone(Model model) const override;

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.AirTerminalSingleDuctVAVHeatAndCoolReheat");

    boost::optional<HVACComponent> optionalReheatCoil() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_AIRTERMINALSINGLEDUCTVAVHEATANDCOOLREHEAT_IMPL_HPP
