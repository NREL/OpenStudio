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

#include "GeneratorFuelCell.hpp"
#include "GeneratorFuelCell_Impl.hpp"

// TODO: Check the following class names against object getters and setters.
#include "GeneratorFuelCellPowerModule.hpp"
#include "GeneratorFuelCellPowerModule_Impl.hpp"
#include "GeneratorFuelCellAirSupply.hpp"
#include "GeneratorFuelCellAirSupply_Impl.hpp"
#include "GeneratorFuelCellWaterSupply.hpp"
#include "GeneratorFuelCellWaterSupply_Impl.hpp"
#include "GeneratorFuelCellAuxiliaryHeater.hpp"
#include "GeneratorFuelCellAuxiliaryHeater_Impl.hpp"
#include "GeneratorFuelCellExhaustGasToWaterHeatExchanger.hpp"
#include "GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl.hpp"
#include "GeneratorFuelCellElectricalStorage.hpp"
#include "GeneratorFuelCellElectricalStorage_Impl.hpp"
#include "GeneratorFuelCellInverter.hpp"
#include "GeneratorFuelCellInverter_Impl.hpp"
#include "GeneratorFuelCellStackCooler.hpp"
#include "GeneratorFuelCellStackCooler_Impl.hpp"
#include "GeneratorFuelSupply.hpp"
#include "GeneratorFuelSupply_Impl.hpp"
#include "CurveQuadratic.hpp"
#include "CurveQuadratic_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Generator_FuelCell_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  GeneratorFuelCell_Impl::GeneratorFuelCell_Impl(const IdfObject& idfObject,
                                                 Model_Impl* model,
                                                 bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == GeneratorFuelCell::iddObjectType());
  }

  GeneratorFuelCell_Impl::GeneratorFuelCell_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                 Model_Impl* model,
                                                 bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == GeneratorFuelCell::iddObjectType());
  }

  GeneratorFuelCell_Impl::GeneratorFuelCell_Impl(const GeneratorFuelCell_Impl& other,
                                                 Model_Impl* model,
                                                 bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& GeneratorFuelCell_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType GeneratorFuelCell_Impl::iddObjectType() const {
    return GeneratorFuelCell::iddObjectType();
  }

  GeneratorFuelCellPowerModule GeneratorFuelCell_Impl::powerModule() const {
    boost::optional<GeneratorFuelCellPowerModule> value = optionalPowerModule();
    if (!value) {
      LOG_AND_THROW(" does not have an Power Module attached.");
    }
    return value.get();
  }

  GeneratorFuelCellAirSupply GeneratorFuelCell_Impl::airSupply() const {
    boost::optional<GeneratorFuelCellAirSupply> value = optionalAirSupply();
    if (!value) {
      LOG_AND_THROW(" does not have an Air Supply attached.");
    }
    return value.get();
  }

  GeneratorFuelSupply GeneratorFuelCell_Impl::fuelSupply() const {
    boost::optional<GeneratorFuelSupply> value = optionalFuelSupply();
    if (!value) {
      LOG_AND_THROW(" does not have an Fuel Supply attached.");
    }
    return value.get();
  }

  GeneratorFuelCellWaterSupply GeneratorFuelCell_Impl::waterSupply() const {
    boost::optional<GeneratorFuelCellWaterSupply> value = optionalWaterSupply();
    if (!value) {
      LOG_AND_THROW(" does not have an Water Supply attached.");
    }
    return value.get();
  }

  GeneratorFuelCellAuxiliaryHeater GeneratorFuelCell_Impl::auxiliaryHeater() const {
    boost::optional<GeneratorFuelCellAuxiliaryHeater> value = optionalAuxiliaryHeater();
    if (!value) {
      LOG_AND_THROW(" does not have an Auxiliary Heater attached.");
    }
    return value.get();
  }

  GeneratorFuelCellExhaustGasToWaterHeatExchanger GeneratorFuelCell_Impl::heatExchanger() const {
    boost::optional<GeneratorFuelCellExhaustGasToWaterHeatExchanger> value = optionalHeatExchanger();
    if (!value) {
      LOG_AND_THROW(" does not have an Heat Exchanger attached.");
    }
    return value.get();
  }

  GeneratorFuelCellElectricalStorage GeneratorFuelCell_Impl::electricalStorage() const {
    boost::optional<GeneratorFuelCellElectricalStorage> value = optionalElectricalStorage();
    if (!value) {
      LOG_AND_THROW(" does not have an Electrical Storage attached.");
    }
    return value.get();
  }

  GeneratorFuelCellInverter GeneratorFuelCell_Impl::inverter() const {
    boost::optional<GeneratorFuelCellInverter> value = optionalInverter();
    if (!value) {
      LOG_AND_THROW(" does not have an Inverter attached.");
    }
    return value.get();
  }

  boost::optional<GeneratorFuelCellStackCooler> GeneratorFuelCell_Impl::stackCooler() const {
    return getObject<ModelObject>().getModelObjectTarget<GeneratorFuelCellStackCooler>(OS_Generator_FuelCellFields::StackCoolerName);
  }

  bool GeneratorFuelCell_Impl::setPowerModule(const GeneratorFuelCellPowerModule& fCPM) {
    bool result = setPointer(OS_Generator_FuelCellFields::PowerModuleName, fCPM.handle());
    return result;
  }

  bool GeneratorFuelCell_Impl::setAirSupply(const GeneratorFuelCellAirSupply& fCAirSup) {
    bool result = setPointer(OS_Generator_FuelCellFields::AirSupplyName, fCAirSup.handle());
    return result;
  }

  bool GeneratorFuelCell_Impl::setFuelSupply(const GeneratorFuelSupply& genFuelSup) {
    bool result = setPointer(OS_Generator_FuelCellFields::FuelSupplyName, genFuelSup.handle());
    return result;
  }

  bool GeneratorFuelCell_Impl::setWaterSupply(const GeneratorFuelCellWaterSupply& fCWaterSup) {
    bool result = setPointer(OS_Generator_FuelCellFields::WaterSupplyName, fCWaterSup.handle());
    return result;
  }

  bool GeneratorFuelCell_Impl::setAuxiliaryHeater(const GeneratorFuelCellAuxiliaryHeater& fCAuxHeat) {
    bool result = setPointer(OS_Generator_FuelCellFields::AuxiliaryHeaterName, fCAuxHeat.handle());
    return result;
  }

  bool GeneratorFuelCell_Impl::setHeatExchanger(const GeneratorFuelCellExhaustGasToWaterHeatExchanger& fCExhaustHX) {
    bool result = setPointer(OS_Generator_FuelCellFields::HeatExchangerName, fCExhaustHX.handle());
    return result;
  }

  bool GeneratorFuelCell_Impl::setElectricalStorage(const GeneratorFuelCellElectricalStorage& fCStorage) {
    bool result = setPointer(OS_Generator_FuelCellFields::ElectricalStorageName, fCStorage.handle());
    return result;
  }

  bool GeneratorFuelCell_Impl::setInverter(const GeneratorFuelCellInverter& fCInverter) {
    bool result = setPointer(OS_Generator_FuelCellFields::InverterName, fCInverter.handle());
    return result;
  }

  bool GeneratorFuelCell_Impl::setStackCooler(const GeneratorFuelCellStackCooler& fCStackCooler) {
    bool result = setPointer(OS_Generator_FuelCellFields::StackCoolerName, fCStackCooler.handle());
    return result;
  }

  void GeneratorFuelCell_Impl::resetStackCooler() {
    bool result = setString(OS_Generator_FuelCellFields::StackCoolerName, "");
    OS_ASSERT(result);
  }

  boost::optional<GeneratorFuelCellPowerModule> GeneratorFuelCell_Impl::optionalPowerModule() const {
    return getObject<ModelObject>().getModelObjectTarget<GeneratorFuelCellPowerModule>(OS_Generator_FuelCellFields::PowerModuleName);
  }

  boost::optional<GeneratorFuelCellAirSupply> GeneratorFuelCell_Impl::optionalAirSupply() const {
    return getObject<ModelObject>().getModelObjectTarget<GeneratorFuelCellAirSupply>(OS_Generator_FuelCellFields::AirSupplyName);
  }

  boost::optional<GeneratorFuelSupply> GeneratorFuelCell_Impl::optionalFuelSupply() const {
    return getObject<ModelObject>().getModelObjectTarget<GeneratorFuelSupply>(OS_Generator_FuelCellFields::FuelSupplyName);
  }

  boost::optional<GeneratorFuelCellWaterSupply> GeneratorFuelCell_Impl::optionalWaterSupply() const {
    return getObject<ModelObject>().getModelObjectTarget<GeneratorFuelCellWaterSupply>(OS_Generator_FuelCellFields::WaterSupplyName);
  }

  boost::optional<GeneratorFuelCellAuxiliaryHeater> GeneratorFuelCell_Impl::optionalAuxiliaryHeater() const {
    return getObject<ModelObject>().getModelObjectTarget<GeneratorFuelCellAuxiliaryHeater>(OS_Generator_FuelCellFields::AuxiliaryHeaterName);
  }

  boost::optional<GeneratorFuelCellExhaustGasToWaterHeatExchanger> GeneratorFuelCell_Impl::optionalHeatExchanger() const {
    return getObject<ModelObject>().getModelObjectTarget<GeneratorFuelCellExhaustGasToWaterHeatExchanger>(OS_Generator_FuelCellFields::HeatExchangerName);
  }

  boost::optional<GeneratorFuelCellElectricalStorage> GeneratorFuelCell_Impl::optionalElectricalStorage() const {
    return getObject<ModelObject>().getModelObjectTarget<GeneratorFuelCellElectricalStorage>(OS_Generator_FuelCellFields::ElectricalStorageName);
  }

  boost::optional<GeneratorFuelCellInverter> GeneratorFuelCell_Impl::optionalInverter() const {
    return getObject<ModelObject>().getModelObjectTarget<GeneratorFuelCellInverter>(OS_Generator_FuelCellFields::InverterName);
  }

} // detail

GeneratorFuelCell::GeneratorFuelCell(const Model& model,
                                     const GeneratorFuelCellPowerModule& fCPM,
                                     const GeneratorFuelCellAirSupply& fCAS,
                                     const GeneratorFuelCellWaterSupply& fCWS,
                                     const GeneratorFuelCellAuxiliaryHeater& fCAH,
                                     const GeneratorFuelCellExhaustGasToWaterHeatExchanger& fCExhaustHX,
                                     const GeneratorFuelCellElectricalStorage& fCES,
                                     const GeneratorFuelCellInverter& fCInverter,
                                     const GeneratorFuelSupply& fS)
  : ModelObject(GeneratorFuelCell::iddObjectType(), model) {
  OS_ASSERT(getImpl<detail::GeneratorFuelCell_Impl>());

  bool ok = setPowerModule(fCPM);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s GeneratorFuelCellPowerModule to "
      << fCPM.briefDescription() << ".");
  }
  ok = setAirSupply(fCAS);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s GeneratorFuelCellAirSupply to "
      << fCAS.briefDescription() << ".");
  }
  ok = setFuelSupply(fS);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s GeneratorFuelSupply to "
      << fS.briefDescription() << ".");
  }
  ok = setWaterSupply(fCWS);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s GeneratorFuelCellWaterSupply to "
      << fCWS.briefDescription() << ".");
  }
  ok = setAuxiliaryHeater(fCAH);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s GeneratorFuelCellAuxiliaryHeater to "
      << fCAH.briefDescription() << ".");
  }
  ok = setHeatExchanger(fCExhaustHX);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s GeneratorFuelCellExhaustGasToWaterHeatExchanger to "
      << fCExhaustHX.briefDescription() << ".");
  }
  ok = setElectricalStorage(fCES);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s GeneratorFuelCellElectricalStorage to "
      << fCES.briefDescription() << ".");
  }
  ok = setInverter(fCInverter);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s GeneratorFuelCellInverter to "
      << fCInverter.briefDescription() << ".");
  }
}

GeneratorFuelCell::GeneratorFuelCell(const Model& model)
  : ModelObject(GeneratorFuelCell::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::GeneratorFuelCell_Impl>());

  //create generic curve
  CurveQuadratic curveQuadratic(model);
  curveQuadratic.setCoefficient1Constant(1);
  curveQuadratic.setCoefficient2x(0);
  curveQuadratic.setCoefficient3xPOW2(0);
  curveQuadratic.setMinimumValueofx(0.0);
  curveQuadratic.setMaximumValueofx(1.0);
  //create generic FCPM
  GeneratorFuelCellPowerModule fCPM(model, curveQuadratic);
  bool ok = setPowerModule(fCPM);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s GeneratorFuelCellPowerModule");
  }
  //create generic FCAS
  GeneratorFuelCellAirSupply fCAS(model);
  ok = setAirSupply(fCAS);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s GeneratorFuelCellAirSupply");
  }
  //create generic fuel supply
  GeneratorFuelSupply fS(model);
  ok = setFuelSupply(fS);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s GeneratorFuelSupply");
  }
  //create generic water supply
  GeneratorFuelCellWaterSupply fCWS(model);
  ok = setWaterSupply(fCWS);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s GeneratorFuelCellWaterSupply");
  }
  //create generic aux heater
  GeneratorFuelCellAuxiliaryHeater fCAH(model);
  ok = setAuxiliaryHeater(fCAH);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s GeneratorFuelCellAuxiliaryHeater");
  }
  //create generic exhaust HX
  GeneratorFuelCellExhaustGasToWaterHeatExchanger fCExhaustHX(model);
  ok = setHeatExchanger(fCExhaustHX);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s GeneratorFuelCellExhaustGasToWaterHeatExchanger");
  }
  //create generic storage
  GeneratorFuelCellElectricalStorage fCES(model);
  ok = setElectricalStorage(fCES);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s GeneratorFuelCellElectricalStorage");
  }
  //create generic inverter
  GeneratorFuelCellInverter fCInverter(model);
  ok = setInverter(fCInverter);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s GeneratorFuelCellInverter");
  }
}

IddObjectType GeneratorFuelCell::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Generator_FuelCell);
}

GeneratorFuelCellPowerModule GeneratorFuelCell::powerModule() const {
  return getImpl<detail::GeneratorFuelCell_Impl>()->powerModule();
}

GeneratorFuelCellAirSupply GeneratorFuelCell::airSupply() const {
  return getImpl<detail::GeneratorFuelCell_Impl>()->airSupply();
}

GeneratorFuelSupply GeneratorFuelCell::fuelSupply() const {
  return getImpl<detail::GeneratorFuelCell_Impl>()->fuelSupply();
}

GeneratorFuelCellWaterSupply GeneratorFuelCell::waterSupply() const {
  return getImpl<detail::GeneratorFuelCell_Impl>()->waterSupply();
}

GeneratorFuelCellAuxiliaryHeater GeneratorFuelCell::auxiliaryHeater() const {
  return getImpl<detail::GeneratorFuelCell_Impl>()->auxiliaryHeater();
}

GeneratorFuelCellExhaustGasToWaterHeatExchanger GeneratorFuelCell::heatExchanger() const {
  return getImpl<detail::GeneratorFuelCell_Impl>()->heatExchanger();
}

GeneratorFuelCellElectricalStorage GeneratorFuelCell::electricalStorage() const {
  return getImpl<detail::GeneratorFuelCell_Impl>()->electricalStorage();
}

GeneratorFuelCellInverter GeneratorFuelCell::inverter() const {
  return getImpl<detail::GeneratorFuelCell_Impl>()->inverter();
}

boost::optional<GeneratorFuelCellStackCooler> GeneratorFuelCell::stackCooler() const {
  return getImpl<detail::GeneratorFuelCell_Impl>()->stackCooler();
}

bool GeneratorFuelCell::setPowerModule(const GeneratorFuelCellPowerModule& fCPM) {
  return getImpl<detail::GeneratorFuelCell_Impl>()->setPowerModule(fCPM);
}

bool GeneratorFuelCell::setAirSupply(const GeneratorFuelCellAirSupply& fCAirSup) {
  return getImpl<detail::GeneratorFuelCell_Impl>()->setAirSupply(fCAirSup);
}

bool GeneratorFuelCell::setFuelSupply(const GeneratorFuelSupply& genFuelSup) {
  return getImpl<detail::GeneratorFuelCell_Impl>()->setFuelSupply(genFuelSup);
}

bool GeneratorFuelCell::setWaterSupply(const GeneratorFuelCellWaterSupply& fCWaterSup) {
  return getImpl<detail::GeneratorFuelCell_Impl>()->setWaterSupply(fCWaterSup);
}

bool GeneratorFuelCell::setAuxiliaryHeater(const GeneratorFuelCellAuxiliaryHeater& fCAuxHeat) {
  return getImpl<detail::GeneratorFuelCell_Impl>()->setAuxiliaryHeater(fCAuxHeat);
}

bool GeneratorFuelCell::setHeatExchanger(const GeneratorFuelCellExhaustGasToWaterHeatExchanger& fCExhaustHX) {
  return getImpl<detail::GeneratorFuelCell_Impl>()->setHeatExchanger(fCExhaustHX);
}

bool GeneratorFuelCell::setElectricalStorage(const GeneratorFuelCellElectricalStorage& fCStorage) {
  return getImpl<detail::GeneratorFuelCell_Impl>()->setElectricalStorage(fCStorage);
}

bool GeneratorFuelCell::setInverter(const GeneratorFuelCellInverter& fCInverter) {
  return getImpl<detail::GeneratorFuelCell_Impl>()->setInverter(fCInverter);
}

bool GeneratorFuelCell::setStackCooler(const GeneratorFuelCellStackCooler& fCStackCooler) {
  return getImpl<detail::GeneratorFuelCell_Impl>()->setStackCooler(fCStackCooler);
}

void GeneratorFuelCell::resetStackCooler() {
  getImpl<detail::GeneratorFuelCell_Impl>()->resetStackCooler();
}

/// @cond
GeneratorFuelCell::GeneratorFuelCell(std::shared_ptr<detail::GeneratorFuelCell_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

