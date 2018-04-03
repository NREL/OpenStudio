/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/OutputVariable.hpp"
#include "../../model/OutputVariable_Impl.hpp"
#include "../../model/OutputMeter.hpp"
#include "../../model/OutputMeter_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/GeneratorFuelCell.hpp"
#include "../../model/GeneratorFuelCell_Impl.hpp"
#include "../../model/GeneratorFuelCellPowerModule.hpp"
#include "../../model/GeneratorFuelCellPowerModule_Impl.hpp"
#include "../../model/GeneratorFuelCellAirSupply.hpp"
#include "../../model/GeneratorFuelCellAirSupply_Impl.hpp"
#include "../../model/GeneratorFuelCellWaterSupply.hpp"
#include "../../model/GeneratorFuelCellWaterSupply_Impl.hpp"
#include "../../model/GeneratorFuelCellAuxiliaryHeater.hpp"
#include "../../model/GeneratorFuelCellAuxiliaryHeater_Impl.hpp"
#include "../../model/GeneratorFuelCellExhaustGasToWaterHeatExchanger.hpp"
#include "../../model/GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl.hpp"
#include "../../model/GeneratorFuelCellElectricalStorage.hpp"
#include "../../model/GeneratorFuelCellElectricalStorage_Impl.hpp"
#include "../../model/GeneratorFuelCellInverter.hpp"
#include "../../model/GeneratorFuelCellInverter_Impl.hpp"
#include "../../model/GeneratorFuelCellStackCooler.hpp"
#include "../../model/GeneratorFuelCellStackCooler_Impl.hpp"
#include "../../model/GeneratorFuelSupply.hpp"
#include "../../model/GeneratorFuelSupply_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/Generator_FuelCell_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateGeneratorFuelCell(GeneratorFuelCell & modelObject)
{

  IdfObject pcm = createAndRegisterIdfObject(openstudio::IddObjectType::Generator_FuelCell, modelObject);
  //Name
  boost::optional<std::string> s = modelObject.name();
  if (s) {
    pcm.setName(*s);
  }

  //PowerModuleName
  boost::optional<GeneratorFuelCellPowerModule> pm = modelObject.powerModule();
  if (pm) {
    pcm.setString(Generator_FuelCellFields::PowerModuleName, pm.get().nameString());
  }

  //AirSupplyName
  boost::optional<GeneratorFuelCellAirSupply> as = modelObject.airSupply();
  if (as) {
    pcm.setString(Generator_FuelCellFields::AirSupplyName, as.get().nameString());
  }

  //FuelSupplyName
  boost::optional<GeneratorFuelSupply> fs = modelObject.fuelSupply();
  if (fs) {
    pcm.setString(Generator_FuelCellFields::FuelSupplyName, fs.get().nameString());
  }

  //WaterSupplyName
  boost::optional<GeneratorFuelCellWaterSupply> ws = modelObject.waterSupply();
  if (ws) {
    pcm.setString(Generator_FuelCellFields::WaterSupplyName, ws.get().nameString());
  }
  //AuxiliaryHeaterName
  boost::optional<GeneratorFuelCellAuxiliaryHeater> ah = modelObject.auxiliaryHeater();
  if (ah) {
    pcm.setString(Generator_FuelCellFields::AuxiliaryHeaterName, ah.get().nameString());
  }

  //HeatExchangerName
  boost::optional<GeneratorFuelCellExhaustGasToWaterHeatExchanger> he = modelObject.heatExchanger();
  if (he) {
    pcm.setString(Generator_FuelCellFields::HeatExchangerName, he.get().nameString());
  }

  //ElectricalStorageName
  boost::optional<GeneratorFuelCellElectricalStorage> es = modelObject.electricalStorage();
  if (es) {
    pcm.setString(Generator_FuelCellFields::ElectricalStorageName, es.get().nameString());
  }

  //InverterName
  boost::optional<GeneratorFuelCellInverter> inv = modelObject.inverter();
  if (inv) {
    pcm.setString(Generator_FuelCellFields::InverterName, inv.get().nameString());
  }

  //StackCoolerName
  boost::optional<GeneratorFuelCellStackCooler> sc = modelObject.stackCooler();
  if (sc) {
    pcm.setString(Generator_FuelCellFields::StackCoolerName, sc.get().nameString());
  }

  return pcm;

}

} // energyplus

} // openstudio

