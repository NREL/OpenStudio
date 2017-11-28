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

#include "RefrigerationCompressor.hpp"
#include "RefrigerationCompressor_Impl.hpp"

#include "CurveBicubic.hpp"
#include "CurveBicubic_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Refrigeration_Compressor_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  RefrigerationCompressor_Impl::RefrigerationCompressor_Impl(const IdfObject& idfObject,
                                                             Model_Impl* model,
                                                             bool keepHandle)
    : ParentObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == RefrigerationCompressor::iddObjectType());
  }

  RefrigerationCompressor_Impl::RefrigerationCompressor_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                             Model_Impl* model,
                                                             bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == RefrigerationCompressor::iddObjectType());
  }

  RefrigerationCompressor_Impl::RefrigerationCompressor_Impl(const RefrigerationCompressor_Impl& other,
                                                             Model_Impl* model,
                                                             bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
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

  ModelObject RefrigerationCompressor_Impl::clone(Model model) const
  {
    RefrigerationCompressor modelObjectClone = ModelObject_Impl::clone(model).cast<RefrigerationCompressor>();

    return modelObjectClone;
  }

  std::vector<IddObjectType> RefrigerationCompressor_Impl::allowableChildTypes() const
  {
    std::vector<IddObjectType> result;
    result.push_back(IddObjectType::OS_Curve_Bicubic);
    return result;
  }

  std::vector<ModelObject> RefrigerationCompressor_Impl::children() const
  {
    std::vector<ModelObject> result;
    boost::optional<CurveBicubic> intermediate;
    if ( (intermediate = refrigerationCompressorPowerCurve()) ) {
      result.push_back(*intermediate);
    }
    if ( (intermediate = refrigerationCompressorCapacityCurve()) ) {
      result.push_back(*intermediate);
    }
    if ( (intermediate = transcriticalCompressorPowerCurve()) ) {
      result.push_back(*intermediate);
    }
    if ( (intermediate = transcriticalCompressorCapacityCurve()) ) {
      result.push_back(*intermediate);
    }
    return result;
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
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationCompressor_Impl::isEndUseSubcategoryDefaulted() const {
    return isEmpty(OS_Refrigeration_CompressorFields::EndUseSubcategory);
  }

  std::string RefrigerationCompressor_Impl::modeofOperation() const {
    if ( transcriticalCompressorPowerCurve() && transcriticalCompressorCapacityCurve() ) {
      return "Transcritical";
    }
    else {
      return "Subcritical";
    }
   // boost::optional<std::string> value = getString(OS_Refrigeration_CompressorFields::ModeofOperation,true);
   // OS_ASSERT(value);
   // return value.get();
  }

  // bool RefrigerationCompressor_Impl::isModeofOperationDefaulted() const {
  //  return isEmpty(OS_Refrigeration_CompressorFields::ModeofOperation);
  // }

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
      resetRatedReturnGasTemperature();
    }
    else {
      resetRatedSuperheat();
      result = true;
    }
    OS_ASSERT(result);
  }

  void RefrigerationCompressor_Impl::resetRatedSuperheat() {
    bool result = setString(OS_Refrigeration_CompressorFields::RatedSuperheat, "");
    OS_ASSERT(result);
  }

  void RefrigerationCompressor_Impl::setRatedReturnGasTemperature(boost::optional<double> ratedReturnGasTemperature) {
    bool result(false);
    if (ratedReturnGasTemperature) {
      result = setDouble(OS_Refrigeration_CompressorFields::RatedReturnGasTemperature, ratedReturnGasTemperature.get());
      resetRatedSuperheat();
    }
    else {
      resetRatedReturnGasTemperature();
      result = true;
    }
    OS_ASSERT(result);
  }

  void RefrigerationCompressor_Impl::resetRatedReturnGasTemperature() {
    bool result = setString(OS_Refrigeration_CompressorFields::RatedReturnGasTemperature, "");
    OS_ASSERT(result);
  }

  void RefrigerationCompressor_Impl::setRatedLiquidTemperature(boost::optional<double> ratedLiquidTemperature) {
    bool result(false);
    if (ratedLiquidTemperature) {
      result = setDouble(OS_Refrigeration_CompressorFields::RatedLiquidTemperature, ratedLiquidTemperature.get());
      resetRatedSubcooling();
    }
    else {
      resetRatedLiquidTemperature();
      result = true;
    }
    OS_ASSERT(result);
  }

  void RefrigerationCompressor_Impl::resetRatedLiquidTemperature() {
    bool result = setString(OS_Refrigeration_CompressorFields::RatedLiquidTemperature, "");
    OS_ASSERT(result);
  }

  void RefrigerationCompressor_Impl::setRatedSubcooling(boost::optional<double> ratedSubcooling) {
    bool result(false);
    if (ratedSubcooling) {
      result = setDouble(OS_Refrigeration_CompressorFields::RatedSubcooling, ratedSubcooling.get());
      resetRatedLiquidTemperature();
    }
    else {
      resetRatedSubcooling();
      result = true;
    }
    OS_ASSERT(result);
  }

  void RefrigerationCompressor_Impl::resetRatedSubcooling() {
    bool result = setString(OS_Refrigeration_CompressorFields::RatedSubcooling, "");
    OS_ASSERT(result);
  }

  void RefrigerationCompressor_Impl::setEndUseSubcategory(std::string endUseSubcategory) {
    bool result = setString(OS_Refrigeration_CompressorFields::EndUseSubcategory, endUseSubcategory);
    OS_ASSERT(result);
  }

  void RefrigerationCompressor_Impl::resetEndUseSubcategory() {
    bool result = setString(OS_Refrigeration_CompressorFields::EndUseSubcategory, "");
    OS_ASSERT(result);
  }

  // bool RefrigerationCompressor_Impl::setModeofOperation(std::string modeofOperation) {
  //  bool result = setString(OS_Refrigeration_CompressorFields::ModeofOperation, modeofOperation);
  //  return result;
  // }

  // void RefrigerationCompressor_Impl::resetModeofOperation() {
  //  bool result = setString(OS_Refrigeration_CompressorFields::ModeofOperation, "");
  //  OS_ASSERT(result);
  // }

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
    OS_ASSERT(result);
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
    OS_ASSERT(result);
  }

  boost::optional<CurveBicubic> RefrigerationCompressor_Impl::optionalRefrigerationCompressorPowerCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveBicubic>(OS_Refrigeration_CompressorFields::RefrigerationCompressorPowerCurveName);
  }

  boost::optional<CurveBicubic> RefrigerationCompressor_Impl::optionalRefrigerationCompressorCapacityCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveBicubic>(OS_Refrigeration_CompressorFields::RefrigerationCompressorCapacityCurveName);
  }

} // detail

RefrigerationCompressor::RefrigerationCompressor(const Model& model)
  : ParentObject(RefrigerationCompressor::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::RefrigerationCompressor_Impl>());

  CurveBicubic powerCurve = CurveBicubic(model);
  powerCurve.setName("Refrigeration Compressor Power Curve");
  powerCurve.setCoefficient1Constant(4451.46);
  powerCurve.setCoefficient2x(-166.108);
  powerCurve.setCoefficient3xPOW2(-5.35664);
  powerCurve.setCoefficient4y(263.553);
  powerCurve.setCoefficient5yPOW2(-0.958849);
  powerCurve.setCoefficient6xTIMESY(8.09252);
  powerCurve.setCoefficient7xPOW3(-0.0239354);
  powerCurve.setCoefficient8yPOW3(0.000633306);
  powerCurve.setCoefficient9xPOW2TIMESY(0.0778798);
  powerCurve.setCoefficient10xTIMESYPOW2(-0.0167386);
  powerCurve.setMinimumValueofx(-23.3);
  powerCurve.setMaximumValueofx(7.2);
  powerCurve.setMinimumValueofy(10.0);
  powerCurve.setMaximumValueofy(60.0);
  powerCurve.setInputUnitTypeforX("Temperature");
  powerCurve.setInputUnitTypeforY("Temperature");
  powerCurve.setOutputUnitType("Power");

  bool ok = setRefrigerationCompressorPowerCurve(powerCurve);
  OS_ASSERT(ok);

  CurveBicubic capacityCurve = CurveBicubic(model);
  capacityCurve.setName("Refrigeration Compressor Capacity Curve");
  capacityCurve.setCoefficient1Constant(83249.9);
  capacityCurve.setCoefficient2x(3147.96);
  capacityCurve.setCoefficient3xPOW2(44.9276);
  capacityCurve.setCoefficient4y(-1035.51);
  capacityCurve.setCoefficient5yPOW2(3.74692);
  capacityCurve.setCoefficient6xTIMESY(-37.3907);
  capacityCurve.setCoefficient7xPOW3(0.179897);
  capacityCurve.setCoefficient8yPOW3(-0.0199971);
  capacityCurve.setCoefficient9xPOW2TIMESY(-0.481641);
  capacityCurve.setCoefficient10xTIMESYPOW2(0.0428673);
  capacityCurve.setMinimumValueofx(-23.3);
  capacityCurve.setMaximumValueofx(7.2);
  capacityCurve.setMinimumValueofy(10.0);
  capacityCurve.setMaximumValueofy(60.0);
  capacityCurve.setInputUnitTypeforX("Temperature");
  capacityCurve.setInputUnitTypeforY("Temperature");
  capacityCurve.setOutputUnitType("Capacity");

  ok = setRefrigerationCompressorCapacityCurve(capacityCurve);
  OS_ASSERT(ok);

  setRatedReturnGasTemperature(18.3);
  setRatedSubcooling(0.0);
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

// bool RefrigerationCompressor::isModeofOperationDefaulted() const {
//   return getImpl<detail::RefrigerationCompressor_Impl>()->isModeofOperationDefaulted();
// }

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

// bool RefrigerationCompressor::setModeofOperation(std::string modeofOperation) {
//   return getImpl<detail::RefrigerationCompressor_Impl>()->setModeofOperation(modeofOperation);
// }

// void RefrigerationCompressor::resetModeofOperation() {
//   getImpl<detail::RefrigerationCompressor_Impl>()->resetModeofOperation();
// }

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
RefrigerationCompressor::RefrigerationCompressor(std::shared_ptr<detail::RefrigerationCompressor_Impl> impl)
  : ParentObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

