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

#ifndef MODEL_SCHEDULETYPELIMITS_HPP
#define MODEL_SCHEDULETYPELIMITS_HPP

#include "ModelAPI.hpp"
#include "ResourceObject.hpp"

namespace openstudio {
class Unit;

namespace model {

  namespace detail {

    class ScheduleTypeLimits_Impl;

  }  // namespace detail

  /** ScheduleTypeLimits is a ResourceObject that wraps the OpenStudio IDD object 'OS:ScheduleTypeLimits'.
 *  Once attached to an object of type ScheduleBase, it defines the units and bounds for that object's
 *  values. */
  class MODEL_API ScheduleTypeLimits : public ResourceObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ScheduleTypeLimits(const Model& model);

    virtual ~ScheduleTypeLimits() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ScheduleTypeLimits(const ScheduleTypeLimits& other) = default;
    ScheduleTypeLimits(ScheduleTypeLimits&& other) = default;
    ScheduleTypeLimits& operator=(const ScheduleTypeLimits&) = default;
    ScheduleTypeLimits& operator=(ScheduleTypeLimits&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> numericTypeValues();

    static std::vector<std::string> unitTypeValues();

    // std::string by val on purpose (boost::to_lower)
    static boost::optional<Unit> units(std::string unitType, bool returnIP = false);

    /** @name Getters */
    //@{

    boost::optional<double> lowerLimitValue() const;

    boost::optional<double> upperLimitValue() const;

    boost::optional<std::string> numericType() const;

    std::string unitType() const;

    bool isUnitTypeDefaulted() const;

    boost::optional<Unit> units(bool returnIP = false) const;

    //@}
    /** @name Setters */
    //@{

    // ETH@20120919 TODO: Have these setters fail if they are going to invalidate the current
    // use of a ScheduleBase object.

    bool setLowerLimitValue(double lowerLimitValue);

    void resetLowerLimitValue();

    bool setUpperLimitValue(double upperLimitValue);

    void resetUpperLimitValue();

    bool setNumericType(const std::string& numericType);

    void resetNumericType();

    bool setUnitType(const std::string& unitType);

    void resetUnitType();

    //@}
   protected:
    /// @cond
    using ImplType = detail::ScheduleTypeLimits_Impl;

    explicit ScheduleTypeLimits(std::shared_ptr<detail::ScheduleTypeLimits_Impl> impl);

    friend class detail::ScheduleTypeLimits_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ScheduleTypeLimits");
  };

  /** \relates ScheduleTypeLimits */
  using OptionalScheduleTypeLimits = boost::optional<ScheduleTypeLimits>;

  /** \relates ScheduleTypeLimits */
  using ScheduleTypeLimitsVector = std::vector<ScheduleTypeLimits>;

  /** Returns true if candidate is compatible with parentLimits, that is, if it is the same
 *  type and has bounds that are as least as strict as parentLimits's.
 *  \relates ScheduleTypeLimits */
  MODEL_API bool isCompatible(const ScheduleTypeLimits& parentLimits, const ScheduleTypeLimits& candidate);

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SCHEDULETYPELIMITS_HPP
