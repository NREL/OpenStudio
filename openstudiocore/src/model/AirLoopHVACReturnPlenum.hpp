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

#ifndef MODEL_AIRLOOPHVACRETURNPLENUM_HPP
#define MODEL_AIRLOOPHVACRETURNPLENUM_HPP

#include "ModelAPI.hpp"
#include "Mixer.hpp"

namespace openstudio {
namespace model {

class ThermalZone;

namespace detail {

  class AirLoopHVACReturnPlenum_Impl;

} // detail

/** AirLoopHVACReturnPlenum is a Mixer that wraps the OpenStudio IDD object 'OS:AirLoopHVAC:ReturnPlenum'. */
class MODEL_API AirLoopHVACReturnPlenum : public Mixer {
 public:

  explicit AirLoopHVACReturnPlenum(const Model& model);

  virtual ~AirLoopHVACReturnPlenum() {}

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

  unsigned outletPort();

  unsigned inletPort(unsigned branchIndex);

  unsigned nextInletPort();

  bool addToNode(Node & node);

  bool addBranchForZone(openstudio::model::ThermalZone & thermalZone);

  bool addBranchForZone(openstudio::model::ThermalZone & thermalZone, StraightComponent & terminal);

 protected:
  /// @cond
  typedef detail::AirLoopHVACReturnPlenum_Impl ImplType;

  explicit AirLoopHVACReturnPlenum(std::shared_ptr<detail::AirLoopHVACReturnPlenum_Impl> impl);

  friend class detail::AirLoopHVACReturnPlenum_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.AirLoopHVACReturnPlenum");
};

/** \relates AirLoopHVACReturnPlenum*/
typedef boost::optional<AirLoopHVACReturnPlenum> OptionalAirLoopHVACReturnPlenum;

/** \relates AirLoopHVACReturnPlenum*/
typedef std::vector<AirLoopHVACReturnPlenum> AirLoopHVACReturnPlenumVector;

} // model
} // openstudio

#endif // MODEL_AIRLOOPHVACRETURNPLENUM_HPP

