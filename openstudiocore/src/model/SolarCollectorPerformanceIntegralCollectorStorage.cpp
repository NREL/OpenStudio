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

#include "SolarCollectorPerformanceIntegralCollectorStorage.hpp"
#include "SolarCollectorPerformanceIntegralCollectorStorage_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_SolarCollectorPerformance_IntegralCollectorStorage_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  SolarCollectorPerformanceIntegralCollectorStorage_Impl::SolarCollectorPerformanceIntegralCollectorStorage_Impl(const IdfObject& idfObject,
                                                                                                                 Model_Impl* model,
                                                                                                                 bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SolarCollectorPerformanceIntegralCollectorStorage::iddObjectType());
  }

  SolarCollectorPerformanceIntegralCollectorStorage_Impl::SolarCollectorPerformanceIntegralCollectorStorage_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                                                 Model_Impl* model,
                                                                                                                 bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SolarCollectorPerformanceIntegralCollectorStorage::iddObjectType());
  }

  SolarCollectorPerformanceIntegralCollectorStorage_Impl::SolarCollectorPerformanceIntegralCollectorStorage_Impl(const SolarCollectorPerformanceIntegralCollectorStorage_Impl& other,
                                                                                                                 Model_Impl* model,
                                                                                                                 bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& SolarCollectorPerformanceIntegralCollectorStorage_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType SolarCollectorPerformanceIntegralCollectorStorage_Impl::iddObjectType() const {
    return SolarCollectorPerformanceIntegralCollectorStorage::iddObjectType();
  }

  std::string SolarCollectorPerformanceIntegralCollectorStorage_Impl::iCSCollectorType() const {
    boost::optional<std::string> value = getString(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::ICSCollectorType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SolarCollectorPerformanceIntegralCollectorStorage_Impl::isICSCollectorTypeDefaulted() const {
    return isEmpty(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::ICSCollectorType);
  }

  double SolarCollectorPerformanceIntegralCollectorStorage_Impl::grossArea() const {
    OptionalDouble d = getDouble(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::GrossArea, true);
    OS_ASSERT(d);
    return *d;
  }

  double SolarCollectorPerformanceIntegralCollectorStorage_Impl::collectorWaterVolume() const {
    OptionalDouble d = getDouble(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::CollectorWaterVolume,true);
    OS_ASSERT(d);
    return *d;
  }

  double SolarCollectorPerformanceIntegralCollectorStorage_Impl::bottomHeatLossConductance() const {
    boost::optional<double> value = getDouble(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::BottomHeatLossConductance,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SolarCollectorPerformanceIntegralCollectorStorage_Impl::isBottomHeatLossConductanceDefaulted() const {
    return isEmpty(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::BottomHeatLossConductance);
  }

  double SolarCollectorPerformanceIntegralCollectorStorage_Impl::sideHeatLossConductance() const {
    boost::optional<double> value = getDouble(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::SideHeatLossConductance,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SolarCollectorPerformanceIntegralCollectorStorage_Impl::isSideHeatLossConductanceDefaulted() const {
    return isEmpty(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::SideHeatLossConductance);
  }

  double SolarCollectorPerformanceIntegralCollectorStorage_Impl::aspectRatio() const {
    boost::optional<double> value = getDouble(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::AspectRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SolarCollectorPerformanceIntegralCollectorStorage_Impl::isAspectRatioDefaulted() const {
    return isEmpty(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::AspectRatio);
  }

  double SolarCollectorPerformanceIntegralCollectorStorage_Impl::collectorSideHeight() const {
    boost::optional<double> value = getDouble(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::CollectorSideHeight,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SolarCollectorPerformanceIntegralCollectorStorage_Impl::isCollectorSideHeightDefaulted() const {
    return isEmpty(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::CollectorSideHeight);
  }

  double SolarCollectorPerformanceIntegralCollectorStorage_Impl::thermalMassOfAbsorberPlate() const {
    boost::optional<double> value = getDouble(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::ThermalMassofAbsorberPlate,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SolarCollectorPerformanceIntegralCollectorStorage_Impl::isThermalMassOfAbsorberPlateDefaulted() const {
    return isEmpty(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::ThermalMassofAbsorberPlate);
  }

  int SolarCollectorPerformanceIntegralCollectorStorage_Impl::numberOfCovers() const {
    boost::optional<int> value = getInt(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::NumberofCovers,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SolarCollectorPerformanceIntegralCollectorStorage_Impl::isNumberOfCoversDefaulted() const {
    return isEmpty(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::NumberofCovers);
  }

  double SolarCollectorPerformanceIntegralCollectorStorage_Impl::coverSpacing() const {
    boost::optional<double> value = getDouble(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::CoverSpacing,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SolarCollectorPerformanceIntegralCollectorStorage_Impl::isCoverSpacingDefaulted() const {
    return isEmpty(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::CoverSpacing);
  }

  double SolarCollectorPerformanceIntegralCollectorStorage_Impl::refractiveIndexOfOuterCover() const {
    boost::optional<double> value = getDouble(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::RefractiveIndexofOuterCover,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SolarCollectorPerformanceIntegralCollectorStorage_Impl::isRefractiveIndexOfOuterCoverDefaulted() const {
    return isEmpty(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::RefractiveIndexofOuterCover);
  }

  double SolarCollectorPerformanceIntegralCollectorStorage_Impl::extinctionCoefficientTimesThicknessOfOuterCover() const {
    boost::optional<double> value = getDouble(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::ExtinctionCoefficientTimesThicknessofOuterCover,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SolarCollectorPerformanceIntegralCollectorStorage_Impl::isExtinctionCoefficientTimesThicknessOfOuterCoverDefaulted() const {
    return isEmpty(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::ExtinctionCoefficientTimesThicknessofOuterCover);
  }

  double SolarCollectorPerformanceIntegralCollectorStorage_Impl::emissivityOfOuterCover() const {
    boost::optional<double> value = getDouble(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::EmissivityofOuterCover,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SolarCollectorPerformanceIntegralCollectorStorage_Impl::isEmissivityOfOuterCoverDefaulted() const {
    return isEmpty(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::EmissivityofOuterCover);
  }

  double SolarCollectorPerformanceIntegralCollectorStorage_Impl::refractiveIndexOfInnerCover() const {
    boost::optional<double> value = getDouble(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::RefractiveIndexofInnerCover,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SolarCollectorPerformanceIntegralCollectorStorage_Impl::isRefractiveIndexOfInnerCoverDefaulted() const {
    return isEmpty(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::RefractiveIndexofInnerCover);
  }

  double SolarCollectorPerformanceIntegralCollectorStorage_Impl::extinctionCoefficientTimesThicknessOfTheInnerCover() const {
    boost::optional<double> value = getDouble(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::ExtinctionCoefficientTimesThicknessoftheinnerCover,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SolarCollectorPerformanceIntegralCollectorStorage_Impl::isExtinctionCoefficientTimesThicknessOfTheInnerCoverDefaulted() const {
    return isEmpty(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::ExtinctionCoefficientTimesThicknessoftheinnerCover);
  }

  double SolarCollectorPerformanceIntegralCollectorStorage_Impl::emissivityOfInnerCover() const {
    boost::optional<double> value = getDouble(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::EmissivityofInnerCover,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SolarCollectorPerformanceIntegralCollectorStorage_Impl::isEmissivityOfInnerCoverDefaulted() const {
    return isEmpty(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::EmissivityofInnerCover);
  }

  double SolarCollectorPerformanceIntegralCollectorStorage_Impl::absorptanceOfAbsorberPlate() const {
    boost::optional<double> value = getDouble(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::AbsorptanceofAbsorberPlate,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SolarCollectorPerformanceIntegralCollectorStorage_Impl::isAbsorptanceOfAbsorberPlateDefaulted() const {
    return isEmpty(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::AbsorptanceofAbsorberPlate);
  }

  double SolarCollectorPerformanceIntegralCollectorStorage_Impl::emissivityOfAbsorberPlate() const {
    boost::optional<double> value = getDouble(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::EmissivityofAbsorberPlate,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SolarCollectorPerformanceIntegralCollectorStorage_Impl::isEmissivityOfAbsorberPlateDefaulted() const {
    return isEmpty(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::EmissivityofAbsorberPlate);
  }

  bool SolarCollectorPerformanceIntegralCollectorStorage_Impl::setICSCollectorType(std::string iCSCollectorType) {
    bool result = setString(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::ICSCollectorType, iCSCollectorType);
    return result;
  }

  void SolarCollectorPerformanceIntegralCollectorStorage_Impl::resetICSCollectorType() {
    bool result = setString(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::ICSCollectorType, "");
    OS_ASSERT(result);
  }

  bool SolarCollectorPerformanceIntegralCollectorStorage_Impl::setGrossArea(double grossArea) {
    return setDouble(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::GrossArea, grossArea);
  }

  //void SolarCollectorPerformanceIntegralCollectorStorage_Impl::resetGrossArea() {
  //  bool result = setString(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::GrossArea, "");
  //  OS_ASSERT(result);
  //}

  bool SolarCollectorPerformanceIntegralCollectorStorage_Impl::setCollectorWaterVolume(double collectorWaterVolume) {
    return setDouble(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::CollectorWaterVolume, collectorWaterVolume);
  }

  //void SolarCollectorPerformanceIntegralCollectorStorage_Impl::resetCollectorWaterVolume() {
  //  bool result = setString(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::CollectorWaterVolume, "");
  //  OS_ASSERT(result);
  //}

  bool SolarCollectorPerformanceIntegralCollectorStorage_Impl::setBottomHeatLossConductance(double bottomHeatLossConductance) {
    bool result = setDouble(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::BottomHeatLossConductance, bottomHeatLossConductance);
    return result;
  }

  void SolarCollectorPerformanceIntegralCollectorStorage_Impl::resetBottomHeatLossConductance() {
    bool result = setString(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::BottomHeatLossConductance, "");
    OS_ASSERT(result);
  }

  bool SolarCollectorPerformanceIntegralCollectorStorage_Impl::setSideHeatLossConductance(double sideHeatLossConductance) {
    bool result = setDouble(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::SideHeatLossConductance, sideHeatLossConductance);
    return result;
  }

  void SolarCollectorPerformanceIntegralCollectorStorage_Impl::resetSideHeatLossConductance() {
    bool result = setString(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::SideHeatLossConductance, "");
    OS_ASSERT(result);
  }

  bool SolarCollectorPerformanceIntegralCollectorStorage_Impl::setAspectRatio(double aspectRatio) {
    bool result = setDouble(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::AspectRatio, aspectRatio);
    return result;
  }

  void SolarCollectorPerformanceIntegralCollectorStorage_Impl::resetAspectRatio() {
    bool result = setString(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::AspectRatio, "");
    OS_ASSERT(result);
  }

  bool SolarCollectorPerformanceIntegralCollectorStorage_Impl::setCollectorSideHeight(double collectorSideHeight) {
    bool result = setDouble(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::CollectorSideHeight, collectorSideHeight);
    return result;
  }

  void SolarCollectorPerformanceIntegralCollectorStorage_Impl::resetCollectorSideHeight() {
    bool result = setString(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::CollectorSideHeight, "");
    OS_ASSERT(result);
  }

  bool SolarCollectorPerformanceIntegralCollectorStorage_Impl::setThermalMassOfAbsorberPlate(double thermalMassOfAbsorberPlate) {
    bool result = setDouble(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::ThermalMassofAbsorberPlate, thermalMassOfAbsorberPlate);
    return result;
  }

  void SolarCollectorPerformanceIntegralCollectorStorage_Impl::resetThermalMassOfAbsorberPlate() {
    bool result = setString(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::ThermalMassofAbsorberPlate, "");
    OS_ASSERT(result);
  }

  bool SolarCollectorPerformanceIntegralCollectorStorage_Impl::setNumberOfCovers(int numberOfCovers) {
    bool result = setInt(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::NumberofCovers, numberOfCovers);
    return result;
  }

  void SolarCollectorPerformanceIntegralCollectorStorage_Impl::resetNumberOfCovers() {
    bool result = setString(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::NumberofCovers, "");
    OS_ASSERT(result);
  }

  bool SolarCollectorPerformanceIntegralCollectorStorage_Impl::setCoverSpacing(double coverSpacing) {
    bool result = setDouble(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::CoverSpacing, coverSpacing);
    return result;
  }

  void SolarCollectorPerformanceIntegralCollectorStorage_Impl::resetCoverSpacing() {
    bool result = setString(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::CoverSpacing, "");
    OS_ASSERT(result);
  }

  bool SolarCollectorPerformanceIntegralCollectorStorage_Impl::setRefractiveIndexOfOuterCover(double refractiveIndexOfOuterCover) {
    bool result = setDouble(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::RefractiveIndexofOuterCover, refractiveIndexOfOuterCover);
    return result;
  }

  void SolarCollectorPerformanceIntegralCollectorStorage_Impl::resetRefractiveIndexOfOuterCover() {
    bool result = setString(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::RefractiveIndexofOuterCover, "");
    OS_ASSERT(result);
  }

  bool SolarCollectorPerformanceIntegralCollectorStorage_Impl::setExtinctionCoefficientTimesThicknessOfOuterCover(double extinctionCoefficientTimesThicknessOfOuterCover) {
    bool result = setDouble(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::ExtinctionCoefficientTimesThicknessofOuterCover, extinctionCoefficientTimesThicknessOfOuterCover);
    return result;
  }

  void SolarCollectorPerformanceIntegralCollectorStorage_Impl::resetExtinctionCoefficientTimesThicknessOfOuterCover() {
    bool result = setString(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::ExtinctionCoefficientTimesThicknessofOuterCover, "");
    OS_ASSERT(result);
  }

  bool SolarCollectorPerformanceIntegralCollectorStorage_Impl::setEmissivityOfOuterCover(double emissivityOfOuterCover) {
    bool result = setDouble(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::EmissivityofOuterCover, emissivityOfOuterCover);
    return result;
  }

  void SolarCollectorPerformanceIntegralCollectorStorage_Impl::resetEmissivityOfOuterCover() {
    bool result = setString(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::EmissivityofOuterCover, "");
    OS_ASSERT(result);
  }

  bool SolarCollectorPerformanceIntegralCollectorStorage_Impl::setRefractiveIndexOfInnerCover(double refractiveIndexOfInnerCover) {
    bool result = setDouble(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::RefractiveIndexofInnerCover, refractiveIndexOfInnerCover);
    return result;
  }

  void SolarCollectorPerformanceIntegralCollectorStorage_Impl::resetRefractiveIndexOfInnerCover() {
    bool result = setString(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::RefractiveIndexofInnerCover, "");
    OS_ASSERT(result);
  }

  bool SolarCollectorPerformanceIntegralCollectorStorage_Impl::setExtinctionCoefficientTimesThicknessOfTheInnerCover(double extinctionCoefficientTimesThicknessOfTheInnerCover) {
    bool result = setDouble(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::ExtinctionCoefficientTimesThicknessoftheinnerCover, extinctionCoefficientTimesThicknessOfTheInnerCover);
    return result;
  }

  void SolarCollectorPerformanceIntegralCollectorStorage_Impl::resetExtinctionCoefficientTimesThicknessOfTheInnerCover() {
    bool result = setString(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::ExtinctionCoefficientTimesThicknessoftheinnerCover, "");
    OS_ASSERT(result);
  }

  bool SolarCollectorPerformanceIntegralCollectorStorage_Impl::setEmissivityOfInnerCover(double emissivityOfInnerCover) {
    bool result = setDouble(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::EmissivityofInnerCover, emissivityOfInnerCover);
    return result;
  }

  void SolarCollectorPerformanceIntegralCollectorStorage_Impl::resetEmissivityOfInnerCover() {
    bool result = setString(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::EmissivityofInnerCover, "");
    OS_ASSERT(result);
  }

  bool SolarCollectorPerformanceIntegralCollectorStorage_Impl::setAbsorptanceOfAbsorberPlate(double absorptanceOfAbsorberPlate) {
    bool result = setDouble(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::AbsorptanceofAbsorberPlate, absorptanceOfAbsorberPlate);
    return result;
  }

  void SolarCollectorPerformanceIntegralCollectorStorage_Impl::resetAbsorptanceOfAbsorberPlate() {
    bool result = setString(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::AbsorptanceofAbsorberPlate, "");
    OS_ASSERT(result);
  }

  bool SolarCollectorPerformanceIntegralCollectorStorage_Impl::setEmissivityOfAbsorberPlate(double emissivityOfAbsorberPlate) {
    bool result = setDouble(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::EmissivityofAbsorberPlate, emissivityOfAbsorberPlate);
    return result;
  }

  void SolarCollectorPerformanceIntegralCollectorStorage_Impl::resetEmissivityOfAbsorberPlate() {
    bool result = setString(OS_SolarCollectorPerformance_IntegralCollectorStorageFields::EmissivityofAbsorberPlate, "");
    OS_ASSERT(result);
  }

} // detail

SolarCollectorPerformanceIntegralCollectorStorage::SolarCollectorPerformanceIntegralCollectorStorage(const Model& model)
  : ModelObject(SolarCollectorPerformanceIntegralCollectorStorage::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>());

  // non-idd defaults from HP_wICSSolarCollector.idf
  bool ok = true;
  ok = setGrossArea(2.9800);
  OS_ASSERT(ok);
  ok = setCollectorWaterVolume(0.1862);
  OS_ASSERT(ok);

}

IddObjectType SolarCollectorPerformanceIntegralCollectorStorage::iddObjectType() {
  return IddObjectType(IddObjectType::OS_SolarCollectorPerformance_IntegralCollectorStorage);
}

std::vector<std::string> SolarCollectorPerformanceIntegralCollectorStorage::iCSCollectorTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SolarCollectorPerformance_IntegralCollectorStorageFields::ICSCollectorType);
}

std::string SolarCollectorPerformanceIntegralCollectorStorage::iCSCollectorType() const {
  return getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->iCSCollectorType();
}

bool SolarCollectorPerformanceIntegralCollectorStorage::isICSCollectorTypeDefaulted() const {
  return getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->isICSCollectorTypeDefaulted();
}

double SolarCollectorPerformanceIntegralCollectorStorage::grossArea() const {
  return getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->grossArea();
}

double SolarCollectorPerformanceIntegralCollectorStorage::collectorWaterVolume() const {
  return getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->collectorWaterVolume();
}

double SolarCollectorPerformanceIntegralCollectorStorage::bottomHeatLossConductance() const {
  return getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->bottomHeatLossConductance();
}

bool SolarCollectorPerformanceIntegralCollectorStorage::isBottomHeatLossConductanceDefaulted() const {
  return getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->isBottomHeatLossConductanceDefaulted();
}

double SolarCollectorPerformanceIntegralCollectorStorage::sideHeatLossConductance() const {
  return getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->sideHeatLossConductance();
}

bool SolarCollectorPerformanceIntegralCollectorStorage::isSideHeatLossConductanceDefaulted() const {
  return getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->isSideHeatLossConductanceDefaulted();
}

double SolarCollectorPerformanceIntegralCollectorStorage::aspectRatio() const {
  return getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->aspectRatio();
}

bool SolarCollectorPerformanceIntegralCollectorStorage::isAspectRatioDefaulted() const {
  return getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->isAspectRatioDefaulted();
}

double SolarCollectorPerformanceIntegralCollectorStorage::collectorSideHeight() const {
  return getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->collectorSideHeight();
}

bool SolarCollectorPerformanceIntegralCollectorStorage::isCollectorSideHeightDefaulted() const {
  return getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->isCollectorSideHeightDefaulted();
}

double SolarCollectorPerformanceIntegralCollectorStorage::thermalMassOfAbsorberPlate() const {
  return getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->thermalMassOfAbsorberPlate();
}

bool SolarCollectorPerformanceIntegralCollectorStorage::isThermalMassOfAbsorberPlateDefaulted() const {
  return getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->isThermalMassOfAbsorberPlateDefaulted();
}

int SolarCollectorPerformanceIntegralCollectorStorage::numberOfCovers() const {
  return getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->numberOfCovers();
}

bool SolarCollectorPerformanceIntegralCollectorStorage::isNumberOfCoversDefaulted() const {
  return getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->isNumberOfCoversDefaulted();
}

double SolarCollectorPerformanceIntegralCollectorStorage::coverSpacing() const {
  return getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->coverSpacing();
}

bool SolarCollectorPerformanceIntegralCollectorStorage::isCoverSpacingDefaulted() const {
  return getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->isCoverSpacingDefaulted();
}

double SolarCollectorPerformanceIntegralCollectorStorage::refractiveIndexOfOuterCover() const {
  return getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->refractiveIndexOfOuterCover();
}

bool SolarCollectorPerformanceIntegralCollectorStorage::isRefractiveIndexOfOuterCoverDefaulted() const {
  return getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->isRefractiveIndexOfOuterCoverDefaulted();
}

double SolarCollectorPerformanceIntegralCollectorStorage::extinctionCoefficientTimesThicknessOfOuterCover() const {
  return getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->extinctionCoefficientTimesThicknessOfOuterCover();
}

bool SolarCollectorPerformanceIntegralCollectorStorage::isExtinctionCoefficientTimesThicknessOfOuterCoverDefaulted() const {
  return getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->isExtinctionCoefficientTimesThicknessOfOuterCoverDefaulted();
}

double SolarCollectorPerformanceIntegralCollectorStorage::emissivityOfOuterCover() const {
  return getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->emissivityOfOuterCover();
}

bool SolarCollectorPerformanceIntegralCollectorStorage::isEmissivityOfOuterCoverDefaulted() const {
  return getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->isEmissivityOfOuterCoverDefaulted();
}

double SolarCollectorPerformanceIntegralCollectorStorage::refractiveIndexOfInnerCover() const {
  return getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->refractiveIndexOfInnerCover();
}

bool SolarCollectorPerformanceIntegralCollectorStorage::isRefractiveIndexOfInnerCoverDefaulted() const {
  return getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->isRefractiveIndexOfInnerCoverDefaulted();
}

double SolarCollectorPerformanceIntegralCollectorStorage::extinctionCoefficientTimesThicknessOfTheInnerCover() const {
  return getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->extinctionCoefficientTimesThicknessOfTheInnerCover();
}

bool SolarCollectorPerformanceIntegralCollectorStorage::isExtinctionCoefficientTimesThicknessOfTheInnerCoverDefaulted() const {
  return getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->isExtinctionCoefficientTimesThicknessOfTheInnerCoverDefaulted();
}

double SolarCollectorPerformanceIntegralCollectorStorage::emissivityOfInnerCover() const {
  return getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->emissivityOfInnerCover();
}

bool SolarCollectorPerformanceIntegralCollectorStorage::isEmissivityOfInnerCoverDefaulted() const {
  return getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->isEmissivityOfInnerCoverDefaulted();
}

double SolarCollectorPerformanceIntegralCollectorStorage::absorptanceOfAbsorberPlate() const {
  return getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->absorptanceOfAbsorberPlate();
}

bool SolarCollectorPerformanceIntegralCollectorStorage::isAbsorptanceOfAbsorberPlateDefaulted() const {
  return getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->isAbsorptanceOfAbsorberPlateDefaulted();
}

double SolarCollectorPerformanceIntegralCollectorStorage::emissivityOfAbsorberPlate() const {
  return getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->emissivityOfAbsorberPlate();
}

bool SolarCollectorPerformanceIntegralCollectorStorage::isEmissivityOfAbsorberPlateDefaulted() const {
  return getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->isEmissivityOfAbsorberPlateDefaulted();
}

bool SolarCollectorPerformanceIntegralCollectorStorage::setICSCollectorType(std::string iCSCollectorType) {
  return getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->setICSCollectorType(iCSCollectorType);
}

void SolarCollectorPerformanceIntegralCollectorStorage::resetICSCollectorType() {
  getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->resetICSCollectorType();
}

bool SolarCollectorPerformanceIntegralCollectorStorage::setGrossArea(double grossArea) {
  return getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->setGrossArea(grossArea);
}

//void SolarCollectorPerformanceIntegralCollectorStorage::resetGrossArea() {
//  getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->resetGrossArea();
//}

bool SolarCollectorPerformanceIntegralCollectorStorage::setCollectorWaterVolume(double collectorWaterVolume) {
  return getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->setCollectorWaterVolume(collectorWaterVolume);
}

//void SolarCollectorPerformanceIntegralCollectorStorage::resetCollectorWaterVolume() {
//  getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->resetCollectorWaterVolume();
//}

bool SolarCollectorPerformanceIntegralCollectorStorage::setBottomHeatLossConductance(double bottomHeatLossConductance) {
  return getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->setBottomHeatLossConductance(bottomHeatLossConductance);
}

void SolarCollectorPerformanceIntegralCollectorStorage::resetBottomHeatLossConductance() {
  getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->resetBottomHeatLossConductance();
}

bool SolarCollectorPerformanceIntegralCollectorStorage::setSideHeatLossConductance(double sideHeatLossConductance) {
  return getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->setSideHeatLossConductance(sideHeatLossConductance);
}

void SolarCollectorPerformanceIntegralCollectorStorage::resetSideHeatLossConductance() {
  getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->resetSideHeatLossConductance();
}

bool SolarCollectorPerformanceIntegralCollectorStorage::setAspectRatio(double aspectRatio) {
  return getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->setAspectRatio(aspectRatio);
}

void SolarCollectorPerformanceIntegralCollectorStorage::resetAspectRatio() {
  getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->resetAspectRatio();
}

bool SolarCollectorPerformanceIntegralCollectorStorage::setCollectorSideHeight(double collectorSideHeight) {
  return getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->setCollectorSideHeight(collectorSideHeight);
}

void SolarCollectorPerformanceIntegralCollectorStorage::resetCollectorSideHeight() {
  getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->resetCollectorSideHeight();
}

bool SolarCollectorPerformanceIntegralCollectorStorage::setThermalMassOfAbsorberPlate(double thermalMassOfAbsorberPlate) {
  return getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->setThermalMassOfAbsorberPlate(thermalMassOfAbsorberPlate);
}

void SolarCollectorPerformanceIntegralCollectorStorage::resetThermalMassOfAbsorberPlate() {
  getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->resetThermalMassOfAbsorberPlate();
}

bool SolarCollectorPerformanceIntegralCollectorStorage::setNumberOfCovers(int numberOfCovers) {
  return getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->setNumberOfCovers(numberOfCovers);
}

void SolarCollectorPerformanceIntegralCollectorStorage::resetNumberOfCovers() {
  getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->resetNumberOfCovers();
}

bool SolarCollectorPerformanceIntegralCollectorStorage::setCoverSpacing(double coverSpacing) {
  return getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->setCoverSpacing(coverSpacing);
}

void SolarCollectorPerformanceIntegralCollectorStorage::resetCoverSpacing() {
  getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->resetCoverSpacing();
}

bool SolarCollectorPerformanceIntegralCollectorStorage::setRefractiveIndexOfOuterCover(double refractiveIndexOfOuterCover) {
  return getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->setRefractiveIndexOfOuterCover(refractiveIndexOfOuterCover);
}

void SolarCollectorPerformanceIntegralCollectorStorage::resetRefractiveIndexOfOuterCover() {
  getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->resetRefractiveIndexOfOuterCover();
}

bool SolarCollectorPerformanceIntegralCollectorStorage::setExtinctionCoefficientTimesThicknessOfOuterCover(double extinctionCoefficientTimesThicknessOfOuterCover) {
  return getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->setExtinctionCoefficientTimesThicknessOfOuterCover(extinctionCoefficientTimesThicknessOfOuterCover);
}

void SolarCollectorPerformanceIntegralCollectorStorage::resetExtinctionCoefficientTimesThicknessOfOuterCover() {
  getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->resetExtinctionCoefficientTimesThicknessOfOuterCover();
}

bool SolarCollectorPerformanceIntegralCollectorStorage::setEmissivityOfOuterCover(double emissivityOfOuterCover) {
  return getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->setEmissivityOfOuterCover(emissivityOfOuterCover);
}

void SolarCollectorPerformanceIntegralCollectorStorage::resetEmissivityOfOuterCover() {
  getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->resetEmissivityOfOuterCover();
}

bool SolarCollectorPerformanceIntegralCollectorStorage::setRefractiveIndexOfInnerCover(double refractiveIndexOfInnerCover) {
  return getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->setRefractiveIndexOfInnerCover(refractiveIndexOfInnerCover);
}

void SolarCollectorPerformanceIntegralCollectorStorage::resetRefractiveIndexOfInnerCover() {
  getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->resetRefractiveIndexOfInnerCover();
}

bool SolarCollectorPerformanceIntegralCollectorStorage::setExtinctionCoefficientTimesThicknessOfTheInnerCover(double extinctionCoefficientTimesThicknessOfTheInnerCover) {
  return getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->setExtinctionCoefficientTimesThicknessOfTheInnerCover(extinctionCoefficientTimesThicknessOfTheInnerCover);
}

void SolarCollectorPerformanceIntegralCollectorStorage::resetExtinctionCoefficientTimesThicknessOfTheInnerCover() {
  getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->resetExtinctionCoefficientTimesThicknessOfTheInnerCover();
}

bool SolarCollectorPerformanceIntegralCollectorStorage::setEmissivityOfInnerCover(double emissivityOfInnerCover) {
  return getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->setEmissivityOfInnerCover(emissivityOfInnerCover);
}

void SolarCollectorPerformanceIntegralCollectorStorage::resetEmissivityOfInnerCover() {
  getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->resetEmissivityOfInnerCover();
}

bool SolarCollectorPerformanceIntegralCollectorStorage::setAbsorptanceOfAbsorberPlate(double absorptanceOfAbsorberPlate) {
  return getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->setAbsorptanceOfAbsorberPlate(absorptanceOfAbsorberPlate);
}

void SolarCollectorPerformanceIntegralCollectorStorage::resetAbsorptanceOfAbsorberPlate() {
  getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->resetAbsorptanceOfAbsorberPlate();
}

bool SolarCollectorPerformanceIntegralCollectorStorage::setEmissivityOfAbsorberPlate(double emissivityOfAbsorberPlate) {
  return getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->setEmissivityOfAbsorberPlate(emissivityOfAbsorberPlate);
}

void SolarCollectorPerformanceIntegralCollectorStorage::resetEmissivityOfAbsorberPlate() {
  getImpl<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl>()->resetEmissivityOfAbsorberPlate();
}

/// @cond
SolarCollectorPerformanceIntegralCollectorStorage::SolarCollectorPerformanceIntegralCollectorStorage(std::shared_ptr<detail::SolarCollectorPerformanceIntegralCollectorStorage_Impl> impl)
  : ModelObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

