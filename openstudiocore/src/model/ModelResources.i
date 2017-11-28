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

  #include <QColor>
  #include <utilities/data/TimeSeries.hpp>
  #include <utilities/sql/SqlFile.hpp>
%}

#if defined(SWIGCSHARP) || defined(SWIGJAVA)

  #undef _csharp_module_name
  #define _csharp_module_name OpenStudioModelResources

  // ignore space for now
  %ignore openstudio::model::SpaceType::spaces;
    
  // ignore space load instance for now
  %ignore openstudio::model::SpaceLoadDefinition::instances;
  
  // ignore schedule type
  %ignore openstudio::model::ScheduleType;
  
#endif

#if defined(SWIGJAVA)
%ignore openstudio::model::OpaqueMaterial::solarAbsorptance;
%ignore openstudio::model::OpaqueMaterial::visibleAbsorptance;
%ignore openstudio::model::OpaqueMaterial::thermalAbsorptance;
%ignore openstudio::model::SimpleGlazing::getVisibleTransmittance;
%ignore openstudio::model::Shade::getVisibleTransmittance;

#endif

namespace openstudio {
namespace model {

// forward declarations
class ShadingControl;

}
}

MODELOBJECT_TEMPLATES(ScheduleInterval); 
MODELOBJECT_TEMPLATES(ScheduleFixedInterval); 
MODELOBJECT_TEMPLATES(ScheduleVariableInterval);
MODELOBJECT_TEMPLATES(ScheduleCompact); 
MODELOBJECT_TEMPLATES(ScheduleConstant); 
MODELOBJECT_TEMPLATES(DefaultScheduleSet); 
MODELOBJECT_TEMPLATES(MaterialPropertyGlazingSpectralData); 
MODELOBJECT_TEMPLATES(MaterialPropertyMoisturePenetrationDepthSettings); 
MODELOBJECT_TEMPLATES(Material);
MODELOBJECT_TEMPLATES(FenestrationMaterial);
MODELOBJECT_TEMPLATES(GasLayer);
MODELOBJECT_TEMPLATES(Glazing);
MODELOBJECT_TEMPLATES(ShadingMaterial);
MODELOBJECT_TEMPLATES(ModelPartitionMaterial);
MODELOBJECT_TEMPLATES(OpaqueMaterial);
MODELOBJECT_TEMPLATES(AirWallMaterial);
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
MODELOBJECT_TEMPLATES(CurveQuadratic);
MODELOBJECT_TEMPLATES(CurveQuadraticLinear);
MODELOBJECT_TEMPLATES(CurveQuartic);
MODELOBJECT_TEMPLATES(CurveRectangularHyperbola1);
MODELOBJECT_TEMPLATES(CurveRectangularHyperbola2);
MODELOBJECT_TEMPLATES(CurveSigmoid);
MODELOBJECT_TEMPLATES(CurveTriquadratic);
MODELOBJECT_TEMPLATES(TableMultiVariableLookup);
MODELOBJECT_TEMPLATES(SpaceLoadDefinition);
MODELOBJECT_TEMPLATES(PeopleDefinition);
MODELOBJECT_TEMPLATES(LightsDefinition); 
MODELOBJECT_TEMPLATES(LuminaireDefinition);
MODELOBJECT_TEMPLATES(ElectricEquipmentDefinition);
MODELOBJECT_TEMPLATES(GasEquipmentDefinition);
MODELOBJECT_TEMPLATES(HotWaterEquipmentDefinition);
MODELOBJECT_TEMPLATES(SteamEquipmentDefinition);
MODELOBJECT_TEMPLATES(OtherEquipmentDefinition);
MODELOBJECT_TEMPLATES(InternalMassDefinition);
MODELOBJECT_TEMPLATES(ExteriorLightsDefinition)
MODELOBJECT_TEMPLATES(RenderingColor); 
MODELOBJECT_TEMPLATES(DesignSpecificationOutdoorAir); 

SWIG_MODELOBJECT(ScheduleInterval, 0); 
SWIG_MODELOBJECT(ScheduleFixedInterval, 1); 
SWIG_MODELOBJECT(ScheduleVariableInterval, 1);
SWIG_MODELOBJECT(ScheduleCompact, 1); 
SWIG_MODELOBJECT(ScheduleConstant, 1); 
SWIG_MODELOBJECT(DefaultScheduleSet, 1); 
SWIG_MODELOBJECT(MaterialPropertyGlazingSpectralData, 1);
SWIG_MODELOBJECT(MaterialPropertyMoisturePenetrationDepthSettings, 1);
SWIG_MODELOBJECT(Material, 0);
SWIG_MODELOBJECT(FenestrationMaterial, 0);
SWIG_MODELOBJECT(GasLayer, 0);
SWIG_MODELOBJECT(Glazing, 0);
SWIG_MODELOBJECT(ShadingMaterial, 0);
SWIG_MODELOBJECT(ModelPartitionMaterial, 0);
SWIG_MODELOBJECT(OpaqueMaterial, 0);
SWIG_MODELOBJECT(AirWallMaterial, 1);
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
SWIG_MODELOBJECT(CurveQuadratic, 1);
SWIG_MODELOBJECT(CurveQuadraticLinear, 1);
SWIG_MODELOBJECT(CurveQuartic, 1);
SWIG_MODELOBJECT(CurveRectangularHyperbola1, 1);
SWIG_MODELOBJECT(CurveRectangularHyperbola2, 1);
SWIG_MODELOBJECT(CurveSigmoid, 1);
SWIG_MODELOBJECT(CurveTriquadratic, 1);
SWIG_MODELOBJECT(TableMultiVariableLookup, 1);
SWIG_MODELOBJECT(SpaceLoadDefinition, 0);  
SWIG_MODELOBJECT(PeopleDefinition, 1);
SWIG_MODELOBJECT(LightsDefinition, 1); 
SWIG_MODELOBJECT(LuminaireDefinition, 1);
SWIG_MODELOBJECT(ElectricEquipmentDefinition, 1);
SWIG_MODELOBJECT(GasEquipmentDefinition, 1);
SWIG_MODELOBJECT(HotWaterEquipmentDefinition, 1);
SWIG_MODELOBJECT(SteamEquipmentDefinition, 1);
SWIG_MODELOBJECT(OtherEquipmentDefinition, 1);
SWIG_MODELOBJECT(InternalMassDefinition, 1);
SWIG_MODELOBJECT(ExteriorLightsDefinition, 1);
SWIG_MODELOBJECT(RenderingColor, 1);
SWIG_MODELOBJECT(DesignSpecificationOutdoorAir, 1); 

%include <model/ScheduleTypeRegistry.hpp>

#endif 




