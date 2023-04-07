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

#ifndef MODEL_SCHEDULEBASE_HPP
#define MODEL_SCHEDULEBASE_HPP

#include "ModelAPI.hpp"
#include "ResourceObject.hpp"

namespace openstudio {
namespace model {

  class ScheduleTypeLimits;

  namespace detail {

    class ScheduleBase_Impl;

  }  // namespace detail

  /** ScheduleBase is a ResourceObject that serves as a base class for Schedule and ScheduleDay,
 *  that is, objects with ScheduleTypeLimits. This class provides getters and setters for
 *  ScheduleTypeLimits. Note that while users can explicitly set their own ScheduleTypeLimits,
 *  we recommend using the \link ScheduleTypeRegistrySingleton ScheduleTypeRegistry\endlink and
 *  related non-member functions, or letting user \link ModelObject ModelObjects\endlink set
 *  this field, instead. */
  class MODEL_API ScheduleBase : public ResourceObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~ScheduleBase() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ScheduleBase(const ScheduleBase& other) = default;
    ScheduleBase(ScheduleBase&& other) = default;
    ScheduleBase& operator=(const ScheduleBase&) = default;
    ScheduleBase& operator=(ScheduleBase&&) = default;

    //@}
    /** @name Getters */
    //@{

    /** Returns the ScheduleTypeLimits of this object, if set. */
    boost::optional<ScheduleTypeLimits> scheduleTypeLimits() const;

    //@}
    /** @name Setters */
    //@{

    /** Returns true if scheduleTypeLimits is compatible with this object and is actually set.
   *  The operation will fail if the Schedule or ScheduleDay is in use in a way that restricts
   *  the allowable ScheduleTypeLimits and the new value (scheduleTypeLimits) is incompatible
   *  with that use. For instance, if a Schedule is used with a Lights object, then only
   *  properly configured fractional ScheduleTypeLimits will succeed. ScheduleDay objects used
   *  in a ScheduleRuleset or a ScheduleRule are restricted to use ScheduleTypeLimits compatible
   *  with those of their (ScheduleRuleset or ScheduleRule) parent. */
    bool setScheduleTypeLimits(const ScheduleTypeLimits& scheduleTypeLimits);

    /** Returns true if the scheduleTypeLimits() of this object is successfully cleared. Will fail
   *  if this object is being used by an object that expects it to have a ScheduleTypeLimits. */
    bool resetScheduleTypeLimits();

    // ensure that this object does not contain the date 2/29
    void ensureNoLeapDays();

    //@}
   protected:
    /// @cond
    using ImplType = detail::ScheduleBase_Impl;

    explicit ScheduleBase(std::shared_ptr<detail::ScheduleBase_Impl> impl);

    ScheduleBase(IddObjectType type, const Model& model);

    friend class openstudio::IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    friend class detail::ScheduleBase_Impl;
    friend class Model;

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ScheduleBase");
  };

  /** \relates ScheduleBase*/
  using OptionalScheduleBase = boost::optional<ScheduleBase>;

  /** \relates ScheduleBase*/
  using ScheduleBaseVector = std::vector<ScheduleBase>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SCHEDULEBASE_HPP
