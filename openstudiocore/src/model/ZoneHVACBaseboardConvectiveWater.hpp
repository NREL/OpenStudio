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

#ifndef MODEL_ZONEHVACBASEBOARDCONVECTIVEWATER_HPP
#define MODEL_ZONEHVACBASEBOARDCONVECTIVEWATER_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent.hpp"
#include "ThermalZone.hpp"
#include "Model.hpp"

namespace openstudio {
namespace model {

class Schedule;
class StraightComponent;

namespace detail {

  class ZoneHVACBaseboardConvectiveWater_Impl;

} // detail

/** ZoneHVACBaseboardConvectiveWater is a ZoneHVACComponent that wraps the OpenStudio IDD object 'OS:ZoneHVAC:Baseboard:Convective:Water'. */
class MODEL_API ZoneHVACBaseboardConvectiveWater : public ZoneHVACComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  ZoneHVACBaseboardConvectiveWater(const Model& model,
                                            Schedule & availabilitySchedule,
                                            StraightComponent & heatingCoilBaseboard  );

  virtual ~ZoneHVACBaseboardConvectiveWater() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  Schedule availabilitySchedule() const;

  StraightComponent heatingCoil() const;


  //@}
  /** @name Setters */
  //@{

  bool setAvailabilitySchedule(Schedule& schedule);

  bool setHeatingCoil(const StraightComponent& heatingCoilBaseboard);
  
   /** Returns the optional ThermalZone that this ZoneHVACBaseboardConvectiveWater is attached to
   **/
  boost::optional<ThermalZone> thermalZone();

  /** Adds this ZoneHVACBaseboardConvectiveWater to the thermal zone.  Returns true if the operation was
   *  successful.
   **/
  bool addToThermalZone(ThermalZone & thermalZone);

  /** Detaches this ZoneHVACBaseboardConvectiveWater from the associated ThermalZone.
   *  If there is no attached ThermalZone there is no effect.
   **/
  void removeFromThermalZone();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::ZoneHVACBaseboardConvectiveWater_Impl ImplType;

  explicit ZoneHVACBaseboardConvectiveWater(std::shared_ptr<detail::ZoneHVACBaseboardConvectiveWater_Impl> impl);

  friend class detail::ZoneHVACBaseboardConvectiveWater_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ZoneHVACBaseboardConvectiveWater");
};

/** \relates ZoneHVACBaseboardConvectiveWater*/
typedef boost::optional<ZoneHVACBaseboardConvectiveWater> OptionalZoneHVACBaseboardConvectiveWater;

/** \relates ZoneHVACBaseboardConvectiveWater*/
typedef std::vector<ZoneHVACBaseboardConvectiveWater> ZoneHVACBaseboardConvectiveWaterVector;

} // model
} // openstudio

#endif // MODEL_ZONEHVACBASEBOARDCONVECTIVEWATER_HPP

