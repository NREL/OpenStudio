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

#ifndef MODEL_SWIMMINGPOOLINDOOR_HPP
#define MODEL_SWIMMINGPOOLINDOOR_HPP

#include <model/ModelAPI.hpp>
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class Surface;
  class Schedule;
  class Node;

  namespace detail {

    class SwimmingPoolIndoor_Impl;

  }  // namespace detail

  /** SwimmingPoolIndoor is a StraightComponent that wraps the OpenStudio IDD object 'OS:SwimmingPool:Indoor'. */
  class MODEL_API SwimmingPoolIndoor : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SwimmingPoolIndoor(const Model& model, const Surface& floorSurface);

    virtual ~SwimmingPoolIndoor() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SwimmingPoolIndoor(const SwimmingPoolIndoor& other) = default;
    SwimmingPoolIndoor(SwimmingPoolIndoor&& other) = default;
    SwimmingPoolIndoor& operator=(const SwimmingPoolIndoor&) = default;
    SwimmingPoolIndoor& operator=(SwimmingPoolIndoor&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    Surface surface() const;

    double averageDepth() const;

    Schedule activityFactorSchedule() const;

    Schedule makeupWaterSupplySchedule() const;

    Schedule coverSchedule() const;

    double coverEvaporationFactor() const;

    double coverConvectionFactor() const;

    double coverShortWavelengthRadiationFactor() const;

    double coverLongWavelengthRadiationFactor() const;

    double poolHeatingSystemMaximumWaterFlowRate() const;

    double poolMiscellaneousEquipmentPower() const;

    Schedule setpointTemperatureSchedule() const;

    double maximumNumberofPeople() const;

    Schedule peopleSchedule() const;

    Schedule peopleHeatGainSchedule() const;

    //@}
    /** @name Setters */
    //@{

    // Set the Pool's surface object. Surface MUST be of type 'Floor'
    bool setSurface(const Surface& floorSurface);

    bool setAverageDepth(double averageDepth);

    bool setActivityFactorSchedule(Schedule& schedule);

    bool setMakeupWaterSupplySchedule(Schedule& schedule);

    bool setCoverSchedule(Schedule& schedule);

    bool setCoverEvaporationFactor(double coverEvaporationFactor);

    bool setCoverConvectionFactor(double coverConvectionFactor);

    bool setCoverShortWavelengthRadiationFactor(double coverShortWavelengthRadiationFactor);

    bool setCoverLongWavelengthRadiationFactor(double coverLongWavelengthRadiationFactor);

    bool setPoolHeatingSystemMaximumWaterFlowRate(double poolHeatingSystemMaximumWaterFlowRate);

    bool setPoolMiscellaneousEquipmentPower(double poolMiscellaneousEquipmentPower);

    bool setSetpointTemperatureSchedule(Schedule& schedule);

    bool setMaximumNumberofPeople(double maximumNumberofPeople);

    bool setPeopleSchedule(Schedule& schedule);

    bool setPeopleHeatGainSchedule(Schedule& schedule);

    //@}
    /** @name Other */
    //@{

    // Convenience function to return the inletNode of the Pool object
    boost::optional<Node> poolWaterInletNode() const;

    // Convenience function to return the Outlet Node of the Pool object
    boost::optional<Node> poolWaterOutletNode() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::SwimmingPoolIndoor_Impl;

    explicit SwimmingPoolIndoor(std::shared_ptr<detail::SwimmingPoolIndoor_Impl> impl);

    friend class detail::SwimmingPoolIndoor_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SwimmingPoolIndoor");
  };

  /** \relates SwimmingPoolIndoor*/
  using OptionalSwimmingPoolIndoor = boost::optional<SwimmingPoolIndoor>;

  /** \relates SwimmingPoolIndoor*/
  using SwimmingPoolIndoorVector = std::vector<SwimmingPoolIndoor>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SWIMMINGPOOLINDOOR_HPP
