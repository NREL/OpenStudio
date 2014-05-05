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
MODELOBJECT_TEMPLATES(Material);
MODELOBJECT_TEMPLATES(FenestrationMaterial);
MODELOBJECT_TEMPLATES(GasLayer);
MODELOBJECT_TEMPLATES(Glazing);
MODELOBJECT_TEMPLATES(ShadingMaterial);
MODELOBJECT_TEMPLATES(ModelPartitionMaterial);
MODELOBJECT_TEMPLATES(OpaqueMaterial);
MODELOBJECT_TEMPLATES(AirWallMaterial);
MODELOBJECT_TEMPLATES(Blind);
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
MODELOBJECT_TEMPLATES(ConstructionBase);
MODELOBJECT_TEMPLATES(LayeredConstruction);
MODELOBJECT_TEMPLATES(Construction);
MODELOBJECT_TEMPLATES(ConstructionWithInternalSource);
MODELOBJECT_TEMPLATES(CFactorUndergroundWallConstruction);
MODELOBJECT_TEMPLATES(FFactorGroundFloorConstruction);
MODELOBJECT_TEMPLATES(StandardsInformationConstruction);
MODELOBJECT_TEMPLATES(WindowDataFile);
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

SWIG_MODELOBJECT(ScheduleInterval); 
SWIG_MODELOBJECT(ScheduleFixedInterval); 
SWIG_MODELOBJECT(ScheduleVariableInterval);
SWIG_MODELOBJECT(ScheduleCompact); 
SWIG_MODELOBJECT(ScheduleConstant); 
SWIG_MODELOBJECT(DefaultScheduleSet); 
SWIG_MODELOBJECT(Material);
SWIG_MODELOBJECT(FenestrationMaterial);
SWIG_MODELOBJECT(GasLayer);
SWIG_MODELOBJECT(Glazing);
SWIG_MODELOBJECT(ShadingMaterial);
SWIG_MODELOBJECT(ModelPartitionMaterial);
SWIG_MODELOBJECT(OpaqueMaterial);
SWIG_MODELOBJECT(AirWallMaterial);
SWIG_MODELOBJECT(Blind);
SWIG_MODELOBJECT(AirGap);
SWIG_MODELOBJECT(Gas);
SWIG_MODELOBJECT(GasMixture);
SWIG_MODELOBJECT(InfraredTransparentMaterial);
SWIG_MODELOBJECT(MasslessOpaqueMaterial);
SWIG_MODELOBJECT(RefractionExtinctionGlazing);
SWIG_MODELOBJECT(RoofVegetation);
SWIG_MODELOBJECT(Screen);
SWIG_MODELOBJECT(Shade);
SWIG_MODELOBJECT(SimpleGlazing);
SWIG_MODELOBJECT(StandardGlazing);
SWIG_MODELOBJECT(StandardOpaqueMaterial);
SWIG_MODELOBJECT(ThermochromicGlazing);
SWIG_MODELOBJECT(ConstructionBase);
SWIG_MODELOBJECT(LayeredConstruction);
SWIG_MODELOBJECT(Construction);
SWIG_MODELOBJECT(ConstructionWithInternalSource);
SWIG_MODELOBJECT(CFactorUndergroundWallConstruction);
SWIG_MODELOBJECT(FFactorGroundFloorConstruction);
SWIG_MODELOBJECT(StandardsInformationConstruction);
SWIG_MODELOBJECT(WindowDataFile);
SWIG_MODELOBJECT(DefaultSubSurfaceConstructions); 
SWIG_MODELOBJECT(DefaultSurfaceConstructions); 
SWIG_MODELOBJECT(ShadingControl);
SWIG_MODELOBJECT(Curve);
SWIG_MODELOBJECT(CurveBicubic);
SWIG_MODELOBJECT(CurveBiquadratic);
SWIG_MODELOBJECT(CurveCubic);
SWIG_MODELOBJECT(CurveDoubleExponentialDecay);
SWIG_MODELOBJECT(CurveExponent);
SWIG_MODELOBJECT(CurveExponentialDecay);
SWIG_MODELOBJECT(CurveExponentialSkewNormal);
SWIG_MODELOBJECT(CurveFanPressureRise);
SWIG_MODELOBJECT(CurveFunctionalPressureDrop);
SWIG_MODELOBJECT(CurveLinear);
SWIG_MODELOBJECT(CurveQuadratic);
SWIG_MODELOBJECT(CurveQuadraticLinear);
SWIG_MODELOBJECT(CurveQuartic);
SWIG_MODELOBJECT(CurveRectangularHyperbola1);
SWIG_MODELOBJECT(CurveRectangularHyperbola2);
SWIG_MODELOBJECT(CurveSigmoid);
SWIG_MODELOBJECT(CurveTriquadratic);
SWIG_MODELOBJECT(SpaceLoadDefinition);  
SWIG_MODELOBJECT(PeopleDefinition);
SWIG_MODELOBJECT(LightsDefinition); 
SWIG_MODELOBJECT(LuminaireDefinition);
SWIG_MODELOBJECT(ElectricEquipmentDefinition);
SWIG_MODELOBJECT(GasEquipmentDefinition);
SWIG_MODELOBJECT(HotWaterEquipmentDefinition);
SWIG_MODELOBJECT(SteamEquipmentDefinition);
SWIG_MODELOBJECT(OtherEquipmentDefinition);
SWIG_MODELOBJECT(InternalMassDefinition);
SWIG_MODELOBJECT(ExteriorLightsDefinition);
SWIG_MODELOBJECT(RenderingColor);
SWIG_MODELOBJECT(DesignSpecificationOutdoorAir); 

%include <model/ScheduleTypeRegistry.hpp>

#endif 




