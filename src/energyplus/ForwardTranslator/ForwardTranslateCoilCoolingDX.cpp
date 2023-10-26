/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"

#include "../../model/CoilCoolingDX.hpp"

#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/CoilCoolingDXCurveFitPerformance.hpp"
#include "../../model/CoilCoolingDXCurveFitPerformance_Impl.hpp"
#include <utilities/idd/CoilSystem_Cooling_DX_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_FieldEnums.hxx>
// #include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateCoilCoolingDXWithoutUnitary(model::CoilCoolingDX& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> value;

    // PerformanceObjectName, is required, so start by that
    CoilCoolingDXCurveFitPerformance performance = modelObject.performanceObject();
    if (boost::optional<IdfObject> _performance = translateAndMapModelObject(performance)) {
      s = _performance->name().get();
    } else {
      LOG(Warn, modelObject.briefDescription() << " cannot be translated as its performance object cannot be translated: "
                                               << performance.briefDescription() << ".");
      return boost::none;
    }

    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::Coil_Cooling_DX, modelObject);

    // PerformanceObjectName
    idfObject.setString(Coil_Cooling_DXFields::PerformanceObjectName, s.get());

    // Evaporator Nodes are handled in the FT for AirLoopHVACUnitarySystem when inside a Unitary, but we set them here in case it's NOT inside a
    // unitary (yet, we'll put it on a CoilSystem:Cooling:DX)
    // EvaporatorInletNodeName
    // EvaporatorOutletNodeName
    OptionalModelObject omo = modelObject.inletModelObject();
    if (omo) {
      translateAndMapModelObject(*omo);
      s = omo->name();
      if (s) {
        idfObject.setString(Coil_Cooling_DXFields::EvaporatorInletNodeName, *s);
      }
    }

    omo = modelObject.outletModelObject();
    if (omo) {
      translateAndMapModelObject(*omo);
      s = omo->name();
      if (s) {
        idfObject.setString(Coil_Cooling_DXFields::EvaporatorOutletNodeName, *s);
      }
    }

    // AvailabilityScheduleName
    {
      auto schedule = modelObject.availabilitySchedule();
      if (boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule)) {
        idfObject.setString(Coil_Cooling_DXFields::AvailabilityScheduleName, _schedule->name().get());
      }
    }

    // CondenserZoneName: as of E+ 9.3.0, this appears unused.
    // TODO: eventually handle the condenser inlet/outlet node connections if the Condenser Zone is used?
    if (boost::optional<ThermalZone> thermalZone = modelObject.condenserZone()) {
      if (boost::optional<IdfObject> _thermalZone = translateAndMapModelObject(thermalZone.get())) {
        idfObject.setString(Coil_Cooling_DXFields::CondenserZoneName, _thermalZone->name().get());
      }
    }

    // CondenserInletNodeName
    std::string condenserInletNodeName(modelObject.nameString() + " Condenser Inlet Node");
    // if (auto _s = modelObject.condenserInletNodeName()) {
    //   condenserInletNodeName =  _s.get();
    // } else {
    // Create an OutdoorAir:NodeList for the condenser inlet conditions to be set directly from weather file
    IdfObject oaNodeListIdf(openstudio::IddObjectType::OutdoorAir_NodeList);
    oaNodeListIdf.setString(0, condenserInletNodeName);
    m_idfObjects.push_back(oaNodeListIdf);
    // }
    idfObject.setString(Coil_Cooling_DXFields::CondenserInletNodeName, condenserInletNodeName);

    // CondenserOutletNodeName
    std::string condenserOutletNodeName(modelObject.nameString() + " Condenser Outlet Node");
    //if (auto _s = modelObject.condenserOutletNodeName()) {
    //condenserOutletNodeName =  _s.get();
    //}
    idfObject.setString(Coil_Cooling_DXFields::CondenserOutletNodeName, condenserOutletNodeName);

    // CondensateCollectionWaterStorageTankName

    // EvaporativeCondenserSupplyWaterStorageTankName

    return boost::optional<IdfObject>(idfObject);
  }  // End of translate function

  boost::optional<IdfObject> ForwardTranslator::translateCoilCoolingDX(CoilCoolingDX& modelObject) {
    IdfObject coilSystemCoolingDXIdf(IddObjectType::CoilSystem_Cooling_DX);

    m_idfObjects.push_back(coilSystemCoolingDXIdf);

    boost::optional<IdfObject> oIdfObject = translateCoilCoolingDXWithoutUnitary(modelObject);

    if (!oIdfObject) {
      return boost::none;
    }

    IdfObject idfObject = oIdfObject.get();

    OptionalString s;

    s = modelObject.name();
    if (s) {
      coilSystemCoolingDXIdf.setString(CoilSystem_Cooling_DXFields::CoolingCoilObjectType, idfObject.iddObject().name());

      coilSystemCoolingDXIdf.setString(CoilSystem_Cooling_DXFields::CoolingCoilName, *s);

      coilSystemCoolingDXIdf.setName(*s + " CoilSystem");
    }

    Schedule sched = modelObject.availabilitySchedule();
    translateAndMapModelObject(sched);

    coilSystemCoolingDXIdf.setString(CoilSystem_Cooling_DXFields::AvailabilityScheduleName, sched.name().get());

    OptionalModelObject omo = modelObject.inletModelObject();
    if (omo) {
      translateAndMapModelObject(*omo);
      s = omo->name();
      if (s) {
        coilSystemCoolingDXIdf.setString(CoilSystem_Cooling_DXFields::DXCoolingCoilSystemInletNodeName, *s);
      }
    }

    omo = modelObject.outletModelObject();
    if (omo) {
      translateAndMapModelObject(*omo);
      s = omo->name();
      if (s) {
        coilSystemCoolingDXIdf.setString(CoilSystem_Cooling_DXFields::DXCoolingCoilSystemOutletNodeName, *s);

        coilSystemCoolingDXIdf.setString(CoilSystem_Cooling_DXFields::DXCoolingCoilSystemSensorNodeName, *s);
      }
    }

    return coilSystemCoolingDXIdf;
  }

}  // end namespace energyplus
}  // end namespace openstudio
