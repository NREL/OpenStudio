/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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
  : ConstructionBase(std::move(impl))
{}
/// @endcond

} // model
} // openstudio
