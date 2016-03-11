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

#include "OutsideSurfaceConvectionAlgorithm.hpp"
#include "OutsideSurfaceConvectionAlgorithm_Impl.hpp"
#include "SimulationControl.hpp"
#include "SimulationControl_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_SurfaceConvectionAlgorithm_Outside_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model  {

namespace detail {

  OutsideSurfaceConvectionAlgorithm_Impl::OutsideSurfaceConvectionAlgorithm_Impl(
      const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ModelObject_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == OutsideSurfaceConvectionAlgorithm::iddObjectType());
  }

  OutsideSurfaceConvectionAlgorithm_Impl::OutsideSurfaceConvectionAlgorithm_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other,Model_Impl* model,bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == OutsideSurfaceConvectionAlgorithm::iddObjectType());
  }

  OutsideSurfaceConvectionAlgorithm_Impl::OutsideSurfaceConvectionAlgorithm_Impl(
      const OutsideSurfaceConvectionAlgorithm_Impl& other,Model_Impl* model,bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
  }

  // return the parent object in the hierarchy
  boost::optional<ParentObject> OutsideSurfaceConvectionAlgorithm_Impl::parent() const
  {
    boost::optional<ParentObject> result(model().getUniqueModelObject<SimulationControl>());
    return result;
  }

  // set the parent, child may have to call methods on the parent
  bool OutsideSurfaceConvectionAlgorithm_Impl::setParent(ParentObject& newParent)
  {
    if (newParent.optionalCast<SimulationControl>() && (newParent.model() == model())) {
      return true;
    }
    return false;
  }

  // Get all output variable names that could be associated with this object.
  const std::vector<std::string>& OutsideSurfaceConvectionAlgorithm_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType OutsideSurfaceConvectionAlgorithm_Impl::iddObjectType() const {
    return OutsideSurfaceConvectionAlgorithm::iddObjectType();
  }

  std::string OutsideSurfaceConvectionAlgorithm_Impl::algorithm() const {
    boost::optional<std::string> value = getString(OS_SurfaceConvectionAlgorithm_OutsideFields::Algorithm,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool OutsideSurfaceConvectionAlgorithm_Impl::isAlgorithmDefaulted() const {
    return isEmpty(OS_SurfaceConvectionAlgorithm_OutsideFields::Algorithm);
  }

  bool OutsideSurfaceConvectionAlgorithm_Impl::setAlgorithm(std::string algorithm) {
    bool result = setString(OS_SurfaceConvectionAlgorithm_OutsideFields::Algorithm, algorithm);
    return result;
  }

  void OutsideSurfaceConvectionAlgorithm_Impl::resetAlgorithm() {
    bool result = setString(OS_SurfaceConvectionAlgorithm_OutsideFields::Algorithm, "");
    OS_ASSERT(result);
  }

} // detail

/// constructor
OutsideSurfaceConvectionAlgorithm::OutsideSurfaceConvectionAlgorithm(const Model& model)
  : ModelObject(OutsideSurfaceConvectionAlgorithm::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::OutsideSurfaceConvectionAlgorithm_Impl>());

  setAlgorithm("DOE-2");
}

// constructor
OutsideSurfaceConvectionAlgorithm::OutsideSurfaceConvectionAlgorithm(
    std::shared_ptr<detail::OutsideSurfaceConvectionAlgorithm_Impl> impl)
  : ModelObject(impl)
{}

IddObjectType OutsideSurfaceConvectionAlgorithm::iddObjectType() {
  IddObjectType result(IddObjectType::OS_SurfaceConvectionAlgorithm_Outside);
  return result;
}

std::vector<std::string> OutsideSurfaceConvectionAlgorithm::validAlgorithmValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SurfaceConvectionAlgorithm_OutsideFields::Algorithm);
}

std::string OutsideSurfaceConvectionAlgorithm::algorithm() const {
  return getImpl<detail::OutsideSurfaceConvectionAlgorithm_Impl>()->algorithm();
}

bool OutsideSurfaceConvectionAlgorithm::isAlgorithmDefaulted() const {
  return getImpl<detail::OutsideSurfaceConvectionAlgorithm_Impl>()->isAlgorithmDefaulted();
}

bool OutsideSurfaceConvectionAlgorithm::setAlgorithm(std::string algorithm) {
  return getImpl<detail::OutsideSurfaceConvectionAlgorithm_Impl>()->setAlgorithm(algorithm);
}

void OutsideSurfaceConvectionAlgorithm::resetAlgorithm() {
  getImpl<detail::OutsideSurfaceConvectionAlgorithm_Impl>()->resetAlgorithm();
}

} // model
} // openstudio
