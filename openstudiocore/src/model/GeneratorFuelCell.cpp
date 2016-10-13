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
    boost::optional<FCPM> value = optionalPowerModule();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Power Module attached.");
    }
    return value.get();
  }

  GeneratorFuelCellAirSupply_Impl GeneratorFuelCell_Impl::airSupply() const {
    boost::optional<FCAirSup> value = optionalAirSupply();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Air Supply attached.");
    }
    return value.get();
  }

  GeneratorFuelSupply GeneratorFuelCell_Impl::fuelSupply() const {
    boost::optional<GenFuelSup> value = optionalFuelSupply();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Fuel Supply attached.");
    }
    return value.get();
  }

  GeneratorFuelCellWaterSupply GeneratorFuelCell_Impl::waterSupply() const {
    boost::optional<FCWaterSup> value = optionalWaterSupply();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Water Supply attached.");
    }
    return value.get();
  }

  GeneratorFuelCellAuxiliaryHeater GeneratorFuelCell_Impl::auxiliaryHeater() const {
    boost::optional<FCAuxHeat> value = optionalAuxiliaryHeater();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Auxiliary Heater attached.");
    }
    return value.get();
  }

  GeneratorFuelCellExhaustGasToWaterHeatExchanger GeneratorFuelCell_Impl::heatExchanger() const {
    boost::optional<FCExhaustHX> value = optionalHeatExchanger();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Heat Exchanger attached.");
    }
    return value.get();
  }

  GeneratorFuelCellElectricalStorage GeneratorFuelCell_Impl::electricalStorage() const {
    boost::optional<FCStorage> value = optionalElectricalStorage();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Electrical Storage attached.");
    }
    return value.get();
  }

  GeneratorFuelCellInverter GeneratorFuelCell_Impl::inverter() const {
    boost::optional<FCInverter> value = optionalInverter();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Inverter attached.");
    }
    return value.get();
  }

  boost::optional<GeneratorFuelCellStackCooler> GeneratorFuelCell_Impl::stackCooler() const {
    return getObject<ModelObject>().getModelObjectTarget<FCStackCooler>(OS_Generator_FuelCellFields::StackCoolerName);
  }

  bool GeneratorFuelCell_Impl::setPowerModule(const FCPM& fCPM) {
    bool result = setPointer(OS_Generator_FuelCellFields::PowerModuleName, fCPM.handle());
    return result;
  }

  bool GeneratorFuelCell_Impl::setAirSupply(const FCAirSup& fCAirSup) {
    bool result = setPointer(OS_Generator_FuelCellFields::AirSupplyName, fCAirSup.handle());
    return result;
  }

  bool GeneratorFuelCell_Impl::setFuelSupply(const GenFuelSup& genFuelSup) {
    bool result = setPointer(OS_Generator_FuelCellFields::FuelSupplyName, genFuelSup.handle());
    return result;
  }

  bool GeneratorFuelCell_Impl::setWaterSupply(const FCWaterSup& fCWaterSup) {
    bool result = setPointer(OS_Generator_FuelCellFields::WaterSupplyName, fCWaterSup.handle());
    return result;
  }

  bool GeneratorFuelCell_Impl::setAuxiliaryHeater(const FCAuxHeat& fCAuxHeat) {
    bool result = setPointer(OS_Generator_FuelCellFields::AuxiliaryHeaterName, fCAuxHeat.handle());
    return result;
  }

  bool GeneratorFuelCell_Impl::setHeatExchanger(const FCExhaustHX& fCExhaustHX) {
    bool result = setPointer(OS_Generator_FuelCellFields::HeatExchangerName, fCExhaustHX.handle());
    return result;
  }

  bool GeneratorFuelCell_Impl::setElectricalStorage(const FCStorage& fCStorage) {
    bool result = setPointer(OS_Generator_FuelCellFields::ElectricalStorageName, fCStorage.handle());
    return result;
  }

  bool GeneratorFuelCell_Impl::setInverter(const FCInverter& fCInverter) {
    bool result = setPointer(OS_Generator_FuelCellFields::InverterName, fCInverter.handle());
    return result;
  }

  bool GeneratorFuelCell_Impl::setStackCooler(const FCStackCooler& fCStackCooler) {
    bool result = setPointer(OS_Generator_FuelCellFields::StackCoolerName, fCStackCooler.handle());
    return result;
  }

  void GeneratorFuelCell_Impl::resetStackCooler() {
    bool result = setString(OS_Generator_FuelCellFields::StackCoolerName, "");
    OS_ASSERT(result);
  }

  boost::optional<FCPM> GeneratorFuelCell_Impl::optionalPowerModule() const {
    return getObject<ModelObject>().getModelObjectTarget<FCPM>(OS_Generator_FuelCellFields::PowerModuleName);
  }

  boost::optional<FCAirSup> GeneratorFuelCell_Impl::optionalAirSupply() const {
    return getObject<ModelObject>().getModelObjectTarget<FCAirSup>(OS_Generator_FuelCellFields::AirSupplyName);
  }

  boost::optional<GenFuelSup> GeneratorFuelCell_Impl::optionalFuelSupply() const {
    return getObject<ModelObject>().getModelObjectTarget<GenFuelSup>(OS_Generator_FuelCellFields::FuelSupplyName);
  }

  boost::optional<FCWaterSup> GeneratorFuelCell_Impl::optionalWaterSupply() const {
    return getObject<ModelObject>().getModelObjectTarget<FCWaterSup>(OS_Generator_FuelCellFields::WaterSupplyName);
  }

  boost::optional<FCAuxHeat> GeneratorFuelCell_Impl::optionalAuxiliaryHeater() const {
    return getObject<ModelObject>().getModelObjectTarget<FCAuxHeat>(OS_Generator_FuelCellFields::AuxiliaryHeaterName);
  }

  boost::optional<FCExhaustHX> GeneratorFuelCell_Impl::optionalHeatExchanger() const {
    return getObject<ModelObject>().getModelObjectTarget<FCExhaustHX>(OS_Generator_FuelCellFields::HeatExchangerName);
  }

  boost::optional<FCStorage> GeneratorFuelCell_Impl::optionalElectricalStorage() const {
    return getObject<ModelObject>().getModelObjectTarget<FCStorage>(OS_Generator_FuelCellFields::ElectricalStorageName);
  }

  boost::optional<FCInverter> GeneratorFuelCell_Impl::optionalInverter() const {
    return getObject<ModelObject>().getModelObjectTarget<FCInverter>(OS_Generator_FuelCellFields::InverterName);
  }

} // detail

GeneratorFuelCell::GeneratorFuelCell(const Model& model)
  : ModelObject(GeneratorFuelCell::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::GeneratorFuelCell_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  //     OS_Generator_FuelCellFields::PowerModuleName
  //     OS_Generator_FuelCellFields::AirSupplyName
  //     OS_Generator_FuelCellFields::FuelSupplyName
  //     OS_Generator_FuelCellFields::WaterSupplyName
  //     OS_Generator_FuelCellFields::AuxiliaryHeaterName
  //     OS_Generator_FuelCellFields::HeatExchangerName
  //     OS_Generator_FuelCellFields::ElectricalStorageName
  //     OS_Generator_FuelCellFields::InverterName
  bool ok = true;
  // ok = setPowerModule();
  OS_ASSERT(ok);
  // ok = setAirSupply();
  OS_ASSERT(ok);
  // ok = setFuelSupply();
  OS_ASSERT(ok);
  // ok = setWaterSupply();
  OS_ASSERT(ok);
  // ok = setAuxiliaryHeater();
  OS_ASSERT(ok);
  // ok = setHeatExchanger();
  OS_ASSERT(ok);
  // ok = setElectricalStorage();
  OS_ASSERT(ok);
  // ok = setInverter();
  OS_ASSERT(ok);
}

IddObjectType GeneratorFuelCell::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Generator_FuelCell);
}

FCPM GeneratorFuelCell::powerModule() const {
  return getImpl<detail::GeneratorFuelCell_Impl>()->powerModule();
}

FCAirSup GeneratorFuelCell::airSupply() const {
  return getImpl<detail::GeneratorFuelCell_Impl>()->airSupply();
}

GenFuelSup GeneratorFuelCell::fuelSupply() const {
  return getImpl<detail::GeneratorFuelCell_Impl>()->fuelSupply();
}

FCWaterSup GeneratorFuelCell::waterSupply() const {
  return getImpl<detail::GeneratorFuelCell_Impl>()->waterSupply();
}

FCAuxHeat GeneratorFuelCell::auxiliaryHeater() const {
  return getImpl<detail::GeneratorFuelCell_Impl>()->auxiliaryHeater();
}

FCExhaustHX GeneratorFuelCell::heatExchanger() const {
  return getImpl<detail::GeneratorFuelCell_Impl>()->heatExchanger();
}

FCStorage GeneratorFuelCell::electricalStorage() const {
  return getImpl<detail::GeneratorFuelCell_Impl>()->electricalStorage();
}

FCInverter GeneratorFuelCell::inverter() const {
  return getImpl<detail::GeneratorFuelCell_Impl>()->inverter();
}

boost::optional<FCStackCooler> GeneratorFuelCell::stackCooler() const {
  return getImpl<detail::GeneratorFuelCell_Impl>()->stackCooler();
}

bool GeneratorFuelCell::setPowerModule(const FCPM& fCPM) {
  return getImpl<detail::GeneratorFuelCell_Impl>()->setPowerModule(fCPM);
}

bool GeneratorFuelCell::setAirSupply(const FCAirSup& fCAirSup) {
  return getImpl<detail::GeneratorFuelCell_Impl>()->setAirSupply(fCAirSup);
}

bool GeneratorFuelCell::setFuelSupply(const GenFuelSup& genFuelSup) {
  return getImpl<detail::GeneratorFuelCell_Impl>()->setFuelSupply(genFuelSup);
}

bool GeneratorFuelCell::setWaterSupply(const FCWaterSup& fCWaterSup) {
  return getImpl<detail::GeneratorFuelCell_Impl>()->setWaterSupply(fCWaterSup);
}

bool GeneratorFuelCell::setAuxiliaryHeater(const FCAuxHeat& fCAuxHeat) {
  return getImpl<detail::GeneratorFuelCell_Impl>()->setAuxiliaryHeater(fCAuxHeat);
}

bool GeneratorFuelCell::setHeatExchanger(const FCExhaustHX& fCExhaustHX) {
  return getImpl<detail::GeneratorFuelCell_Impl>()->setHeatExchanger(fCExhaustHX);
}

bool GeneratorFuelCell::setElectricalStorage(const FCStorage& fCStorage) {
  return getImpl<detail::GeneratorFuelCell_Impl>()->setElectricalStorage(fCStorage);
}

bool GeneratorFuelCell::setInverter(const FCInverter& fCInverter) {
  return getImpl<detail::GeneratorFuelCell_Impl>()->setInverter(fCInverter);
}

bool GeneratorFuelCell::setStackCooler(const FCStackCooler& fCStackCooler) {
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

