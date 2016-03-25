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

#ifndef MODEL_ZONECONTROLCONTAMINANTCONTROLLER_HPP
#define MODEL_ZONECONTROLCONTAMINANTCONTROLLER_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

class ThermalZone;
class Schedule;

namespace detail {

  class ZoneControlContaminantController_Impl;

} // detail

/** ZoneControlContaminantController is a ModelObject that wraps the OpenStudio IDD object 'OS:ZoneControl:ContaminantController'. */
class MODEL_API ZoneControlContaminantController : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit ZoneControlContaminantController(const Model& model);

  virtual ~ZoneControlContaminantController() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  boost::optional<ThermalZone> controlledZone() const;

  boost::optional<Schedule> carbonDioxideControlAvailabilitySchedule() const;

  boost::optional<Schedule> carbonDioxideSetpointSchedule() const;

  boost::optional<Schedule> minimumCarbonDioxideConcentrationSchedule() const;

  boost::optional<Schedule> genericContaminantControlAvailabilitySchedule() const;

  boost::optional<Schedule> genericContaminantSetpointSchedule() const;

  //@}
  /** @name Setters */
  //@{

  bool setCarbonDioxideControlAvailabilitySchedule(Schedule& schedule);

  void resetCarbonDioxideControlAvailabilitySchedule();

  bool setCarbonDioxideSetpointSchedule(Schedule& schedule);

  void resetCarbonDioxideSetpointSchedule();

  bool setMinimumCarbonDioxideConcentrationSchedule(Schedule& schedule);

  void resetMinimumCarbonDioxideConcentrationSchedule();

  bool setGenericContaminantControlAvailabilitySchedule(Schedule& schedule);

  void resetGenericContaminantControlAvailabilitySchedule();

  bool setGenericContaminantSetpointSchedule(Schedule& schedule);

  void resetGenericContaminantSetpointSchedule();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::ZoneControlContaminantController_Impl ImplType;

  explicit ZoneControlContaminantController(std::shared_ptr<detail::ZoneControlContaminantController_Impl> impl);

  friend class detail::ZoneControlContaminantController_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ZoneControlContaminantController");
};

/** \relates ZoneControlContaminantController*/
typedef boost::optional<ZoneControlContaminantController> OptionalZoneControlContaminantController;

/** \relates ZoneControlContaminantController*/
typedef std::vector<ZoneControlContaminantController> ZoneControlContaminantControllerVector;

} // model
} // openstudio

#endif // MODEL_ZONECONTROLCONTAMINANTCONTROLLER_HPP

