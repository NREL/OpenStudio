/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef MODEL_SCHEDULEINTERVAL_HPP
#define MODEL_SCHEDULEINTERVAL_HPP

#include "ModelAPI.hpp"
#include "Schedule.hpp"

namespace openstudio {

class TimeSeries;

namespace model {

namespace detail {
  class ScheduleInterval_Impl;
} // detail

// ETH@20120919 Perhaps all Schedules should be able to return their data as TimeSeries (and TimeSeries should be
// convertible to and/or implemented using OSQuantityVector).

/** ScheduleInterval is a Schedule that serves as an abstract base class for ScheduleFixedInterval
 *  and ScheduleVariableInterval. ScheduleIntervals may be constructed from TimeSeries, and can return
 *  their values as TimeSeries. */
class MODEL_API ScheduleInterval : public Schedule {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~ScheduleInterval() {}

  //@}

  static boost::optional<ScheduleInterval> fromTimeSeries(const openstudio::TimeSeries& timeSeries, Model& model);

  /** @name Getters */
  //@{

  openstudio::TimeSeries timeSeries() const;

  //@}
  /** @name Setters */
  //@{

  bool setTimeSeries(const openstudio::TimeSeries& timeSeries);

  //@}
 protected:

  typedef detail::ScheduleInterval_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;

  ScheduleInterval(IddObjectType type,const Model& model);

  // constructor
  explicit ScheduleInterval(std::shared_ptr<detail::ScheduleInterval_Impl> impl);

 private:

  REGISTER_LOGGER("openstudio.model.ScheduleInterval");
};

/** \relates ScheduleInterval */
typedef boost::optional<ScheduleInterval> OptionalScheduleInterval;

/** \relates ScheduleInterval */
typedef std::vector<ScheduleInterval> ScheduleIntervalVector;

} // model
} // openstudio

#endif // MODEL_SCHEDULEINTERVAL_HPP

