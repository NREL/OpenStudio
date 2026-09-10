#ifndef MODEL_ZONEHVAC_I
#define MODEL_ZONEHVAC_I


#ifdef SWIGPYTHON
  %module openstudiomodelzonehvac
#endif

%include <model/Model_Common_Include.i>
%import <model/ModelCore.i>
%import <model/ModelSimulation.i>
%import <model/ModelResources.i>
%import <model/ModelGeometry.i>
%import <model/ModelHVAC.i>

#if defined SWIGCSHARP
  #undef _csharp_module_name
  #define _csharp_module_name OpenStudioModelZoneHVAC

  // ignore airflow objects for now, add back in with partial classes in ModelAirflow.i (swigged after us)
  // TODO: haven't added them to ModelAirflow.i but I don't see any other that are indeed implemented...
  %ignore openstudio::model::FanZoneExhaust::getAirflowNetworkZoneExhaustFan;
  %ignore openstudio::model::FanZoneExhaust::airflowNetworkZoneExhaustFan;

#endif

#if defined SWIGPYTHON
  %pythoncode %{
    Model = openstudiomodelcore.Model
  %}
#endif

namespace openstudio {
  namespace model {

    // forward declarations
    // For FanZoneExhaust
    %feature("valuewrapper") AirflowNetworkCrack ;
    %feature("valuewrapper") AirflowNetworkFan;
    class AirflowNetworkEquivalentDuct;
    class AirflowNetworkFan;

  }
}
MODELOBJECT_TEMPLATES(AirLoopHVACUnitarySystem);
MODELOBJECT_TEMPLATES(FanZoneExhaust);
MODELOBJECT_TEMPLATES(WaterHeaterHeatPump);
MODELOBJECT_TEMPLATES(WaterHeaterHeatPumpWrappedCondenser);
MODELOBJECT_TEMPLATES(ZoneHVACBaseboardConvectiveElectric);
MODELOBJECT_TEMPLATES(ZoneHVACBaseboardConvectiveWater);
MODELOBJECT_TEMPLATES(ZoneHVACBaseboardRadiantConvectiveElectric);
MODELOBJECT_TEMPLATES(ZoneHVACBaseboardRadiantConvectiveWater);
MODELOBJECT_TEMPLATES(ZoneHVACBaseboardRadiantConvectiveSteam);
MODELOBJECT_TEMPLATES(ZoneHVACCoolingPanelRadiantConvectiveWater);
MODELOBJECT_TEMPLATES(ZoneHVACDehumidifierDX);
MODELOBJECT_TEMPLATES(ZoneHVACEnergyRecoveryVentilator);
MODELOBJECT_TEMPLATES(ZoneHVACEnergyRecoveryVentilatorController);
MODELOBJECT_TEMPLATES(ZoneHVACEvaporativeCoolerUnit);
MODELOBJECT_TEMPLATES(ZoneHVACFourPipeFanCoil);
MODELOBJECT_TEMPLATES(ZoneHVACHighTemperatureRadiant);
MODELOBJECT_TEMPLATES(ZoneHVACIdealLoadsAirSystem);
MODELOBJECT_TEMPLATES(ZoneHVACLowTemperatureRadiantElectric);
MODELOBJECT_TEMPLATES(ZoneHVACLowTempRadiantConstFlow);
MODELOBJECT_TEMPLATES(ZoneHVACLowTempRadiantVarFlow);
MODELOBJECT_TEMPLATES(ZoneHVACPackagedTerminalHeatPump);
MODELOBJECT_TEMPLATES(ZoneHVACPackagedTerminalAirConditioner);
MODELOBJECT_TEMPLATES(ZoneHVACTerminalUnitVariableRefrigerantFlow);
MODELOBJECT_TEMPLATES(ZoneHVACUnitHeater);
MODELOBJECT_TEMPLATES(ZoneHVACUnitVentilator);
MODELOBJECT_TEMPLATES(ZoneHVACWaterToAirHeatPump);

SWIG_MODELOBJECT(AirLoopHVACUnitarySystem,1);
SWIG_MODELOBJECT(FanZoneExhaust,1);
SWIG_MODELOBJECT(WaterHeaterHeatPump,1);
SWIG_MODELOBJECT(WaterHeaterHeatPumpWrappedCondenser,1);
SWIG_MODELOBJECT(ZoneHVACBaseboardConvectiveElectric,1);
SWIG_MODELOBJECT(ZoneHVACBaseboardConvectiveWater,1);
SWIG_MODELOBJECT(ZoneHVACBaseboardRadiantConvectiveElectric,1);
SWIG_MODELOBJECT(ZoneHVACBaseboardRadiantConvectiveWater,1);
SWIG_MODELOBJECT(ZoneHVACBaseboardRadiantConvectiveSteam,1);
SWIG_MODELOBJECT(ZoneHVACCoolingPanelRadiantConvectiveWater,1);
SWIG_MODELOBJECT(ZoneHVACDehumidifierDX,1);
SWIG_MODELOBJECT(ZoneHVACEnergyRecoveryVentilator,1);
SWIG_MODELOBJECT(ZoneHVACEnergyRecoveryVentilatorController,1);
SWIG_MODELOBJECT(ZoneHVACEvaporativeCoolerUnit,1);
SWIG_MODELOBJECT(ZoneHVACFourPipeFanCoil,1);
SWIG_MODELOBJECT(ZoneHVACHighTemperatureRadiant,1);
SWIG_MODELOBJECT(ZoneHVACIdealLoadsAirSystem,1);
SWIG_MODELOBJECT(ZoneHVACLowTemperatureRadiantElectric,1);
SWIG_MODELOBJECT(ZoneHVACLowTempRadiantConstFlow,1);
SWIG_MODELOBJECT(ZoneHVACLowTempRadiantVarFlow,1);
SWIG_MODELOBJECT(ZoneHVACPackagedTerminalHeatPump,1);
SWIG_MODELOBJECT(ZoneHVACPackagedTerminalAirConditioner,1);
SWIG_MODELOBJECT(ZoneHVACTerminalUnitVariableRefrigerantFlow,1);
SWIG_MODELOBJECT(ZoneHVACUnitHeater,1);
SWIG_MODELOBJECT(ZoneHVACUnitVentilator,1);
SWIG_MODELOBJECT(ZoneHVACWaterToAirHeatPump,1);

#if defined(SWIGCSHARP) || defined(SWIGJAVA)
  %inline {
    namespace openstudio {
      namespace model {

        // AirCondVRF, reimplemented from ModelHVAC.i
        std::vector<ZoneHVACTerminalUnitVariableRefrigerantFlow> terminals(const openstudio::model::AirConditionerVariableRefrigerantFlowFluidTemperatureControl& airCondVRF) {
          return airCondVRF.terminals();
        }
        bool addTerminal(openstudio::model::AirConditionerVariableRefrigerantFlowFluidTemperatureControl airCondVRF, openstudio::model::ZoneHVACTerminalUnitVariableRefrigerantFlow vrf) {
          return airCondVRF.addTerminal(vrf);
        }
        void removeTerminal(openstudio::model::AirConditionerVariableRefrigerantFlowFluidTemperatureControl airCondVRF, openstudio::model::ZoneHVACTerminalUnitVariableRefrigerantFlow vrf) {
          airCondVRF.removeTerminal(vrf);
        }

        std::vector<ZoneHVACTerminalUnitVariableRefrigerantFlow> terminals(const openstudio::model::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR& airCondVRF) {
          return airCondVRF.terminals();
        }
        bool addTerminal(openstudio::model::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR airCondVRF, openstudio::model::ZoneHVACTerminalUnitVariableRefrigerantFlow vrf) {
          return airCondVRF.addTerminal(vrf);
        }
        void removeTerminal(openstudio::model::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR airCondVRF, openstudio::model::ZoneHVACTerminalUnitVariableRefrigerantFlow vrf) {
          airCondVRF.removeTerminal(vrf);
        }

      } // namespace model
    } // namespace openstudio
  } // %inline
#endif

#if defined(SWIGCSHARP)
  //%pragma(csharp) imclassimports=%{
  %pragma(csharp) moduleimports=%{

    using System;
    using System.Runtime.InteropServices;

    public partial class AirConditionerVariableRefrigerantFlowFluidTemperatureControl : HVACComponent {
      public ZoneHVACTerminalUnitVariableRefrigerantFlowVector terminals(){
        return OpenStudio.OpenStudioModelZoneHVAC.terminals(this);
      }

      public bool addTerminal(OpenStudio.ZoneHVACTerminalUnitVariableRefrigerantFlow vrf) {
        return OpenStudio.OpenStudioModelZoneHVAC.addTerminal(this, vrf);
      }

      public void removeTerminal(OpenStudio.ZoneHVACTerminalUnitVariableRefrigerantFlow vrf) {
        OpenStudio.OpenStudioModelZoneHVAC.removeTerminal(this, vrf);
      }
    }

    public partial class AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR : HVACComponent {
      public ZoneHVACTerminalUnitVariableRefrigerantFlowVector terminals(){
        return OpenStudio.OpenStudioModelZoneHVAC.terminals(this);
      }

      public bool addTerminal(OpenStudio.ZoneHVACTerminalUnitVariableRefrigerantFlow vrf) {
        return OpenStudio.OpenStudioModelZoneHVAC.addTerminal(this, vrf);
      }

      public void removeTerminal(OpenStudio.ZoneHVACTerminalUnitVariableRefrigerantFlow vrf) {
        OpenStudio.OpenStudioModelZoneHVAC.removeTerminal(this, vrf);
      }
    }

  %}
#endif

#endif
