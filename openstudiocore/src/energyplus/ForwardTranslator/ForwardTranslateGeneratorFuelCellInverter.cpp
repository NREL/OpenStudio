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
#include "../../model/GeneratorFuelCellInverter.hpp"
#include "../../model/GeneratorFuelCellInverter_Impl.hpp"

#include "../../model/CurveCubic.hpp"
#include "../../model/CurveCubic_Impl.hpp"
#include "../../model/CurveQuadratic.hpp"
#include "../../model/CurveQuadratic_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/Generator_FuelCell_Inverter_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateGeneratorFuelCellInverter(GeneratorFuelCellInverter & modelObject)
{
  boost::optional<std::string> s;
  boost::optional<double> d;
  boost::optional<CurveQuadratic> curvequad;

  IdfObject pcm = createAndRegisterIdfObject(openstudio::IddObjectType::Generator_FuelCell_Inverter, modelObject);
  //Name
  s = modelObject.name();
  if (s) {
    pcm.setName(*s);
  }

  //InverterEfficiencyCalculationMode
  s = modelObject.inverterEfficiencyCalculationMode();
  if (s) {
    pcm.setString(Generator_FuelCell_InverterFields::InverterEfficiencyCalculationMode, s.get());
  }

  //InverterEfficiency
  d = modelObject.inverterEfficiency();
  if (d) {
    pcm.setDouble(Generator_FuelCell_InverterFields::InverterEfficiency, d.get());
  }

  //EfficiencyFunctionofDCPowerCurveName
  curvequad = modelObject.efficiencyFunctionofDCPowerCurve();
  if (curvequad) {
    pcm.setString(Generator_FuelCell_InverterFields::EfficiencyFunctionofDCPowerCurveName, curvequad.get().nameString());
  }

  return pcm;

}

} // energyplus

} // openstudio

