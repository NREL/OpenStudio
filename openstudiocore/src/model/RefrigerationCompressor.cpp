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

#include <model/RefrigerationCompressor.hpp>
#include <model/RefrigerationCompressor_Impl.hpp>

// TODO: Check the following class names against object getters and setters.
#include <model/CurveBicubic.hpp>
#include <model/CurveBicubic_Impl.hpp>

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_Refrigeration_Compressor_FieldEnums.hxx>

#include <utilities/units/Unit.hpp>

#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace model {

namespace detail {

  RefrigerationCompressor_Impl::RefrigerationCompressor_Impl(const IdfObject& idfObject,
                                                             Model_Impl* model,
                                                             bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    BOOST_ASSERT(idfObject.iddObject().type() == RefrigerationCompressor::iddObjectType());
  }

  RefrigerationCompressor_Impl::RefrigerationCompressor_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                             Model_Impl* model,
                                                             bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    BOOST_ASSERT(other.iddObject().type() == RefrigerationCompressor::iddObjectType());
  }

  RefrigerationCompressor_Impl::RefrigerationCompressor_Impl(const RefrigerationCompressor_Impl& other,
                                                             Model_Impl* model,
                                                             bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& RefrigerationCompressor_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType RefrigerationCompressor_Impl::iddObjectType() const {
    return RefrigerationCompressor::iddObjectType();
  }

  CurveBicubic RefrigerationCompressor_Impl::refrigerationCompressorPowerCurve() const {
    boost::optional<CurveBicubic> value = optionalRefrigerationCompressorPowerCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Refrigeration Compressor Power Curve attached.");
    }
    return value.get();
  }

  CurveBicubic RefrigerationCompressor_Impl::refrigerationCompressorCapacityCurve() const {
    boost::optional<CurveBicubic> value = optionalRefrigerationCompressorCapacityCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Refrigeration Compressor Capacity Curve attached.");
    }
    return value.get();
  }

  boost::optional<double> RefrigerationCompressor_Impl::ratedSuperheat() const {
    return getDouble(OS_Refrigeration_CompressorFields::RatedSuperheat,true);
  }

  boost::optional<double> RefrigerationCompressor_Impl::ratedReturnGasTemperature() const {
    return getDouble(OS_Refrigeration_CompressorFields::RatedReturnGasTemperature,true);
  }

  boost::optional<double> RefrigerationCompressor_Impl::ratedLiquidTemperature() const {
    return getDouble(OS_Refrigeration_CompressorFields::RatedLiquidTemperature,true);
  }

  boost::optional<double> RefrigerationCompressor_Impl::ratedSubcooling() const {
    return getDouble(OS_Refrigeration_CompressorFields::RatedSubcooling,true);
  }

  std::string RefrigerationCompressor_Impl::endUseSubcategory() const {
    boost::optional<std::string> value = getString(OS_Refrigeration_CompressorFields::EndUseSubcategory,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  bool RefrigerationCompressor_Impl::isEndUseSubcategoryDefaulted() const {
    return isEmpty(OS_Refrigeration_CompressorFields::EndUseSubcategory);
  }

  std::string RefrigerationCompressor_Impl::modeofOperation() const {
    boost::optional<std::string> value = getString(OS_Refrigeration_CompressorFields::ModeofOperation,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  bool RefrigerationCompressor_Impl::isModeofOperationDefaulted() const {
    return isEmpty(OS_Refrigeration_CompressorFields::ModeofOperation);
  }

  boost::optional<CurveBicubic> RefrigerationCompressor_Impl::transcriticalCompressorPowerCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveBicubic>(OS_Refrigeration_CompressorFields::TranscriticalCompressorPowerCurveName);
  }

  boost::optional<CurveBicubic> RefrigerationCompressor_Impl::transcriticalCompressorCapacityCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveBicubic>(OS_Refrigeration_CompressorFields::TranscriticalCompressorCapacityCurveName);
  }

  bool RefrigerationCompressor_Impl::setRefrigerationCompressorPowerCurve(const CurveBicubic& curveBicubic) {
    bool result = setPointer(OS_Refrigeration_CompressorFields::RefrigerationCompressorPowerCurveName, curveBicubic.handle());
    return result;
  }

  bool RefrigerationCompressor_Impl::setRefrigerationCompressorCapacityCurve(const CurveBicubic& curveBicubic) {
    bool result = setPointer(OS_Refrigeration_CompressorFields::RefrigerationCompressorCapacityCurveName, curveBicubic.handle());
    return result;
  }

  void RefrigerationCompressor_Impl::setRatedSuperheat(boost::optional<double> ratedSuperheat) {
    bool result(false);
    if (ratedSuperheat) {
      result = setDouble(OS_Refrigeration_CompressorFields::RatedSuperheat, ratedSuperheat.get());
    }
    else {
      resetRatedSuperheat();
      result = true;
    }
    BOOST_ASSERT(result);
  }

  void RefrigerationCompressor_Impl::resetRatedSuperheat() {
    bool result = setString(OS_Refrigeration_CompressorFields::RatedSuperheat, "");
    BOOST_ASSERT(result);
  }

  void RefrigerationCompressor_Impl::setRatedReturnGasTemperature(boost::optional<double> ratedReturnGasTemperature) {
    bool result(false);
    if (ratedReturnGasTemperature) {
      result = setDouble(OS_Refrigeration_CompressorFields::RatedReturnGasTemperature, ratedReturnGasTemperature.get());
    }
    else {
      resetRatedReturnGasTemperature();
      result = true;
    }
    BOOST_ASSERT(result);
  }

  void RefrigerationCompressor_Impl::resetRatedReturnGasTemperature() {
    bool result = setString(OS_Refrigeration_CompressorFields::RatedReturnGasTemperature, "");
    BOOST_ASSERT(result);
  }

  void RefrigerationCompressor_Impl::setRatedLiquidTemperature(boost::optional<double> ratedLiquidTemperature) {
    bool result(false);
    if (ratedLiquidTemperature) {
      result = setDouble(OS_Refrigeration_CompressorFields::RatedLiquidTemperature, ratedLiquidTemperature.get());
    }
    else {
      resetRatedLiquidTemperature();
      result = true;
    }
    BOOST_ASSERT(result);
  }

  void RefrigerationCompressor_Impl::resetRatedLiquidTemperature() {
    bool result = setString(OS_Refrigeration_CompressorFields::RatedLiquidTemperature, "");
    BOOST_ASSERT(result);
  }

  void RefrigerationCompressor_Impl::setRatedSubcooling(boost::optional<double> ratedSubcooling) {
    bool result(false);
    if (ratedSubcooling) {
      result = setDouble(OS_Refrigeration_CompressorFields::RatedSubcooling, ratedSubcooling.get());
    }
    else {
      resetRatedSubcooling();
      result = true;
    }
    BOOST_ASSERT(result);
  }

  void RefrigerationCompressor_Impl::resetRatedSubcooling() {
    bool result = setString(OS_Refrigeration_CompressorFields::RatedSubcooling, "");
    BOOST_ASSERT(result);
  }

  void RefrigerationCompressor_Impl::setEndUseSubcategory(std::string endUseSubcategory) {
    bool result = setString(OS_Refrigeration_CompressorFields::EndUseSubcategory, endUseSubcategory);
    BOOST_ASSERT(result);
  }

  void RefrigerationCompressor_Impl::resetEndUseSubcategory() {
    bool result = setString(OS_Refrigeration_CompressorFields::EndUseSubcategory, "");
    BOOST_ASSERT(result);
  }

  bool RefrigerationCompressor_Impl::setModeofOperation(std::string modeofOperation) {
    bool result = setString(OS_Refrigeration_CompressorFields::ModeofOperation, modeofOperation);
    return result;
  }

  void RefrigerationCompressor_Impl::resetModeofOperation() {
    bool result = setString(OS_Refrigeration_CompressorFields::ModeofOperation, "");
    BOOST_ASSERT(result);
  }

  bool RefrigerationCompressor_Impl::setTranscriticalCompressorPowerCurve(const boost::optional<CurveBicubic>& curveBicubic) {
    bool result(false);
    if (curveBicubic) {
      result = setPointer(OS_Refrigeration_CompressorFields::TranscriticalCompressorPowerCurveName, curveBicubic.get().handle());
    }
    else {
      resetTranscriticalCompressorPowerCurve();
      result = true;
    }
    return result;
  }

  void RefrigerationCompressor_Impl::resetTranscriticalCompressorPowerCurve() {
    bool result = setString(OS_Refrigeration_CompressorFields::TranscriticalCompressorPowerCurveName, "");
    BOOST_ASSERT(result);
  }

  bool RefrigerationCompressor_Impl::setTranscriticalCompressorCapacityCurve(const boost::optional<CurveBicubic>& curveBicubic) {
    bool result(false);
    if (curveBicubic) {
      result = setPointer(OS_Refrigeration_CompressorFields::TranscriticalCompressorCapacityCurveName, curveBicubic.get().handle());
    }
    else {
      resetTranscriticalCompressorCapacityCurve();
      result = true;
    }
    return result;
  }

  void RefrigerationCompressor_Impl::resetTranscriticalCompressorCapacityCurve() {
    bool result = setString(OS_Refrigeration_CompressorFields::TranscriticalCompressorCapacityCurveName, "");
    BOOST_ASSERT(result);
  }

  boost::optional<CurveBicubic> RefrigerationCompressor_Impl::optionalRefrigerationCompressorPowerCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveBicubic>(OS_Refrigeration_CompressorFields::RefrigerationCompressorPowerCurveName);
  }

  boost::optional<CurveBicubic> RefrigerationCompressor_Impl::optionalRefrigerationCompressorCapacityCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveBicubic>(OS_Refrigeration_CompressorFields::RefrigerationCompressorCapacityCurveName);
  }

} // detail

RefrigerationCompressor::RefrigerationCompressor(const Model& model)
  : ModelObject(RefrigerationCompressor::iddObjectType(),model)
{
  BOOST_ASSERT(getImpl<detail::RefrigerationCompressor_Impl>());
  
  bool ok = true;
  BOOST_ASSERT(ok);
  //ok = setRefrigerationCompressorPowerCurve(powerCurve);
  BOOST_ASSERT(ok);
  //ok = setRefrigerationCompressorCapacityCurve(capacityCurve);
  BOOST_ASSERT(ok);
}

IddObjectType RefrigerationCompressor::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Refrigeration_Compressor);
}

std::vector<std::string> RefrigerationCompressor::modeofOperationValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Refrigeration_CompressorFields::ModeofOperation);
}

CurveBicubic RefrigerationCompressor::refrigerationCompressorPowerCurve() const {
  return getImpl<detail::RefrigerationCompressor_Impl>()->refrigerationCompressorPowerCurve();
}

CurveBicubic RefrigerationCompressor::refrigerationCompressorCapacityCurve() const {
  return getImpl<detail::RefrigerationCompressor_Impl>()->refrigerationCompressorCapacityCurve();
}

boost::optional<double> RefrigerationCompressor::ratedSuperheat() const {
  return getImpl<detail::RefrigerationCompressor_Impl>()->ratedSuperheat();
}

boost::optional<double> RefrigerationCompressor::ratedReturnGasTemperature() const {
  return getImpl<detail::RefrigerationCompressor_Impl>()->ratedReturnGasTemperature();
}

boost::optional<double> RefrigerationCompressor::ratedLiquidTemperature() const {
  return getImpl<detail::RefrigerationCompressor_Impl>()->ratedLiquidTemperature();
}

boost::optional<double> RefrigerationCompressor::ratedSubcooling() const {
  return getImpl<detail::RefrigerationCompressor_Impl>()->ratedSubcooling();
}

std::string RefrigerationCompressor::endUseSubcategory() const {
  return getImpl<detail::RefrigerationCompressor_Impl>()->endUseSubcategory();
}

bool RefrigerationCompressor::isEndUseSubcategoryDefaulted() const {
  return getImpl<detail::RefrigerationCompressor_Impl>()->isEndUseSubcategoryDefaulted();
}

std::string RefrigerationCompressor::modeofOperation() const {
  return getImpl<detail::RefrigerationCompressor_Impl>()->modeofOperation();
}

bool RefrigerationCompressor::isModeofOperationDefaulted() const {
  return getImpl<detail::RefrigerationCompressor_Impl>()->isModeofOperationDefaulted();
}

boost::optional<CurveBicubic> RefrigerationCompressor::transcriticalCompressorPowerCurve() const {
  return getImpl<detail::RefrigerationCompressor_Impl>()->transcriticalCompressorPowerCurve();
}

boost::optional<CurveBicubic> RefrigerationCompressor::transcriticalCompressorCapacityCurve() const {
  return getImpl<detail::RefrigerationCompressor_Impl>()->transcriticalCompressorCapacityCurve();
}

bool RefrigerationCompressor::setRefrigerationCompressorPowerCurve(const CurveBicubic& curveBicubic) {
  return getImpl<detail::RefrigerationCompressor_Impl>()->setRefrigerationCompressorPowerCurve(curveBicubic);
}

bool RefrigerationCompressor::setRefrigerationCompressorCapacityCurve(const CurveBicubic& curveBicubic) {
  return getImpl<detail::RefrigerationCompressor_Impl>()->setRefrigerationCompressorCapacityCurve(curveBicubic);
}

void RefrigerationCompressor::setRatedSuperheat(double ratedSuperheat) {
  getImpl<detail::RefrigerationCompressor_Impl>()->setRatedSuperheat(ratedSuperheat);
}

void RefrigerationCompressor::resetRatedSuperheat() {
  getImpl<detail::RefrigerationCompressor_Impl>()->resetRatedSuperheat();
}

void RefrigerationCompressor::setRatedReturnGasTemperature(double ratedReturnGasTemperature) {
  getImpl<detail::RefrigerationCompressor_Impl>()->setRatedReturnGasTemperature(ratedReturnGasTemperature);
}

void RefrigerationCompressor::resetRatedReturnGasTemperature() {
  getImpl<detail::RefrigerationCompressor_Impl>()->resetRatedReturnGasTemperature();
}

void RefrigerationCompressor::setRatedLiquidTemperature(double ratedLiquidTemperature) {
  getImpl<detail::RefrigerationCompressor_Impl>()->setRatedLiquidTemperature(ratedLiquidTemperature);
}

void RefrigerationCompressor::resetRatedLiquidTemperature() {
  getImpl<detail::RefrigerationCompressor_Impl>()->resetRatedLiquidTemperature();
}

void RefrigerationCompressor::setRatedSubcooling(double ratedSubcooling) {
  getImpl<detail::RefrigerationCompressor_Impl>()->setRatedSubcooling(ratedSubcooling);
}

void RefrigerationCompressor::resetRatedSubcooling() {
  getImpl<detail::RefrigerationCompressor_Impl>()->resetRatedSubcooling();
}

void RefrigerationCompressor::setEndUseSubcategory(std::string endUseSubcategory) {
  getImpl<detail::RefrigerationCompressor_Impl>()->setEndUseSubcategory(endUseSubcategory);
}

void RefrigerationCompressor::resetEndUseSubcategory() {
  getImpl<detail::RefrigerationCompressor_Impl>()->resetEndUseSubcategory();
}

bool RefrigerationCompressor::setModeofOperation(std::string modeofOperation) {
  return getImpl<detail::RefrigerationCompressor_Impl>()->setModeofOperation(modeofOperation);
}

void RefrigerationCompressor::resetModeofOperation() {
  getImpl<detail::RefrigerationCompressor_Impl>()->resetModeofOperation();
}

bool RefrigerationCompressor::setTranscriticalCompressorPowerCurve(const CurveBicubic& curveBicubic) {
  return getImpl<detail::RefrigerationCompressor_Impl>()->setTranscriticalCompressorPowerCurve(curveBicubic);
}

void RefrigerationCompressor::resetTranscriticalCompressorPowerCurve() {
  getImpl<detail::RefrigerationCompressor_Impl>()->resetTranscriticalCompressorPowerCurve();
}

bool RefrigerationCompressor::setTranscriticalCompressorCapacityCurve(const CurveBicubic& curveBicubic) {
  return getImpl<detail::RefrigerationCompressor_Impl>()->setTranscriticalCompressorCapacityCurve(curveBicubic);
}

void RefrigerationCompressor::resetTranscriticalCompressorCapacityCurve() {
  getImpl<detail::RefrigerationCompressor_Impl>()->resetTranscriticalCompressorCapacityCurve();
}

/// @cond
RefrigerationCompressor::RefrigerationCompressor(boost::shared_ptr<detail::RefrigerationCompressor_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

