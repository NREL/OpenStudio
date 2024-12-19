#ifndef MODEL_RESOURCES_I
#define MODEL_RESOURCES_I

#ifdef SWIGPYTHON
  %module openstudiomodelresources
#endif


%include <model/Model_Common_Include.i>
%import <model/ModelCore.i>
%import <model/ModelSimulation.i>

%{
  #include <model/ScheduleTypeRegistry.hpp>
  #include <utilities/data/TimeSeries.hpp>
  #include <utilities/sql/SqlFile.hpp>
%}

#if defined(SWIGCSHARP) || defined(SWIGJAVA)

  #undef _csharp_module_name
  #define _csharp_module_name OpenStudioModelResources

  // ignore geometry objects for now, add back in with partial classes in ModelGeometry.i
  %ignore openstudio::model::SpaceType::spaces;
  %ignore openstudio::model::SpaceLoadDefinition::instances;
  %ignore openstudio::model::ExteriorLoadDefinition::instances;
  %ignore openstudio::model::ShadingControl::subSurfaces;
  %ignore openstudio::model::ShadingControl::subSurfaceIndex;
  %ignore openstudio::model::ShadingControl::addSubSurface;
  %ignore openstudio::model::ShadingControl::setSubSurfaceIndex;
  %ignore openstudio::model::ShadingControl::removeSubSurface(const SubSurface& subSurface); // The unsigned index overload is fine
  %ignore openstudio::model::ShadingControl::addSubSurfaces;
  %ignore openstudio::model::ShadingControl::setSubSurfaces;

  // CoilCoolingDX is defined in StraightComponent.i
  %ignore openstudio::model::CoilCoolingDXCurveFitPerformance::coilCoolingDXs;

  // HeatExchangerDesiccantBalancedFlow is done later in ModelHVAC.i
  %ignore openstudio::model::HeatExchangerDesiccantBalancedFlowPerformanceDataType1::heatExchangerDesiccantBalancedFlows;

  // TODO: why?
  // ignore schedule type
  // %ignore openstudio::model::ScheduleType;

#endif

#if defined(SWIGJAVA)
%ignore openstudio::model::OpaqueMaterial::solarAbsorptance;
%ignore openstudio::model::OpaqueMaterial::visibleAbsorptance;
%ignore openstudio::model::OpaqueMaterial::thermalAbsorptance;
%ignore openstudio::model::SimpleGlazing::getVisibleTransmittance;
%ignore openstudio::model::Shade::getVisibleTransmittance;

#endif

#if defined SWIGPYTHON
  %pythoncode %{
    Model = openstudiomodelcore.Model
  %}
#endif

namespace openstudio {
namespace model {

// forward declarations
class ShadingControl;
%feature("valuewrapper") SubSurface;
class SubSurface;
%feature("valuewrapper") CoilCoolingDX;
class CoilCoolingDX;
%feature("valuewrapper") HeatExchangerDesiccantBalancedFlow;
class HeatExchangerDesiccantBalancedFlow;

}
}

// extend classes
%extend openstudio::model::TableMultiVariableLookupPoint {
  // Use the overloaded operator<< for string representation
  // puts point will return something like "(x1, x2) = (10.5, 0.75)"
  std::string __str__() {
    std::ostringstream os;
    os << *$self;
    return os.str();
  }
};

// extend classes
%extend openstudio::model::TemperatureEnthalpy {
  // Use the overloaded operator<< for string representation
  std::string __str__() {
    std::ostringstream os;
    os << *$self;
    return os.str();
  }
};

MODELOBJECT_TEMPLATES(ScheduleType)
MODELOBJECT_TEMPLATES(ScheduleInterval);
MODELOBJECT_TEMPLATES(ScheduleFixedInterval);
MODELOBJECT_TEMPLATES(ExternalFile);
MODELOBJECT_TEMPLATES(ScheduleFile);
MODELOBJECT_TEMPLATES(PythonPluginInstance);
MODELOBJECT_TEMPLATES(PythonPluginVariable);
MODELOBJECT_TEMPLATES(PythonPluginTrendVariable);
MODELOBJECT_TEMPLATES(PythonPluginOutputVariable);
UNIQUEMODELOBJECT_TEMPLATES(PythonPluginSearchPaths);
MODELOBJECT_TEMPLATES(ScheduleVariableInterval);
MODELOBJECT_TEMPLATES(ScheduleCompact);
MODELOBJECT_TEMPLATES(ScheduleConstant);
MODELOBJECT_TEMPLATES(DefaultScheduleSet);
MODELOBJECT_TEMPLATES(SpectralDataField); // Helper class defined in MaterialPropertyGlazingSpectralData
MODELOBJECT_TEMPLATES(MaterialPropertyGlazingSpectralData);
MODELOBJECT_TEMPLATES(MaterialPropertyMoisturePenetrationDepthSettings);
MODELOBJECT_TEMPLATES(TemperatureEnthalpy); // Helper class defined in MaterialPropertyPhaseChange
MODELOBJECT_TEMPLATES(MaterialPropertyPhaseChange);
MODELOBJECT_TEMPLATES(MaterialPropertyPhaseChangeHysteresis);
MODELOBJECT_TEMPLATES(Material);
MODELOBJECT_TEMPLATES(FenestrationMaterial);
MODELOBJECT_TEMPLATES(GasLayer);
MODELOBJECT_TEMPLATES(Glazing);
MODELOBJECT_TEMPLATES(ShadingMaterial);
MODELOBJECT_TEMPLATES(ModelPartitionMaterial);
MODELOBJECT_TEMPLATES(OpaqueMaterial);
MODELOBJECT_TEMPLATES(Blind);
MODELOBJECT_TEMPLATES(DaylightRedirectionDevice);
MODELOBJECT_TEMPLATES(AirGap);
MODELOBJECT_TEMPLATES(Gas);
MODELOBJECT_TEMPLATES(GasMixture);
MODELOBJECT_TEMPLATES(InfraredTransparentMaterial);
MODELOBJECT_TEMPLATES(MasslessOpaqueMaterial);
MODELOBJECT_TEMPLATES(RefractionExtinctionGlazing);
MODELOBJECT_TEMPLATES(RoofVegetation);
MODELOBJECT_TEMPLATES(Screen);
MODELOBJECT_TEMPLATES(Shade);
MODELOBJECT_TEMPLATES(SimpleGlazing);
MODELOBJECT_TEMPLATES(StandardGlazing);
MODELOBJECT_TEMPLATES(StandardOpaqueMaterial);
MODELOBJECT_TEMPLATES(ThermochromicGlazing);
MODELOBJECT_TEMPLATES(StandardsInformationMaterial);
MODELOBJECT_TEMPLATES(ConstructionBase);
MODELOBJECT_TEMPLATES(LayeredConstruction);
MODELOBJECT_TEMPLATES(Construction);
MODELOBJECT_TEMPLATES(ConstructionAirBoundary);
MODELOBJECT_TEMPLATES(ConstructionWithInternalSource);
MODELOBJECT_TEMPLATES(CFactorUndergroundWallConstruction);
MODELOBJECT_TEMPLATES(FFactorGroundFloorConstruction);
MODELOBJECT_TEMPLATES(StandardsInformationConstruction);
MODELOBJECT_TEMPLATES(WindowDataFile);
MODELOBJECT_TEMPLATES(WindowPropertyFrameAndDivider);
MODELOBJECT_TEMPLATES(DefaultSubSurfaceConstructions);
MODELOBJECT_TEMPLATES(DefaultSurfaceConstructions);
MODELOBJECT_TEMPLATES(ShadingControl);
MODELOBJECT_TEMPLATES(Curve);
MODELOBJECT_TEMPLATES(CurveBicubic);
MODELOBJECT_TEMPLATES(CurveBiquadratic);
MODELOBJECT_TEMPLATES(CurveCubic);
MODELOBJECT_TEMPLATES(CurveDoubleExponentialDecay);
MODELOBJECT_TEMPLATES(CurveExponent);
MODELOBJECT_TEMPLATES(CurveExponentialDecay);
MODELOBJECT_TEMPLATES(CurveExponentialSkewNormal);
MODELOBJECT_TEMPLATES(CurveFanPressureRise);
MODELOBJECT_TEMPLATES(CurveFunctionalPressureDrop);
MODELOBJECT_TEMPLATES(CurveLinear);
MODELOBJECT_TEMPLATES(CurveQuadLinear);
MODELOBJECT_TEMPLATES(CurveQuintLinear);
MODELOBJECT_TEMPLATES(CurveQuadratic);
MODELOBJECT_TEMPLATES(CurveQuadraticLinear);
MODELOBJECT_TEMPLATES(CurveQuartic);
MODELOBJECT_TEMPLATES(CurveRectangularHyperbola1);
MODELOBJECT_TEMPLATES(CurveRectangularHyperbola2);
MODELOBJECT_TEMPLATES(CurveSigmoid);
MODELOBJECT_TEMPLATES(CurveTriquadratic);
MODELOBJECT_TEMPLATES(TableMultiVariableLookupPoint);  // Helper class defined in TableMultiVariableLookup
MODELOBJECT_TEMPLATES(TableMultiVariableLookup);
MODELOBJECT_TEMPLATES(TableLookup);
MODELOBJECT_TEMPLATES(TableIndependentVariable);
MODELOBJECT_TEMPLATES(SpaceLoadDefinition);
MODELOBJECT_TEMPLATES(PeopleDefinition);
MODELOBJECT_TEMPLATES(LightsDefinition);
MODELOBJECT_TEMPLATES(LuminaireDefinition);
MODELOBJECT_TEMPLATES(ElectricEquipmentDefinition);
MODELOBJECT_TEMPLATES(ElectricEquipmentITEAirCooledDefinition);
MODELOBJECT_TEMPLATES(GasEquipmentDefinition);
MODELOBJECT_TEMPLATES(HotWaterEquipmentDefinition);
MODELOBJECT_TEMPLATES(SteamEquipmentDefinition);
MODELOBJECT_TEMPLATES(OtherEquipmentDefinition);
MODELOBJECT_TEMPLATES(InternalMassDefinition);

MODELOBJECT_TEMPLATES(ExteriorLoadDefinition);
MODELOBJECT_TEMPLATES(ExteriorLightsDefinition)
MODELOBJECT_TEMPLATES(ExteriorFuelEquipmentDefinition)
MODELOBJECT_TEMPLATES(ExteriorWaterEquipmentDefinition)
MODELOBJECT_TEMPLATES(RenderingColor);
MODELOBJECT_TEMPLATES(DesignSpecificationOutdoorAir);

MODELOBJECT_TEMPLATES(CoilCoolingDXCurveFitPerformance);
MODELOBJECT_TEMPLATES(CoilCoolingDXCurveFitOperatingMode);
MODELOBJECT_TEMPLATES(CoilCoolingDXCurveFitSpeed);

MODELOBJECT_TEMPLATES(HeatExchangerDesiccantBalancedFlowPerformanceDataType1);

SWIG_MODELOBJECT(ScheduleInterval, 0);
SWIG_MODELOBJECT(ScheduleFile, 1);
SWIG_MODELOBJECT(PythonPluginInstance, 1);
SWIG_MODELOBJECT(PythonPluginVariable, 1);
SWIG_MODELOBJECT(PythonPluginTrendVariable, 1);
SWIG_MODELOBJECT(PythonPluginOutputVariable, 1);
SWIG_UNIQUEMODELOBJECT(PythonPluginSearchPaths);
SWIG_MODELOBJECT(ExternalFile, 1);
SWIG_MODELOBJECT(ScheduleFixedInterval, 1);
SWIG_MODELOBJECT(ScheduleVariableInterval, 1);
SWIG_MODELOBJECT(ScheduleCompact, 1);
SWIG_MODELOBJECT(ScheduleConstant, 1);
SWIG_MODELOBJECT(DefaultScheduleSet, 1);
SWIG_MODELOBJECT(MaterialPropertyGlazingSpectralData, 1);
SWIG_MODELOBJECT(MaterialPropertyMoisturePenetrationDepthSettings, 1);
SWIG_MODELOBJECT(MaterialPropertyPhaseChange, 1);
SWIG_MODELOBJECT(MaterialPropertyPhaseChangeHysteresis, 1);
SWIG_MODELOBJECT(Material, 0);
SWIG_MODELOBJECT(FenestrationMaterial, 0);
SWIG_MODELOBJECT(GasLayer, 0);
SWIG_MODELOBJECT(Glazing, 0);
SWIG_MODELOBJECT(ShadingMaterial, 0);
SWIG_MODELOBJECT(ModelPartitionMaterial, 0);
SWIG_MODELOBJECT(OpaqueMaterial, 0);
SWIG_MODELOBJECT(Blind, 1);
SWIG_MODELOBJECT(DaylightRedirectionDevice, 1);
SWIG_MODELOBJECT(AirGap, 1);
SWIG_MODELOBJECT(Gas, 1);
SWIG_MODELOBJECT(GasMixture, 1);
SWIG_MODELOBJECT(InfraredTransparentMaterial, 1);
SWIG_MODELOBJECT(MasslessOpaqueMaterial, 1);
SWIG_MODELOBJECT(RefractionExtinctionGlazing, 1);
SWIG_MODELOBJECT(RoofVegetation, 1);
SWIG_MODELOBJECT(Screen, 1);
SWIG_MODELOBJECT(Shade, 1);
SWIG_MODELOBJECT(SimpleGlazing, 1);
SWIG_MODELOBJECT(StandardGlazing, 1);
SWIG_MODELOBJECT(StandardOpaqueMaterial, 1);
SWIG_MODELOBJECT(ThermochromicGlazing, 1);
SWIG_MODELOBJECT(StandardsInformationMaterial, 1);
SWIG_MODELOBJECT(ConstructionBase, 0);
SWIG_MODELOBJECT(LayeredConstruction, 0);
SWIG_MODELOBJECT(Construction, 1);
SWIG_MODELOBJECT(ConstructionAirBoundary, 1);
SWIG_MODELOBJECT(ConstructionWithInternalSource, 1);
SWIG_MODELOBJECT(CFactorUndergroundWallConstruction, 1);
SWIG_MODELOBJECT(FFactorGroundFloorConstruction, 1);
SWIG_MODELOBJECT(StandardsInformationConstruction, 1);
SWIG_MODELOBJECT(WindowDataFile, 1);
SWIG_MODELOBJECT(WindowPropertyFrameAndDivider, 1);
SWIG_MODELOBJECT(DefaultSubSurfaceConstructions, 1);
SWIG_MODELOBJECT(DefaultSurfaceConstructions, 1);
SWIG_MODELOBJECT(ShadingControl, 1);
SWIG_MODELOBJECT(Curve, 0);
SWIG_MODELOBJECT(CurveBicubic, 1);
SWIG_MODELOBJECT(CurveBiquadratic, 1);
SWIG_MODELOBJECT(CurveCubic, 1);
SWIG_MODELOBJECT(CurveDoubleExponentialDecay, 1);
SWIG_MODELOBJECT(CurveExponent, 1);
SWIG_MODELOBJECT(CurveExponentialDecay, 1);
SWIG_MODELOBJECT(CurveExponentialSkewNormal, 1);
SWIG_MODELOBJECT(CurveFanPressureRise, 1);
SWIG_MODELOBJECT(CurveFunctionalPressureDrop, 1);
SWIG_MODELOBJECT(CurveLinear, 1);
SWIG_MODELOBJECT(CurveQuadLinear, 1);
SWIG_MODELOBJECT(CurveQuintLinear, 1);
SWIG_MODELOBJECT(CurveQuadratic, 1);
SWIG_MODELOBJECT(CurveQuadraticLinear, 1);
SWIG_MODELOBJECT(CurveQuartic, 1);
SWIG_MODELOBJECT(CurveRectangularHyperbola1, 1);
SWIG_MODELOBJECT(CurveRectangularHyperbola2, 1);
SWIG_MODELOBJECT(CurveSigmoid, 1);
SWIG_MODELOBJECT(CurveTriquadratic, 1);
SWIG_MODELOBJECT(TableMultiVariableLookup, 1);
SWIG_MODELOBJECT(TableLookup, 1);
SWIG_MODELOBJECT(TableIndependentVariable, 1);
SWIG_MODELOBJECT(SpaceLoadDefinition, 0);
SWIG_MODELOBJECT(PeopleDefinition, 1);
SWIG_MODELOBJECT(LightsDefinition, 1);
SWIG_MODELOBJECT(LuminaireDefinition, 1);
SWIG_MODELOBJECT(ElectricEquipmentDefinition, 1);
SWIG_MODELOBJECT(ElectricEquipmentITEAirCooledDefinition, 1);
SWIG_MODELOBJECT(GasEquipmentDefinition, 1);
SWIG_MODELOBJECT(HotWaterEquipmentDefinition, 1);
SWIG_MODELOBJECT(SteamEquipmentDefinition, 1);
SWIG_MODELOBJECT(OtherEquipmentDefinition, 1);
SWIG_MODELOBJECT(InternalMassDefinition, 1);
SWIG_MODELOBJECT(ExteriorLoadDefinition, 0);
SWIG_MODELOBJECT(ExteriorLightsDefinition, 1);
SWIG_MODELOBJECT(ExteriorFuelEquipmentDefinition, 1);
SWIG_MODELOBJECT(ExteriorWaterEquipmentDefinition, 1);
SWIG_MODELOBJECT(RenderingColor, 1);
SWIG_MODELOBJECT(DesignSpecificationOutdoorAir, 1);

SWIG_MODELOBJECT(CoilCoolingDXCurveFitPerformance, 1);
SWIG_MODELOBJECT(CoilCoolingDXCurveFitOperatingMode, 1);
SWIG_MODELOBJECT(CoilCoolingDXCurveFitSpeed, 1);

SWIG_MODELOBJECT(HeatExchangerDesiccantBalancedFlowPerformanceDataType1, 1);

%include <model/ScheduleTypeRegistry.hpp>

#if defined(SWIGCSHARP) || defined(SWIGJAVA)
  %inline {
    namespace openstudio {
      namespace model {

        // EMS Curve setter  (reimplemented from ModelCore.i)
        bool setCurveForEMS(openstudio::model::EnergyManagementSystemCurveOrTableIndexVariable ems_curve, openstudio::model::Curve curve) {
          return ems_curve.setCurveOrTableObject(curve);
        }

        boost::optional<PythonPluginSearchPaths> pythonPluginSearchPaths(const openstudio::model::Model& model) {
          return model.pythonPluginSearchPaths();
        }

      }
    }
  }
#endif

#if defined(SWIGCSHARP)
  //%pragma(csharp) imclassimports=%{
  %pragma(csharp) moduleimports=%{

    using System;
    using System.Runtime.InteropServices;

    public partial class EnergyManagementSystemCurveOrTableIndexVariable : ModelObject {
      public bool setCurveOrTableObject(OpenStudio.Curve curve) {
        return OpenStudio.OpenStudioModelResources.setCurveForEMS(this, curve);
      }

      // Overloaded Ctor, calling Ctor that doesn't use Curve
      public EnergyManagementSystemCurveOrTableIndexVariable(Model model, OpenStudio.Curve curve)
        : this(model) {
        this.setCurveOrTableObject(curve);
      }

      public OptionalPythonPluginSearchPaths pythonPluginSearchPaths() {
        return OpenStudio.OpenStudioModelResources.pythonPluginSearchPaths(this);
      }
    }
  %}
#endif
#endif




