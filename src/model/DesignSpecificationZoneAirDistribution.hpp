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

#ifndef MODEL_DESIGNSPECIFICATIONZONEAIRDISTRIBUTION_HPP
#define MODEL_DESIGNSPECIFICATIONZONEAIRDISTRIBUTION_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  class Schedule;

  namespace detail {

    class DesignSpecificationZoneAirDistribution_Impl;

  }  // namespace detail

  /** DesignSpecificationZoneAirDistribution is a ModelObject that wraps the OpenStudio IDD object 'OS:DesignSpecification:ZoneAirDistribution'. */
  class MODEL_API DesignSpecificationZoneAirDistribution : public ModelObject
  {
   public:
    explicit DesignSpecificationZoneAirDistribution(const Model& model);

    virtual ~DesignSpecificationZoneAirDistribution() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    DesignSpecificationZoneAirDistribution(const DesignSpecificationZoneAirDistribution& other) = default;
    DesignSpecificationZoneAirDistribution(DesignSpecificationZoneAirDistribution&& other) = default;
    DesignSpecificationZoneAirDistribution& operator=(const DesignSpecificationZoneAirDistribution&) = default;
    DesignSpecificationZoneAirDistribution& operator=(DesignSpecificationZoneAirDistribution&&) = default;

    static IddObjectType iddObjectType();

    double zoneAirDistributionEffectivenessinCoolingMode() const;

    bool isZoneAirDistributionEffectivenessinCoolingModeDefaulted() const;

    bool setZoneAirDistributionEffectivenessinCoolingMode(double zoneAirDistributionEffectivenessinCoolingMode);

    void resetZoneAirDistributionEffectivenessinCoolingMode();

    double zoneAirDistributionEffectivenessinHeatingMode() const;

    bool isZoneAirDistributionEffectivenessinHeatingModeDefaulted() const;

    bool setZoneAirDistributionEffectivenessinHeatingMode(double zoneAirDistributionEffectivenessinHeatingMode);

    void resetZoneAirDistributionEffectivenessinHeatingMode();

    boost::optional<Schedule> zoneAirDistributionEffectivenessSchedule() const;

    bool setZoneAirDistributionEffectivenessSchedule(Schedule& schedule);

    void resetZoneAirDistributionEffectivenessSchedule();

    double zoneSecondaryRecirculationFraction() const;

    bool isZoneSecondaryRecirculationFractionDefaulted() const;

    bool setZoneSecondaryRecirculationFraction(double zoneSecondaryRecirculationFraction);

    void resetZoneSecondaryRecirculationFraction();

   protected:
    /// @cond
    using ImplType = detail::DesignSpecificationZoneAirDistribution_Impl;

    explicit DesignSpecificationZoneAirDistribution(std::shared_ptr<detail::DesignSpecificationZoneAirDistribution_Impl> impl);

    friend class detail::DesignSpecificationZoneAirDistribution_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.DesignSpecificationZoneAirDistribution");
  };

  /** \relates DesignSpecificationZoneAirDistribution*/
  using OptionalDesignSpecificationZoneAirDistribution = boost::optional<DesignSpecificationZoneAirDistribution>;

  /** \relates DesignSpecificationZoneAirDistribution*/
  using DesignSpecificationZoneAirDistributionVector = std::vector<DesignSpecificationZoneAirDistribution>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_DESIGNSPECIFICATIONZONEAIRDISTRIBUTION_HPP
