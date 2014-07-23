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

#ifndef MODEL_OTHEREQUIPMENT_HPP
#define MODEL_OTHEREQUIPMENT_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadInstance.hpp"

namespace openstudio {
namespace model {

class OtherEquipmentDefinition;
class Schedule;

namespace detail {

  class OtherEquipment_Impl;

} // detail

/** OtherEquipment is a SpaceLoadInstance that wraps the OpenStudio IDD object
 *  'OS:OtherEquipment'. \sa OtherEquipmentDefinition */
class MODEL_API OtherEquipment : public SpaceLoadInstance {
 public:

  /** @name Constructors and Destructors */
  //@{

  explicit OtherEquipment(const OtherEquipmentDefinition& definition);

  virtual ~OtherEquipment() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  OtherEquipmentDefinition otherEquipmentDefinition() const;

  /** Returns the (fractional) equipment schedule.  If this object does not
   *  specify a schedule this function will search the hierarchy. */
  boost::optional<Schedule> schedule() const;

  /** Returns true if this object does not specify a schedule directly. */
  bool isScheduleDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  bool setOtherEquipmentDefinition(const OtherEquipmentDefinition& definition);

  /** Sets the (fractional) Schedule. */
  bool setSchedule(Schedule& schedule);

  /** Resets the (fractional) Schedule. */
  void resetSchedule();

  bool setMultiplier(double multiplier);

  void resetMultiplier();

  //@}
  /** @name Other */
  //@{

  /** Returns the design level represented by this instance, assuming floorArea (m^2) and 
   *  numPeople. */
  double getDesignLevel(double floorArea, double numPeople) const;

  /** Returns the watts/m^2 represented by this instance, assuming floorArea (m^2) and 
   *  numPeople. */
  double getPowerPerFloorArea(double floorArea, double numPeople) const;

  /** Returns the watts/person represented by this instance, assuming floorArea (m^2) and 
   *  numPeople. */
  double getPowerPerPerson(double floorArea, double numPeople) const;

  //@}
 protected:
  /// @cond
  typedef detail::OtherEquipment_Impl ImplType;

  friend class detail::OtherEquipment_Impl;
  friend class openstudio::detail::IdfObject_Impl;
  friend class IdfObject;
  friend class Model;

  explicit OtherEquipment(std::shared_ptr<detail::OtherEquipment_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.OtherEquipment");
};

/** \relates OtherEquipment*/
typedef boost::optional<OtherEquipment> OptionalOtherEquipment;

/** \relates OtherEquipment*/
typedef std::vector<OtherEquipment> OtherEquipmentVector;

} // model
} // openstudio

#endif // MODEL_OTHEREQUIPMENT_HPP

