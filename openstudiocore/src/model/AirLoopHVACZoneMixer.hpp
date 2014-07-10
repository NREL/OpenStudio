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

#ifndef MODEL_AIRLOOPHVACZONEMIXER_HPP
#define MODEL_AIRLOOPHVACZONEMIXER_HPP

#include "ModelAPI.hpp"
#include "Mixer.hpp"

namespace openstudio {
namespace model {

namespace detail {
  class AirLoopHVACZoneMixer_Impl;
} // detail

/** AirLoopHVACZoneMixer is an interface to the EnergyPlus IDD object named "AirLoopHVAC:ZoneMixer"
 *
 *  The purpose of this class is to simplify the construction and manipulation
 *  of zone mixer objects in EnergyPlus.  Methods are built around the 
 *  acts of getting the inlet and outlet ports to the mixer.  Branch indexes
 *  are used to refer to the many inlet ports of the mixer
 */
class MODEL_API AirLoopHVACZoneMixer : public Mixer {

  public:

  /** Constructs a new AirLoopHVACZoneMixer object and places it inside the model. */
  explicit AirLoopHVACZoneMixer(const Model& model);

  virtual ~AirLoopHVACZoneMixer() {}

  unsigned outletPort();

  unsigned inletPort(unsigned branchIndex);
  
  unsigned nextInletPort();

  bool addToNode(Node & node);

  std::vector<openstudio::IdfObject> remove();

  ModelObject clone(Model model) const;

  void disconnect();

  static IddObjectType iddObjectType();

  protected:

  friend class Model;

  friend class openstudio::IdfObject;

  /// @cond 

  typedef detail::AirLoopHVACZoneMixer_Impl ImplType;

  explicit AirLoopHVACZoneMixer(std::shared_ptr<detail::AirLoopHVACZoneMixer_Impl> impl);

  private:

  REGISTER_LOGGER("openstudio.model.AirLoopHVACZoneMixer");

  AirLoopHVACZoneMixer(const Handle& handle, const Model& model);

  /// @endcond

};

/** \relates AirLoopHVACZoneMixer */
typedef boost::optional<AirLoopHVACZoneMixer> OptionalAirLoopHVACZoneMixer;

/** \relates AirLoopHVACZoneMixer */
typedef std::vector<AirLoopHVACZoneMixer> AirLoopHVACZoneMixerVector;

} // model
} // openstudio

#endif // MODEL_AIRLOOPHVACZONEMIXER_HPP
