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

#ifndef MODEL_SCHEDULECOMPACT_HPP
#define MODEL_SCHEDULECOMPACT_HPP

#include "ModelAPI.hpp"
#include "Schedule.hpp"

namespace openstudio {
namespace model {

  namespace detail {
    class ScheduleCompact_Impl;
  }  // namespace detail

  /** ScheduleCompact is a Schedule that wraps the OpenStudio IDD object 'OS:Schedule:Compact'. */
  class MODEL_API ScheduleCompact : public Schedule
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Creates an empty (no values set for any days) ScheduleCompact. */
    explicit ScheduleCompact(const Model& model);

    /** Creates a ScheduleCompact with constantValue applied to the entire year. */
    ScheduleCompact(const Model& model, double constantValue);

    /** Creates a ScheduleCompact with constantValue applied to the entire year, if
   *  scheduleTypeLimits() and the units are compatible. Otherwise creates an empty
   *  ScheduleCompact. */
    virtual ~ScheduleCompact() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ScheduleCompact(const ScheduleCompact& other) = default;
    ScheduleCompact(ScheduleCompact&& other) = default;
    ScheduleCompact& operator=(const ScheduleCompact&) = default;
    ScheduleCompact& operator=(ScheduleCompact&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Setters */
    //@{

    /** Clears all existing data and sets the data so that value will be applied to the entire
   *  year. */
    bool setToConstantValue(double value);

    /** If value can be converted to scheduleTypeLimit()'s units, then all existing data is cleared
   *  and value is set (after unit conversion, if necessary). */

    //@}
    /** @name Queries */
    //@{

    /** Returns true if this ScheduleCompact has a single value applied to the entire year.
   *  (Roughly--the text fields are not checked.) */
    bool isConstantValue() const;

    /** Returns this ScheduleCompact's constant value, if this ScheduleCompact conforms to that
   *  pattern. Otherwise returns boost::none (evaluates to false). */
    boost::optional<double> constantValue() const;

    //@}
   protected:
    using ImplType = detail::ScheduleCompact_Impl;

    friend class Model;
    friend class openstudio::IdfObject;

    // constructor
    explicit ScheduleCompact(std::shared_ptr<detail::ScheduleCompact_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.ScheduleCompact");
  };

  /** \relates ScheduleCompact */
  using OptionalScheduleCompact = boost::optional<ScheduleCompact>;

  /** \relates ScheduleCompact */
  using ScheduleCompactVector = std::vector<ScheduleCompact>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SCHEDULECOMPACT_HPP
