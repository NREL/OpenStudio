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

#ifndef MODEL_AIRTERMINALSINGLEDUCTINLETSIDEMIXER_HPP
#define MODEL_AIRTERMINALSINGLEDUCTINLETSIDEMIXER_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class AirTerminalSingleDuctInletSideMixer_Impl;

  }  // namespace detail

  /** AirTerminalSingleDuctInletSideMixer is a StraightComponent that wraps the OpenStudio IDD object 'OS:AirTerminal:SingleDuct:InletSideMixer'.
 *  As of EnergyPlus version 8.6 this object maps to AirTerminal:SingleDuct:Mixer. **/
  class MODEL_API AirTerminalSingleDuctInletSideMixer : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit AirTerminalSingleDuctInletSideMixer(const Model& model);

    virtual ~AirTerminalSingleDuctInletSideMixer() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirTerminalSingleDuctInletSideMixer(const AirTerminalSingleDuctInletSideMixer& other) = default;
    AirTerminalSingleDuctInletSideMixer(AirTerminalSingleDuctInletSideMixer&& other) = default;
    AirTerminalSingleDuctInletSideMixer& operator=(const AirTerminalSingleDuctInletSideMixer&) = default;
    AirTerminalSingleDuctInletSideMixer& operator=(AirTerminalSingleDuctInletSideMixer&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> perPersonVentilationRateModeValues();
    /** @name Getters */
    //@{

    /** If true, OpenStudio will attach the DesignSpecificationOutdoorAir object associated
    * with the terminal's zone on export to EnergyPlus idf format.
    * This field replaces the functionality of the EnergyPlus field: Design Specification Outdoor Air Object Name.*/
    bool controlForOutdoorAir() const;

    std::string perPersonVentilationRateMode() const;

    //@}
    /** @name Setters */
    //@{

    bool setControlForOutdoorAir(bool controlForOutdoorAir);

    bool setPerPersonVentilationRateMode(const std::string& perPersonVentilationRateMode);
    //@}
    /** @name Other */
    //@{

    unsigned secondaryAirInletPort() const;

    boost::optional<Node> secondaryAirInletNode() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::AirTerminalSingleDuctInletSideMixer_Impl;

    explicit AirTerminalSingleDuctInletSideMixer(std::shared_ptr<detail::AirTerminalSingleDuctInletSideMixer_Impl> impl);

    friend class detail::AirTerminalSingleDuctInletSideMixer_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirTerminalSingleDuctInletSideMixer");
  };

  /** \relates AirTerminalSingleDuctInletSideMixer*/
  using OptionalAirTerminalSingleDuctInletSideMixer = boost::optional<AirTerminalSingleDuctInletSideMixer>;

  /** \relates AirTerminalSingleDuctInletSideMixer*/
  using AirTerminalSingleDuctInletSideMixerVector = std::vector<AirTerminalSingleDuctInletSideMixer>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRTERMINALSINGLEDUCTINLETSIDEMIXER_HPP
