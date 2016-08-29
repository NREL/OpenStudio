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
  // Field: Gas Burner Efficiency ///////////////////////////////////////////
  idfObject.setDouble(openstudio::Coil_Heating_FuelFields::GasBurnerEfficiency,modelObject.gasBurnerEfficiency());
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
  idfObject.setDouble(openstudio::Coil_Heating_FuelFields::ParasiticGasLoad,modelObject.parasiticGasLoad());


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

  // New field in EP 8.6
  idfObject.setString(Coil_Heating_FuelFields::FuelType,"NaturalGas");

  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio

