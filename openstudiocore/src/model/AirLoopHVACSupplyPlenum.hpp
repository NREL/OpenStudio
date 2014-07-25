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

#ifndef MODEL_AIRLOOPHVACSUPPLYPLENUM_HPP
#define MODEL_AIRLOOPHVACSUPPLYPLENUM_HPP

#include "ModelAPI.hpp"
#include "Splitter.hpp"

namespace openstudio {
namespace model {

class ThermalZone;

namespace detail {

  class AirLoopHVACSupplyPlenum_Impl;

} // detail

/** AirLoopHVACSupplyPlenum is a Splitter that wraps the OpenStudio IDD object 'OS:AirLoopHVAC:SupplyPlenum'. */
class MODEL_API AirLoopHVACSupplyPlenum : public Splitter {
 public:

  explicit AirLoopHVACSupplyPlenum(const Model& model);

  virtual ~AirLoopHVACSupplyPlenum() {}

  static IddObjectType iddObjectType();

  boost::optional<ThermalZone> thermalZone() const;

  /** Establish thermalZone as the plenum zone.
   *  If thermalZone is already conditioned by ThermalZone::equipment(),
   *  then this method will return false.  A ThermalZone cannot be conditioned 
   *  by equipment and and be a plenum zone.  Remove any existing ZoneHVACComponent objects,
   *  and associated AirLoopHVAC objects before using this method on thermalZone.
  **/
  bool setThermalZone(const ThermalZone& thermalZone);

  void resetThermalZone();

  unsigned inletPort();

  unsigned outletPort(unsigned branchIndex);

  unsigned nextOutletPort();

  /** Insert this AirLoopHVACSupplyPlenum into HVAC system at node.
   *  Node must be immediatly after AirLoopHVACZoneSplitter and there
   *  cannot be an existing AirLoopHVACSupplyPlenum on the node's branch
   */
  bool addToNode(Node & node);

  bool addBranchForZone(openstudio::model::ThermalZone & thermalZone);

  bool addBranchForZone(openstudio::model::ThermalZone & thermalZone, StraightComponent & terminal);

 protected:
  /// @cond
  typedef detail::AirLoopHVACSupplyPlenum_Impl ImplType;

  explicit AirLoopHVACSupplyPlenum(std::shared_ptr<detail::AirLoopHVACSupplyPlenum_Impl> impl);

  friend class detail::AirLoopHVACSupplyPlenum_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.AirLoopHVACSupplyPlenum");
};

/** \relates AirLoopHVACSupplyPlenum*/
typedef boost::optional<AirLoopHVACSupplyPlenum> OptionalAirLoopHVACSupplyPlenum;

/** \relates AirLoopHVACSupplyPlenum*/
typedef std::vector<AirLoopHVACSupplyPlenum> AirLoopHVACSupplyPlenumVector;

} // model
} // openstudio

#endif // MODEL_AIRLOOPHVACSUPPLYPLENUM_HPP

