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

#include "CFactorUndergroundWallConstruction.hpp"
#include "CFactorUndergroundWallConstruction_Impl.hpp"

#include "Material.hpp"
#include "Material_Impl.hpp"
#include "ModelExtensibleGroup.hpp"

#include <utilities/idd/OS_Construction_CfactorUndergroundWall_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  CFactorUndergroundWallConstruction_Impl::CFactorUndergroundWallConstruction_Impl(
      const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ConstructionBase_Impl(idfObject, model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() ==
                 CFactorUndergroundWallConstruction::iddObjectType());
  }

  CFactorUndergroundWallConstruction_Impl::CFactorUndergroundWallConstruction_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other,Model_Impl* model,bool keepHandle)
    : ConstructionBase_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CFactorUndergroundWallConstruction::iddObjectType());
  }

  CFactorUndergroundWallConstruction_Impl::CFactorUndergroundWallConstruction_Impl(
      const CFactorUndergroundWallConstruction_Impl& other,Model_Impl* model,bool keepHandle)
    : ConstructionBase_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CFactorUndergroundWallConstruction_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CFactorUndergroundWallConstruction_Impl::iddObjectType() const {
    return CFactorUndergroundWallConstruction::iddObjectType();
  }

  bool CFactorUndergroundWallConstruction_Impl::isOpaque() const { return true; }

  bool CFactorUndergroundWallConstruction_Impl::isFenestration() const { return false; }

  bool CFactorUndergroundWallConstruction_Impl::isSolarDiffusing() const { return false; }

  bool CFactorUndergroundWallConstruction_Impl::isModelPartition() const { return false; }

  int CFactorUndergroundWallConstruction_Impl::renderingColorIndex() const
  {
    return OS_Construction_CfactorUndergroundWallFields::SurfaceRenderingName;
  }

  //double CFactorUndergroundWallConstruction_Impl::cFactor() const
  //{
  //  boost::optional<double> value = getDouble(OS_Construction_CfactorUndergroundWallFields::CFactor,true);
  //  OS_ASSERT(value);
  //  return value.get();
  //}

  double CFactorUndergroundWallConstruction_Impl::cFactor() const {
    Quantity q = getCFactor(false);
    return q.value();
  }

  Quantity CFactorUndergroundWallConstruction_Impl::getCFactor(bool returnIP) const {
    OSOptionalQuantity value = getQuantity(OS_Construction_CfactorUndergroundWallFields::CFactor,true,returnIP);
    OS_ASSERT(value.isSet());
    return value.get();
  }

  openstudio::Quantity CFactorUndergroundWallConstruction_Impl::cFactor_SI() const {
    return getCFactor(false);
  }

  openstudio::Quantity CFactorUndergroundWallConstruction_Impl::cFactor_IP() const {
    return getCFactor(true);
  }

  bool CFactorUndergroundWallConstruction_Impl::setCFactor(double cFactor) {
    OSOptionalQuantity temp = getQuantity(OS_Construction_CfactorUndergroundWallFields::CFactor,true,false);
    bool result = setCFactor(Quantity(cFactor,temp.units()));
    return result;
  }

  bool CFactorUndergroundWallConstruction_Impl::setCFactor(const Quantity& cFactor) {
    return setQuantity(OS_Construction_CfactorUndergroundWallFields::CFactor,cFactor);
  }

  //double CFactorUndergroundWallConstruction_Impl::height() const
  //{
  //  boost::optional<double> value = getDouble(OS_Construction_CfactorUndergroundWallFields::Height,true);
  //  OS_ASSERT(value);
  //  return value.get();
  //}

  double CFactorUndergroundWallConstruction_Impl::height() const {
    Quantity q = getHeight(false);
    return q.value();
  }

  Quantity CFactorUndergroundWallConstruction_Impl::getHeight(bool returnIP) const {
    OSOptionalQuantity value = getQuantity(OS_Construction_CfactorUndergroundWallFields::Height,true,returnIP);
    OS_ASSERT(value.isSet());
    return value.get();
  }

  openstudio::Quantity CFactorUndergroundWallConstruction_Impl::height_SI() const {
    return getHeight(false);
  }

  openstudio::Quantity CFactorUndergroundWallConstruction_Impl::height_IP() const {
    return getHeight(true);
  }

  bool CFactorUndergroundWallConstruction_Impl::setHeight(double height) {
    OSOptionalQuantity temp = getQuantity(OS_Construction_CfactorUndergroundWallFields::Height,true,false);
    bool result = setHeight(Quantity(height,temp.units()));
    return result;
  }

  bool CFactorUndergroundWallConstruction_Impl::setHeight(const Quantity& height) {
    return setQuantity(OS_Construction_CfactorUndergroundWallFields::Height,height);
  }

} // detail

CFactorUndergroundWallConstruction::CFactorUndergroundWallConstruction(const Model& model,
                                                                       double cFactor,
                                                                       double height)
  : ConstructionBase(CFactorUndergroundWallConstruction::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CFactorUndergroundWallConstruction_Impl>());
  
  this->setCFactor(cFactor);
  this->setHeight(height);
}

IddObjectType CFactorUndergroundWallConstruction::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Construction_CfactorUndergroundWall);
  return result;
}

double CFactorUndergroundWallConstruction::cFactor() const
{
  return getImpl<detail::CFactorUndergroundWallConstruction_Impl>()->cFactor();
}

Quantity CFactorUndergroundWallConstruction::getCFactor(bool returnIP) const
{
  return getImpl<detail::CFactorUndergroundWallConstruction_Impl>()->getCFactor(returnIP);
}

bool CFactorUndergroundWallConstruction::setCFactor(double cFactor)
{
  return getImpl<detail::CFactorUndergroundWallConstruction_Impl>()->setCFactor(cFactor);
}

bool CFactorUndergroundWallConstruction::setCFactor(const Quantity& cFactor)
{
  return getImpl<detail::CFactorUndergroundWallConstruction_Impl>()->setCFactor(cFactor);
}

double CFactorUndergroundWallConstruction::height() const {
  return getImpl<detail::CFactorUndergroundWallConstruction_Impl>()->height();
}

Quantity CFactorUndergroundWallConstruction::getHeight(bool returnIP) const {
  return getImpl<detail::CFactorUndergroundWallConstruction_Impl>()->getHeight(returnIP);
}

bool CFactorUndergroundWallConstruction::setHeight(double height) {
  return getImpl<detail::CFactorUndergroundWallConstruction_Impl>()->setHeight(height);
}

bool CFactorUndergroundWallConstruction::setHeight(const Quantity& height) {
  return getImpl<detail::CFactorUndergroundWallConstruction_Impl>()->setHeight(height);
}

/// @cond
CFactorUndergroundWallConstruction::CFactorUndergroundWallConstruction(
    std::shared_ptr<detail::CFactorUndergroundWallConstruction_Impl> impl)
  : ConstructionBase(impl)
{}
/// @endcond

} // model
} // openstudio
