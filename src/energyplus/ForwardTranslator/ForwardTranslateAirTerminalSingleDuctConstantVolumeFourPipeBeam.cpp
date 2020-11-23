/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../../model/AirTerminalSingleDuctConstantVolumeFourPipeBeam.hpp"
#include "../../model/AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl.hpp"
#include "../../model/CoilCoolingFourPipeBeam.hpp"
#include "../../model/CoilCoolingFourPipeBeam_Impl.hpp"
#include "../../model/CoilHeatingFourPipeBeam.hpp"
#include "../../model/CoilHeatingFourPipeBeam_Impl.hpp"

#include "../../model/HVACComponent.hpp"
#include "../../model/HVACComponent_Impl.hpp"
#include "../../model/StraightComponent.hpp"
#include "../../model/StraightComponent_Impl.hpp"
#include "../../model/AirLoopHVACZoneSplitter.hpp"
#include "../../model/AirLoopHVACZoneSplitter_Impl.hpp"
#include "../../model/AirLoopHVACZoneMixer.hpp"
#include "../../model/AirLoopHVACZoneMixer_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/PlantLoop.hpp"
#include "../../model/Curve.hpp"

#include <utilities/idd/AirTerminal_SingleDuct_ConstantVolume_FourPipeBeam_FieldEnums.hxx>
#include <utilities/idd/ZoneHVAC_AirDistributionUnit_FieldEnums.hxx>

#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject>
    ForwardTranslator::translateAirTerminalSingleDuctConstantVolumeFourPipeBeam(AirTerminalSingleDuctConstantVolumeFourPipeBeam& modelObject) {

    // Sanity check: this object must be connected to an AirLoopHVAC, and it must have at least one cooling coil or one heating coil that is connected
    // to a PlantLoop
    if (!(modelObject.airLoopHVAC().is_initialized()
          && (modelObject.chilledWaterPlantLoop().is_initialized() || modelObject.hotWaterPlantLoop().is_initialized()))) {
      LOG(Warn, modelObject.briefDescription() << " is not connected to at least one AirLoopHVAC AND one PlantLoop, it will NOT be translated");
      return boost::none;
    }

    // Name
    IdfObject idfObject(openstudio::IddObjectType::AirTerminal_SingleDuct_ConstantVolume_FourPipeBeam);
    m_idfObjects.push_back(idfObject);

    // A1 , \field Name
    std::string baseName = modelObject.name().get();
    idfObject.setName(baseName);

    //register the air distribution unit in the map rather than the chilled beam air terminal itself.
    //this is necessary because the air distribution unit encapsulating the chilled beam air terminal
    //goes on both the brachlist and the zone HVAC equipment list
    //since the branches are translated first, when the chilled beam's coil is encountered in the branchlist, the air distribution unit
    //that encapsulates the chilled beam is returned and put onto the branch list.
    //When the chilled beam air terminal is encountered again on the zone HVAC equipment list,
    //the map must return the air distribution unit to ensure that the air distribution unit is on the translated
    //equipment list.

    IdfObject _airDistributionUnit(IddObjectType::ZoneHVAC_AirDistributionUnit);
    _airDistributionUnit.setName("ADU " + baseName);  //ADU: Air Distribution Unit
    m_idfObjects.push_back(_airDistributionUnit);

    boost::optional<std::string> s;
    boost::optional<double> optD;
    boost::optional<ModelObject> temp;

    // A2 , \field Primary Air Availability Schedule Name
    {
      Schedule schedule = modelObject.primaryAirAvailabilitySchedule();
      if (boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule)) {
        idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::PrimaryAirAvailabilityScheduleName, _schedule->name().get());
      }
    }

    // A3 , \field Cooling Availability Schedule Name
    {
      Schedule schedule = modelObject.coolingAvailabilitySchedule();
      if (boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule)) {
        idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::CoolingAvailabilityScheduleName, _schedule->name().get());
      }
    }

    // A4 , \field Heating Availability Schedule Name
    {
      Schedule schedule = modelObject.heatingAvailabilitySchedule();
      if (boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule)) {
        idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::HeatingAvailabilityScheduleName, _schedule->name().get());
      }
    }

    // A5 , \field Primary Air Inlet Node Name
    temp = modelObject.inletModelObject();
    if (temp) {
      if ((s = temp->name())) {
        idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::PrimaryAirInletNodeName, s.get());
      }
    }

    // A6 , \field Primary Air Outlet Node Name
    temp = modelObject.outletModelObject();
    if (temp) {
      if ((s = temp->name())) {
        idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::PrimaryAirOutletNodeName, s.get());
      }
    }

    /* Double autosizable fields */

    // N1 , \field Design Primary Air Volume Flow Rate
    if (modelObject.isDesignPrimaryAirVolumeFlowRateAutosized()) {
      idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::DesignPrimaryAirVolumeFlowRate, "Autosize");
    } else if ((optD = modelObject.designPrimaryAirVolumeFlowRate())) {
      idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::DesignPrimaryAirVolumeFlowRate, optD.get());
    }

    // N2 , \field Design Chilled Water Volume Flow Rate
    if (modelObject.isDesignChilledWaterVolumeFlowRateAutosized()) {
      idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::DesignChilledWaterVolumeFlowRate, "Autosize");
    } else if ((optD = modelObject.designChilledWaterVolumeFlowRate())) {
      idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::DesignChilledWaterVolumeFlowRate, optD.get());
    }

    // N3 , \field Design Hot Water Volume Flow Rate
    if (modelObject.isDesignHotWaterVolumeFlowRateAutosized()) {
      idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::DesignHotWaterVolumeFlowRate, "Autosize");
    } else if ((optD = modelObject.designHotWaterVolumeFlowRate())) {
      idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::DesignHotWaterVolumeFlowRate, optD.get());
    }

    // N4 , \field Zone Total Beam Length
    if (modelObject.isZoneTotalBeamLengthAutosized()) {
      idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::ZoneTotalBeamLength, "Autosize");
    } else if ((optD = modelObject.zoneTotalBeamLength())) {
      idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::ZoneTotalBeamLength, optD.get());
    }

    /* Double, with defaults */

    // N5 , \field Rated Primary Air Flow Rate per Beam Length
    {
      double value = modelObject.ratedPrimaryAirFlowRateperBeamLength();
      idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::RatedPrimaryAirFlowRateperBeamLength, value);
    }

    // Cooling Side: everything is carried on the subclass CoilCoolingFourPipeBeam

    if (boost::optional<HVACComponent> _comp = modelObject.coolingCoil()) {

      boost::optional<CoilCoolingFourPipeBeam> _cc = _comp->optionalCast<CoilCoolingFourPipeBeam>();

      if (!_cc.is_initialized()) {
        LOG(Error, "Cooling coil is not a CoilCoolingFourPipeBeam for " << modelObject.briefDescription() << ". Coil will not be translated");
      } else {
        CoilCoolingFourPipeBeam cc = _cc.get();

        // It it isn't connected to a plantLoop, it's useless...
        // If it is connected to a plantloop, all three curves are required fields

        /* Curves: all three are required if the FourPipeBeam is connected to a chilled water plant loop, which is the case if we have reached this
     * point, so I will log and not translate is they aren't defined
     * Note that since I assign curves in the Ctor and there isn't a reset function for the curves, we shouldn't really have a reason to have a
     * problem unless something went wrong at some point (or someone used setString("") on the field or something) */

        // A11, \field Beam Cooling Capacity Temperature Difference Modification Factor Curve Name
        boost::optional<Curve> capModFuncOfTempDiff = cc.beamCoolingCapacityTemperatureDifferenceModificationFactorCurve();

        // A12, \field Beam Cooling Capacity Air Flow Modification Factor Curve Name
        boost::optional<Curve> capModFuncOfSAFlow = cc.beamCoolingCapacityAirFlowModificationFactorCurve();

        // A13, \field Beam Cooling Capacity Chilled Water Flow Modification Factor Curve Name
        boost::optional<Curve> capModFuncOfWaterFlow = cc.beamCoolingCapacityChilledWaterFlowModificationFactorCurve();
        // Check that it is connected to a plantLoop
        if (!_cc->plantLoop().is_initialized()) {
          LOG(Warn, "The Cooling side of " << modelObject.briefDescription() << " isn't connected to a plantLoop, it will not be translated");

          // It MUST have the three curves defined in this case
        } else if (!capModFuncOfTempDiff.is_initialized() || !capModFuncOfSAFlow.is_initialized() || !capModFuncOfWaterFlow.is_initialized()) {
          LOG(Warn, "If you connect the cooling side of " << modelObject.briefDescription()
                                                          << " to a plantLoop, you MUST supply all three curves in the subobject "
                                                          << cc.briefDescription());

        } else {
          // A7 , \field Chilled Water Inlet Node Name
          temp = cc.inletModelObject();
          if (temp) {
            if ((s = temp->name())) {
              idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::ChilledWaterInletNodeName, s.get());
            }
          }
          // A8 , \field Chilled Water Outlet Node Name
          temp = cc.outletModelObject();
          if (temp) {
            if ((s = temp->name())) {
              idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::ChilledWaterOutletNodeName, s.get());
            }
          }

          /* Double with defaults (not optionals) */

          // N6 , \field Beam Rated Cooling Capacity per Beam Length
          {
            double value = cc.beamRatedCoolingCapacityperBeamLength();
            idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::BeamRatedCoolingCapacityperBeamLength, value);
          }

          // N7 , \field Beam Rated Cooling Room Air Chilled Water Temperature Difference
          {
            double value = cc.beamRatedCoolingRoomAirChilledWaterTemperatureDifference();
            idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::BeamRatedCoolingRoomAirChilledWaterTemperatureDifference,
                                value);
          }

          // N8 , \field Beam Rated Chilled Water Volume Flow Rate per Beam Length
          {
            double value = cc.beamRatedChilledWaterVolumeFlowRateperBeamLength();
            idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::BeamRatedChilledWaterVolumeFlowRateperBeamLength, value);
          }

          /* Curves: At this point, I have alreaady checked above that all three curves are defined */

          // A11, \field Beam Cooling Capacity Temperature Difference Modification Factor Curve Name
          if (auto _curve = translateAndMapModelObject(capModFuncOfTempDiff.get())) {
            idfObject.setString(
              AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::BeamCoolingCapacityTemperatureDifferenceModificationFactorCurveName,
              _curve->name().get());
          }

          // A12, \field Beam Cooling Capacity Air Flow Modification Factor Curve Name
          if (auto _curve = translateAndMapModelObject(capModFuncOfSAFlow.get())) {
            idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::BeamCoolingCapacityAirFlowModificationFactorCurveName,
                                _curve->name().get());
          }

          // A13, \field Beam Cooling Capacity Chilled Water Flow Modification Factor Curve Name
          if (auto _curve = translateAndMapModelObject(capModFuncOfWaterFlow.get())) {
            idfObject.setString(
              AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::BeamCoolingCapacityChilledWaterFlowModificationFactorCurveName,
              _curve->name().get());
          }
        }
      }
    }

    /**
   * HEATING SIDE: everything is carried on the subclass CoilHeatingFourPipeBeam
   */

    if (boost::optional<HVACComponent> _comp = modelObject.heatingCoil()) {

      boost::optional<CoilHeatingFourPipeBeam> _hc = _comp->optionalCast<CoilHeatingFourPipeBeam>();

      if (!_hc.is_initialized()) {
        LOG(Error, "Heating coil is not a CoilHeatingFourPipeBeam for " << modelObject.briefDescription() << ". Coil will not be translated");
      } else {

        CoilHeatingFourPipeBeam hc = _hc.get();

        // It it isn't connected to a plantLoop, it's useless...
        // If it is connected to a plantloop, all three curves are required fields

        /* Curves: all three are required if the FourPipeBeam is connected to a hot water plant loop, which is the case if we have reached this
       * point, so I will log and not translate is they aren't defined */

        // A11, \field Beam Heating Capacity Temperature Difference Modification Factor Curve Name
        boost::optional<Curve> capModFuncOfTempDiff = hc.beamHeatingCapacityTemperatureDifferenceModificationFactorCurve();

        // A12, \field Beam Heating Capacity Air Flow Modification Factor Curve Name
        boost::optional<Curve> capModFuncOfSAFlow = hc.beamHeatingCapacityAirFlowModificationFactorCurve();

        // A13, \field Beam Heating Capacity Hot Water Flow Modification Factor Curve Name
        boost::optional<Curve> capModFuncOfWaterFlow = hc.beamHeatingCapacityHotWaterFlowModificationFactorCurve();
        // Check that it is connected to a plantLoop
        if (!_hc->plantLoop().is_initialized()) {
          LOG(Warn, "The Heating side of " << modelObject.briefDescription() << " isn't connected to a plantLoop, it will not be translated");

          // It MUST have the three curves defined in this case
        } else if (!capModFuncOfTempDiff.is_initialized() || !capModFuncOfSAFlow.is_initialized() || !capModFuncOfWaterFlow.is_initialized()) {
          LOG(Warn, "If you connect the heating side of " << modelObject.briefDescription()
                                                          << " to a plantLoop, you MUST supply all three curves in the subobject "
                                                          << hc.briefDescription());

        } else {
          // A9 , \field Hot Water Inlet Node Name
          temp = hc.inletModelObject();
          if (temp) {
            if ((s = temp->name())) {
              idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::HotWaterInletNodeName, s.get());
            }
          }
          // A10 , \field Hot Water Outlet Node Name
          temp = hc.outletModelObject();
          if (temp) {
            if ((s = temp->name())) {
              idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::HotWaterOutletNodeName, s.get());
            }
          }

          /* Double with defaults (not optionals) */

          // N9 , \field Beam Rated Heating Capacity per Beam Length
          {
            double value = hc.beamRatedHeatingCapacityperBeamLength();
            idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::BeamRatedHeatingCapacityperBeamLength, value);
          }

          // N10, \field Beam Rated Heating Room Air Hot Water Temperature Difference
          {
            double value = hc.beamRatedHeatingRoomAirHotWaterTemperatureDifference();
            idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::BeamRatedHeatingRoomAirHotWaterTemperatureDifference,
                                value);
          }

          // N11, \field Beam Rated Hot Water Volume Flow Rate per Beam Length
          {
            double value = hc.beamRatedHotWaterVolumeFlowRateperBeamLength();
            idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::BeamRatedHotWaterVolumeFlowRateperBeamLength, value);
          }

          /* Curves: At this point, I have already checked above that all three curves are defined */

          // A14, \field Beam Heating Capacity Temperature Difference Modification Factor Curve Name
          if (auto _curve = translateAndMapModelObject(capModFuncOfTempDiff.get())) {
            idfObject.setString(
              AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::BeamHeatingCapacityTemperatureDifferenceModificationFactorCurveName,
              _curve->name().get());
          }

          // A15, \field Beam Heating Capacity Air Flow Modification Factor Curve Name
          if (auto _curve = translateAndMapModelObject(capModFuncOfSAFlow.get())) {
            idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::BeamHeatingCapacityAirFlowModificationFactorCurveName,
                                _curve->name().get());
          }

          // A16, \field Beam Heating Capacity Hot Water Flow Modification Factor Curve Name
          if (auto _curve = translateAndMapModelObject(capModFuncOfWaterFlow.get())) {
            idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::BeamHeatingCapacityHotWaterFlowModificationFactorCurveName,
                                _curve->name().get());
          }
        }
      }
    }

    // Populate fields for AirDistributionUnit
    if (boost::optional<ModelObject> outletNode = modelObject.outletModelObject()) {
      _airDistributionUnit.setString(ZoneHVAC_AirDistributionUnitFields::AirDistributionUnitOutletNodeName, outletNode->name().get());
    }
    _airDistributionUnit.setString(ZoneHVAC_AirDistributionUnitFields::AirTerminalObjectType, idfObject.iddObject().name());
    _airDistributionUnit.setString(ZoneHVAC_AirDistributionUnitFields::AirTerminalName, idfObject.name().get());

    return _airDistributionUnit;
  }

}  // namespace energyplus

}  // namespace openstudio
