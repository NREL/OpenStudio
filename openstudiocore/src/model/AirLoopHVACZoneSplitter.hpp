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

#ifndef MODEL_AIRLOOPHVACZONESPLITTER_HPP
#define MODEL_AIRLOOPHVACZONESPLITTER_HPP

#include "ModelAPI.hpp"
#include "Splitter.hpp"

namespace openstudio {

namespace model {

class ThermalZone;

namespace detail {
  class AirLoopHVACZoneSplitter_Impl;
} // detail

/** AirLoopHVACZoneSplitter is an interface to the EnergyPlus IDD object named "AirLoopHVAC:ZoneSplitter"
 *
 *  The purpose of this class is to simplify the construction and manipulation
 *  of zone splitter objects in EnergyPlus.  Methods are built around the
 *  acts of getting the inlet and outlet ports to the splitter.  Branch indexes
 *  are used to refer to the many outlet ports of the mixer
 */
class MODEL_API AirLoopHVACZoneSplitter : public Splitter {
  public:

  /** Constructs a new AirLoopHVACZoneSplitter object and places it inside the model. */
  explicit AirLoopHVACZoneSplitter(const Model& model);

  virtual ~AirLoopHVACZoneSplitter() {}

  /// Returns the IddObjectType.
  static IddObjectType iddObjectType();

  /** Returns the inlet port to the zone splitter. */
  unsigned inletPort();

  /** Returns the outlet port for branchIndex.  Branches are sequentially
   *  indexed starting from 0.
   */
  unsigned outletPort(unsigned branchIndex);

  /** Returns the next available outlet port.  This will be the first port
   *  with no connected objects */
  unsigned nextOutletPort();

  /** Returns a vector of all ZoneHVACEquipmentConnections objects connected
   *  to the splitter's outlets.
   */
  std::vector<ThermalZone> thermalZones();

  std::vector<openstudio::IdfObject> remove();

  void disconnect();

  protected:

  /// @cond

  friend class Model;

  friend class openstudio::IdfObject;

  typedef detail::AirLoopHVACZoneSplitter_Impl ImplType;

  explicit AirLoopHVACZoneSplitter(std::shared_ptr<detail::AirLoopHVACZoneSplitter_Impl> impl);

  private:

  REGISTER_LOGGER("openstudio.model.AirLoopHVACZoneSplitter");

  /// @endcond

};

/** \relates AirLoopHVACZoneSplitter */
typedef boost::optional<AirLoopHVACZoneSplitter> OptionalAirLoopHVACZoneSplitter;

/** \relates AirLoopHVACZoneSplitter */
typedef std::vector<AirLoopHVACZoneSplitter> AirLoopHVACZoneSplitterVector;

} // model
} // openstudio

#endif // MODEL_AIRLOOPHVACZONESPLITTER_HPP
