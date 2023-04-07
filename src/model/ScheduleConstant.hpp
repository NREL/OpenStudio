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

#ifndef MODEL_SCHEDULECONSTANT_HPP
#define MODEL_SCHEDULECONSTANT_HPP

#include "ModelAPI.hpp"
#include "Schedule.hpp"

namespace openstudio {
namespace model {

  namespace detail {
    class ScheduleConstant_Impl;
  }  // namespace detail

  /** ScheduleConstant is a Schedule that wraps the OpenStudio IDD object 'OS:Schedule:Constant'. */
  class MODEL_API ScheduleConstant : public Schedule
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Creates new ScheduleConstant in model and sets its value to 0.0. */
    explicit ScheduleConstant(const Model& model);

    virtual ~ScheduleConstant() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ScheduleConstant(const ScheduleConstant& other) = default;
    ScheduleConstant(ScheduleConstant&& other) = default;
    ScheduleConstant& operator=(const ScheduleConstant&) = default;
    ScheduleConstant& operator=(ScheduleConstant&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    double value() const;

    //@}
    /** @name Setters */
    //@{

    bool setValue(double value);

    /** Sets this ScheduleConstant's value if scheduleTypeLimits() is set and value can be properly
   *  converted to the 'SI' units so implied. */

    //@}
   protected:
    using ImplType = detail::ScheduleConstant_Impl;

    friend class Model;
    friend class openstudio::IdfObject;

    // constructor
    explicit ScheduleConstant(std::shared_ptr<detail::ScheduleConstant_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.ScheduleConstant");
  };

  /** \relates ScheduleConstant */
  using OptionalScheduleConstant = boost::optional<ScheduleConstant>;

  /** \relates ScheduleConstant */
  using ScheduleConstantVector = std::vector<ScheduleConstant>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SCHEDULECONSTANT_HPP
