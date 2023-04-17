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

#ifndef MODEL_CONSTRUCTIONAIRBOUNDARY_HPP
#define MODEL_CONSTRUCTIONAIRBOUNDARY_HPP

#include <model/ModelAPI.hpp>
#include "ConstructionBase.hpp"
#include "../utilities/core/Deprecated.hpp"

namespace openstudio {

namespace model {

  // TODO: Check the following class names against object getters and setters.
  class Schedule;

  namespace detail {

    class ConstructionAirBoundary_Impl;

  }  // namespace detail

  /** ConstructionAirBoundary is a ConstructionBase that wraps the OpenStudio IDD object 'OS:Construction:AirBoundary'. */
  class MODEL_API ConstructionAirBoundary : public ConstructionBase
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ConstructionAirBoundary(const Model& model);

    virtual ~ConstructionAirBoundary() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ConstructionAirBoundary(const ConstructionAirBoundary& other) = default;
    ConstructionAirBoundary(ConstructionAirBoundary&& other) = default;
    ConstructionAirBoundary& operator=(const ConstructionAirBoundary&) = default;
    ConstructionAirBoundary& operator=(ConstructionAirBoundary&&) = default;

    //@}

    static IddObjectType iddObjectType();

    OS_DEPRECATED static std::vector<std::string> solarAndDaylightingMethodValues();

    OS_DEPRECATED static std::vector<std::string> radiantExchangeMethodValues();

    static std::vector<std::string> airExchangeMethodValues();

    /** @name Getters */
    //@{

    // cppcheck-suppress functionStatic
    OS_DEPRECATED std::string solarAndDaylightingMethod() const;

    // cppcheck-suppress functionStatic
    OS_DEPRECATED bool isSolarAndDaylightingMethodDefaulted() const;

    // cppcheck-suppress functionStatic
    OS_DEPRECATED std::string radiantExchangeMethod() const;

    // cppcheck-suppress functionStatic
    OS_DEPRECATED bool isRadiantExchangeMethodDefaulted() const;

    std::string airExchangeMethod() const;

    bool isAirExchangeMethodDefaulted() const;

    double simpleMixingAirChangesPerHour() const;

    bool isSimpleMixingAirChangesPerHourDefaulted() const;

    boost::optional<Schedule> simpleMixingSchedule() const;

    //@}
    /** @name Setters */
    //@{

    // cppcheck-suppress functionStatic
    OS_DEPRECATED bool setSolarAndDaylightingMethod(const std::string& solarAndDaylightingMethod);

    // cppcheck-suppress functionStatic
    OS_DEPRECATED void resetSolarAndDaylightingMethod();

    // cppcheck-suppress functionStatic
    OS_DEPRECATED bool setRadiantExchangeMethod(const std::string& radiantExchangeMethod);

    // cppcheck-suppress functionStatic
    OS_DEPRECATED void resetRadiantExchangeMethod();

    bool setAirExchangeMethod(const std::string& airExchangeMethod);

    void resetAirExchangeMethod();

    bool setSimpleMixingAirChangesPerHour(double simpleMixingAirChangesPerHour);

    void resetSimpleMixingAirChangesPerHour();

    bool setSimpleMixingSchedule(Schedule& schedule);

    void resetSimpleMixingSchedule();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::ConstructionAirBoundary_Impl;

    explicit ConstructionAirBoundary(std::shared_ptr<detail::ConstructionAirBoundary_Impl> impl);

    friend class detail::ConstructionAirBoundary_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ConstructionAirBoundary");
  };

  /** \relates ConstructionAirBoundary*/
  using OptionalConstructionAirBoundary = boost::optional<ConstructionAirBoundary>;

  /** \relates ConstructionAirBoundary*/
  using ConstructionAirBoundaryVector = std::vector<ConstructionAirBoundary>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CONSTRUCTIONAIRBOUNDARY_HPP
