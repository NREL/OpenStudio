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

#include "ShadowCalculation.hpp"
#include "ShadowCalculation_Impl.hpp"
#include "SimulationControl.hpp"
#include "SimulationControl_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_ShadowCalculation_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

namespace openstudio {
namespace model  {

namespace detail {

  ShadowCalculation_Impl::ShadowCalculation_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ModelObject_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ShadowCalculation::iddObjectType());
  }

  ShadowCalculation_Impl::ShadowCalculation_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other,Model_Impl* model,bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ShadowCalculation::iddObjectType());
  }

  ShadowCalculation_Impl::ShadowCalculation_Impl(const ShadowCalculation_Impl& other,
                                                 Model_Impl* model,
                                                 bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
  }

  // return the parent object in the hierarchy
  boost::optional<ParentObject> ShadowCalculation_Impl::parent() const
  {
    boost::optional<ParentObject> result(model().getUniqueModelObject<SimulationControl>());
    return result;
  }

  // set the parent, child may have to call methods on the parent
  bool ShadowCalculation_Impl::setParent(ParentObject& newParent)
  {
    if (newParent.optionalCast<SimulationControl>() && (newParent.model() == model())) {
      return true;
    }
    return false;
  }

  // Get all output variable names that could be associated with this object.
  const std::vector<std::string>& ShadowCalculation_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType ShadowCalculation_Impl::iddObjectType() const {
    return ShadowCalculation::iddObjectType();
  }

  int ShadowCalculation_Impl::calculationFrequency() const {
    boost::optional<int> value = getInt(OS_ShadowCalculationFields::CalculationFrequency,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ShadowCalculation_Impl::isCalculationFrequencyDefaulted() const {
    return isEmpty(OS_ShadowCalculationFields::CalculationFrequency);
  }

  int ShadowCalculation_Impl::maximumFiguresInShadowOverlapCalculations() const {
    boost::optional<int> value = getInt(OS_ShadowCalculationFields::MaximumFiguresinShadowOverlapCalculations,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ShadowCalculation_Impl::isMaximumFiguresInShadowOverlapCalculationsDefaulted() const {
    return isEmpty(OS_ShadowCalculationFields::MaximumFiguresinShadowOverlapCalculations);
  }

  boost::optional<std::string> ShadowCalculation_Impl::polygonClippingAlgorithm() const {
    return getString(OS_ShadowCalculationFields::PolygonClippingAlgorithm,true);
  }

  boost::optional<std::string> ShadowCalculation_Impl::skyDiffuseModelingAlgorithm() const {
    return getString(OS_ShadowCalculationFields::SkyDiffuseModelingAlgorithm,true);
  }

  bool ShadowCalculation_Impl::setCalculationFrequency(int calculationFrequency) {
    bool result = setInt(OS_ShadowCalculationFields::CalculationFrequency, calculationFrequency);
    return result;
  }

  void ShadowCalculation_Impl::resetCalculationFrequency() {
    bool result = setString(OS_ShadowCalculationFields::CalculationFrequency, "");
    OS_ASSERT(result);
  }

  bool ShadowCalculation_Impl::setMaximumFiguresInShadowOverlapCalculations(int maximumFiguresInShadowOverlapCalculations) {
    bool result = setInt(OS_ShadowCalculationFields::MaximumFiguresinShadowOverlapCalculations, maximumFiguresInShadowOverlapCalculations);
    return result;
  }

  void ShadowCalculation_Impl::resetMaximumFiguresInShadowOverlapCalculations() {
    bool result = setString(OS_ShadowCalculationFields::MaximumFiguresinShadowOverlapCalculations, "");
    OS_ASSERT(result);
  }

  bool ShadowCalculation_Impl::setPolygonClippingAlgorithm(boost::optional<std::string> polygonClippingAlgorithm) {
    bool result = false;
    if (polygonClippingAlgorithm) {
      result = setString(OS_ShadowCalculationFields::PolygonClippingAlgorithm, polygonClippingAlgorithm.get());
    } else {
      result = setString(OS_ShadowCalculationFields::PolygonClippingAlgorithm, "");
    }
    return result;
  }

  void ShadowCalculation_Impl::resetPolygonClippingAlgorithm() {
    bool result = setString(OS_ShadowCalculationFields::PolygonClippingAlgorithm, "");
    OS_ASSERT(result);
  }

  bool ShadowCalculation_Impl::setSkyDiffuseModelingAlgorithm(boost::optional<std::string> skyDiffuseModelingAlgorithm) {
    bool result = false;
    if (skyDiffuseModelingAlgorithm) {
      result = setString(OS_ShadowCalculationFields::SkyDiffuseModelingAlgorithm, skyDiffuseModelingAlgorithm.get());
    } else {
      result = setString(OS_ShadowCalculationFields::SkyDiffuseModelingAlgorithm, "");
    }
    return result;
  }

  void ShadowCalculation_Impl::resetSkyDiffuseModelingAlgorithm() {
    bool result = setString(OS_ShadowCalculationFields::SkyDiffuseModelingAlgorithm, "");
    OS_ASSERT(result);
  }

} // detail

/// constructor
ShadowCalculation::ShadowCalculation(const Model& model)
  : ModelObject(ShadowCalculation::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ShadowCalculation_Impl>());

  this->setCalculationFrequency(20);

  this->setMaximumFiguresInShadowOverlapCalculations(15000);
}

// constructor
ShadowCalculation::ShadowCalculation(std::shared_ptr<detail::ShadowCalculation_Impl> impl)
  : ModelObject(impl)
{}

IddObjectType ShadowCalculation::iddObjectType() {
  IddObjectType result(IddObjectType::OS_ShadowCalculation);
  return result;
}

std::vector<std::string> ShadowCalculation::validPolygonClippingAlgorithmValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ShadowCalculationFields::PolygonClippingAlgorithm);
}

std::vector<std::string> ShadowCalculation::validSkyDiffuseModelingAlgorithmValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ShadowCalculationFields::SkyDiffuseModelingAlgorithm);
}

int ShadowCalculation::calculationFrequency() const {
  return getImpl<detail::ShadowCalculation_Impl>()->calculationFrequency();
}

bool ShadowCalculation::isCalculationFrequencyDefaulted() const {
  return getImpl<detail::ShadowCalculation_Impl>()->isCalculationFrequencyDefaulted();
}

int ShadowCalculation::maximumFiguresInShadowOverlapCalculations() const {
  return getImpl<detail::ShadowCalculation_Impl>()->maximumFiguresInShadowOverlapCalculations();
}

bool ShadowCalculation::isMaximumFiguresInShadowOverlapCalculationsDefaulted() const {
  return getImpl<detail::ShadowCalculation_Impl>()->isMaximumFiguresInShadowOverlapCalculationsDefaulted();
}

boost::optional<std::string> ShadowCalculation::polygonClippingAlgorithm() const {
  return getImpl<detail::ShadowCalculation_Impl>()->polygonClippingAlgorithm();
}

boost::optional<std::string> ShadowCalculation::skyDiffuseModelingAlgorithm() const {
  return getImpl<detail::ShadowCalculation_Impl>()->skyDiffuseModelingAlgorithm();
}

bool ShadowCalculation::setCalculationFrequency(int calculationFrequency) {
  return getImpl<detail::ShadowCalculation_Impl>()->setCalculationFrequency(calculationFrequency);
}

void ShadowCalculation::resetCalculationFrequency() {
  getImpl<detail::ShadowCalculation_Impl>()->resetCalculationFrequency();
}

bool ShadowCalculation::setMaximumFiguresInShadowOverlapCalculations(int maximumFiguresInShadowOverlapCalculations) {
  return getImpl<detail::ShadowCalculation_Impl>()->setMaximumFiguresInShadowOverlapCalculations(maximumFiguresInShadowOverlapCalculations);
}

void ShadowCalculation::resetMaximumFiguresInShadowOverlapCalculations() {
  getImpl<detail::ShadowCalculation_Impl>()->resetMaximumFiguresInShadowOverlapCalculations();
}

bool ShadowCalculation::setPolygonClippingAlgorithm(std::string polygonClippingAlgorithm) {
  return getImpl<detail::ShadowCalculation_Impl>()->setPolygonClippingAlgorithm(polygonClippingAlgorithm);
}

void ShadowCalculation::resetPolygonClippingAlgorithm() {
  getImpl<detail::ShadowCalculation_Impl>()->resetPolygonClippingAlgorithm();
}

bool ShadowCalculation::setSkyDiffuseModelingAlgorithm(std::string skyDiffuseModelingAlgorithm) {
  return getImpl<detail::ShadowCalculation_Impl>()->setSkyDiffuseModelingAlgorithm(skyDiffuseModelingAlgorithm);
}

void ShadowCalculation::resetSkyDiffuseModelingAlgorithm() {
  getImpl<detail::ShadowCalculation_Impl>()->resetSkyDiffuseModelingAlgorithm();
}

} // model
} // openstudio
