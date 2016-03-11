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

#ifndef MODEL_AIRLOOPHVACRETURNPLENUM_IMPL_HPP
#define MODEL_AIRLOOPHVACRETURNPLENUM_IMPL_HPP

#include "ModelAPI.hpp"
#include "Mixer_Impl.hpp"

namespace openstudio {
namespace model {

class ThermalZone;
class PortList;

namespace detail {

  /** AirLoopHVACReturnPlenum_Impl is a Mixer_Impl that is the implementation class for AirLoopHVACReturnPlenum.*/
  class MODEL_API AirLoopHVACReturnPlenum_Impl : public Mixer_Impl {

   public:

    AirLoopHVACReturnPlenum_Impl(const IdfObject& idfObject,
                                 Model_Impl* model,
                                 bool keepHandle);

    AirLoopHVACReturnPlenum_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                 Model_Impl* model,
                                 bool keepHandle);

    AirLoopHVACReturnPlenum_Impl(const AirLoopHVACReturnPlenum_Impl& other,
                                 Model_Impl* model,
                                 bool keepHandle);

    virtual ~AirLoopHVACReturnPlenum_Impl() {}

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    boost::optional<ThermalZone> thermalZone() const;

    bool setThermalZone(const boost::optional<ThermalZone>& thermalZone);

    void resetThermalZone();

    unsigned outletPort() const override;

    PortList inducedAirOutletPortList();

    unsigned inletPort(unsigned branchIndex) const override;

    unsigned nextInletPort() const override;

    bool addToNode(Node & node) override;

    bool addBranchForZone(openstudio::model::ThermalZone & thermalZone);

    bool addBranchForZone(openstudio::model::ThermalZone & thermalZone, HVACComponent & terminal);

    bool addBranchForZoneImpl(openstudio::model::ThermalZone & thermalZone, boost::optional<HVACComponent> & terminal);

    std::vector<IdfObject> remove() override;

   private:

    REGISTER_LOGGER("openstudio.model.AirLoopHVACReturnPlenum");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_AIRLOOPHVACRETURNPLENUM_IMPL_HPP

