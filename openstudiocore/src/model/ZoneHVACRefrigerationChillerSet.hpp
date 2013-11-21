/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#ifndef MODEL_ZONEHVACREFRIGERATIONCHILLERSET_HPP
#define MODEL_ZONEHVACREFRIGERATIONCHILLERSET_HPP

#include <model/ModelAPI.hpp>
#include <model/ZoneHVACComponent.hpp>

namespace openstudio {
namespace model {

// TODO: Check the following class names against object getters and setters.
class Schedule;
// class RefrigerationAirChiller;
// class ThermalZone;

namespace detail {

  class ZoneHVACRefrigerationChillerSet_Impl;

} // detail

/** ZoneHVACRefrigerationChillerSet is a ZoneHVACComponent that wraps the OpenStudio IDD object 'OS:ZoneHVAC:RefrigerationChillerSet'. */
class MODEL_API ZoneHVACRefrigerationChillerSet : public ZoneHVACComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit ZoneHVACRefrigerationChillerSet(const Model& model);

  virtual ~ZoneHVACRefrigerationChillerSet() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  // TODO: Check return type. From object lists, some candidates are: Schedule.
  boost::optional<Schedule> availabilitySchedule() const;

  // TODO: Check return type. From object lists, some candidates are: ThermalZone.
  // boost::optional<ThermalZone> zone() const;

  // boost::optional<std::string> airInletNodeName() const;

  // boost::optional<std::string> airOutletNodeName() const;

  // TODO: Handle this object's extensible fields.

  // std::vector<RefrigerationAirChiller> airChillers() const;

  //@}
  /** @name Setters */
  //@{

  // TODO: Check argument type. From object lists, some candidates are: Schedule.
  bool setAvailabilitySchedule(Schedule& schedule);

  void resetAvailabilitySchedule();

  // TODO: Check argument type. From object lists, some candidates are: ThermalZone.
  // bool setZone(const ThermalZone& thermalZone);

  // void resetZone();

  // void setAirInletNodeName(std::string airInletNodeName);

  // void resetAirInletNodeName();

  // void setAirOutletNodeName(std::string airOutletNodeName);

  // void resetAirOutletNodeName();

  // TODO: Handle this object's extensible fields.

  // bool addAirChiller( const RefrigerationAirChiller & refrigerationAirChiller);

  // bool addAirChiller( const RefrigerationAirChiller& refrigerationAirChiller, unsigned groupIndex );

  // void removeAirChiller( const RefrigerationAirChiller & refrigerationAirChiller);

  // void removeAirChiller( unsigned groupIndex );

  // void removeAllAirChillers();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::ZoneHVACRefrigerationChillerSet_Impl ImplType;

  explicit ZoneHVACRefrigerationChillerSet(boost::shared_ptr<detail::ZoneHVACRefrigerationChillerSet_Impl> impl);

  friend class detail::ZoneHVACRefrigerationChillerSet_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ZoneHVACRefrigerationChillerSet");
};

/** \relates ZoneHVACRefrigerationChillerSet*/
typedef boost::optional<ZoneHVACRefrigerationChillerSet> OptionalZoneHVACRefrigerationChillerSet;

/** \relates ZoneHVACRefrigerationChillerSet*/
typedef std::vector<ZoneHVACRefrigerationChillerSet> ZoneHVACRefrigerationChillerSetVector;

} // model
} // openstudio

#endif // MODEL_ZONEHVACREFRIGERATIONCHILLERSET_HPP

