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

#ifndef MODEL_AIRLOOPHVACSUPPLYPLENUM_IMPL_HPP
#define MODEL_AIRLOOPHVACSUPPLYPLENUM_IMPL_HPP

#include "ModelAPI.hpp"
#include "Splitter_Impl.hpp"

namespace openstudio {
namespace model {

class ThermalZone;

namespace detail {

/** AirLoopHVACSupplyPlenum_Impl is a Splitter_Impl that is the implementation class for AirLoopHVACSupplyPlenum.*/
class MODEL_API AirLoopHVACSupplyPlenum_Impl : public Splitter_Impl {

 public:

  AirLoopHVACSupplyPlenum_Impl(const IdfObject& idfObject,
                               Model_Impl* model,
                               bool keepHandle);

  AirLoopHVACSupplyPlenum_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                               Model_Impl* model,
                               bool keepHandle);

  AirLoopHVACSupplyPlenum_Impl(const AirLoopHVACSupplyPlenum_Impl& other,
                               Model_Impl* model,
                               bool keepHandle);

  virtual ~AirLoopHVACSupplyPlenum_Impl() {}

  virtual const std::vector<std::string>& outputVariableNames() const;

  virtual IddObjectType iddObjectType() const;

  boost::optional<ThermalZone> thermalZone() const;

  bool setThermalZone(const boost::optional<ThermalZone>& thermalZone);

  void resetThermalZone();

  unsigned inletPort();

  unsigned outletPort(unsigned branchIndex);

  unsigned nextOutletPort();

  bool addToNode(Node & node);

  bool addBranchForZone(openstudio::model::ThermalZone & thermalZone);

  bool addBranchForZone(openstudio::model::ThermalZone & thermalZone, StraightComponent & terminal);

  bool addBranchForZoneImpl(openstudio::model::ThermalZone & thermalZone, boost::optional<StraightComponent> & terminal);

  std::vector<IdfObject> remove();

 private:

  REGISTER_LOGGER("openstudio.model.AirLoopHVACSupplyPlenum");
};

} // detail

} // model
} // openstudio

#endif // MODEL_AIRLOOPHVACSUPPLYPLENUM_IMPL_HPP

