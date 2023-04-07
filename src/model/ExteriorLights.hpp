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

#ifndef MODEL_EXTERIORLIGHTS_HPP
#define MODEL_EXTERIORLIGHTS_HPP

#include "ModelAPI.hpp"
#include "ExteriorLoadInstance.hpp"

namespace openstudio {
namespace model {

  class ExteriorLightsDefinition;
  class Schedule;
  class Facility;

  namespace detail {

    class ExteriorLights_Impl;

  }  // namespace detail

  /** ExteriorLights is a ExteriorLoadInstance that wraps the OpenStudio IDD object
 *  'OS:Exterior:Lights'. ExteriorLights is a child of Facility and instances
 *  ExteriorLightsDefinition. */
  class MODEL_API ExteriorLights : public ExteriorLoadInstance
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** This constructor sets schedule to always on, and provides the option to use the
   *  astronomical clock control option. */
    ExteriorLights(const ExteriorLightsDefinition& definition, bool useControlOptionAstronomicalClock = false);

    /** This constructor requires a user-specified schedule. */
    ExteriorLights(const ExteriorLightsDefinition& definition, Schedule& schedule);

    virtual ~ExteriorLights() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ExteriorLights(const ExteriorLights& other) = default;
    ExteriorLights(ExteriorLights&& other) = default;
    ExteriorLights& operator=(const ExteriorLights&) = default;
    ExteriorLights& operator=(ExteriorLights&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> controlOptionValues();

    /** \deprecated */
    static std::vector<std::string> validControlOptionValues();

    /** @name Getters */
    //@{

    ExteriorLightsDefinition exteriorLightsDefinition() const;

    boost::optional<Schedule> schedule() const;

    std::string controlOption() const;

    bool isControlOptionDefaulted() const;

    double multiplier() const;

    bool isMultiplierDefaulted() const;

    std::string endUseSubcategory() const;

    bool isEndUseSubcategoryDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setExteriorLightsDefinition(const ExteriorLightsDefinition& exteriorLightsDefinition);

    bool setSchedule(Schedule& schedule);

    void resetSchedule();

    bool setControlOption(const std::string& controlOption);

    void resetControlOption();

    bool setMultiplier(double multiplier);

    void resetMultiplier();

    bool setEndUseSubcategory(const std::string& endUseSubcategory);

    void resetEndUseSubcategory();

    //@}
    /** @name Other */
    //@{

    Facility facility() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::ExteriorLights_Impl;

    explicit ExteriorLights(std::shared_ptr<detail::ExteriorLights_Impl> impl);

    friend class detail::ExteriorLights_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ExteriorLights");
  };

  /** \relates ExteriorLights*/
  using OptionalExteriorLights = boost::optional<ExteriorLights>;

  /** \relates ExteriorLights*/
  using ExteriorLightsVector = std::vector<ExteriorLights>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_EXTERIORLIGHTS_HPP
