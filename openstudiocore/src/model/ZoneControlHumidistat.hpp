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

#ifndef MODEL_ZONECONTROLHUMIDISTAT_HPP
#define MODEL_ZONECONTROLHUMIDISTAT_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

class Schedule;

namespace detail {

  class ZoneControlHumidistat_Impl;

} // detail

/** ZoneControlHumidistat is a ModelObject that wraps the OpenStudio IDD object 'OS:ZoneControl:Humidistat'. */
class MODEL_API ZoneControlHumidistat : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit ZoneControlHumidistat(const Model& model);

  virtual ~ZoneControlHumidistat() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  boost::optional<Schedule> humidifyingRelativeHumiditySetpointSchedule() const;

  boost::optional<Schedule> dehumidifyingRelativeHumiditySetpointSchedule() const;

  //@}
  /** @name Setters */
  //@{

  bool setHumidifyingRelativeHumiditySetpointSchedule(Schedule& schedule);

  bool setDehumidifyingRelativeHumiditySetpointSchedule(Schedule& schedule);

  void resetHumidifyingRelativeHumiditySetpointSchedule();

  void resetDehumidifyingRelativeHumiditySetpointSchedule();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::ZoneControlHumidistat_Impl ImplType;

  explicit ZoneControlHumidistat(std::shared_ptr<detail::ZoneControlHumidistat_Impl> impl);

  friend class detail::ZoneControlHumidistat_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ZoneControlHumidistat");
};

/** \relates ZoneControlHumidistat*/
typedef boost::optional<ZoneControlHumidistat> OptionalZoneControlHumidistat;

/** \relates ZoneControlHumidistat*/
typedef std::vector<ZoneControlHumidistat> ZoneControlHumidistatVector;

} // model
} // openstudio

#endif // MODEL_ZONECONTROLHUMIDISTAT_HPP

