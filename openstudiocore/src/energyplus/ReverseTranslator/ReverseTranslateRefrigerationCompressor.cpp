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

#include "../ReverseTranslator.hpp"
#include "../../model/RefrigerationCompressor.hpp"
#include "../../model/RefrigerationCompressor_Impl.hpp"
#include "../../model/CurveBicubic.hpp"
#include "../../model/CurveBicubic_Impl.hpp"
#include <utilities/idd/Refrigeration_Compressor_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateRefrigerationCompressor( const WorkspaceObject & workspaceObject )
{
  if( workspaceObject.iddObject().type() != IddObjectType::Refrigeration_Compressor )
  {
     LOG(Error, "WorkspaceObject is not IddObjectType: Refrigeration_Compressor");
     return boost::none;
  }

  boost::optional<RefrigerationCompressor> refrigerationCompressor = RefrigerationCompressor( m_model );
  boost::optional<WorkspaceObject> wo;

  if( refrigerationCompressor )
  {
    boost::optional<double> value;
    boost::optional<std::string> s = workspaceObject.getString(Refrigeration_CompressorFields::Name);

// Name
    if( s )
    {
      refrigerationCompressor->setName(s.get());
    }

// RefrigerationCompressorPowerCurveName
    if( (wo = workspaceObject.getTarget(Refrigeration_CompressorFields::RefrigerationCompressorPowerCurveName)) )
    {
      if( boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(wo.get()) )
      {
        if( boost::optional<CurveBicubic> curve = mo->optionalCast<CurveBicubic>() )
        {
          refrigerationCompressor->setRefrigerationCompressorPowerCurve(curve.get());
        }
      }
    }
// RefrigerationCompressorCapacityCurveName
    if( (wo = workspaceObject.getTarget(Refrigeration_CompressorFields::RefrigerationCompressorCapacityCurveName)) )
    {
      if( boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(wo.get()) )
      {
        if( boost::optional<CurveBicubic> curve = mo->optionalCast<CurveBicubic>() )
        {
          refrigerationCompressor->setRefrigerationCompressorCapacityCurve(curve.get());
        }
      }
    }
// RatedSuperheat
    value = workspaceObject.getDouble(Refrigeration_CompressorFields::RatedSuperheat);
    if( value )
    {
      refrigerationCompressor->setRatedSuperheat(value.get());
    }
// RatedReturnGasTemperature
    value = workspaceObject.getDouble(Refrigeration_CompressorFields::RatedReturnGasTemperature);
    if( value )
    {
      refrigerationCompressor->setRatedReturnGasTemperature(value.get());
    }
// RatedLiquidTemperature
    value = workspaceObject.getDouble(Refrigeration_CompressorFields::RatedLiquidTemperature);
    if( value )
    {
      refrigerationCompressor->setRatedLiquidTemperature(value.get());
    }
// RatedSubcooling
    value = workspaceObject.getDouble(Refrigeration_CompressorFields::RatedSubcooling);
    if( value )
    {
      refrigerationCompressor->setRatedSubcooling(value.get());
    }
// EndUseSubcategory
    s = workspaceObject.getString(Refrigeration_CompressorFields::EndUseSubcategory);
    if( s )
    {
      refrigerationCompressor->setEndUseSubcategory(s.get());
    }
// ModeofOperation
// TranscriticalCompressorPowerCurveName
    if( (wo = workspaceObject.getTarget(Refrigeration_CompressorFields::TranscriticalCompressorPowerCurveName)) )
    {
      if( boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(wo.get()) )
      {
        if( boost::optional<CurveBicubic> curve = mo->optionalCast<CurveBicubic>() )
        {
          refrigerationCompressor->setTranscriticalCompressorPowerCurve(curve.get());
        }
      }
    }
// TranscriticalCompressorCapacityCurveName
    if( (wo = workspaceObject.getTarget(Refrigeration_CompressorFields::TranscriticalCompressorCapacityCurveName)) )
    {
      if( boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(wo.get()) )
      {
        if( boost::optional<CurveBicubic> curve = mo->optionalCast<CurveBicubic>() )
        {
          refrigerationCompressor->setTranscriticalCompressorCapacityCurve(curve.get());
        }
      }
    }

    return refrigerationCompressor.get();
  }
  else
  {
    LOG(Error, "Unknown error translating " << workspaceObject.briefDescription());

    return boost::none;
  }
}

} // energyplus

} // openstudio

