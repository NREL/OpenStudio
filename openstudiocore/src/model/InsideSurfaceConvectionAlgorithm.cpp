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

#include "InsideSurfaceConvectionAlgorithm.hpp"
#include "InsideSurfaceConvectionAlgorithm_Impl.hpp"
#include "SimulationControl.hpp"
#include "SimulationControl_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_SurfaceConvectionAlgorithm_Inside_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model  {

namespace detail {

  InsideSurfaceConvectionAlgorithm_Impl::InsideSurfaceConvectionAlgorithm_Impl(
      const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ModelObject_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == InsideSurfaceConvectionAlgorithm::iddObjectType());
  }

  InsideSurfaceConvectionAlgorithm_Impl::InsideSurfaceConvectionAlgorithm_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other,Model_Impl* model,bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == InsideSurfaceConvectionAlgorithm::iddObjectType());
  }

  InsideSurfaceConvectionAlgorithm_Impl::InsideSurfaceConvectionAlgorithm_Impl(
      const InsideSurfaceConvectionAlgorithm_Impl& other,Model_Impl* model,bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
  }

  // return the parent object in the hierarchy
  boost::optional<ParentObject> InsideSurfaceConvectionAlgorithm_Impl::parent() const
  {
    boost::optional<ParentObject> result(model().getUniqueModelObject<SimulationControl>());
    return result;
  }

  // set the parent, child may have to call methods on the parent
  bool InsideSurfaceConvectionAlgorithm_Impl::setParent(ParentObject& newParent)
  {
    if (newParent.optionalCast<SimulationControl>() && (newParent.model() == model())) {
      return true;
    }
    return false;
  }

  // Get all output variable names that could be associated with this object.
  const std::vector<std::string>& InsideSurfaceConvectionAlgorithm_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType InsideSurfaceConvectionAlgorithm_Impl::iddObjectType() const {
    return InsideSurfaceConvectionAlgorithm::iddObjectType();
  }

  std::string InsideSurfaceConvectionAlgorithm_Impl::algorithm() const {
    boost::optional<std::string> value = getString(OS_SurfaceConvectionAlgorithm_InsideFields::Algorithm,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool InsideSurfaceConvectionAlgorithm_Impl::isAlgorithmDefaulted() const {
    return isEmpty(OS_SurfaceConvectionAlgorithm_InsideFields::Algorithm);
  }

  bool InsideSurfaceConvectionAlgorithm_Impl::setAlgorithm(std::string algorithm) {
    bool result = setString(OS_SurfaceConvectionAlgorithm_InsideFields::Algorithm, algorithm);
    return result;
  }

  void InsideSurfaceConvectionAlgorithm_Impl::resetAlgorithm() {
    bool result = setString(OS_SurfaceConvectionAlgorithm_InsideFields::Algorithm, "");
    OS_ASSERT(result);
  }

} // detail

IddObjectType InsideSurfaceConvectionAlgorithm::iddObjectType() {
  IddObjectType result(IddObjectType::OS_SurfaceConvectionAlgorithm_Inside);
  return result;
}

std::vector<std::string> InsideSurfaceConvectionAlgorithm::validAlgorithmValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SurfaceConvectionAlgorithm_InsideFields::Algorithm);
}

std::string InsideSurfaceConvectionAlgorithm::algorithm() const {
  return getImpl<detail::InsideSurfaceConvectionAlgorithm_Impl>()->algorithm();
}

bool InsideSurfaceConvectionAlgorithm::isAlgorithmDefaulted() const {
  return getImpl<detail::InsideSurfaceConvectionAlgorithm_Impl>()->isAlgorithmDefaulted();
}

bool InsideSurfaceConvectionAlgorithm::setAlgorithm(std::string algorithm) {
  return getImpl<detail::InsideSurfaceConvectionAlgorithm_Impl>()->setAlgorithm(algorithm);
}

void InsideSurfaceConvectionAlgorithm::resetAlgorithm() {
  getImpl<detail::InsideSurfaceConvectionAlgorithm_Impl>()->resetAlgorithm();
}

/// constructor
InsideSurfaceConvectionAlgorithm::InsideSurfaceConvectionAlgorithm(const Model& model)
  : ModelObject(InsideSurfaceConvectionAlgorithm::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::InsideSurfaceConvectionAlgorithm_Impl>());

  setAlgorithm("TARP");
}

// constructor
InsideSurfaceConvectionAlgorithm::InsideSurfaceConvectionAlgorithm(
    std::shared_ptr<detail::InsideSurfaceConvectionAlgorithm_Impl> impl)
  : ModelObject(impl)
{}

} // model
} // openstudio
