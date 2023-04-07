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

#ifndef MODEL_AIRGAP_HPP
#define MODEL_AIRGAP_HPP

#include "ModelAPI.hpp"
#include "OpaqueMaterial.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class AirGap_Impl;

  }  // namespace detail

  /** AirGap is a OpaqueMaterial that wraps the OpenStudio IDD object 'OS:Material:AirGap'. */
  class MODEL_API AirGap : public OpaqueMaterial
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit AirGap(const Model& model, double thermalResistance = 0.1);

    virtual ~AirGap() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirGap(const AirGap& other) = default;
    AirGap(AirGap&& other) = default;
    AirGap& operator=(const AirGap&) = default;
    AirGap& operator=(AirGap&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    double thermalResistance() const;

    //@}
    /** @name Setters */
    //@{

    bool setThermalResistance(double thermalResistance);

    // Resets the thermal resistance to the default of 0.1 m2-K/W
    void resetThermalResistance();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::AirGap_Impl;

    explicit AirGap(std::shared_ptr<detail::AirGap_Impl> impl);

    friend class detail::AirGap_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirGap");
  };

  /** \relates AirGap*/
  using OptionalAirGap = boost::optional<AirGap>;

  /** \relates AirGap*/
  using AirGapVector = std::vector<AirGap>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRGAP_HPP
