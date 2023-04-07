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

#ifndef MODEL_SPACEINFILTRATIONDESIGNFLOWRATE_HPP
#define MODEL_SPACEINFILTRATIONDESIGNFLOWRATE_HPP

#include "ModelAPI.hpp"
#include "SpaceLoad.hpp"

namespace openstudio {
namespace model {

  class Schedule;

  namespace detail {

    class SpaceInfiltrationDesignFlowRate_Impl;

  }  // namespace detail

  /** SpaceInfiltrationDesignFlowRate is a SpaceLoad that wraps the OpenStudio IDD
 *  object 'OS:SpaceInfiltration:DesignFlowRate'. */
  class MODEL_API SpaceInfiltrationDesignFlowRate : public SpaceLoad
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SpaceInfiltrationDesignFlowRate(const Model& model);

    virtual ~SpaceInfiltrationDesignFlowRate() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SpaceInfiltrationDesignFlowRate(const SpaceInfiltrationDesignFlowRate& other) = default;
    SpaceInfiltrationDesignFlowRate(SpaceInfiltrationDesignFlowRate&& other) = default;
    SpaceInfiltrationDesignFlowRate& operator=(const SpaceInfiltrationDesignFlowRate&) = default;
    SpaceInfiltrationDesignFlowRate& operator=(SpaceInfiltrationDesignFlowRate&&) = default;

    //@}
    /** @name Static Methods */
    //@{

    static IddObjectType iddObjectType();

    static std::vector<std::string> validDesignFlowRateCalculationMethodValues();

    /** \deprecated */
    static std::vector<std::string> designFlowRateCalculationMethodValues();

    //@}
    /** @name Getters */
    //@{

    /** Returns the (fractional) infiltration schedule.  If this object does not
   *  specify a schedule this function will search the hierarchy. */
    boost::optional<Schedule> schedule() const;

    /** Returns true if this object does not specify a schedule directly. */
    bool isScheduleDefaulted() const;

    std::string designFlowRateCalculationMethod() const;

    boost::optional<double> designFlowRate() const;

    boost::optional<double> flowperSpaceFloorArea() const;

    boost::optional<double> flowperExteriorSurfaceArea() const;

    boost::optional<double> flowperExteriorWallArea() const;

    boost::optional<double> airChangesperHour() const;

    double constantTermCoefficient() const;

    bool isConstantTermCoefficientDefaulted() const;

    double temperatureTermCoefficient() const;

    bool isTemperatureTermCoefficientDefaulted() const;

    double velocityTermCoefficient() const;

    bool isVelocityTermCoefficientDefaulted() const;

    double velocitySquaredTermCoefficient() const;

    bool isVelocitySquaredTermCoefficientDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    /** Sets the (fractional) Schedule. */
    bool setSchedule(Schedule& schedule);

    /** Resets the (fractional) Schedule. */
    void resetSchedule();

    bool setDesignFlowRate(double designFlowRate);

    bool setFlowperSpaceFloorArea(double flowperSpaceFloorArea);

    bool setFlowperExteriorSurfaceArea(double flowperExteriorSurfaceArea);

    bool setFlowperExteriorWallArea(double flowperExteriorWallArea);

    bool setAirChangesperHour(double airChangesperHour);

    bool setConstantTermCoefficient(double constantTermCoefficient);

    void resetConstantTermCoefficient();

    bool setTemperatureTermCoefficient(double temperatureTermCoefficient);

    void resetTemperatureTermCoefficient();

    bool setVelocityTermCoefficient(double velocityTermCoefficient);

    void resetVelocityTermCoefficient();

    bool setVelocitySquaredTermCoefficient(double velocitySquaredTermCoefficient);

    void resetVelocitySquaredTermCoefficient();

    //@}
    /** @name Other */
    //@{

    /** Returns the design flow rate (m^3/s) based on the floorArea (m^2),
   *  exteriorSurfaceArea (m^2), exteriorWallArea (m^2), and airVolume (m^3). */
    double getDesignFlowRate(double floorArea, double exteriorSurfaceArea, double exteriorWallArea, double airVolume) const;

    /** Returns the design flow per space floor area (m^3/m^2*s) based on the
   *  floorArea (m^2), exteriorSurfaceArea (m^2), exteriorWallArea (m^2), and
   *  airVolume (m^3). */
    double getFlowPerSpaceFloorArea(double floorArea, double exteriorSurfaceArea, double exteriorWallArea, double airVolume) const;

    /** Returns the design flow per exterior surface area (m^3/m^2*s) based on the
   *  floorArea (m^2), exteriorSurfaceArea (m^2), exteriorWallArea (m^2), and
   *  airVolume (m^3). */
    double getFlowPerExteriorSurfaceArea(double floorArea, double exteriorSurfaceArea, double exteriorWallArea, double airVolume) const;

    /** Returns the design flow per exterior wall area (m^3/m^2*s) based on the
   *  floorArea (m^2), exteriorSurfaceArea (m^2), exteriorWallArea (m^2), and
   *  airVolume (m^3). */
    double getFlowPerExteriorWallArea(double floorArea, double exteriorSurfaceArea, double exteriorWallArea, double airVolume) const;

    /** Returns the design air changes per hour (1/h) based on the floorArea (m^2),
   *  exteriorSurfaceArea (m^2), exteriorWallArea (m^2), and airVolume (m^3). */
    double getAirChangesPerHour(double floorArea, double exteriorSurfaceArea, double exteriorWallArea, double airVolume) const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::SpaceInfiltrationDesignFlowRate_Impl;

    friend class Model;
    friend class openstudio::IdfObject;

    explicit SpaceInfiltrationDesignFlowRate(std::shared_ptr<detail::SpaceInfiltrationDesignFlowRate_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SpaceInfiltrationDesignFlowRate");
  };

  /** \relates SpaceInfiltrationDesignFlowRate*/
  using OptionalSpaceInfiltrationDesignFlowRate = boost::optional<SpaceInfiltrationDesignFlowRate>;

  /** \relates SpaceInfiltrationDesignFlowRate*/
  using SpaceInfiltrationDesignFlowRateVector = std::vector<SpaceInfiltrationDesignFlowRate>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SPACEINFILTRATIONDESIGNFLOWRATE_HPP
