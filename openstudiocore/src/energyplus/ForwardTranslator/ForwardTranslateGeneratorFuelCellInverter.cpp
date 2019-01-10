/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

