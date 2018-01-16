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

#ifndef MODEL_SPACEINFILTRATIONEFFECTIVELEAKAGEAREA_HPP
#define MODEL_SPACEINFILTRATIONEFFECTIVELEAKAGEAREA_HPP

#include "ModelAPI.hpp"
#include "SpaceLoad.hpp"

namespace openstudio {
namespace model {

class Schedule;

namespace detail {

  class SpaceInfiltrationEffectiveLeakageArea_Impl;

} // detail

/** SpaceInfiltrationEffectiveLeakageArea is a SpaceLoad that wraps the OpenStudio
 *  IDD object 'OS:SpaceInfiltration:EffectiveLeakageArea'. */
class MODEL_API SpaceInfiltrationEffectiveLeakageArea : public SpaceLoad {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit SpaceInfiltrationEffectiveLeakageArea(const Model& model);

  virtual ~SpaceInfiltrationEffectiveLeakageArea() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  /** Returns the (fractional) infiltration schedule.  If this object does not
   *  specify a schedule this function will search the hierarchy. */
  boost::optional<Schedule> schedule() const;

  /** Returns true if this object does not specify a schedule directly. */
  bool isScheduleDefaulted() const;

  double effectiveAirLeakageArea() const;

  Quantity getEffectiveAirLeakageArea(bool returnIP=false) const;

  double stackCoefficient() const;

  Quantity getStackCoefficient(bool returnIP=false) const;

  double windCoefficient() const;

  Quantity getWindCoefficient(bool returnIP=false) const;

  //@}
  /** @name Setters */
  //@{

  /** Sets the (fractional) Schedule. */
  bool setSchedule(Schedule& schedule);

  /** Resets the (fractional) Schedule. */
  void resetSchedule();

  bool setEffectiveAirLeakageArea(double effectiveAirLeakageArea);

  bool setEffectiveAirLeakageArea(const Quantity& effectiveAirLeakageArea);

  bool setStackCoefficient(double stackCoefficient);

  bool setStackCoefficient(const Quantity& stackCoefficient);

  bool setWindCoefficient(double windCoefficient);

  bool setWindCoefficient(const Quantity& windCoefficient);

  //@}
 protected:
  /// @cond
  typedef detail::SpaceInfiltrationEffectiveLeakageArea_Impl ImplType;

  explicit SpaceInfiltrationEffectiveLeakageArea(std::shared_ptr<detail::SpaceInfiltrationEffectiveLeakageArea_Impl> impl);

  friend class Model;
  friend class IdfObject;

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.SpaceInfiltrationEffectiveLeakageArea");
};

/** \relates SpaceInfiltrationEffectiveLeakageArea*/
typedef boost::optional<SpaceInfiltrationEffectiveLeakageArea> OptionalSpaceInfiltrationEffectiveLeakageArea;

/** \relates SpaceInfiltrationEffectiveLeakageArea*/
typedef std::vector<SpaceInfiltrationEffectiveLeakageArea> SpaceInfiltrationEffectiveLeakageAreaVector;

} // model
} // openstudio

#endif // MODEL_SPACEINFILTRATIONEFFECTIVELEAKAGEAREA_HPP

