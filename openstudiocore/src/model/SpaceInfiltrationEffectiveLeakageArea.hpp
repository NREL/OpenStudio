/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

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

