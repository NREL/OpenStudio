/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/RefrigerationCompressorRack.hpp"
#include "../../model/Curve.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/RefrigerationAirChiller.hpp"
#include "../../model/RefrigerationCase.hpp"
#include "../../model/RefrigerationWalkIn.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/Refrigeration_CompressorRack_FieldEnums.hxx>
#include <utilities/idd/Refrigeration_CaseAndWalkInList_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateRefrigerationCompressorRack(RefrigerationCompressorRack& modelObject) {
    OptionalModelObject temp;
    boost::optional<std::string> s;
    boost::optional<double> d;
    boost::optional<int> i;
    std::string name;

    // Name
    IdfObject object = createRegisterAndNameIdfObject(openstudio::IddObjectType::Refrigeration_CompressorRack, modelObject);

    // Heat Rejection Location
    s = modelObject.heatRejectionLocation();
    if (s) {
      object.setString(Refrigeration_CompressorRackFields::HeatRejectionLocation, s.get());
    }

    // Design Compressor Rack COP
    d = modelObject.designCompressorRackCOP();
    if (d) {
      object.setDouble(Refrigeration_CompressorRackFields::DesignCompressorRackCOP, d.get());
    }

    // Compressor Rack COP Function of Temperature Curve Name
    boost::optional<Curve> compressorRackCOPFunctionofTemperatureCurve = modelObject.compressorRackCOPFunctionofTemperatureCurve();

    if (compressorRackCOPFunctionofTemperatureCurve) {
      boost::optional<IdfObject> _compressorRackCOPFunctionofTemperatureCurve =
        translateAndMapModelObject(compressorRackCOPFunctionofTemperatureCurve.get());

      if (_compressorRackCOPFunctionofTemperatureCurve && _compressorRackCOPFunctionofTemperatureCurve->name()) {
        object.setString(Refrigeration_CompressorRackFields::CompressorRackCOPFunctionofTemperatureCurveName,
                         _compressorRackCOPFunctionofTemperatureCurve->name().get());
      }
    }

    // Design Condenser Fan Power
    d = modelObject.designCondenserFanPower();
    if (d) {
      object.setDouble(Refrigeration_CompressorRackFields::DesignCondenserFanPower, d.get());
    }

    // Condenser Fan Power Function of Temperature Curve Name
    boost::optional<Curve> condenserFanPowerFunctionofTemperatureCurve = modelObject.condenserFanPowerFunctionofTemperatureCurve();

    if (condenserFanPowerFunctionofTemperatureCurve) {
      boost::optional<IdfObject> _condenserFanPowerFunctionofTemperatureCurve =
        translateAndMapModelObject(condenserFanPowerFunctionofTemperatureCurve.get());

      if (_condenserFanPowerFunctionofTemperatureCurve && _condenserFanPowerFunctionofTemperatureCurve->name()) {
        object.setString(Refrigeration_CompressorRackFields::CondenserFanPowerFunctionofTemperatureCurveName,
                         _condenserFanPowerFunctionofTemperatureCurve->name().get());
      }
    }

    // Condenser Type
    s = modelObject.condenserType();
    if (s) {
      object.setString(Refrigeration_CompressorRackFields::CondenserType, s.get());
    }

    // Water-Cooled Condenser Inlet Node Name
    if ((temp = modelObject.inletModelObject())) {
      if (temp->name()) {
        object.setString(Refrigeration_CompressorRackFields::WaterCooledCondenserInletNodeName, temp->name().get());
      }
    }

    // Water-Cooled Condenser Outlet Node Name
    if ((temp = modelObject.outletModelObject())) {
      if (temp->name()) {
        object.setString(Refrigeration_CompressorRackFields::WaterCooledCondenserOutletNodeName, temp->name().get());
      }
    }

    // Water-Cooled Loop Flow Type
    s = modelObject.waterCooledLoopFlowType();
    if (s) {
      object.setString(Refrigeration_CompressorRackFields::WaterCooledLoopFlowType, s.get());
    }

    // Water-Cooled Condenser Outlet Temperature Schedule Name
    boost::optional<Schedule> waterCooledCondenserOutletTemperatureSchedule = modelObject.waterCooledCondenserOutletTemperatureSchedule();

    if (waterCooledCondenserOutletTemperatureSchedule) {
      boost::optional<IdfObject> _waterCooledCondenserOutletTemperatureSchedule =
        translateAndMapModelObject(waterCooledCondenserOutletTemperatureSchedule.get());

      if (_waterCooledCondenserOutletTemperatureSchedule && _waterCooledCondenserOutletTemperatureSchedule->name()) {
        object.setString(Refrigeration_CompressorRackFields::WaterCooledCondenserOutletTemperatureScheduleName,
                         _waterCooledCondenserOutletTemperatureSchedule->name().get());
      }
    }

    // Water-Cooled Condenser Design Flow Rate
    d = modelObject.waterCooledCondenserDesignFlowRate();
    if (d) {
      object.setDouble(Refrigeration_CompressorRackFields::WaterCooledCondenserDesignFlowRate, d.get());
    }

    // Water-Cooled Condenser Maximum Flow Rate
    d = modelObject.waterCooledCondenserMaximumFlowRate();
    if (d) {
      object.setDouble(Refrigeration_CompressorRackFields::WaterCooledCondenserMaximumFlowRate, d.get());
    }

    // Water-Cooled Condenser Maximum Water Outlet Temperature
    d = modelObject.waterCooledCondenserMaximumWaterOutletTemperature();
    if (d) {
      object.setDouble(Refrigeration_CompressorRackFields::WaterCooledCondenserMaximumWaterOutletTemperature, d.get());
    }

    // Water-Cooled Condenser Minimum Water Inlet Temperature
    d = modelObject.waterCooledCondenserMinimumWaterInletTemperature();
    if (d) {
      object.setDouble(Refrigeration_CompressorRackFields::WaterCooledCondenserMinimumWaterInletTemperature, d.get());
    }

    // Evaporative Condenser Availability Schedule Name
    boost::optional<Schedule> evaporativeCondenserAvailabilitySchedule = modelObject.evaporativeCondenserAvailabilitySchedule();

    if (evaporativeCondenserAvailabilitySchedule) {
      boost::optional<IdfObject> _evaporativeCondenserAvailabilitySchedule =
        translateAndMapModelObject(evaporativeCondenserAvailabilitySchedule.get());

      if (_evaporativeCondenserAvailabilitySchedule && _evaporativeCondenserAvailabilitySchedule->name()) {
        object.setString(Refrigeration_CompressorRackFields::EvaporativeCondenserAvailabilityScheduleName,
                         _evaporativeCondenserAvailabilitySchedule->name().get());
      }
    }

    // Evaporative Condenser Effectiveness
    d = modelObject.evaporativeCondenserEffectiveness();
    if (d) {
      object.setDouble(Refrigeration_CompressorRackFields::EvaporativeCondenserEffectiveness, d.get());
    }

    // Evaporative Condenser Air Flow Rate
    d = modelObject.evaporativeCondenserAirFlowRate();
    if (d) {
      object.setDouble(Refrigeration_CompressorRackFields::EvaporativeCondenserAirFlowRate, d.get());
    }

    // Basin Heater Capacity
    d = modelObject.basinHeaterCapacity();
    if (d) {
      object.setDouble(Refrigeration_CompressorRackFields::BasinHeaterCapacity, d.get());
    }

    // Basin Heater Setpoint Temperature
    d = modelObject.basinHeaterSetpointTemperature();
    if (d) {
      object.setDouble(Refrigeration_CompressorRackFields::BasinHeaterSetpointTemperature, d.get());
    }

    // Design Evaporative Condenser Water Pump Power
    d = modelObject.designEvaporativeCondenserWaterPumpPower();
    if (d) {
      object.setDouble(Refrigeration_CompressorRackFields::DesignEvaporativeCondenserWaterPumpPower, d.get());
    }

    // Evaporative Water Supply Tank Name
    object.setString(Refrigeration_CompressorRackFields::EvaporativeWaterSupplyTankName, "");

    // Condenser Air Inlet Node Name
    object.setString(Refrigeration_CompressorRackFields::CondenserAirInletNodeName, "");

    // End-Use Subcategory
    s = modelObject.endUseSubcategory();
    if (s) {
      object.setString(Refrigeration_CompressorRackFields::EndUseSubcategory, s.get());
    }

    // Refrigeration Case Name or WalkIn Name or CaseAndWalkInList Name
    std::vector<RefrigerationCase> cases = modelObject.cases();
    std::vector<RefrigerationWalkIn> walkins = modelObject.walkins();
    std::vector<RefrigerationAirChiller> airChillers = modelObject.airChillers();

    if (!cases.empty() || !walkins.empty() || !airChillers.empty()) {
      // Name
      name = " Case and Walkin List";
      object.setString(Refrigeration_CompressorRackFields::RefrigerationCaseNameorWalkInNameorCaseAndWalkInListName, object.name().get() + name);

      IdfObject _caseAndWalkinList(IddObjectType::Refrigeration_CaseAndWalkInList);

      m_idfObjects.push_back(_caseAndWalkinList);

      _caseAndWalkinList.setName(object.name().get() + name);

      for (auto& elem : cases) {
        boost::optional<IdfObject> _case = translateAndMapModelObject(elem);

        if (_case) {
          IdfExtensibleGroup eg = _caseAndWalkinList.pushExtensibleGroup();

          eg.setString(Refrigeration_CaseAndWalkInListExtensibleFields::CaseorWalkInName, _case->name().get());
        }
      }

      for (auto& elem : walkins) {
        boost::optional<IdfObject> _walkin = translateAndMapModelObject(elem);

        if (_walkin) {
          IdfExtensibleGroup eg = _caseAndWalkinList.pushExtensibleGroup();

          eg.setString(Refrigeration_CaseAndWalkInListExtensibleFields::CaseorWalkInName, _walkin->name().get());
        }
      }

      for (auto& elem : airChillers) {
        boost::optional<IdfObject> _airChiller = translateAndMapModelObject(elem);

        if (_airChiller) {
          IdfExtensibleGroup eg = _caseAndWalkinList.pushExtensibleGroup();

          eg.setString(Refrigeration_CaseAndWalkInListExtensibleFields::CaseorWalkInName, _airChiller->name().get());
        }
      }
    }

    // Heat Rejection Zone Name
    boost::optional<ThermalZone> heatRejectionZone = modelObject.heatRejectionZone();

    if (heatRejectionZone) {
      boost::optional<IdfObject> _heatRejectionZone = translateAndMapModelObject(heatRejectionZone.get());

      if (_heatRejectionZone && _heatRejectionZone->name()) {
        object.setString(Refrigeration_CompressorRackFields::HeatRejectionZoneName, _heatRejectionZone->name().get());
      }
    }

    return object;
  }
}  // namespace energyplus
}  // namespace openstudio
