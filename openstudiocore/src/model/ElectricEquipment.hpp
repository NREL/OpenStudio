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

#ifndef MODEL_ELECTRICEQUIPMENT_HPP
#define MODEL_ELECTRICEQUIPMENT_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadInstance.hpp"

namespace openstudio {
namespace model {

class Schedule;
class ElectricEquipmentDefinition;

namespace detail {

  class ElectricEquipment_Impl;

} // detail

/** ElectricEquipment is a SpaceLoadInstance that wraps the OpenStudio IDD object 
 *  'OS:ElectricEquipment'. Its fields are derived from the EnergyPlus IDD object
 *  'ElectricEquipment'. \sa ElectricEquipmentDefinition */
class MODEL_API ElectricEquipment : public SpaceLoadInstance {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit ElectricEquipment(const ElectricEquipmentDefinition& electricEquipmentDefinition);

  virtual ~ElectricEquipment() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  /** Gets the ElectricEquipmentDefinition object. */
  ElectricEquipmentDefinition electricEquipmentDefinition() const;

  /** Returns the (fractional) equipment schedule.  If this object does not
   *  specify a schedule this function will search the hierarchy. */
  boost::optional<Schedule> schedule() const;

  /** Returns true if this object does not specify a schedule directly. */
  bool isScheduleDefaulted() const;

  std::string endUseSubcategory() const;

  bool isEndUseSubcategoryDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  /** Sets the ElectricEquipmentDefinition object. */
  bool setElectricEquipmentDefinition(const ElectricEquipmentDefinition& definition);

  /** Sets the (fractional) Schedule. */
  bool setSchedule(Schedule& schedule);

  /** Resets the (fractional) Schedule. */
  void resetSchedule();

  bool setMultiplier(double multiplier);

  void resetMultiplier();

  void setEndUseSubcategory(std::string endUseSubcategory);

  void resetEndUseSubcategory();

  //@}
  /** @name Other */
  //@{

  /** Returns the designLevel if possible based on the underlying data of 
   *  electricEquipmentDefinition(). */
  boost::optional<double> designLevel() const;

  /** Returns the powerPerFloorArea if possible based on the underlying data of 
   *  electricEquipmentDefinition(). */
  boost::optional<double> powerPerFloorArea() const;

  /** Returns the powerPerPerson if possible based on the underlying data of 
   *  electricEquipmentDefinition(). */
  boost::optional<double> powerPerPerson() const;

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
  typedef detail::ElectricEquipment_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;

  explicit ElectricEquipment(std::shared_ptr<detail::ElectricEquipment_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.ElectricEquipment");
};

/** \relates ElectricEquipment*/
typedef boost::optional<ElectricEquipment> OptionalElectricEquipment;

/** \relates ElectricEquipment*/
typedef std::vector<ElectricEquipment> ElectricEquipmentVector;

} // model
} // openstudio

#endif // MODEL_ELECTRICEQUIPMENT_HPP

