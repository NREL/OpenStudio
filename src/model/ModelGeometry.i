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
  #include <utilities/geometry/Polyhedron.hpp>
  #include <utilities/data/TimeSeries.hpp>
  #include <utilities/sql/SqlFile.hpp>

%}

#if defined SWIGCSHARP

  #undef _csharp_module_name
  #define _csharp_module_name OpenStudioModelGeometry

  // ignore hvac objects for now, add back in with partial classes in ModelHVAC.i
  %ignore openstudio::model::Building::thermalZones;
  %ignore openstudio::model::Space::thermalZone;
  %ignore openstudio::model::Space::setThermalZone;
  %ignore openstudio::model::Space::waterUseEquipment;

  %ignore openstudio::model::DaylightingDeviceTubular::transitionZones;
  %ignore openstudio::model::DaylightingDeviceTubular::addTransitionZone;
  %ignore openstudio::model::DaylightingDeviceTubular::addTransitionZones;


  // Ignore this ctor, use of zone.getZonePropertyUserViewFactorsBySurfaceName is preferred anyways (so I won't even reimplement it using partial classes)
  %ignore openstudio::model::ZonePropertyUserViewFactorsBySurfaceName::ZonePropertyUserViewFactorsBySurfaceName(const ThermalZone& thermalZone);
  %ignore openstudio::model::ZonePropertyUserViewFactorsBySurfaceName::thermalZone;

  // ignore airflow objects for now, add back in with partial classes in ModelAirflow.i
  %ignore openstudio::model::Surface::getAirflowNetworkSurface;
  %ignore openstudio::model::Surface::airflowNetworkSurface;
  %ignore openstudio::model::SubSurface::getAirflowNetworkSurface;
  %ignore openstudio::model::SubSurface::airflowNetworkSurface;

  // ignore generator objects for now, add back in with partial classes in ModelGenerators.i
  %ignore openstudio::model::PlanarSurface::generatorPhotovoltaics;

  // Overload resolution: prefer std::string over char const *
  %ignore openstudio::model::BuildingUnit::setFeature(std::string const&, char const*);

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

#if defined SWIGPYTHON
  %pythoncode %{
    Model = openstudiomodelcore.Model
  %}
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
class TransitionZone;
class BuildingStory;
class BuildingUnit;
class ShadingSurfaceGroup;
class InteriorPartitionSurfaceGroup;
class PlanarSurface;
class SurfaceIntersection;
class Surface;
class InternalMass;
class People;
class Lights;
class Luminaire;
class ElectricEquipment;
class ElectricEquipmentITEAirCooled;
class GasEquipment;
class HotWaterEquipment;
class SteamEquipment;
class OtherEquipment;
class DaylightingControl;
class IlluminanceMap;
class GlareSensor;
class SpaceInfiltrationDesignFlowRate;
class SpaceInfiltrationEffectiveLeakageArea;
class SpaceInfiltrationFlowCoefficient;
class DesignSpecificationOutdoorAir;
class SpaceLoadInstance;
class SpaceLoadDefinition;
class GeneratorPhotovoltaic;

%feature("valuewrapper") AirflowNetworkSurface;
%feature("valuewrapper") AirflowNetworkCrack;
class AirflowNetworkSurface;
class AirflowNetworkCrack;
class AirflowNetworkDetailedOpening;
class AirflowNetworkEffectiveLeakageArea;
class AirflowNetworkHorizontalOpening;
class AirflowNetworkSimpleOpening;

class ExteriorLights;
class ExteriorFuelEquipment;
class ExteriorWaterEquipment;
class ExteriorLoadDefinition;
class ExteriorLoadInstance;

}
}

// extend classes
%extend openstudio::model::SurfaceIntersection {
  // Use the overloaded operator<< for string representation
  std::string __str__() {
    std::ostringstream os;
    os << *$self;
    return os.str();
  }
};

%extend openstudio::model::CustomBlock {
  // Use the overloaded operator<< for string representation
  std::string __str__() {
    std::ostringstream os;
    os << *$self;
    return os.str();
  }
};

%extend openstudio::model::ViewFactor {
  // Use the overloaded operator<< for string representation
  std::string __str__() {
    std::ostringstream os;
    os << *$self;
    return os.str();
  }
};

%extend openstudio::model::SurroundingSurfaceGroup {
  // Use the overloaded operator<< for string representation
  std::string __str__() {
    std::ostringstream os;
    os << *$self;
    return os.str();
  }
};

%extend openstudio::model::GroundSurfaceGroup {
  // Use the overloaded operator<< for string representation
  std::string __str__() {
    std::ostringstream os;
    os << *$self;
    return os.str();
  }
};

UNIQUEMODELOBJECT_TEMPLATES(Site);
UNIQUEMODELOBJECT_TEMPLATES(Facility);
UNIQUEMODELOBJECT_TEMPLATES(Building);
MODELOBJECT_TEMPLATES(BuildingStory);
MODELOBJECT_TEMPLATES(BuildingUnit);
MODELOBJECT_TEMPLATES(PlanarSurfaceGroup);
MODELOBJECT_TEMPLATES(Space);
MODELOBJECT_TEMPLATES(SpaceItem);
MODELOBJECT_TEMPLATES(SpaceLoad);
MODELOBJECT_TEMPLATES(SpaceLoadInstance);
MODELOBJECT_TEMPLATES(Lights);
MODELOBJECT_TEMPLATES(PlanarSurface);
MODELOBJECT_TEMPLATES(DefaultConstructionSet);
MODELOBJECT_TEMPLATES(Surface);
MODELOBJECT_TEMPLATES(SurfaceIntersection); // Defined in Surface.hpp
MODELOBJECT_TEMPLATES(SubSurface);
MODELOBJECT_TEMPLATES(ShadingSurfaceGroup);
MODELOBJECT_TEMPLATES(ShadingSurface);
MODELOBJECT_TEMPLATES(InteriorPartitionSurfaceGroup);
MODELOBJECT_TEMPLATES(InteriorPartitionSurface);
MODELOBJECT_TEMPLATES(SurfaceControlMovableInsulation);
MODELOBJECT_TEMPLATES(SurfacePropertyOtherSideCoefficients);
MODELOBJECT_TEMPLATES(SurfacePropertyOtherSideConditionsModel);
MODELOBJECT_TEMPLATES(SurfacePropertyLocalEnvironment);
MODELOBJECT_TEMPLATES(SurroundingSurfaceGroup); // helper for extensible fields for SurfacePropertySurroundingSurfaces
MODELOBJECT_TEMPLATES(SurfacePropertySurroundingSurfaces);
MODELOBJECT_TEMPLATES(GroundSurfaceGroup); // helper for extensible fields for SurfacePropertyGroundSurfaces
MODELOBJECT_TEMPLATES(SurfacePropertyGroundSurfaces);
MODELOBJECT_TEMPLATES(SurfacePropertyIncidentSolarMultiplier);
MODELOBJECT_TEMPLATES(SurfacePropertyConvectionCoefficients);
MODELOBJECT_TEMPLATES(People);
MODELOBJECT_TEMPLATES(Luminaire);
MODELOBJECT_TEMPLATES(ElectricEquipment);
MODELOBJECT_TEMPLATES(ElectricEquipmentITEAirCooled);
MODELOBJECT_TEMPLATES(GasEquipment);
MODELOBJECT_TEMPLATES(HotWaterEquipment);
MODELOBJECT_TEMPLATES(SteamEquipment);
MODELOBJECT_TEMPLATES(OtherEquipment);
MODELOBJECT_TEMPLATES(InternalMass);
MODELOBJECT_TEMPLATES(SpaceInfiltrationDesignFlowRate);
MODELOBJECT_TEMPLATES(SpaceInfiltrationEffectiveLeakageArea);
MODELOBJECT_TEMPLATES(SpaceInfiltrationFlowCoefficient);
MODELOBJECT_TEMPLATES(DaylightingControl);
MODELOBJECT_TEMPLATES(GlareSensor);
MODELOBJECT_TEMPLATES(IlluminanceMap);
MODELOBJECT_TEMPLATES(DaylightingDeviceShelf);
MODELOBJECT_TEMPLATES(DaylightingDeviceTubular);
MODELOBJECT_TEMPLATES(DaylightingDeviceLightWell);
MODELOBJECT_TEMPLATES(SpaceType);
MODELOBJECT_TEMPLATES(LightingSimulationZone);
MODELOBJECT_TEMPLATES(CustomBlock); // Helper class defined in FoundationKiva
MODELOBJECT_TEMPLATES(FoundationKiva);
MODELOBJECT_TEMPLATES(SurfacePropertyExposedFoundationPerimeter);
MODELOBJECT_TEMPLATES(ViewFactor); // Helper class defined in ZonePropertyUserViewFactorsBySurfaceName
MODELOBJECT_TEMPLATES(ZonePropertyUserViewFactorsBySurfaceName);
MODELOBJECT_TEMPLATES(ExteriorLoadInstance);
MODELOBJECT_TEMPLATES(ExteriorLights);
MODELOBJECT_TEMPLATES(ExteriorFuelEquipment);
MODELOBJECT_TEMPLATES(ExteriorWaterEquipment);


SWIG_UNIQUEMODELOBJECT(Site);
SWIG_UNIQUEMODELOBJECT(Facility);
SWIG_UNIQUEMODELOBJECT(Building);
SWIG_MODELOBJECT(BuildingStory, 1);
SWIG_MODELOBJECT(BuildingUnit, 1);
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
SWIG_MODELOBJECT(SurfaceControlMovableInsulation, 1);
SWIG_MODELOBJECT(SurfacePropertyOtherSideCoefficients, 1);
SWIG_MODELOBJECT(SurfacePropertyOtherSideConditionsModel, 1);
SWIG_MODELOBJECT(SurfacePropertyLocalEnvironment, 1);
SWIG_MODELOBJECT(SurfacePropertySurroundingSurfaces, 1);
SWIG_MODELOBJECT(SurfacePropertyGroundSurfaces, 1);
SWIG_MODELOBJECT(SurfacePropertyIncidentSolarMultiplier, 1);
SWIG_MODELOBJECT(SurfacePropertyConvectionCoefficients, 1);
SWIG_MODELOBJECT(People, 1);
SWIG_MODELOBJECT(Luminaire, 1);
SWIG_MODELOBJECT(ElectricEquipment, 1);
SWIG_MODELOBJECT(ElectricEquipmentITEAirCooled, 1);
SWIG_MODELOBJECT(GasEquipment, 1);
SWIG_MODELOBJECT(HotWaterEquipment, 1);
SWIG_MODELOBJECT(SteamEquipment, 1);
SWIG_MODELOBJECT(OtherEquipment, 1);
SWIG_MODELOBJECT(InternalMass, 1);
SWIG_MODELOBJECT(SpaceInfiltrationDesignFlowRate, 1);
SWIG_MODELOBJECT(SpaceInfiltrationEffectiveLeakageArea, 1);
SWIG_MODELOBJECT(SpaceInfiltrationFlowCoefficient, 1);
SWIG_MODELOBJECT(DaylightingControl, 1);
SWIG_MODELOBJECT(GlareSensor, 1);
SWIG_MODELOBJECT(IlluminanceMap, 1);
SWIG_MODELOBJECT(DaylightingDeviceShelf, 1);
SWIG_MODELOBJECT(DaylightingDeviceTubular, 1);
SWIG_MODELOBJECT(DaylightingDeviceLightWell, 1);
SWIG_MODELOBJECT(SpaceType, 1);
SWIG_MODELOBJECT(LightingSimulationZone, 1);
SWIG_MODELOBJECT(FoundationKiva, 1);
SWIG_MODELOBJECT(SurfacePropertyExposedFoundationPerimeter, 1);
SWIG_MODELOBJECT(ZonePropertyUserViewFactorsBySurfaceName, 1);
SWIG_MODELOBJECT(ExteriorLoadInstance, 0);
SWIG_MODELOBJECT(ExteriorLights, 1);
SWIG_MODELOBJECT(ExteriorFuelEquipment, 1);
SWIG_MODELOBJECT(ExteriorWaterEquipment, 1);


#if defined SWIGCSHARP || defined(SWIGJAVA)

  %inline {
    namespace openstudio {
      namespace model {

        openstudio::model::SpaceType getPlenumSpaceType(openstudio::model::Model model){
          return model.plenumSpaceType();
        }

        boost::optional<Building> building(const openstudio::model::Model& model) {
          return model.building();
        }

        boost::optional<Facility> facility(const openstudio::model::Model& model) {
          return model.facility();
        }

        boost::optional<Site> site(const openstudio::model::Model& model) {
          return model.site();
        }


        std::vector<openstudio::model::Space> getSpaces(const openstudio::model::SpaceType& spaceType){
          return spaceType.spaces();
        }

        std::vector<openstudio::model::SubSurface> getSubSurfaces(const openstudio::model::ShadingControl& sc) {
          return sc.subSurfaces();
        }

        boost::optional<unsigned> getSubSurfaceIndexForShadingControl(const openstudio::model::ShadingControl& sc, const openstudio::model::SubSurface& subSurface) {
          return sc.subSurfaceIndex(subSurface);
        }

        bool addSubSurfaceForShadingControl(openstudio::model::ShadingControl sc, const openstudio::model::SubSurface& subSurface) {
          return sc.addSubSurface(subSurface);
        }

        bool addSubSurfaceForShadingControlWithIndex(openstudio::model::ShadingControl sc, const openstudio::model::SubSurface& subSurface, unsigned index) {
          return sc.addSubSurface(subSurface, index);
        }

        bool setSubSurfaceIndexForShadingControl(openstudio::model::ShadingControl sc, const openstudio::model::SubSurface& subSurface, unsigned index) {
          return sc.setSubSurfaceIndex(subSurface, index);
        }

        bool removeSubSurfaceForShadingControl(openstudio::model::ShadingControl sc,  const openstudio::model::SubSurface& subSurface) {
          return sc.removeSubSurface(subSurface);
        }

        bool addSubSurfacesForShadingControl(openstudio::model::ShadingControl sc, const std::vector<openstudio::model::SubSurface>& subSurfaces) {
          return sc.addSubSurfaces(subSurfaces);
        }

        bool setSubSurfacesForShadingControl(openstudio::model::ShadingControl sc, const std::vector<openstudio::model::SubSurface>& subSurfaces) {
          return sc.setSubSurfaces(subSurfaces);
        }

        // EMS Actuator setter for Space (reimplemented from ModelCore.i)
        bool setSpaceForEMSActuator(openstudio::model::EnergyManagementSystemActuator actuator, openstudio::model::Space space) {
          return actuator.setSpace(space);
        }

        // EMS Construction setter  (reimplemented from ModelCore.i)
        bool setConstructionForEMS(openstudio::model::EnergyManagementSystemConstructionIndexVariable ems_cons, openstudio::model::ModelObject construction) {
          return ems_cons.setConstructionObject(construction);
        }

        boost::optional<Site> siteForWeatherFile(const openstudio::model::WeatherFile& weatherFile) {
          return weatherFile.site();
        }
        boost::optional<Site> siteForClimateZones(const openstudio::model::ClimateZones& climateZones) {
          return climateZones.site();
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

    public partial class Model : Workspace {
      public SpaceType plenumSpaceType() {
        return OpenStudio.OpenStudioModelGeometry.getPlenumSpaceType(this);
      }

      public OptionalBuilding building() {
        return OpenStudio.OpenStudioModelGeometry.building(this);
      }

      public OptionalFacility facility() {
        return OpenStudio.OpenStudioModelGeometry.facility(this);
      }

      public OptionalSite site() {
        return OpenStudio.OpenStudioModelGeometry.site(this);
      }
    } // partial class Model

    public partial class SpaceType : ResourceObject {
      public SpaceVector spaces()
      {
        return OpenStudio.OpenStudioModelGeometry.getSpaces(this);
      }
    }

    public partial class ShadingControl : ResourceObject {
      public SubSurfaceVector subSurfaces() {
        return OpenStudio.OpenStudioModelGeometry.getSubSurfaces(this);
      }

      public OptionalUnsigned subSurfaceIndex(OpenStudio.SubSurface subSurface) {
        return OpenStudio.OpenStudioModelGeometry.getSubSurfaceIndexForShadingControl(this, subSurface);
      }

      public bool addSubSurface(OpenStudio.SubSurface subSurface) {
        return OpenStudio.OpenStudioModelGeometry.addSubSurfaceForShadingControl(this, subSurface);
      }

      public bool addSubSurface(OpenStudio.SubSurface subSurface, uint index) {
        return OpenStudio.OpenStudioModelGeometry.addSubSurfaceForShadingControlWithIndex(this, subSurface, index);
      }

      public bool setSubSurfaceIndex(OpenStudio.SubSurface subSurface, uint index) {
        return OpenStudio.OpenStudioModelGeometry.setSubSurfaceIndexForShadingControl(this, subSurface, index);
      }

      public bool removeSubSurface(OpenStudio.SubSurface subSurface) {
        return OpenStudio.OpenStudioModelGeometry.removeSubSurfaceForShadingControl(this, subSurface);
      }

      public bool addSubSurfaces(SubSurfaceVector subSurfaces) {
        return OpenStudio.OpenStudioModelGeometry.addSubSurfacesForShadingControl(this, subSurfaces);
      }

      public bool setSubSurfaces(SubSurfaceVector subSurfaces) {
        return OpenStudio.OpenStudioModelGeometry.setSubSurfacesForShadingControl(this, subSurfaces);
      }
    }

    public partial class EnergyManagementSystemActuator : ModelObject {
      public bool setSpace(OpenStudio.Space space) {
        return OpenStudio.OpenStudioModelGeometry.setSpaceForEMSActuator(this, space);
      }

      // Overloaded Ctor, calling Ctor that doesn't use Space
      public EnergyManagementSystemActuator(ModelObject modelObject, string actuatedComponentType, string actuatedComponentControlType, OpenStudio.Space space)
        : this(modelObject, actuatedComponentType, actuatedComponentControlType) {
        this.setSpace(space);
      }
    }

    public partial class EnergyManagementSystemConstructionIndexVariable : ModelObject {
      public bool setConstructionObject(OpenStudio.Construction construction) {
        return OpenStudio.OpenStudioModelGeometry.setConstructionForEMS(this, construction);
      }

      // Overloaded Ctor, calling basic constructor
      public EnergyManagementSystemConstructionIndexVariable(Model model, OpenStudio.Construction construction)
        : this(model) {
        this.setConstructionObject(construction);
      }
    }

    public partial class WeatherFile : ModelObject {
      public OptionalSite site()
      {
        return OpenStudio.OpenStudioModelGeometry.siteForWeatherFile(this);
      }
    }

    public partial class ClimateZones : ModelObject {
      public OptionalSite site()
      {
        return OpenStudio.OpenStudioModelGeometry.siteForClimateZones(this);
      }
    }

  %}
#endif

#endif


