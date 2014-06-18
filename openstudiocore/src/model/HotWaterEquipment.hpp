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

#ifndef MODEL_HOTWATEREQUIPMENT_HPP
#define MODEL_HOTWATEREQUIPMENT_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadInstance.hpp"

namespace openstudio {
namespace model {

class Schedule;
class HotWaterEquipmentDefinition;

namespace detail {

  class HotWaterEquipment_Impl;

} // detail

/** HotWaterEquipment is a SpaceLoadInstance that wraps the OpenStudio IDD object 
 *  'OS:HotWaterEquipment'. Its fields are derived from the EnergyPlus IDD object
 *  'HotWaterEquipment'. \sa HotWaterEquipmentDefinition
 */
class MODEL_API HotWaterEquipment : public SpaceLoadInstance {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit HotWaterEquipment(const HotWaterEquipmentDefinition& hotWaterEquipmentDefinition);

  virtual ~HotWaterEquipment() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  /** Gets the HotWaterEquipmentDefinition object. */
  HotWaterEquipmentDefinition hotWaterEquipmentDefinition() const;

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

  /** Sets the HotWaterEquipmentDefinition object. */
  bool setHotWaterEquipmentDefinition(const HotWaterEquipmentDefinition& definition);

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
  typedef detail::HotWaterEquipment_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;

  explicit HotWaterEquipment(std::shared_ptr<detail::HotWaterEquipment_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.HotWaterEquipment");
};

/** \relates HotWaterEquipment*/
typedef boost::optional<HotWaterEquipment> OptionalHotWaterEquipment;

/** \relates HotWaterEquipment*/
typedef std::vector<HotWaterEquipment> HotWaterEquipmentVector;

} // model
} // openstudio

#endif // MODEL_HOTWATEREQUIPMENT_HPP

