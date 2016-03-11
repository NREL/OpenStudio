#ifndef MODEL_GEOMETRY_I
#define MODEL_GEOMETRY_I

#ifdef SWIGPYTHON
  %module openstudiomodelgeometry
#endif


%include <model/Model_Common_Include.i>
%import <model/ModelCore.i>
%import <model/ModelSimulation.i>
%import <model/ModelResources.i>

%{
  #include <utilities/geometry/Transformation.hpp>
  #include <utilities/geometry/BoundingBox.hpp>
  #include <utilities/data/TimeSeries.hpp>
  #include <utilities/sql/SqlFile.hpp>

%}

#if defined SWIGCSHARP

  #undef _csharp_module_name
  #define _csharp_module_name OpenStudioModelGeometry
  
  // ignore thermal zone for now
  %ignore openstudio::model::Building::thermalZones;
  %ignore openstudio::model::Space::thermalZone;
  %ignore openstudio::model::Space::setThermalZone;
  
  // ignore GeneratorPhotovoltaic
  %ignore openstudio::model::PlanarSurface::generatorPhotovoltaics;
  
  // DLM: this is a proof of concept section to see how attributes work in C#
  %include <attribute.i>

  // partofTotalFloorArea is a read/write boolean attribute
  %attribute(openstudio::model::Space, bool, partofTotalFloorArea, partofTotalFloorArea, setPartofTotalFloorArea);
  
  // floorArea is a read only double attribute
  %attribute(openstudio::model::Space, double, floorArea, floorArea);

  // try spaceType as a read only attribute
  %attributeval(openstudio::model::Space, boost::optional<openstudio::model::SpaceType>, spaceType, spaceType);

  // try surfaces as a read only attribute
  %attributeval(openstudio::model::Space, std::vector<openstudio::model::Surface>, surfaces, surfaces);

#endif

// These cannot easily be wrapped due to default constructor requirements in the swig wrapper of std::pair
%ignore openstudio::model::Space::getDefaultConstructionWithSearchDistance;
%ignore openstudio::model::PlanarSurface::constructionWithSearchDistance;

namespace openstudio {
namespace model {

// forward declarations
class SpaceType;
class ConstructionBase;
class DefaultConstructionSet;
class Schedule;
class DefaultScheduleType;
class DefaultScheduleSet;
class ThermalZone;
class BuildingStory;
class ShadingSurfaceGroup;
class InteriorPartitionSurfaceGroup;
class PlanarSurface;
class Surface;
class InternalMass;
class People;
class Lights;
class Luminaire;
class ElectricEquipment;
class GasEquipment;
class HotWaterEquipment;
class SteamEquipment;
class OtherEquipment;
class DaylightingControl;
class IlluminanceMap;
class GlareSensor;
class SpaceInfiltrationDesignFlowRate;
class SpaceInfiltrationEffectiveLeakageArea;
class DesignSpecificationOutdoorAir;
class SpaceLoadInstance;
class SpaceLoadDefinition;
class GeneratorPhotovoltaic;
}
}

UNIQUEMODELOBJECT_TEMPLATES(Site);
UNIQUEMODELOBJECT_TEMPLATES(Facility);
UNIQUEMODELOBJECT_TEMPLATES(Building);
MODELOBJECT_TEMPLATES(BuildingStory);
MODELOBJECT_TEMPLATES(PlanarSurfaceGroup);
MODELOBJECT_TEMPLATES(Space); 
MODELOBJECT_TEMPLATES(SpaceItem); 
MODELOBJECT_TEMPLATES(SpaceLoad);
MODELOBJECT_TEMPLATES(SpaceLoadInstance);   
MODELOBJECT_TEMPLATES(Lights); 
MODELOBJECT_TEMPLATES(PlanarSurface);
MODELOBJECT_TEMPLATES(DefaultConstructionSet);
MODELOBJECT_TEMPLATES(Surface); 
MODELOBJECT_TEMPLATES(SubSurface);
MODELOBJECT_TEMPLATES(ShadingSurfaceGroup); 
MODELOBJECT_TEMPLATES(ShadingSurface); 
MODELOBJECT_TEMPLATES(InteriorPartitionSurfaceGroup); 
MODELOBJECT_TEMPLATES(InteriorPartitionSurface);
MODELOBJECT_TEMPLATES(SurfacePropertyOtherSideCoefficients);
MODELOBJECT_TEMPLATES(SurfacePropertyOtherSideConditionsModel);
MODELOBJECT_TEMPLATES(People);
MODELOBJECT_TEMPLATES(Luminaire);
MODELOBJECT_TEMPLATES(ElectricEquipment);
MODELOBJECT_TEMPLATES(GasEquipment);
MODELOBJECT_TEMPLATES(HotWaterEquipment);
MODELOBJECT_TEMPLATES(SteamEquipment);
MODELOBJECT_TEMPLATES(OtherEquipment);
MODELOBJECT_TEMPLATES(InternalMass);
MODELOBJECT_TEMPLATES(SpaceInfiltrationDesignFlowRate);
MODELOBJECT_TEMPLATES(SpaceInfiltrationEffectiveLeakageArea);
MODELOBJECT_TEMPLATES(DaylightingControl);
MODELOBJECT_TEMPLATES(GlareSensor); 
MODELOBJECT_TEMPLATES(IlluminanceMap);
MODELOBJECT_TEMPLATES(DaylightingDeviceShelf);
MODELOBJECT_TEMPLATES(SpaceType);
MODELOBJECT_TEMPLATES(ExteriorLights);
MODELOBJECT_TEMPLATES(LightingSimulationZone);

SWIG_UNIQUEMODELOBJECT(Site);
SWIG_UNIQUEMODELOBJECT(Facility);
SWIG_UNIQUEMODELOBJECT(Building);
SWIG_MODELOBJECT(BuildingStory, 1);
SWIG_MODELOBJECT(PlanarSurfaceGroup, 0);
SWIG_MODELOBJECT(Space, 1); 
SWIG_MODELOBJECT(SpaceItem, 0); 
SWIG_MODELOBJECT(SpaceLoad, 0);
SWIG_MODELOBJECT(SpaceLoadInstance, 0); 
SWIG_MODELOBJECT(Lights, 1); 
SWIG_MODELOBJECT(PlanarSurface, 0);
SWIG_MODELOBJECT(DefaultConstructionSet, 1);
SWIG_MODELOBJECT(Surface, 1); 
SWIG_MODELOBJECT(SubSurface, 1);
SWIG_MODELOBJECT(ShadingSurfaceGroup, 1); 
SWIG_MODELOBJECT(ShadingSurface, 1); 
SWIG_MODELOBJECT(InteriorPartitionSurfaceGroup, 1); 
SWIG_MODELOBJECT(InteriorPartitionSurface, 1);
SWIG_MODELOBJECT(SurfacePropertyOtherSideCoefficients, 1);
SWIG_MODELOBJECT(SurfacePropertyOtherSideConditionsModel, 1);
SWIG_MODELOBJECT(People, 1);
SWIG_MODELOBJECT(Luminaire, 1);
SWIG_MODELOBJECT(ElectricEquipment, 1);
SWIG_MODELOBJECT(GasEquipment, 1);
SWIG_MODELOBJECT(HotWaterEquipment, 1);
SWIG_MODELOBJECT(SteamEquipment, 1);
SWIG_MODELOBJECT(OtherEquipment, 1);
SWIG_MODELOBJECT(InternalMass, 1);
SWIG_MODELOBJECT(SpaceInfiltrationDesignFlowRate, 1);
SWIG_MODELOBJECT(SpaceInfiltrationEffectiveLeakageArea, 1);
SWIG_MODELOBJECT(DaylightingControl, 1);
SWIG_MODELOBJECT(GlareSensor, 1);
SWIG_MODELOBJECT(IlluminanceMap, 1); 
SWIG_MODELOBJECT(DaylightingDeviceShelf, 1);
SWIG_MODELOBJECT(SpaceType, 1);
SWIG_MODELOBJECT(ExteriorLights, 1);
SWIG_MODELOBJECT(LightingSimulationZone, 1);

#if defined SWIGCSHARP || defined(SWIGJAVA)

  %inline {
    namespace openstudio {
      namespace model {
        std::vector<openstudio::model::Space> getSpaces(const openstudio::model::SpaceType& spaceType){
          return spaceType.spaces();
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
    
    public partial class SpaceType : ResourceObject {
      public SpaceVector spaces()
      {
        return OpenStudio.OpenStudioModelGeometry.getSpaces(this);
      }
    }  
  %}
#endif

#endif 


