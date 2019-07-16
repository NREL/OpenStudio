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
#include "../../model/RefrigerationCompressor.hpp"
#include "../../model/CurveBicubic.hpp"

#include <utilities/idd/Refrigeration_Compressor_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateRefrigerationCompressor( RefrigerationCompressor & modelObject )
{
  OptionalModelObject temp;
  OptionalString optS;
  boost::optional<std::string> s;
  boost::optional<double> d;

// Name
  IdfObject object = createRegisterAndNameIdfObject(openstudio::IddObjectType::Refrigeration_Compressor, modelObject);

//Refrigeration Compressor Power Curve Name
  boost::optional<CurveBicubic> powerCurve = modelObject.refrigerationCompressorPowerCurve();

  if( powerCurve )
  {
    boost::optional<IdfObject> _powerCurve = translateAndMapModelObject(powerCurve.get());

    if( _powerCurve && _powerCurve->name() )
    {
      object.setString(Refrigeration_CompressorFields::RefrigerationCompressorPowerCurveName,_powerCurve->name().get());
    }
  }

//Refrigeration Compressor Capacity Curve name
  boost::optional<CurveBicubic> capacityCurve = modelObject.refrigerationCompressorCapacityCurve();

  if( capacityCurve )
  {
    boost::optional<IdfObject> _capacityCurve = translateAndMapModelObject(capacityCurve.get());

    if( _capacityCurve && _capacityCurve->name() )
    {
      object.setString(Refrigeration_CompressorFields::RefrigerationCompressorCapacityCurveName,_capacityCurve->name().get());
    }
  }

//Rated Superheat
  d = modelObject.ratedSuperheat();
  if (d) {
    object.setDouble(Refrigeration_CompressorFields::RatedSuperheat,d.get());
  }

//Rated Return Gas Temperature
  d = modelObject.ratedReturnGasTemperature();
  if (d) {
    object.setDouble(Refrigeration_CompressorFields::RatedReturnGasTemperature,d.get());
  }

//Rated Liquid Temperature
  d = modelObject.ratedLiquidTemperature();
  if (d) {
    object.setDouble(Refrigeration_CompressorFields::RatedLiquidTemperature,d.get());
  }

//Rated Subcooling
  d = modelObject.ratedSubcooling();
  if (d) {
    object.setDouble(Refrigeration_CompressorFields::RatedSubcooling,d.get());
  }

//End-Use Subcategory
  s = modelObject.endUseSubcategory();
  if (s) {
    object.setString(Refrigeration_CompressorFields::EndUseSubcategory,s.get());
  }

//Mode of Operation
  s = modelObject.modeofOperation();
  if (s) {
    object.setString(Refrigeration_CompressorFields::ModeofOperation,s.get());
  }

//Transcritical Compressor Power Curve Name
  boost::optional<CurveBicubic> transcriticalPowerCurve = modelObject.transcriticalCompressorPowerCurve();

  if( transcriticalPowerCurve )
  {
    boost::optional<IdfObject> _transcriticalPowerCurve = translateAndMapModelObject(transcriticalPowerCurve.get());

    if( _transcriticalPowerCurve && _transcriticalPowerCurve->name() )
    {
      object.setString(Refrigeration_CompressorFields::TranscriticalCompressorPowerCurveName,_transcriticalPowerCurve->name().get());
    }
  }

//Transcritical Compressor Capacity Curve Name
  boost::optional<CurveBicubic> transcriticalCapacityCurve = modelObject.transcriticalCompressorCapacityCurve();

  if( transcriticalCapacityCurve )
  {
    boost::optional<IdfObject> _transcriticalCapacityCurve = translateAndMapModelObject(transcriticalCapacityCurve.get());

    if( _transcriticalCapacityCurve && _transcriticalCapacityCurve->name() )
    {
      object.setString(Refrigeration_CompressorFields::TranscriticalCompressorCapacityCurveName,_transcriticalCapacityCurve->name().get());
    }
  }

  return object;

}
}
}
