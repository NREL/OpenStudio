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

#ifndef MODEL_AIRTERMINALDUALDUCTVAV_HPP
#define MODEL_AIRTERMINALDUALDUCTVAV_HPP

#include "ModelAPI.hpp"
#include "Mixer.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class DesignSpecificationOutdoorAir;
  class Node;

  namespace detail {

    class AirTerminalDualDuctVAV_Impl;

  }  // namespace detail

  /** AirTerminalDualDuctVAV is a Mixer that wraps the OpenStudio IDD object 'OS:AirTerminal:DualDuct:VAV'. */
  class MODEL_API AirTerminalDualDuctVAV : public Mixer
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit AirTerminalDualDuctVAV(const Model& model);

    virtual ~AirTerminalDualDuctVAV() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirTerminalDualDuctVAV(const AirTerminalDualDuctVAV& other) = default;
    AirTerminalDualDuctVAV(AirTerminalDualDuctVAV&& other) = default;
    AirTerminalDualDuctVAV& operator=(const AirTerminalDualDuctVAV&) = default;
    AirTerminalDualDuctVAV& operator=(AirTerminalDualDuctVAV&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    boost::optional<Schedule> availabilitySchedule() const;

    boost::optional<double> maximumDamperAirFlowRate() const;

    bool isMaximumDamperAirFlowRateAutosized() const;

    double zoneMinimumAirFlowFraction() const;

    boost::optional<DesignSpecificationOutdoorAir> designSpecificationOutdoorAirObject() const;

    boost::optional<Schedule> minimumAirFlowTurndownSchedule() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    bool setMaximumDamperAirFlowRate(double maximumDamperAirFlowRate);

    void autosizeMaximumDamperAirFlowRate();

    bool setZoneMinimumAirFlowFraction(double zoneMinimumAirFlowFraction);

    bool setDesignSpecificationOutdoorAirObject(const DesignSpecificationOutdoorAir& designSpecificationOutdoorAir);

    void resetDesignSpecificationOutdoorAirObject();

    bool setMinimumAirFlowTurndownSchedule(Schedule& schedule);

    void resetMinimumAirFlowTurndownSchedule();

    //@}
    /** @name Other */
    //@{

    /** This corresponds to Mixer::inletModelObject(0), as well as the system's branch 0
    * (ie. AirLoopHVAC::demandInletNodes()[0] and AirLoopHVAC::supplyOutletNodes()[0])
    * In OpenStudio we avoid using hot / cold nomenclature in the dual duct system api,
    * but here in the terminal interface we use the language for transparency with the idf.
    */
    boost::optional<Node> hotAirInletNode() const;

    /** This corresponds to Mixer::inletModelObject(1), as well as the system's branch 1
    * (ie. AirLoopHVAC::demandInletNodes()[1] and AirLoopHVAC::supplyOutletNodes()[1])
    */
    boost::optional<Node> coldAirInletNode() const;

    /* Queries the autosized value from the SQL file */
    boost::optional<double> autosizedMaximumDamperAirFlowRate() const;

    /* Autosize all autosizeable fields */
    // void autosize();

    /* Hard applies the autosized values from SQL file */
    // void applySizingValues();

    //@}
   protected:
    /// @cond
    using ImplType = detail::AirTerminalDualDuctVAV_Impl;

    explicit AirTerminalDualDuctVAV(std::shared_ptr<detail::AirTerminalDualDuctVAV_Impl> impl);

    friend class detail::AirTerminalDualDuctVAV_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirTerminalDualDuctVAV");
  };

  /** \relates AirTerminalDualDuctVAV*/
  using OptionalAirTerminalDualDuctVAV = boost::optional<AirTerminalDualDuctVAV>;

  /** \relates AirTerminalDualDuctVAV*/
  using AirTerminalDualDuctVAVVector = std::vector<AirTerminalDualDuctVAV>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRTERMINALDUALDUCTVAV_HPP
