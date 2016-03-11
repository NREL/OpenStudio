/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#ifndef MODEL_ELECTRICLOADCENTERINVERTERSIMPLE_HPP
#define MODEL_ELECTRICLOADCENTERINVERTERSIMPLE_HPP

#include "ModelAPI.hpp"
#include "Inverter.hpp"

namespace openstudio {

namespace model {

class Schedule;
class ThermalZone;

namespace detail {

  class ElectricLoadCenterInverterSimple_Impl;

} // detail

/** ElectricLoadCenterInverterSimple is a Inverter that wraps the OpenStudio IDD object 'OS:ElectricLoadCenter:Inverter:Simple'. */
class MODEL_API ElectricLoadCenterInverterSimple : public Inverter {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit ElectricLoadCenterInverterSimple(const Model& model);

  virtual ~ElectricLoadCenterInverterSimple() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  boost::optional<Schedule> availabilitySchedule() const;

  boost::optional<double> radiativeFraction() const;

  boost::optional<double> inverterEfficiency() const;

  //@}
  /** @name Setters */
  //@{

  bool setAvailabilitySchedule(Schedule& schedule);

  void resetAvailabilitySchedule();

  bool setRadiativeFraction(double radiativeFraction);

  void resetRadiativeFraction();

  bool setInverterEfficiency(double inverterEfficiency);

  void resetInverterEfficiency();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::ElectricLoadCenterInverterSimple_Impl ImplType;

  explicit ElectricLoadCenterInverterSimple(std::shared_ptr<detail::ElectricLoadCenterInverterSimple_Impl> impl);

  friend class detail::ElectricLoadCenterInverterSimple_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ElectricLoadCenterInverterSimple");
};

/** \relates ElectricLoadCenterInverterSimple*/
typedef boost::optional<ElectricLoadCenterInverterSimple> OptionalElectricLoadCenterInverterSimple;

/** \relates ElectricLoadCenterInverterSimple*/
typedef std::vector<ElectricLoadCenterInverterSimple> ElectricLoadCenterInverterSimpleVector;

} // model
} // openstudio

#endif // MODEL_ELECTRICLOADCENTERINVERTERSIMPLE_HPP

