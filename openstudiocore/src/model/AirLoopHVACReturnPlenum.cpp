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

#include <model/AirLoopHVACReturnPlenum.hpp>
#include <model/AirLoopHVACReturnPlenum_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <utilities/idd/OS_AirLoopHVAC_ReturnPlenum_FieldEnums.hxx>
#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace model {

namespace detail {

  AirLoopHVACReturnPlenum_Impl::AirLoopHVACReturnPlenum_Impl(const IdfObject& idfObject,
                                                             Model_Impl* model,
                                                             bool keepHandle)
    : Mixer_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == AirLoopHVACReturnPlenum::iddObjectType());
  }

  AirLoopHVACReturnPlenum_Impl::AirLoopHVACReturnPlenum_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                             Model_Impl* model,
                                                             bool keepHandle)
    : Mixer_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == AirLoopHVACReturnPlenum::iddObjectType());
  }

  AirLoopHVACReturnPlenum_Impl::AirLoopHVACReturnPlenum_Impl(const AirLoopHVACReturnPlenum_Impl& other,
                                                             Model_Impl* model,
                                                             bool keepHandle)
    : Mixer_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& AirLoopHVACReturnPlenum_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType AirLoopHVACReturnPlenum_Impl::iddObjectType() const {
    return AirLoopHVACReturnPlenum::iddObjectType();
  }

  boost::optional<ThermalZone> AirLoopHVACReturnPlenum_Impl::thermalZone() const {
    return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_AirLoopHVAC_ReturnPlenumFields::ThermalZone);
  }

  bool AirLoopHVACReturnPlenum_Impl::setThermalZone(const boost::optional<ThermalZone>& thermalZone) {
    bool result(false);
    if (thermalZone) {
      result = setPointer(OS_AirLoopHVAC_ReturnPlenumFields::ThermalZone, thermalZone.get().handle());
    }
    else {
      resetThermalZone();
      result = true;
    }
    return result;
  }

  void AirLoopHVACReturnPlenum_Impl::resetThermalZone() {
    bool result = setString(OS_AirLoopHVAC_ReturnPlenumFields::ThermalZone, "");
    OS_ASSERT(result);
  }

  unsigned AirLoopHVACReturnPlenum_Impl::outletPort()
  {
    return OS_AirLoopHVAC_ReturnPlenumFields::OutletNode;
  }

  unsigned AirLoopHVACReturnPlenum_Impl::inletPort(unsigned branchIndex)
  {
    unsigned result;
    result = numNonextensibleFields();
    result = result + branchIndex;
    return result;
  }

  unsigned AirLoopHVACReturnPlenum_Impl::nextInletPort()
  {
    return inletPort( this->nextBranchIndex() );
  }

} // detail

AirLoopHVACReturnPlenum::AirLoopHVACReturnPlenum(const Model& model)
  : Mixer(AirLoopHVACReturnPlenum::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::AirLoopHVACReturnPlenum_Impl>());
}

IddObjectType AirLoopHVACReturnPlenum::iddObjectType() {
  return IddObjectType(IddObjectType::OS_AirLoopHVAC_ReturnPlenum);
}

boost::optional<ThermalZone> AirLoopHVACReturnPlenum::thermalZone() const {
  return getImpl<detail::AirLoopHVACReturnPlenum_Impl>()->thermalZone();
}

bool AirLoopHVACReturnPlenum::setThermalZone(const ThermalZone& thermalZone) {
  return getImpl<detail::AirLoopHVACReturnPlenum_Impl>()->setThermalZone(thermalZone);
}

void AirLoopHVACReturnPlenum::resetThermalZone() {
  getImpl<detail::AirLoopHVACReturnPlenum_Impl>()->resetThermalZone();
}

unsigned AirLoopHVACReturnPlenum::outletPort()
{
  return getImpl<detail::AirLoopHVACReturnPlenum_Impl>()->outletPort();
}

unsigned AirLoopHVACReturnPlenum::inletPort(unsigned branchIndex)
{
  return getImpl<detail::AirLoopHVACReturnPlenum_Impl>()->inletPort(branchIndex);
}

unsigned AirLoopHVACReturnPlenum::nextInletPort()
{
  return getImpl<detail::AirLoopHVACReturnPlenum_Impl>()->nextInletPort();
}

/// @cond
AirLoopHVACReturnPlenum::AirLoopHVACReturnPlenum(boost::shared_ptr<detail::AirLoopHVACReturnPlenum_Impl> impl)
  : Mixer(impl)
{}
/// @endcond

} // model
} // openstudio

