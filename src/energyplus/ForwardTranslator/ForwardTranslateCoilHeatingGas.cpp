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
#include "../../model/CoilHeatingGas.hpp"
#include "../../model/Curve.hpp"
#include "../../model/Schedule.hpp"
#include <utilities/idd/Coil_Heating_Fuel_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateCoilHeatingGas( CoilHeatingGas & modelObject )
{
  OptionalString s;
  OptionalModelObject temp;

  IdfObject idfObject(IddObjectType::Coil_Heating_Fuel);

  ///////////////////////////////////////////////////////////////////////////
  // Field: Name ////////////////////////////////////////////////////////////
  s = modelObject.name();
  if( s )
  {
    idfObject.setName(*s);
  }
  ///////////////////////////////////////////////////////////////////////////

  // hook up required objects
  try {
    Schedule sched = modelObject.availableSchedule();
    translateAndMapModelObject(sched);
    idfObject.setString(Coil_Heating_FuelFields::AvailabilityScheduleName,
                        sched.name().get() );
  }
  catch (std::exception& e) {
    LOG(Error,"Could not translate " << modelObject.briefDescription() << ", because "
        << e.what() << ".");
    return boost::none;
  }

  ///////////////////////////////////////////////////////////////////////////
  // Field: Fuel Type
  ///////////////////////////////////////////////////////////////////////////
  if (modelObject.fuelType() == "PropaneGas") {
    LOG(Warn, "'PropaneGas' is deprecated for Coil_Heating_FuelFields:FuelType, use 'Propane' instead.")
    idfObject.setString(openstudio::Coil_Heating_FuelFields::FuelType, "Propane");
  } else {
    idfObject.setString(openstudio::Coil_Heating_FuelFields::FuelType, modelObject.fuelType());
  }

  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  // Field: Gas Burner Efficiency ///////////////////////////////////////////
  idfObject.setDouble(openstudio::Coil_Heating_FuelFields::BurnerEfficiency,modelObject.gasBurnerEfficiency());
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  // Field: Nominal Capacity ////////////////////////////////////////////////
  OptionalDouble d=modelObject.nominalCapacity();
  if(d)
  {
    idfObject.setDouble(openstudio::Coil_Heating_FuelFields::NominalCapacity,*d);
  }
  else
  {
    idfObject.setString(openstudio::Coil_Heating_FuelFields::NominalCapacity,"AutoSize");
  }
  ///////////////////////////////////////////////////////////////////////////
  idfObject.setDouble(openstudio::Coil_Heating_FuelFields::ParasiticElectricLoad,modelObject.parasiticElectricLoad());
  idfObject.setDouble(openstudio::Coil_Heating_FuelFields::ParasiticFuelLoad,modelObject.parasiticGasLoad());


  ///////////////////////////////////////////////////////////////////////////
  // Field: Air Inlet Node Name /////////////////////////////////////////////
  temp = modelObject.inletModelObject();
  if(temp)
  {
    s=temp->name();
    if( s )
    {
      idfObject.setString(openstudio::Coil_Heating_FuelFields::AirInletNodeName,*s);
    }
  }
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  // Field: Air Outlet Node Name ////////////////////////////////////////////
  temp = modelObject.outletModelObject();
  if(temp)
  {
    s=temp->name();
    if( s)
    {
      idfObject.setString(openstudio::Coil_Heating_FuelFields::AirOutletNodeName,*s);
      idfObject.setString(openstudio::Coil_Heating_FuelFields::TemperatureSetpointNodeName,*s);
    }
  }
  ///////////////////////////////////////////////////////////////////////////

  m_idfObjects.push_back(idfObject);

  // Part Load Fraction Correlation Curve
  if( boost::optional<model::Curve> curve = modelObject.partLoadFractionCorrelationCurve() )
  {
    if( boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get()) )
    {
      idfObject.setString(Coil_Heating_FuelFields::PartLoadFractionCorrelationCurveName,_curve->name().get());
    }
  }

  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio

