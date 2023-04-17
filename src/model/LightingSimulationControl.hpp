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

#ifndef MODEL_LIGHTINGSIMULATIONCONTROL_HPP
#define MODEL_LIGHTINGSIMULATIONCONTROL_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class LightingSimulationControl_Impl;

  }  // namespace detail

  /** LightingSimulationControl is a ModelObject that wraps the OpenStudio IDD object 'OS_LightingSimulationControl'. */
  class MODEL_API LightingSimulationControl : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~LightingSimulationControl() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    LightingSimulationControl(const LightingSimulationControl& other) = default;
    LightingSimulationControl(LightingSimulationControl&& other) = default;
    LightingSimulationControl& operator=(const LightingSimulationControl&) = default;
    LightingSimulationControl& operator=(LightingSimulationControl&&) = default;

    //@}
    /** @name Static Methods */
    //@{

    static IddObjectType iddObjectType();

    //@}
    /** @name Getters */
    //@{

    bool runSimulationforDesignDays() const;

    bool isRunSimulationforDesignDaysDefaulted() const;

    bool runSimulationforWeatherFileRunPeriods() const;

    bool isRunSimulationforWeatherFileRunPeriodsDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setRunSimulationforDesignDays(bool runSimulationforDesignDays);

    void resetRunSimulationforDesignDays();

    bool setRunSimulationforWeatherFileRunPeriods(bool runSimulationforWeatherFileRunPeriods);

    void resetRunSimulationforWeatherFileRunPeriods();

    //@}
   protected:
    /// @cond
    using ImplType = detail::LightingSimulationControl_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class detail::LightingSimulationControl_Impl;

    explicit LightingSimulationControl(std::shared_ptr<detail::LightingSimulationControl_Impl> impl);

    explicit LightingSimulationControl(Model& model);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.LightingSimulationControl");
  };

  /** \relates LightingSimulationControl*/
  using OptionalLightingSimulationControl = boost::optional<LightingSimulationControl>;

  /** \relates LightingSimulationControl*/
  using LightingSimulationControlVector = std::vector<LightingSimulationControl>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_LIGHTINGSIMULATIONCONTROL_HPP
