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

#ifndef MODEL_AIRTERMINALDUALDUCTVAVOUTDOORAIR_HPP
#define MODEL_AIRTERMINALDUALDUCTVAVOUTDOORAIR_HPP

#include "ModelAPI.hpp"
#include "Mixer.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class Node;
  class DesignSpecificationOutdoorAir;

  namespace detail {

    class AirTerminalDualDuctVAVOutdoorAir_Impl;

  }  // namespace detail

  /** AirTerminalDualDuctVAVOutdoorAir is a Mixer that wraps the OpenStudio IDD object 'OS:AirTerminal:DualDuct:VAV:OutdoorAir'. */
  class MODEL_API AirTerminalDualDuctVAVOutdoorAir : public Mixer
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit AirTerminalDualDuctVAVOutdoorAir(const Model& model);

    virtual ~AirTerminalDualDuctVAVOutdoorAir() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirTerminalDualDuctVAVOutdoorAir(const AirTerminalDualDuctVAVOutdoorAir& other) = default;
    AirTerminalDualDuctVAVOutdoorAir(AirTerminalDualDuctVAVOutdoorAir&& other) = default;
    AirTerminalDualDuctVAVOutdoorAir& operator=(const AirTerminalDualDuctVAVOutdoorAir&) = default;
    AirTerminalDualDuctVAVOutdoorAir& operator=(AirTerminalDualDuctVAVOutdoorAir&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> perPersonVentilationRateModeValues();

    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

    boost::optional<double> maximumTerminalAirFlowRate() const;

    bool isMaximumTerminalAirFlowRateAutosized() const;

    std::string perPersonVentilationRateMode() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    bool setMaximumTerminalAirFlowRate(double maximumTerminalAirFlowRate);

    void autosizeMaximumTerminalAirFlowRate();

    /** If true, OpenStudio will attach the DesignSpecificationOutdoorAir object associated
    * with the terminal's zone on export to EnergyPlus idf format.
    * This field replaces the functionality of the EnergyPlus field: Design Specification Outdoor Air Object Name.*/
    bool controlForOutdoorAir() const;
    bool setControlForOutdoorAir(bool controlForOutdoorAir);

    bool setPerPersonVentilationRateMode(const std::string& perPersonVentilationRateMode);

    /** This corresponds to Mixer::inletModelObject(0), as well as the system's branch 0
    * (ie. AirLoopHVAC::demandInletNodes()[0] and AirLoopHVAC::supplyOutletNodes()[0])
    * In OpenStudio we avoid using hot / cold nomenclature in the dual duct system api,
    * but here in the terminal interface we use the language for transparency with the idf.
    */
    boost::optional<Node> outdoorAirInletNode() const;

    /** This corresponds to Mixer::inletModelObject(1), as well as the system's branch 1
    * (ie. AirLoopHVAC::demandInletNodes()[1] and AirLoopHVAC::supplyOutletNodes()[1])
    */
    boost::optional<Node> recirculatedAirInletNode() const;

    //@}
    /** @name Other */
    //@{

    /* Queries the autosized maximum terminal air flow rate from the SQL file */
    boost::optional<double> autosizedMaximumTerminalAirFlowRate() const;

    /* Autosize all autosizeable fields */
    // void autosize();

    /* Hard applies the autosized values from SQL file */
    // void applySizingValues();

    //@}
   protected:
    /// @cond
    using ImplType = detail::AirTerminalDualDuctVAVOutdoorAir_Impl;

    explicit AirTerminalDualDuctVAVOutdoorAir(std::shared_ptr<detail::AirTerminalDualDuctVAVOutdoorAir_Impl> impl);

    friend class detail::AirTerminalDualDuctVAVOutdoorAir_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirTerminalDualDuctVAVOutdoorAir");
  };

  /** \relates AirTerminalDualDuctVAVOutdoorAir*/
  using OptionalAirTerminalDualDuctVAVOutdoorAir = boost::optional<AirTerminalDualDuctVAVOutdoorAir>;

  /** \relates AirTerminalDualDuctVAVOutdoorAir*/
  using AirTerminalDualDuctVAVOutdoorAirVector = std::vector<AirTerminalDualDuctVAVOutdoorAir>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRTERMINALDUALDUCTVAVOUTDOORAIR_HPP
