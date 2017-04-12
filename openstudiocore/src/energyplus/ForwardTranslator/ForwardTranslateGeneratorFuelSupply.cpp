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
#include "../../model/GeneratorFuelSupply.hpp"
#include "../../model/GeneratorFuelSupply_Impl.hpp"

#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/ScheduleConstant.hpp"
#include "../../model/ScheduleConstant_Impl.hpp"
#include "../../model/CurveCubic.hpp"
#include "../../model/CurveCubic_Impl.hpp"
#include "../../model/CurveQuadratic.hpp"
#include "../../model/CurveQuadratic_Impl.hpp"
#include "../../model/ScheduleTypeLimits.hpp"
#include "../../model/ScheduleTypeRegistry.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/Generator_FuelSupply_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateGeneratorFuelSupply(GeneratorFuelSupply & modelObject)
{
  boost::optional<std::string> s;
  boost::optional<double> d;
  boost::optional<int> i;
  boost::optional<Node> node;
  boost::optional<Schedule> schedule;
  boost::optional<CurveCubic> curve;
  std::vector< std::pair<std::string, double> > constituents;
  
  IdfObject pcm = createAndRegisterIdfObject(openstudio::IddObjectType::Generator_FuelSupply, modelObject);
  //Name
  s = modelObject.name();
  if (s) {
    pcm.setName(*s);
  }

  //fuelTemperatureModelingMode
  s = modelObject.fuelTemperatureModelingMode();
  if (s) {
    pcm.setString(Generator_FuelSupplyFields::FuelTemperatureModelingMode, s.get());
  }

  //FuelTemperatureReferenceNodeName
  node = modelObject.fuelTemperatureReferenceNode();
  if (node) {
    pcm.setString(Generator_FuelSupplyFields::FuelTemperatureReferenceNodeName, node.get().nameString());
  }

  //FuelTemperatureScheduleName
  schedule = modelObject.fuelTemperatureSchedule();
  if (schedule) {
    pcm.setString(Generator_FuelSupplyFields::FuelTemperatureScheduleName, schedule.get().nameString());
  }

  //CompressorPowerMultiplierFunctionofFuelRateCurveName
  curve = modelObject.compressorPowerMultiplierFunctionofFuelRateCurve();
  if (curve) {
    pcm.setString(Generator_FuelSupplyFields::CompressorPowerMultiplierFunctionofFuelRateCurveName, curve.get().nameString());
  }

  //CompressorHeatLossFactor
  d = modelObject.compressorHeatLossFactor();
  if (d) {
    pcm.setDouble(Generator_FuelSupplyFields::CompressorHeatLossFactor, d.get());
  }

  //FuelType
  s = modelObject.fuelType();
  if (s) {
    pcm.setString(Generator_FuelSupplyFields::FuelType, s.get());
  }

  //LiquidGenericFuelLowerHeatingValue
  d = modelObject.liquidGenericFuelLowerHeatingValue();
  if (d) {
    pcm.setDouble(Generator_FuelSupplyFields::LiquidGenericFuelLowerHeatingValue, d.get());
  }

  //LiquidGenericFuelHigherHeatingValue
  d = modelObject.liquidGenericFuelHigherHeatingValue();
  if (d) {
    pcm.setDouble(Generator_FuelSupplyFields::LiquidGenericFuelHigherHeatingValue, d.get());
  }

  //LiquidGenericFuelMolecularWeight
  d = modelObject.liquidGenericFuelMolecularWeight();
  if (d) {
    pcm.setDouble(Generator_FuelSupplyFields::LiquidGenericFuelMolecularWeight, d.get());
  }

  //LiquidGenericFuelCO2EmissionFactor
  d = modelObject.liquidGenericFuelCO2EmissionFactor();
  if (d) {
    pcm.setDouble(Generator_FuelSupplyFields::LiquidGenericFuelCO2EmissionFactor, d.get());
  }

  //UserDefinedConstituents
  constituents = modelObject.constituents();
  if (!constituents.empty()) {
    for (auto constituent : constituents) {
      auto eg = pcm.pushExtensibleGroup();
      eg.setString(Generator_FuelSupplyExtensibleFields::ConstituentName, constituent.first);
      eg.setDouble(Generator_FuelSupplyExtensibleFields::ConstituentMolarFraction, constituent.second);
    }
  }

  //NumberofUserDefinedConstituents
  i = constituents.size();
  if (i) {
    pcm.setDouble(Generator_FuelSupplyFields::NumberofConstituentsinGaseousConstituentFuelSupply, i.get());
  }

  return pcm;
 
}

} // energyplus

} // openstudio

