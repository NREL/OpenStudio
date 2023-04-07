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

#ifndef MODEL_AVAILABILITYMANAGEROPTIMUMSTART_HPP
#define MODEL_AVAILABILITYMANAGEROPTIMUMSTART_HPP

#include "ModelAPI.hpp"
#include "AvailabilityManager.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class ThermalZone;

  namespace detail {

    class AvailabilityManagerOptimumStart_Impl;

  }  // namespace detail

  /** AvailabilityManagerOptimumStart is a AvailabilityManager that wraps the OpenStudio IDD object 'OS:AvailabilityManager:OptimumStart'. */
  class MODEL_API AvailabilityManagerOptimumStart : public AvailabilityManager
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit AvailabilityManagerOptimumStart(const Model& model);

    virtual ~AvailabilityManagerOptimumStart() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AvailabilityManagerOptimumStart(const AvailabilityManagerOptimumStart& other) = default;
    AvailabilityManagerOptimumStart(AvailabilityManagerOptimumStart&& other) = default;
    AvailabilityManagerOptimumStart& operator=(const AvailabilityManagerOptimumStart&) = default;
    AvailabilityManagerOptimumStart& operator=(AvailabilityManagerOptimumStart&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> controlTypeValues();

    static std::vector<std::string> controlAlgorithmValues();

    /** @name Getters */
    //@{

    Schedule applicabilitySchedule() const;

    std::string controlType() const;

    boost::optional<ThermalZone> controlZone() const;

    double maximumValueforOptimumStartTime() const;

    std::string controlAlgorithm() const;

    double constantTemperatureGradientduringCooling() const;

    double constantTemperatureGradientduringHeating() const;

    double initialTemperatureGradientduringCooling() const;

    double initialTemperatureGradientduringHeating() const;

    double constantStartTime() const;

    int numberofPreviousDays() const;

    //@}
    /** @name Setters */
    //@{

    bool setApplicabilitySchedule(Schedule& schedule);

    bool setControlType(const std::string& controlType);

    bool setControlZone(const ThermalZone& thermalZone);

    void resetControlZone();

    bool setMaximumValueforOptimumStartTime(double maximumValueforOptimumStartTime);

    bool setControlAlgorithm(const std::string& controlAlgorithm);

    bool setConstantTemperatureGradientduringCooling(double constantTemperatureGradientduringCooling);

    bool setConstantTemperatureGradientduringHeating(double constantTemperatureGradientduringHeating);

    bool setInitialTemperatureGradientduringCooling(double initialTemperatureGradientduringCooling);

    bool setInitialTemperatureGradientduringHeating(double initialTemperatureGradientduringHeating);

    bool setConstantStartTime(double constantStartTime);

    bool setNumberofPreviousDays(int numberofPreviousDays);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::AvailabilityManagerOptimumStart_Impl;

    explicit AvailabilityManagerOptimumStart(std::shared_ptr<detail::AvailabilityManagerOptimumStart_Impl> impl);

    friend class detail::AvailabilityManagerOptimumStart_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AvailabilityManagerOptimumStart");
  };

  /** \relates AvailabilityManagerOptimumStart*/
  using OptionalAvailabilityManagerOptimumStart = boost::optional<AvailabilityManagerOptimumStart>;

  /** \relates AvailabilityManagerOptimumStart*/
  using AvailabilityManagerOptimumStartVector = std::vector<AvailabilityManagerOptimumStart>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AVAILABILITYMANAGEROPTIMUMSTART_HPP
