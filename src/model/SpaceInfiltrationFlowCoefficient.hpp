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

#ifndef MODEL_SPACEINFILTRATIONFLOWCOEFFICIENT_HPP
#define MODEL_SPACEINFILTRATIONFLOWCOEFFICIENT_HPP

#include "ModelAPI.hpp"
#include "SpaceLoad.hpp"

namespace openstudio {
namespace model {

  class Schedule;

  namespace detail {

    class SpaceInfiltrationFlowCoefficient_Impl;

  }  // namespace detail

  /** SpaceInfiltrationFlowCoefficient is a SpaceLoad that wraps the OpenStudio IDD object 'OS:SpaceInfiltration:FlowCoefficient'. */
  class MODEL_API SpaceInfiltrationFlowCoefficient : public SpaceLoad
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SpaceInfiltrationFlowCoefficient(const Model& model);

    virtual ~SpaceInfiltrationFlowCoefficient() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SpaceInfiltrationFlowCoefficient(const SpaceInfiltrationFlowCoefficient& other) = default;
    SpaceInfiltrationFlowCoefficient(SpaceInfiltrationFlowCoefficient&& other) = default;
    SpaceInfiltrationFlowCoefficient& operator=(const SpaceInfiltrationFlowCoefficient&) = default;
    SpaceInfiltrationFlowCoefficient& operator=(SpaceInfiltrationFlowCoefficient&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    /** Returns the (fractional) infiltration schedule.  If this object does not
   *  specify a schedule this function will search the hierarchy. */
    boost::optional<Schedule> schedule() const;

    /** Returns true if this object does not specify a schedule directly. */
    bool isScheduleDefaulted() const;

    double flowCoefficient() const;

    double stackCoefficient() const;

    double pressureExponent() const;

    double windCoefficient() const;

    double shelterFactor() const;

    //@}
    /** @name Setters */
    //@{

    /** Sets the (fractional) Schedule. */
    bool setSchedule(Schedule& schedule);

    /** Resets the (fractional) Schedule. */
    void resetSchedule();

    bool setFlowCoefficient(double flowCoefficient);

    bool setStackCoefficient(double stackCoefficient);

    bool setPressureExponent(double pressureExponent);

    bool setWindCoefficient(double windCoefficient);

    bool setShelterFactor(double shelterFactor);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::SpaceInfiltrationFlowCoefficient_Impl;

    explicit SpaceInfiltrationFlowCoefficient(std::shared_ptr<detail::SpaceInfiltrationFlowCoefficient_Impl> impl);

    friend class Model;
    friend class IdfObject;

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SpaceInfiltrationFlowCoefficient");
  };

  /** \relates SpaceInfiltrationFlowCoefficient*/
  using OptionalSpaceInfiltrationFlowCoefficient = boost::optional<SpaceInfiltrationFlowCoefficient>;

  /** \relates SpaceInfiltrationFlowCoefficient*/
  using SpaceInfiltrationFlowCoefficientVector = std::vector<SpaceInfiltrationFlowCoefficient>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SPACEINFILTRATIONFLOWCOEFFICIENT_HPP
