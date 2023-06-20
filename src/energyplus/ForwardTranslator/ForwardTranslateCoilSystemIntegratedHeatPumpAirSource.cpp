/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/CoilSystemIntegratedHeatPumpAirSource.hpp"
#include "../../model/CoilSystemIntegratedHeatPumpAirSource_Impl.hpp"
#include "../../model/Model.hpp"
#include <utilities/idd/CoilSystem_IntegratedHeatPump_AirSource_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;
using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateCoilSystemIntegratedHeatPumpAirSource(CoilSystemIntegratedHeatPumpAirSource& modelObject) {
    IdfObject idfObject(IddObjectType::CoilSystem_IntegratedHeatPump_AirSource);
    m_idfObjects.push_back(idfObject);

    // Name
    if (auto s = modelObject.name()) {
      idfObject.setName(*s);
    }

    boost::optional<std::string> s;
    boost::optional<double> value;
    boost::optional<int> i;

    // Space Cooling Coil Name
    boost::optional<IdfObject> _spaceCoolingCoil;
    auto spaceCoolingCoil = modelObject.spaceCoolingCoil();
    if ((_spaceCoolingCoil = translateAndMapModelObject(spaceCoolingCoil))) {
      idfObject.setString(CoilSystem_IntegratedHeatPump_AirSourceFields::SpaceCoolingCoilName, _spaceCoolingCoil->name().get());
    }

    // Space Heating Coil Name
    boost::optional<IdfObject> _spaceHeatingCoil;
    auto spaceHeatingCoil = modelObject.spaceHeatingCoil();
    if ((_spaceHeatingCoil = translateAndMapModelObject(spaceHeatingCoil))) {
      idfObject.setString(CoilSystem_IntegratedHeatPump_AirSourceFields::SpaceHeatingCoilName, _spaceHeatingCoil->name().get());
    }

    // Dedicated Water Heating Coil Name
    boost::optional<IdfObject> _dedicatedWaterHeatingCoil;
    auto dedicatedWaterHeatingCoil = modelObject.dedicatedWaterHeatingCoil();
    if ((_dedicatedWaterHeatingCoil = translateAndMapModelObject(dedicatedWaterHeatingCoil))) {
      idfObject.setString(CoilSystem_IntegratedHeatPump_AirSourceFields::DedicatedWaterHeatingCoilName, _dedicatedWaterHeatingCoil->name().get());
    }

    // SCWH Coil Name
    boost::optional<IdfObject> _scwhCoil;
    auto scwhCoil = modelObject.scwhCoil();
    if ((_scwhCoil = translateAndMapModelObject(scwhCoil))) {
      idfObject.setString(CoilSystem_IntegratedHeatPump_AirSourceFields::SCWHCoilName, _scwhCoil->name().get());
    }

    // SCDWH Cooling Coil Name
    boost::optional<IdfObject> _scdwhCoolingCoil;
    auto scdwhCoolingCoil = modelObject.scdwhCoolingCoil();
    if ((_scdwhCoolingCoil = translateAndMapModelObject(scdwhCoolingCoil))) {
      idfObject.setString(CoilSystem_IntegratedHeatPump_AirSourceFields::SCDWHCoolingCoilName, _scdwhCoolingCoil->name().get());
    }

    // SCDWH Water Heating Coil Name
    boost::optional<IdfObject> _scdwhWaterHeatingCoil;
    auto scdwhWaterHeatingCoil = modelObject.scdwhWaterHeatingCoil();
    if ((_scdwhWaterHeatingCoil = translateAndMapModelObject(scdwhWaterHeatingCoil))) {
      idfObject.setString(CoilSystem_IntegratedHeatPump_AirSourceFields::SCDWHWaterHeatingCoilName, _scdwhWaterHeatingCoil->name().get());
    }

    // SHDWH Heating Coil Name
    boost::optional<IdfObject> _shdwhHeatingCoil;
    auto shdwhHeatingCoil = modelObject.shdwhHeatingCoil();
    if ((_shdwhHeatingCoil = translateAndMapModelObject(shdwhHeatingCoil))) {
      idfObject.setString(CoilSystem_IntegratedHeatPump_AirSourceFields::SHDWHHeatingCoilName, _shdwhHeatingCoil->name().get());
    }

    // SHDWH Water Heating Coil Name
    boost::optional<IdfObject> _shdwhWaterHeatingCoil;
    auto shdwhWaterHeatingCoil = modelObject.shdwhWaterHeatingCoil();
    if ((_shdwhWaterHeatingCoil = translateAndMapModelObject(shdwhWaterHeatingCoil))) {
      idfObject.setString(CoilSystem_IntegratedHeatPump_AirSourceFields::SHDWHWaterHeatingCoilName, _shdwhWaterHeatingCoil->name().get());
    }

    // Indoor Temperature Limit for SCWH Mode
    if ((value = modelObject.indoorTemperatureLimitForSCWHMode())) {
      idfObject.setDouble(CoilSystem_IntegratedHeatPump_AirSourceFields::IndoorTemperatureLimitforSCWHMode, value.get());
    }

    // Ambient Temperature Limit for SCWH Mode
    if ((value = modelObject.ambientTemperatureLimitForSCWHMode())) {
      idfObject.setDouble(CoilSystem_IntegratedHeatPump_AirSourceFields::AmbientTemperatureLimitforSCWHMode, value.get());
    }

    // Indoor Temperature above Which WH has Higher Priority
    if ((value = modelObject.indoorTemperatureAboveWhichWHHasHigherPriority())) {
      idfObject.setDouble(CoilSystem_IntegratedHeatPump_AirSourceFields::IndoorTemperatureaboveWhichWHhasHigherPriority, value.get());
    }

    // Ambient Temperature above Which WH has Higher Priority
    if ((value = modelObject.ambientTemperatureAboveWhichWHHasHigherPriority())) {
      idfObject.setDouble(CoilSystem_IntegratedHeatPump_AirSourceFields::AmbientTemperatureaboveWhichWHhasHigherPriority, value.get());
    }

    // Flag to Indicate Load Control in SCWH Mode
    if ((i = modelObject.flagtoIndicateLoadControlInSCWHMode())) {
      idfObject.setInt(CoilSystem_IntegratedHeatPump_AirSourceFields::FlagtoIndicateLoadControlinSCWHMode, i.get());
    }

    // Minimum Speed Level for SCWH Mode
    if ((i = modelObject.minimumSpeedLevelForSCWHMode())) {
      idfObject.setInt(CoilSystem_IntegratedHeatPump_AirSourceFields::MinimumSpeedLevelforSCWHMode, i.get());
    }

    // Maximum Water Flow Volume before Switching from SCDWH to SCWH Mode
    if ((value = modelObject.maximumWaterFlowVolumeBeforeSwitchingfromSCDWHtoSCWHMode())) {
      idfObject.setDouble(CoilSystem_IntegratedHeatPump_AirSourceFields::MaximumWaterFlowVolumebeforeSwitchingfromSCDWHtoSCWHMode, value.get());
    }

    // Minimum Speed Level for SCDWH Mode
    if ((i = modelObject.minimumSpeedLevelForSCDWHMode())) {
      idfObject.setInt(CoilSystem_IntegratedHeatPump_AirSourceFields::MinimumSpeedLevelforSCDWHMode, i.get());
    }

    // Maximum Running Time before Allowing Electric Resistance Heat Use during SHDWH Mode
    if ((value = modelObject.maximumRunningTimeBeforeAllowingElectricResistanceHeatUseDuringSHDWHMode())) {
      idfObject.setDouble(CoilSystem_IntegratedHeatPump_AirSourceFields::MaximumRunningTimebeforeAllowingElectricResistanceHeatUseduringSHDWHMode,
                          value.get());
    }

    // Minimum Speed Level for SHDWH Mode
    if ((i = modelObject.minimumSpeedLevelForSHDWHMode())) {
      idfObject.setInt(CoilSystem_IntegratedHeatPump_AirSourceFields::MinimumSpeedLevelforSHDWHMode, i.get());
    }

    return idfObject;
  }

}  // namespace energyplus
}  // namespace openstudio
