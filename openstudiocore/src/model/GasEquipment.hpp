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

#ifndef MODEL_GASEQUIPMENT_HPP
#define MODEL_GASEQUIPMENT_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadInstance.hpp"

namespace openstudio {
namespace model {

class Schedule;
class GasEquipmentDefinition;

namespace detail {

  class GasEquipment_Impl;

} // detail

/** GasEquipment is a SpaceLoadInstance that wraps the OpenStudio IDD object
 *  'OS:GasEquipment'. \sa GasEquipmentDefinition */
class MODEL_API GasEquipment : public SpaceLoadInstance {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit GasEquipment(const GasEquipmentDefinition& gasEquipmentDefinition);

  virtual ~GasEquipment() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  /** Gets the GasEquipmentDefinition object. */
  GasEquipmentDefinition gasEquipmentDefinition() const;

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

  /** Sets the GasEquipmentDefinition object. */
  bool setGasEquipmentDefinition(const GasEquipmentDefinition& definition);

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
   *  gasEquipmentDefinition(). */
  boost::optional<double> designLevel() const;

  /** Returns the powerPerFloorArea if possible based on the underlying data of 
   *  gasEquipmentDefinition(). */
  boost::optional<double> powerPerFloorArea() const;

  /** Returns the powerPerPerson if possible based on the underlying data of 
   *  gasEquipmentDefinition(). */
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
  typedef detail::GasEquipment_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;

  explicit GasEquipment(std::shared_ptr<detail::GasEquipment_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.GasEquipment");
};

/** \relates GasEquipment*/
typedef boost::optional<GasEquipment> OptionalGasEquipment;

/** \relates GasEquipment*/
typedef std::vector<GasEquipment> GasEquipmentVector;

} // model
} // openstudio

#endif // MODEL_GASEQUIPMENT_HPP

