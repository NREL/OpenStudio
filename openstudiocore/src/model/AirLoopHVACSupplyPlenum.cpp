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

#include <model/AirLoopHVACSupplyPlenum.hpp>
#include <model/AirLoopHVACSupplyPlenum_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <utilities/idd/OS_AirLoopHVAC_SupplyPlenum_FieldEnums.hxx>
#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace model {

namespace detail {

  AirLoopHVACSupplyPlenum_Impl::AirLoopHVACSupplyPlenum_Impl(const IdfObject& idfObject,
                                                             Model_Impl* model,
                                                             bool keepHandle)
    : Splitter_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == AirLoopHVACSupplyPlenum::iddObjectType());
  }

  AirLoopHVACSupplyPlenum_Impl::AirLoopHVACSupplyPlenum_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                             Model_Impl* model,
                                                             bool keepHandle)
    : Splitter_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == AirLoopHVACSupplyPlenum::iddObjectType());
  }

  AirLoopHVACSupplyPlenum_Impl::AirLoopHVACSupplyPlenum_Impl(const AirLoopHVACSupplyPlenum_Impl& other,
                                                             Model_Impl* model,
                                                             bool keepHandle)
    : Splitter_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& AirLoopHVACSupplyPlenum_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType AirLoopHVACSupplyPlenum_Impl::iddObjectType() const {
    return AirLoopHVACSupplyPlenum::iddObjectType();
  }

  boost::optional<ThermalZone> AirLoopHVACSupplyPlenum_Impl::thermalZone() const {
    return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_AirLoopHVAC_SupplyPlenumFields::ThermalZone);
  }

  bool AirLoopHVACSupplyPlenum_Impl::setThermalZone(const boost::optional<ThermalZone>& thermalZone) {
    bool result(false);
    if (thermalZone) {
      result = setPointer(OS_AirLoopHVAC_SupplyPlenumFields::ThermalZone, thermalZone.get().handle());
    }
    else {
      resetThermalZone();
      result = true;
    }
    return result;
  }

  void AirLoopHVACSupplyPlenum_Impl::resetThermalZone() {
    bool result = setString(OS_AirLoopHVAC_SupplyPlenumFields::ThermalZone, "");
    OS_ASSERT(result);
  }

  unsigned AirLoopHVACSupplyPlenum_Impl::inletPort()
  {
    return OS_AirLoopHVAC_SupplyPlenumFields::InletNode;
  }

  unsigned AirLoopHVACSupplyPlenum_Impl::outletPort(unsigned branchIndex)
  {
    unsigned result;
    result = numNonextensibleFields();
    result = result + branchIndex;
    return result;
  }

  unsigned AirLoopHVACSupplyPlenum_Impl::nextOutletPort()
  {
    return outletPort( this->nextBranchIndex() );
  }

} // detail

AirLoopHVACSupplyPlenum::AirLoopHVACSupplyPlenum(const Model& model)
  : Splitter(AirLoopHVACSupplyPlenum::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::AirLoopHVACSupplyPlenum_Impl>());
}

IddObjectType AirLoopHVACSupplyPlenum::iddObjectType() {
  return IddObjectType(IddObjectType::OS_AirLoopHVAC_SupplyPlenum);
}

boost::optional<ThermalZone> AirLoopHVACSupplyPlenum::thermalZone() const {
  return getImpl<detail::AirLoopHVACSupplyPlenum_Impl>()->thermalZone();
}

bool AirLoopHVACSupplyPlenum::setThermalZone(const ThermalZone& thermalZone) {
  return getImpl<detail::AirLoopHVACSupplyPlenum_Impl>()->setThermalZone(thermalZone);
}

void AirLoopHVACSupplyPlenum::resetThermalZone() {
  getImpl<detail::AirLoopHVACSupplyPlenum_Impl>()->resetThermalZone();
}

unsigned AirLoopHVACSupplyPlenum::inletPort()
{
  return getImpl<detail::AirLoopHVACSupplyPlenum_Impl>()->inletPort();
}

unsigned AirLoopHVACSupplyPlenum::outletPort(unsigned branchIndex)
{
  return getImpl<detail::AirLoopHVACSupplyPlenum_Impl>()->outletPort(branchIndex);
}

unsigned AirLoopHVACSupplyPlenum::nextOutletPort()
{
  return getImpl<detail::AirLoopHVACSupplyPlenum_Impl>()->nextOutletPort();
}

/// @cond
AirLoopHVACSupplyPlenum::AirLoopHVACSupplyPlenum(boost::shared_ptr<detail::AirLoopHVACSupplyPlenum_Impl> impl)
  : Splitter(impl)
{}
/// @endcond

} // model
} // openstudio

