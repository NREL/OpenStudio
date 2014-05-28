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

#ifndef MODEL_LIGHTS_HPP
#define MODEL_LIGHTS_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadInstance.hpp"

namespace openstudio {
namespace model {

class Schedule;
class LightsDefinition;

namespace detail {

  class Lights_Impl;

} // detail

/** Lights is a SpaceLoadInstance that wraps the OpenStudio IDD object 'OS:Lights'.
 *  \sa LightsDefinition */
class MODEL_API Lights : public SpaceLoadInstance {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit Lights(const LightsDefinition& lightsDefinition);

  virtual ~Lights() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  /** Gets the LightsDefinition object. */
  LightsDefinition lightsDefinition() const;

  /** Returns the (fractional) lighting schedule.  If this object does not
   *  specify a schedule this function will search the hierarchy. */
  boost::optional<Schedule> schedule() const;

  /** Returns true if this object does not specify a schedule directly. */
  bool isScheduleDefaulted() const;

  double fractionReplaceable() const;

  bool isFractionReplaceableDefaulted() const;

  std::string endUseSubcategory() const;

  bool isEndUseSubcategoryDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  /** Sets the LightsDefinition object. */
  bool setLightsDefinition(const LightsDefinition& definition);

  /** Sets the (fractional) Schedule. */
  bool setSchedule(Schedule& schedule);

  /** Resets the (fractional) Schedule. */
  void resetSchedule();

  bool setFractionReplaceable(double fractionReplaceable);

  void resetFractionReplaceable();

  bool setMultiplier(double multiplier);

  void resetMultiplier();

  void setEndUseSubcategory(std::string endUseSubcategory);

  void resetEndUseSubcategory();

  //@}
  /** @name Other */
  //@{

  /** Returns the lightingLevel if possible based on the underlying data of 
   *  lightsDefinition(). */
  boost::optional<double> lightingLevel() const;

  /** Returns the powerPerFloorArea if possible based on the underlying data of 
   *  lightsDefinition(). */
  boost::optional<double> powerPerFloorArea() const;

  /** Returns the powerPerPerson if possible based on the underlying data of 
   *  lightsDefinition(). */
  boost::optional<double> powerPerPerson() const;

  /** Returns the lighting level represented by this instance, assuming floorArea (m^2) and 
   *  numPeople. */
  double getLightingPower(double floorArea, double numPeople) const;

  /** Returns the watts/m^2 represented by this instance, assuming floorArea (m^2) and 
   *  numPeople. */
  double getPowerPerFloorArea(double floorArea, double numPeople) const;

  /** Returns the watts/person represented by this instance, assuming floorArea (m^2) and 
   *  numPeople. */
  double getPowerPerPerson(double floorArea, double numPeople) const;

  //@}
 protected:
  /// @cond
  typedef detail::Lights_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;

  explicit Lights(std::shared_ptr<detail::Lights_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.Lights");
};

/** \relates Lights*/
typedef boost::optional<Lights> OptionalLights;

/** \relates Lights*/
typedef std::vector<Lights> LightsVector;

} // model
} // openstudio

#endif // MODEL_LIGHTS_HPP

