/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

  boost::optional<IdfObject> ForwardTranslator::translateCoilHeatingGas(CoilHeatingGas& modelObject) {
    OptionalString s;
    OptionalModelObject temp;

    IdfObject idfObject(IddObjectType::Coil_Heating_Fuel);

    ///////////////////////////////////////////////////////////////////////////
    // Field: Name ////////////////////////////////////////////////////////////
    s = modelObject.name();
    if (s) {
      idfObject.setName(*s);
    }
    ///////////////////////////////////////////////////////////////////////////

    // hook up required objects
    try {
      Schedule sched = modelObject.availableSchedule();
      translateAndMapModelObject(sched);
      idfObject.setString(Coil_Heating_FuelFields::AvailabilityScheduleName, sched.name().get());
    } catch (std::exception& e) {
      LOG(Error, "Could not translate " << modelObject.briefDescription() << ", because " << e.what() << ".");
      return boost::none;
    }

    ///////////////////////////////////////////////////////////////////////////
    // Field: Fuel Type
    ///////////////////////////////////////////////////////////////////////////
    idfObject.setString(openstudio::Coil_Heating_FuelFields::FuelType, modelObject.fuelType());

    ///////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
    // Field: Gas Burner Efficiency ///////////////////////////////////////////
    idfObject.setDouble(openstudio::Coil_Heating_FuelFields::BurnerEfficiency, modelObject.gasBurnerEfficiency());
    ///////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
    // Field: Nominal Capacity ////////////////////////////////////////////////
    OptionalDouble d = modelObject.nominalCapacity();
    if (d) {
      idfObject.setDouble(openstudio::Coil_Heating_FuelFields::NominalCapacity, *d);
    } else {
      idfObject.setString(openstudio::Coil_Heating_FuelFields::NominalCapacity, "AutoSize");
    }
    ///////////////////////////////////////////////////////////////////////////
    idfObject.setDouble(openstudio::Coil_Heating_FuelFields::ParasiticElectricLoad, modelObject.parasiticElectricLoad());
    idfObject.setDouble(openstudio::Coil_Heating_FuelFields::ParasiticFuelLoad, modelObject.parasiticGasLoad());

    ///////////////////////////////////////////////////////////////////////////
    // Field: Air Inlet Node Name /////////////////////////////////////////////
    temp = modelObject.inletModelObject();
    if (temp) {
      s = temp->name();
      if (s) {
        idfObject.setString(openstudio::Coil_Heating_FuelFields::AirInletNodeName, *s);
      }
    }
    ///////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
    // Field: Air Outlet Node Name ////////////////////////////////////////////
    temp = modelObject.outletModelObject();
    if (temp) {
      s = temp->name();
      if (s) {
        idfObject.setString(openstudio::Coil_Heating_FuelFields::AirOutletNodeName, *s);
        idfObject.setString(openstudio::Coil_Heating_FuelFields::TemperatureSetpointNodeName, *s);
      }
    }
    ///////////////////////////////////////////////////////////////////////////

    m_idfObjects.push_back(idfObject);

    // Part Load Fraction Correlation Curve
    if (boost::optional<model::Curve> curve = modelObject.partLoadFractionCorrelationCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(Coil_Heating_FuelFields::PartLoadFractionCorrelationCurveName, _curve->name().get());
      }
    }

    return boost::optional<IdfObject>(idfObject);
  }

}  // namespace energyplus

}  // namespace openstudio
