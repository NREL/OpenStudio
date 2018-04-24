/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef MODEL_SCHEDULEFIXEDINTERVAL_HPP
#define MODEL_SCHEDULEFIXEDINTERVAL_HPP

#include "ModelAPI.hpp"
#include "ScheduleInterval.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class ScheduleFixedInterval_Impl;

} // detail

/** ScheduleFixedInterval is a ScheduleInterval that wraps the OpenStudio IDD object
 *  'OS_Schedule_FixedInterval'. */
class MODEL_API ScheduleFixedInterval : public ScheduleInterval {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit ScheduleFixedInterval(const Model& model);

  virtual ~ScheduleFixedInterval() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  bool interpolatetoTimestep() const;

  bool isInterpolatetoTimestepDefaulted() const;

  double intervalLength() const;

  double outOfRangeValue() const;

  bool isOutOfRangeValueDefaulted() const;

  int startMonth() const;

  int startDay() const;

  //@}
  /** @name Setters */
  //@{

  bool setInterpolatetoTimestep(bool interpolatetoTimestep);

  void resetInterpolatetoTimestep();

  bool setIntervalLength(double intervalLength);

  bool setOutOfRangeValue(double outOfRangeValue);

  void resetOutOfRangeValue();

  bool setStartMonth(int startMonth);

  bool setStartDay(int startDay);

  //@}
 protected:
  /// @cond
  typedef detail::ScheduleFixedInterval_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;
  friend class detail::ScheduleFixedInterval_Impl;

  explicit ScheduleFixedInterval(std::shared_ptr<detail::ScheduleFixedInterval_Impl> impl);

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ScheduleFixedInterval");
};

/** \relates ScheduleFixedInterval*/
typedef boost::optional<ScheduleFixedInterval> OptionalScheduleFixedInterval;

/** \relates ScheduleFixedInterval*/
typedef std::vector<ScheduleFixedInterval> ScheduleFixedIntervalVector;

} // model
} // openstudio

#endif // MODEL_SCHEDULEFIXEDINTERVAL_HPP
