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
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/Curve.hpp"
#include "../../model/Curve_Impl.hpp"
#include "../../model/BoilerSteam.hpp"
#include "../../model/BoilerSteam_Impl.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/Boiler_Steam_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateBoilerSteam( BoilerSteam & modelObject )
{
  boost::optional<std::string> s;
  boost::optional<double> value;

  IdfObject idfObject(IddObjectType::Boiler_Steam);

  m_idfObjects.push_back(idfObject);

  // Name

  s = modelObject.name();
  if( s )
  {
    idfObject.setName(*s);
  }

  // FuelType

  if( (s = modelObject.fuelType()) )
  {
    idfObject.setString(Boiler_SteamFields::FuelType,s.get());
  }

  // MaximumOperatingPressure

  if( (value = modelObject.maximumOperatingPressure()) )
  {
    idfObject.setDouble(Boiler_SteamFields::MaximumOperatingPressure,value.get());
  }

  // TheoreticalEfficiency

  if( (value = modelObject.theoreticalEfficiency()) )
  {
    idfObject.setDouble(Boiler_SteamFields::TheoreticalEfficiency,value.get());
  }

  // DesignOutletSteamTemperature

  if( (value = modelObject.designOutletSteamTemperature()) )
  {
    idfObject.setDouble(Boiler_SteamFields::DesignOutletSteamTemperature,value.get());
  }

  // NominalCapacity

  if( modelObject.isNominalCapacityAutosized() )
  {
    idfObject.setString(Boiler_SteamFields::NominalCapacity,"Autosize");
  }
  else if( (value = modelObject.nominalCapacity()) )
  {
    idfObject.setDouble(Boiler_SteamFields::NominalCapacity,value.get());
  }

  // MinimumPartLoadRatio

  if( (value = modelObject.minimumPartLoadRatio()) )
  {
    idfObject.setDouble(Boiler_SteamFields::MinimumPartLoadRatio,value.get());
  }

  // MaximumPartLoadRatio

  if( (value = modelObject.maximumPartLoadRatio()) )
  {
    idfObject.setDouble(Boiler_SteamFields::MaximumPartLoadRatio,value.get());
  }

  // OptimumPartLoadRatio

  if( (value = modelObject.optimumPartLoadRatio()) )
  {
    idfObject.setDouble(Boiler_SteamFields::OptimumPartLoadRatio,value.get());
  }

  // Coefficient1ofFuelUseFunctionofPartLoadRatioCurve

  if( (value = modelObject.coefficient1ofFuelUseFunctionofPartLoadRatioCurve()) )
  {
    idfObject.setDouble(Boiler_SteamFields::Coefficient1ofFuelUseFunctionofPartLoadRatioCurve,value.get());
  }

  // Coefficient2ofFuelUseFunctionofPartLoadRatioCurve

  if( (value = modelObject.coefficient2ofFuelUseFunctionofPartLoadRatioCurve()) )
  {
    idfObject.setDouble(Boiler_SteamFields::Coefficient2ofFuelUseFunctionofPartLoadRatioCurve,value.get());
  }

  // Coefficient3ofFuelUseFunctionofPartLoadRatioCurve

  if( (value = modelObject.coefficient3ofFuelUseFunctionofPartLoadRatioCurve()) )
  {
    idfObject.setDouble(Boiler_SteamFields::Coefficient3ofFuelUseFunctionofPartLoadRatioCurve,value.get());
  }

  // WaterInletNodeName

  if( boost::optional<ModelObject> mo = modelObject.inletModelObject() )
  {
    if( boost::optional<Node> node = mo->optionalCast<Node>() )
    {
      idfObject.setString(Boiler_SteamFields::WaterInletNodeName,node->name().get());
    }
  }

  // SteamOutletNodeName

  if( boost::optional<ModelObject> mo = modelObject.outletModelObject() )
  {
    if( boost::optional<Node> node = mo->optionalCast<Node>() )
    {
      idfObject.setString(Boiler_SteamFields::SteamOutletNodeName,node->name().get());
    }
  }

  // SizingFactor

  if( (value = modelObject.sizingFactor()) )
  {
    idfObject.setDouble(Boiler_SteamFields::SizingFactor,value.get());
  }

  // End Use Subcategory
  if( (s = modelObject.endUseSubcategory()) ) {
    idfObject.setString(Boiler_SteamFields::EndUseSubcategory,s.get());
  }

  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio

