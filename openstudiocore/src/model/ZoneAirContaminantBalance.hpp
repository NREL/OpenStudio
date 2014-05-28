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

#ifndef MODEL_ZONEAIRCONTAMINANTBALANCE_HPP
#define MODEL_ZONEAIRCONTAMINANTBALANCE_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

// TODO: Check the following class names against object getters and setters.
class Schedule;

namespace detail {

  class ZoneAirContaminantBalance_Impl;

} // detail

/** ZoneAirContaminantBalance is a ModelObject that wraps the OpenStudio IDD object 'OS:ZoneAirContaminantBalance'. */
class MODEL_API ZoneAirContaminantBalance : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~ZoneAirContaminantBalance() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  bool carbonDioxideConcentration() const;

  bool isCarbonDioxideConcentrationDefaulted() const;

  // TODO: Check return type. From object lists, some candidates are: Schedule.
  boost::optional<Schedule> outdoorCarbonDioxideSchedule() const;

  //@}
  /** @name Setters */
  //@{

  void setCarbonDioxideConcentration(bool carbonDioxideConcentration);

  void resetCarbonDioxideConcentration();

  // TODO: Check argument type. From object lists, some candidates are: Schedule.
  bool setOutdoorCarbonDioxideSchedule(Schedule& schedule);

  void resetOutdoorCarbonDioxideSchedule();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::ZoneAirContaminantBalance_Impl ImplType;

  explicit ZoneAirContaminantBalance(std::shared_ptr<detail::ZoneAirContaminantBalance_Impl> impl);

  friend class detail::ZoneAirContaminantBalance_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  explicit ZoneAirContaminantBalance(Model& model);

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ZoneAirContaminantBalance");
};

/** \relates ZoneAirContaminantBalance*/
typedef boost::optional<ZoneAirContaminantBalance> OptionalZoneAirContaminantBalance;

/** \relates ZoneAirContaminantBalance*/
typedef std::vector<ZoneAirContaminantBalance> ZoneAirContaminantBalanceVector;

} // model
} // openstudio

#endif // MODEL_ZONEAIRCONTAMINANTBALANCE_HPP

