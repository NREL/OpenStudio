/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
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
#include "Model.hpp"
#include "Model_Impl.hpp"

#include "GeneratorFuelCell.hpp"
#include "GeneratorFuelCell_Impl.hpp"
#include "GeneratorFuelCellAuxiliaryHeater.hpp"
#include "GeneratorFuelCellAuxiliaryHeater_Impl.hpp"

#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Generator_FuelCell_AuxiliaryHeater_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  GeneratorFuelCellAuxiliaryHeater_Impl::GeneratorFuelCellAuxiliaryHeater_Impl(const IdfObject& idfObject,
                                                                               Model_Impl* model,
                                                                               bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == GeneratorFuelCellAuxiliaryHeater::iddObjectType());
  }

  GeneratorFuelCellAuxiliaryHeater_Impl::GeneratorFuelCellAuxiliaryHeater_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                               Model_Impl* model,
                                                                               bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == GeneratorFuelCellAuxiliaryHeater::iddObjectType());
  }

  GeneratorFuelCellAuxiliaryHeater_Impl::GeneratorFuelCellAuxiliaryHeater_Impl(const GeneratorFuelCellAuxiliaryHeater_Impl& other,
                                                                               Model_Impl* model,
                                                                               bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& GeneratorFuelCellAuxiliaryHeater_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType GeneratorFuelCellAuxiliaryHeater_Impl::iddObjectType() const {
    return GeneratorFuelCellAuxiliaryHeater::iddObjectType();
  }

  // This will clone both the GeneratorFuelCellExhaustGasToWaterHeatExchanger and its linked GeneratorFuelCell
  // and will return a reference to the GeneratorMicroTurbineHeatRecovery
  ModelObject GeneratorFuelCellAuxiliaryHeater_Impl::clone(Model model) const {

    // We call the parent generator's Clone method which will clone both the fuelCell and fuelCellHX
    GeneratorFuelCell fs = fuelCell();
    GeneratorFuelCell fsClone = fs.clone(model).cast<GeneratorFuelCell>();

    // We get the clone of the parent generator's MTHR so we can return that
    GeneratorFuelCellAuxiliaryHeater hxClone = fsClone.auxiliaryHeater();


    return hxClone;
  }

  std::vector<IddObjectType> GeneratorFuelCellAuxiliaryHeater_Impl::allowableChildTypes() const {
    std::vector<IddObjectType> result;
    return result;
  }

  // Returns the children object
  std::vector<ModelObject> GeneratorFuelCellAuxiliaryHeater_Impl::children() const {
    std::vector<ModelObject> result;

    return result;
  }

  // Get the parent GeneratorFuelCell
  GeneratorFuelCell GeneratorFuelCellAuxiliaryHeater_Impl::fuelCell() const {

    boost::optional<GeneratorFuelCell> value;
    for (const GeneratorFuelCell& fc : this->model().getConcreteModelObjects<GeneratorFuelCell>()) {
      if (boost::optional<GeneratorFuelCellAuxiliaryHeater> fcHX = fc.auxiliaryHeater()) {
        if (fcHX->handle() == this->handle()) {
          value = fc;
        }
      }
    }
    OS_ASSERT(value);
    return value.get();

  }

  double GeneratorFuelCellAuxiliaryHeater_Impl::excessAirRatio() const {
    boost::optional<double> value = getDouble(OS_Generator_FuelCell_AuxiliaryHeaterFields::ExcessAirRatio,true);
    if (!value) {
      LOG(Info, " does not have excessAirRatio.");
      return 0;
    }
    return value.get();
  }

  double GeneratorFuelCellAuxiliaryHeater_Impl::ancillaryPowerConstantTerm() const {
    boost::optional<double> value = getDouble(OS_Generator_FuelCell_AuxiliaryHeaterFields::AncillaryPowerConstantTerm, true);
    if (!value) {
      LOG(Info, " does not have ancillaryPowerConstantTerm.");
      return 0;
    }
    return value.get();
  }

  double GeneratorFuelCellAuxiliaryHeater_Impl::ancillaryPowerLinearTerm() const {
    boost::optional<double> value = getDouble(OS_Generator_FuelCell_AuxiliaryHeaterFields::AncillaryPowerLinearTerm, true);
    if (!value) {
      LOG(Info, " does not have ancillaryPowerLinearTerm.");
      return 0;
    }
    return value.get();
  }

  double GeneratorFuelCellAuxiliaryHeater_Impl::skinLossUFactorTimesAreaValue() const {
    boost::optional<double> value = getDouble(OS_Generator_FuelCell_AuxiliaryHeaterFields::SkinLossUFactorTimesAreaValue, true);
    if (!value) {
      LOG(Info, " does not have skinLossUFactorTimesAreaValue.");
      return 0;
    }
    return value.get();
  }

  std::string GeneratorFuelCellAuxiliaryHeater_Impl::skinLossDestination() const {
    boost::optional<std::string> value = getString(OS_Generator_FuelCell_AuxiliaryHeaterFields::SkinLossDestination,true);
    if (!value) {
      LOG(Info, " does not have skinLossDestination.");
      return "";
    }
    return value.get();
  }

  boost::optional<ThermalZone> GeneratorFuelCellAuxiliaryHeater_Impl::zonetoReceiveSkinLosses() const {
    return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_Generator_FuelCell_AuxiliaryHeaterFields::ZoneNametoReceiveSkinLosses);
  }

  std::string GeneratorFuelCellAuxiliaryHeater_Impl::heatingCapacityUnits() const {
    boost::optional<std::string> value = getString(OS_Generator_FuelCell_AuxiliaryHeaterFields::HeatingCapacityUnits,true);
    if (!value) {
      LOG(Info, " does not have heatingCapacityUnits.");
      return "";
    }
    return value.get();
  }

  double GeneratorFuelCellAuxiliaryHeater_Impl::maximumHeatingCapacityinWatts() const {
    boost::optional<double> value = getDouble(OS_Generator_FuelCell_AuxiliaryHeaterFields::MaximumHeatingCapacityinWatts, true);
    if (!value) {
      LOG(Info, " does not have maximumHeatingCapacityinWatts.");
      return 0;
    }
    return value.get();
  }

  double GeneratorFuelCellAuxiliaryHeater_Impl::minimumHeatingCapacityinWatts() const {
    boost::optional<double> value = getDouble(OS_Generator_FuelCell_AuxiliaryHeaterFields::MinimumHeatingCapacityinWatts, true);
    if (!value) {
      LOG(Info, " does not have minimumHeatingCapacityinWatts.");
      return 0;
    }
    return value.get();
  }

  double GeneratorFuelCellAuxiliaryHeater_Impl::minimumHeatingCapacityinKmolperSecond() const {
    boost::optional<double> value = getDouble(OS_Generator_FuelCell_AuxiliaryHeaterFields::MaximumHeatingCapacityinKmolperSecond, true);
    if (!value) {
      LOG(Info, " does not have minimumHeatingCapacityinKmolperSecond.");
      return 0;
    }
    return value.get();
  }

  double GeneratorFuelCellAuxiliaryHeater_Impl::maximumHeatingCapacityinKmolperSecond() const {
    boost::optional<double> value = getDouble(OS_Generator_FuelCell_AuxiliaryHeaterFields::MinimumHeatingCapacityinKmolperSecond, true);
    if (!value) {
      LOG(Info, " does not have maximumHeatingCapacityinKmolperSecond.");
      return 0;
    }
    return value.get();
  }

  void GeneratorFuelCellAuxiliaryHeater_Impl::setExcessAirRatio(double excessAirRatio) {
    bool result = setDouble(OS_Generator_FuelCell_AuxiliaryHeaterFields::ExcessAirRatio, excessAirRatio);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellAuxiliaryHeater_Impl::resetExcessAirRatio() {
    bool result = setDouble(OS_Generator_FuelCell_AuxiliaryHeaterFields::ExcessAirRatio, 0);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellAuxiliaryHeater_Impl::setAncillaryPowerConstantTerm(double ancillaryPowerConstantTerm) {
    bool result = setDouble(OS_Generator_FuelCell_AuxiliaryHeaterFields::AncillaryPowerConstantTerm, ancillaryPowerConstantTerm);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellAuxiliaryHeater_Impl::resetAncillaryPowerConstantTerm() {
    bool result = setDouble(OS_Generator_FuelCell_AuxiliaryHeaterFields::AncillaryPowerConstantTerm, 0);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellAuxiliaryHeater_Impl::setAncillaryPowerLinearTerm(double ancillaryPowerLinearTerm) {
    bool result = setDouble(OS_Generator_FuelCell_AuxiliaryHeaterFields::AncillaryPowerLinearTerm, ancillaryPowerLinearTerm);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellAuxiliaryHeater_Impl::resetAncillaryPowerLinearTerm() {
    bool result = setDouble(OS_Generator_FuelCell_AuxiliaryHeaterFields::AncillaryPowerLinearTerm, 0);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellAuxiliaryHeater_Impl::setSkinLossUFactorTimesAreaValue(double skinLossUFactorTimesAreaValue) {
    bool result = setDouble(OS_Generator_FuelCell_AuxiliaryHeaterFields::SkinLossUFactorTimesAreaValue, skinLossUFactorTimesAreaValue);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellAuxiliaryHeater_Impl::resetSkinLossUFactorTimesAreaValue() {
    bool result = setDouble(OS_Generator_FuelCell_AuxiliaryHeaterFields::SkinLossUFactorTimesAreaValue, 0.5);
    OS_ASSERT(result);
  }

  bool GeneratorFuelCellAuxiliaryHeater_Impl::setSkinLossDestination(const std::string& skinLossDestination) {
    bool result = setString(OS_Generator_FuelCell_AuxiliaryHeaterFields::SkinLossDestination, skinLossDestination);
    return result;
  }

  void GeneratorFuelCellAuxiliaryHeater_Impl::resetSkinLossDestination() {
    bool result = setString(OS_Generator_FuelCell_AuxiliaryHeaterFields::SkinLossDestination, "AirInletForFuelCell");
    OS_ASSERT(result);
  }

  bool GeneratorFuelCellAuxiliaryHeater_Impl::setZonetoReceiveSkinLosses(const ThermalZone& zone) {
    bool result = setPointer(OS_Generator_FuelCell_AuxiliaryHeaterFields::ZoneNametoReceiveSkinLosses, zone.handle());
    this->setSkinLossDestination("SurroundingZone");
    return result;
  }

  void GeneratorFuelCellAuxiliaryHeater_Impl::resetZonetoReceiveSkinLosses() {
    bool result = setString(OS_Generator_FuelCell_AuxiliaryHeaterFields::ZoneNametoReceiveSkinLosses, "");
    this->setSkinLossDestination("AirInletForFuelCell");
    OS_ASSERT(result);
  }

  bool GeneratorFuelCellAuxiliaryHeater_Impl::setHeatingCapacityUnits(const std::string& heatingCapacityUnits) {
    bool result = setString(OS_Generator_FuelCell_AuxiliaryHeaterFields::HeatingCapacityUnits, heatingCapacityUnits);
    return result;
  }

  void GeneratorFuelCellAuxiliaryHeater_Impl::resetHeatingCapacityUnits() {
    bool result = setString(OS_Generator_FuelCell_AuxiliaryHeaterFields::HeatingCapacityUnits, "Watts");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellAuxiliaryHeater_Impl::setMaximumHeatingCapacityinWatts(double maximumHeatingCapacityinWatts) {
    bool result = setDouble(OS_Generator_FuelCell_AuxiliaryHeaterFields::MaximumHeatingCapacityinWatts, maximumHeatingCapacityinWatts);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellAuxiliaryHeater_Impl::resetMaximumHeatingCapacityinWatts() {
    bool result = setDouble(OS_Generator_FuelCell_AuxiliaryHeaterFields::MaximumHeatingCapacityinWatts, 0);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellAuxiliaryHeater_Impl::setMinimumHeatingCapacityinWatts(double minimumHeatingCapacityinWatts) {
    bool result = setDouble(OS_Generator_FuelCell_AuxiliaryHeaterFields::MinimumHeatingCapacityinWatts, minimumHeatingCapacityinWatts);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellAuxiliaryHeater_Impl::resetMinimumHeatingCapacityinWatts() {
    bool result = setDouble(OS_Generator_FuelCell_AuxiliaryHeaterFields::MinimumHeatingCapacityinWatts, 0);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellAuxiliaryHeater_Impl::setMaximumHeatingCapacityinKmolperSecond(double maximumHeatingCapacityinKmolperSecond) {
    bool result = setDouble(OS_Generator_FuelCell_AuxiliaryHeaterFields::MaximumHeatingCapacityinKmolperSecond, maximumHeatingCapacityinKmolperSecond);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellAuxiliaryHeater_Impl::resetMaximumHeatingCapacityinKmolperSecond() {
    bool result = setDouble(OS_Generator_FuelCell_AuxiliaryHeaterFields::MaximumHeatingCapacityinKmolperSecond, 0);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellAuxiliaryHeater_Impl::setMinimumHeatingCapacityinKmolperSecond(double minimumHeatingCapacityinKmolperSecond) {
    bool result = setDouble(OS_Generator_FuelCell_AuxiliaryHeaterFields::MinimumHeatingCapacityinKmolperSecond, minimumHeatingCapacityinKmolperSecond);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellAuxiliaryHeater_Impl::resetMinimumHeatingCapacityinKmolperSecond() {
    bool result = setDouble(OS_Generator_FuelCell_AuxiliaryHeaterFields::MinimumHeatingCapacityinKmolperSecond, 0);
    OS_ASSERT(result);
  }

} // detail

GeneratorFuelCellAuxiliaryHeater::GeneratorFuelCellAuxiliaryHeater(const Model& model)
  : ModelObject(GeneratorFuelCellAuxiliaryHeater::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::GeneratorFuelCellAuxiliaryHeater_Impl>());
  setExcessAirRatio(0.0);
  setAncillaryPowerConstantTerm(0.0);
  setAncillaryPowerLinearTerm(0.0);
  setSkinLossUFactorTimesAreaValue(0.5);
  setSkinLossDestination("AirInletForFuelCell");
  setHeatingCapacityUnits("Watts");
  setMaximumHeatingCapacityinWatts(0.0);
  setMinimumHeatingCapacityinWatts(0.0);
}

IddObjectType GeneratorFuelCellAuxiliaryHeater::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Generator_FuelCell_AuxiliaryHeater);
}

std::vector<std::string> GeneratorFuelCellAuxiliaryHeater::skinLossDestinationValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Generator_FuelCell_AuxiliaryHeaterFields::SkinLossDestination);
}

std::vector<std::string> GeneratorFuelCellAuxiliaryHeater::heatingCapacityUnitsValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Generator_FuelCell_AuxiliaryHeaterFields::HeatingCapacityUnits);
}

double GeneratorFuelCellAuxiliaryHeater::excessAirRatio() const {
  return getImpl<detail::GeneratorFuelCellAuxiliaryHeater_Impl>()->excessAirRatio();
}

double GeneratorFuelCellAuxiliaryHeater::ancillaryPowerConstantTerm() const {
  return getImpl<detail::GeneratorFuelCellAuxiliaryHeater_Impl>()->ancillaryPowerConstantTerm();
}

double GeneratorFuelCellAuxiliaryHeater::ancillaryPowerLinearTerm() const {
  return getImpl<detail::GeneratorFuelCellAuxiliaryHeater_Impl>()->ancillaryPowerLinearTerm();
}

double GeneratorFuelCellAuxiliaryHeater::skinLossUFactorTimesAreaValue() const {
  return getImpl<detail::GeneratorFuelCellAuxiliaryHeater_Impl>()->skinLossUFactorTimesAreaValue();
}

std::string GeneratorFuelCellAuxiliaryHeater::skinLossDestination() const {
  return getImpl<detail::GeneratorFuelCellAuxiliaryHeater_Impl>()->skinLossDestination();
}

boost::optional<ThermalZone> GeneratorFuelCellAuxiliaryHeater::zonetoReceiveSkinLosses() const {
  return getImpl<detail::GeneratorFuelCellAuxiliaryHeater_Impl>()->zonetoReceiveSkinLosses();
}

std::string GeneratorFuelCellAuxiliaryHeater::heatingCapacityUnits() const {
  return getImpl<detail::GeneratorFuelCellAuxiliaryHeater_Impl>()->heatingCapacityUnits();
}

double GeneratorFuelCellAuxiliaryHeater::maximumHeatingCapacityinWatts() const {
  return getImpl<detail::GeneratorFuelCellAuxiliaryHeater_Impl>()->maximumHeatingCapacityinWatts();
}

double GeneratorFuelCellAuxiliaryHeater::minimumHeatingCapacityinWatts() const {
  return getImpl<detail::GeneratorFuelCellAuxiliaryHeater_Impl>()->minimumHeatingCapacityinWatts();
}

double GeneratorFuelCellAuxiliaryHeater::maximumHeatingCapacityinKmolperSecond() const {
  return getImpl<detail::GeneratorFuelCellAuxiliaryHeater_Impl>()->maximumHeatingCapacityinKmolperSecond();
}

double GeneratorFuelCellAuxiliaryHeater::minimumHeatingCapacityinKmolperSecond() const {
  return getImpl<detail::GeneratorFuelCellAuxiliaryHeater_Impl>()->minimumHeatingCapacityinKmolperSecond();
}

void GeneratorFuelCellAuxiliaryHeater::setExcessAirRatio(double excessAirRatio) {
  getImpl<detail::GeneratorFuelCellAuxiliaryHeater_Impl>()->setExcessAirRatio(excessAirRatio);
}

void GeneratorFuelCellAuxiliaryHeater::resetExcessAirRatio() {
  getImpl<detail::GeneratorFuelCellAuxiliaryHeater_Impl>()->resetExcessAirRatio();
}

void GeneratorFuelCellAuxiliaryHeater::setAncillaryPowerConstantTerm(double ancillaryPowerConstantTerm) {
  getImpl<detail::GeneratorFuelCellAuxiliaryHeater_Impl>()->setAncillaryPowerConstantTerm(ancillaryPowerConstantTerm);
}

void GeneratorFuelCellAuxiliaryHeater::resetAncillaryPowerConstantTerm() {
  getImpl<detail::GeneratorFuelCellAuxiliaryHeater_Impl>()->resetAncillaryPowerConstantTerm();
}

void GeneratorFuelCellAuxiliaryHeater::setAncillaryPowerLinearTerm(double ancillaryPowerLinearTerm) {
  getImpl<detail::GeneratorFuelCellAuxiliaryHeater_Impl>()->setAncillaryPowerLinearTerm(ancillaryPowerLinearTerm);
}

void GeneratorFuelCellAuxiliaryHeater::resetAncillaryPowerLinearTerm() {
  getImpl<detail::GeneratorFuelCellAuxiliaryHeater_Impl>()->resetAncillaryPowerLinearTerm();
}

void GeneratorFuelCellAuxiliaryHeater::setSkinLossUFactorTimesAreaValue(double skinLossUFactorTimesAreaValue) {
  getImpl<detail::GeneratorFuelCellAuxiliaryHeater_Impl>()->setSkinLossUFactorTimesAreaValue(skinLossUFactorTimesAreaValue);
}

void GeneratorFuelCellAuxiliaryHeater::resetSkinLossUFactorTimesAreaValue() {
  getImpl<detail::GeneratorFuelCellAuxiliaryHeater_Impl>()->resetSkinLossUFactorTimesAreaValue();
}

bool GeneratorFuelCellAuxiliaryHeater::setSkinLossDestination(const std::string& skinLossDestination) {
  return getImpl<detail::GeneratorFuelCellAuxiliaryHeater_Impl>()->setSkinLossDestination(skinLossDestination);
}

void GeneratorFuelCellAuxiliaryHeater::resetSkinLossDestination() {
  getImpl<detail::GeneratorFuelCellAuxiliaryHeater_Impl>()->resetSkinLossDestination();
}

bool GeneratorFuelCellAuxiliaryHeater::setZonetoReceiveSkinLosses(const ThermalZone& zone) {
  return getImpl<detail::GeneratorFuelCellAuxiliaryHeater_Impl>()->setZonetoReceiveSkinLosses(zone);
}

void GeneratorFuelCellAuxiliaryHeater::resetZonetoReceiveSkinLosses() {
  getImpl<detail::GeneratorFuelCellAuxiliaryHeater_Impl>()->resetZonetoReceiveSkinLosses();
}

bool GeneratorFuelCellAuxiliaryHeater::setHeatingCapacityUnits(const std::string& heatingCapacityUnits) {
  return getImpl<detail::GeneratorFuelCellAuxiliaryHeater_Impl>()->setHeatingCapacityUnits(heatingCapacityUnits);
}

void GeneratorFuelCellAuxiliaryHeater::resetHeatingCapacityUnits() {
  getImpl<detail::GeneratorFuelCellAuxiliaryHeater_Impl>()->resetHeatingCapacityUnits();
}

void GeneratorFuelCellAuxiliaryHeater::setMaximumHeatingCapacityinWatts(double maximumHeatingCapacityinWatts) {
  getImpl<detail::GeneratorFuelCellAuxiliaryHeater_Impl>()->setMaximumHeatingCapacityinWatts(maximumHeatingCapacityinWatts);
}

void GeneratorFuelCellAuxiliaryHeater::resetMaximumHeatingCapacityinWatts() {
  getImpl<detail::GeneratorFuelCellAuxiliaryHeater_Impl>()->resetMaximumHeatingCapacityinWatts();
}

void GeneratorFuelCellAuxiliaryHeater::setMinimumHeatingCapacityinWatts(double minimumHeatingCapacityinWatts) {
  getImpl<detail::GeneratorFuelCellAuxiliaryHeater_Impl>()->setMinimumHeatingCapacityinWatts(minimumHeatingCapacityinWatts);
}

void GeneratorFuelCellAuxiliaryHeater::resetMinimumHeatingCapacityinWatts() {
  getImpl<detail::GeneratorFuelCellAuxiliaryHeater_Impl>()->resetMinimumHeatingCapacityinWatts();
}

void GeneratorFuelCellAuxiliaryHeater::setMaximumHeatingCapacityinKmolperSecond(double maximumHeatingCapacityinKmolperSecond) {
  getImpl<detail::GeneratorFuelCellAuxiliaryHeater_Impl>()->setMaximumHeatingCapacityinKmolperSecond(maximumHeatingCapacityinKmolperSecond);
}

void GeneratorFuelCellAuxiliaryHeater::resetMaximumHeatingCapacityinKmolperSecond() {
  getImpl<detail::GeneratorFuelCellAuxiliaryHeater_Impl>()->resetMaximumHeatingCapacityinKmolperSecond();
}

void GeneratorFuelCellAuxiliaryHeater::setMinimumHeatingCapacityinKmolperSecond(double minimumHeatingCapacityinKmolperSecond) {
  getImpl<detail::GeneratorFuelCellAuxiliaryHeater_Impl>()->setMinimumHeatingCapacityinKmolperSecond(minimumHeatingCapacityinKmolperSecond);
}

void GeneratorFuelCellAuxiliaryHeater::resetMinimumHeatingCapacityinKmolperSecond() {
  getImpl<detail::GeneratorFuelCellAuxiliaryHeater_Impl>()->resetMinimumHeatingCapacityinKmolperSecond();
}

GeneratorFuelCell GeneratorFuelCellAuxiliaryHeater::fuelCell() const {
  return getImpl<detail::GeneratorFuelCellAuxiliaryHeater_Impl>()->fuelCell();
}

/// @cond
GeneratorFuelCellAuxiliaryHeater::GeneratorFuelCellAuxiliaryHeater(std::shared_ptr<detail::GeneratorFuelCellAuxiliaryHeater_Impl> impl)
  : ModelObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

