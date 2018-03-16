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
#include "../../model/CurveQuadratic.hpp"
#include "../../model/CurveQuadratic_Impl.hpp"
#include "../../model/CurveCubic.hpp"
#include "../../model/CurveCubic_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/Generator_FuelCell_AirSupply_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateGeneratorFuelCellAirSupply(GeneratorFuelCellAirSupply & modelObject)
{
  boost::optional<std::string> s;
  boost::optional<double> d;
  boost::optional<int> i;
  boost::optional<Node> node;
  boost::optional<CurveCubic> curve;
  boost::optional<CurveQuadratic> curvequad;
  std::vector< std::pair<std::string, double> > constituents;

  IdfObject pcm = createAndRegisterIdfObject(openstudio::IddObjectType::Generator_FuelCell_AirSupply, modelObject);
  //Name
  s = modelObject.name();
  if (s) {
    pcm.setName(*s);
  }

  //AirInletNodeName
  node = modelObject.airInletNode();
  if (node) {
    pcm.setString(Generator_FuelCell_AirSupplyFields::AirInletNodeName, node.get().nameString());
  } else {
    auto name = modelObject.nameString() + " OA Node";
    IdfObject oaNodeListIdf(openstudio::IddObjectType::OutdoorAir_NodeList);
    oaNodeListIdf.setString(0, name);
    m_idfObjects.push_back(oaNodeListIdf);

    pcm.setString(openstudio::Generator_FuelCell_AirSupplyFields::AirInletNodeName, name);
  }

  //blowerPowerCurve
  curve = modelObject.blowerPowerCurve();
  if (curve) {
    pcm.setString(Generator_FuelCell_AirSupplyFields::BlowerPowerCurveName, curve.get().nameString());
  }

  //BlowerHeatLossFactor
  d = modelObject.blowerHeatLossFactor();
  if (d) {
    pcm.setDouble(Generator_FuelCell_AirSupplyFields::BlowerHeatLossFactor, d.get());
  }

  //AirSupplyRateCalculationMode
  s = modelObject.airSupplyRateCalculationMode();
  if (s) {
    pcm.setString(Generator_FuelCell_AirSupplyFields::AirSupplyRateCalculationMode, s.get());
  }

  //StoichiometricRatio
  d = modelObject.stoichiometricRatio();
  if (d) {
    pcm.setDouble(Generator_FuelCell_AirSupplyFields::StoichiometricRatio, d.get());
  }

  //AirRateFunctionofElectricPowerCurveName
  curvequad = modelObject.airRateFunctionofElectricPowerCurve();
  if (curvequad) {
    pcm.setString(Generator_FuelCell_AirSupplyFields::AirRateFunctionofElectricPowerCurveName, curvequad.get().nameString());
  }

  //AirRateAirTemperatureCoefficient
  d = modelObject.airRateAirTemperatureCoefficient();
  if (d) {
    pcm.setDouble(Generator_FuelCell_AirSupplyFields::AirRateAirTemperatureCoefficient, d.get());
  }

  //AirRateFunctionofElectricPowerCurveName
  curvequad = modelObject.airRateFunctionofElectricPowerCurve();
  if (curvequad) {
    pcm.setString(Generator_FuelCell_AirSupplyFields::AirRateFunctionofElectricPowerCurveName, curvequad.get().nameString());
  }

  //AirIntakeHeatRecoveryMode
  s = modelObject.airIntakeHeatRecoveryMode();
  if (s) {
    pcm.setString(Generator_FuelCell_AirSupplyFields::AirIntakeHeatRecoveryMode, s.get());
  }

  //AirSupplyConstituentMode
  s = modelObject.airSupplyConstituentMode();
  if (s) {
    pcm.setString(Generator_FuelCell_AirSupplyFields::AirSupplyConstituentMode, s.get());
  }

  //UserDefinedConstituents
  constituents = modelObject.constituents();
  if (!constituents.empty()) {
    for (auto constituent : constituents) {
      auto eg = pcm.pushExtensibleGroup();
      eg.setString(Generator_FuelCell_AirSupplyExtensibleFields::ConstituentName, constituent.first);
      eg.setDouble(Generator_FuelCell_AirSupplyExtensibleFields::MolarFraction, constituent.second);
    }
  }

  //NumberofUserDefinedConstituents
  i = constituents.size();
  if (i) {
    pcm.setDouble(Generator_FuelCell_AirSupplyFields::NumberofUserDefinedConstituents, i.get());
  }

  return pcm;

}

} // energyplus

} // openstudio

