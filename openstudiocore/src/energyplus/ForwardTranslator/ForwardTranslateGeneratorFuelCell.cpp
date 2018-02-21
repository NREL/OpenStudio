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

