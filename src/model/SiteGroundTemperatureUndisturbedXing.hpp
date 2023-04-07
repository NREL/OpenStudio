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

#ifndef MODEL_SITEGROUNDTEMPERATUREUNDISTURBEDXING_HPP
#define MODEL_SITEGROUNDTEMPERATUREUNDISTURBEDXING_HPP

#include <vector>
#include "ModelAPI.hpp"
#include "ModelObject.hpp"
#include "../utilities/time/Date.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class SiteGroundTemperatureUndisturbedXing_Impl;

  }  // namespace detail

  /** SiteGroundTemperatureUndisturbedXing is a ModelObject that wraps the OpenStudio IDD object 'OS_Site_GroundTemperature_UndisturbedXing'. */
  class MODEL_API SiteGroundTemperatureUndisturbedXing : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SiteGroundTemperatureUndisturbedXing(const Model& model);

    virtual ~SiteGroundTemperatureUndisturbedXing() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SiteGroundTemperatureUndisturbedXing(const SiteGroundTemperatureUndisturbedXing& other) = default;
    SiteGroundTemperatureUndisturbedXing(SiteGroundTemperatureUndisturbedXing&& other) = default;
    SiteGroundTemperatureUndisturbedXing& operator=(const SiteGroundTemperatureUndisturbedXing&) = default;
    SiteGroundTemperatureUndisturbedXing& operator=(SiteGroundTemperatureUndisturbedXing&&) = default;

    //@}
    /** @name Static Methods */
    //@{

    static IddObjectType iddObjectType();

    //@}
    /** @name Getters */
    //@{

    double soilThermalConductivity() const;

    double soilDensity() const;

    double soilSpecificHeat() const;

    double averageSoilSurfaceTemperature() const;

    double soilSurfaceTemperatureAmplitude1() const;

    double soilSurfaceTemperatureAmplitude2() const;

    double phaseShiftofTemperatureAmplitude1() const;

    double phaseShiftofTemperatureAmplitude2() const;

    //@}
    /** @name Setters */
    //@{

    bool setSoilThermalConductivity(double soilThermalConductivity);

    bool setSoilDensity(double soilDensity);

    bool setSoilSpecificHeat(double soilSpecificHeat);

    bool setAverageSoilSurfaceTemperature(double averageSoilSurfaceTemperature);

    bool setSoilSurfaceTemperatureAmplitude1(double soilSurfaceTemperatureAmplitude1);

    bool setSoilSurfaceTemperatureAmplitude2(double soilSurfaceTemperatureAmplitude2);

    bool setPhaseShiftofTemperatureAmplitude1(double phaseShiftofTemperatureAmplitude1);

    bool setPhaseShiftofTemperatureAmplitude2(double phaseShiftofTemperatureAmplitude2);

    //@}
   protected:
    /// @cond
    using ImplType = detail::SiteGroundTemperatureUndisturbedXing_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class detail::SiteGroundTemperatureUndisturbedXing_Impl;

    explicit SiteGroundTemperatureUndisturbedXing(std::shared_ptr<detail::SiteGroundTemperatureUndisturbedXing_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SiteGroundTemperatureUndisturbedXing");
  };

  /** \relates SiteGroundTemperatureUndisturbedXing*/
  using OptionalSiteGroundTemperatureUndisturbedXing = boost::optional<SiteGroundTemperatureUndisturbedXing>;

  /** \relates SiteGroundTemperatureUndisturbedXing*/
  using SiteGroundTemperatureUndisturbedXingVector = std::vector<SiteGroundTemperatureUndisturbedXing>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SITEGROUNDTEMPERATUREUNDISTURBEDXING_HPP
