#ifndef UTILITIES_GEOMETRY_GEOMETRY_I
#define UTILITIES_GEOMETRY_GEOMETRY_I

%{
  namespace openstudio{}
  using namespace openstudio;
  #include <utilities/geometry/Vector3d.hpp>
  #include <utilities/geometry/Point3d.hpp>
  #include <utilities/geometry/PointLatLon.hpp>
  #include <utilities/geometry/Plane.hpp>
  #include <utilities/geometry/EulerAngles.hpp>
  #include <utilities/geometry/Geometry.hpp>
  #include <utilities/geometry/Transformation.hpp>
  #include <utilities/geometry/BoundingBox.hpp>
  #include <utilities/geometry/Intersection.hpp>
  #include <utilities/geometry/ThreeJS.hpp>
  #include <utilities/geometry/FloorplanJS.hpp>
  
  #include <utilities/units/Quantity.hpp>
  #include <utilities/units/Unit.hpp>
  #include <utilities/units/BTUUnit.hpp>
  #include <utilities/units/CelsiusUnit.hpp>
  #include <utilities/units/CFMUnit.hpp>
  #include <utilities/units/FahrenheitUnit.hpp>
  #include <utilities/units/GPDUnit.hpp>
  #include <utilities/units/IPUnit.hpp>
  #include <utilities/units/Misc1Unit.hpp>
  #include <utilities/units/MPHUnit.hpp>
  #include <utilities/units/SIUnit.hpp>
  #include <utilities/units/ThermUnit.hpp>
  #include <utilities/units/WhUnit.hpp>  
%}

// create an instantiation of the optional classes
%template(OptionalPoint3d) boost::optional<openstudio::Point3d>;
%template(OptionalPointLatLon) boost::optional<openstudio::PointLatLon>;
%template(OptionalVector3d) boost::optional<openstudio::Vector3d>;
%template(OptionalPlane) boost::optional<openstudio::Plane>;
%template(OptionalEulerAngles) boost::optional<openstudio::EulerAngles>;
%template(OptionalBoundingBox) boost::optional<openstudio::BoundingBox>;
%template(OptionalIntersectionResult) boost::optional<openstudio::IntersectionResult>;
%template(OptionalThreeScene) boost::optional<openstudio::ThreeScene>;
%template(OptionalThreeMaterial) boost::optional<openstudio::ThreeMaterial>;
%template(OptionalThreeGeometry) boost::optional<openstudio::ThreeGeometry>;
%template(OptionalFloorplanJS) boost::optional<openstudio::FloorplanJS>;

// create an instantiation of the vector classes
%template(Point3dVector) std::vector<openstudio::Point3d>;
%template(Point3dVectorVector) std::vector<std::vector<openstudio::Point3d> >; // for polygon subtraction routines
%template(PointLatLonVector) std::vector<openstudio::PointLatLon>;
%template(Vector3dVector) std::vector<openstudio::Vector3d>;  
%ignore std::vector<openstudio::Plane>::vector(size_type);
%ignore std::vector<openstudio::Plane>::resize(size_type);
%template(PlaneVector) std::vector<openstudio::Plane>;
%template(EulerAnglesVector) std::vector<openstudio::EulerAngles>;
%template(BoundingBoxVector) std::vector<openstudio::BoundingBox>;
%ignore std::vector<openstudio::ThreeSceneChild>::vector(size_type);
%ignore std::vector<openstudio::ThreeSceneChild>::resize(size_type);
%template(ThreeSceneChildVector) std::vector<openstudio::ThreeSceneChild>;
%ignore std::vector<openstudio::ThreeGeometry>::vector(size_type);
%ignore std::vector<openstudio::ThreeGeometry>::resize(size_type);
%template(ThreeGeometryVector) std::vector<openstudio::ThreeGeometry>;
%ignore std::vector<openstudio::ThreeMaterial>::vector(size_type);
%ignore std::vector<openstudio::ThreeMaterial>::resize(size_type);
%template(ThreeMaterialVector) std::vector<openstudio::ThreeMaterial>;
%template(ThreeModelObjectMetadataVector) std::vector<openstudio::ThreeModelObjectMetadata>;
%ignore std::vector<openstudio::ThreeModelObjectMetadata>::vector(size_type);
%ignore std::vector<openstudio::ThreeModelObjectMetadata>::resize(size_type);

%ignore openstudio::operator<<;

%include <utilities/geometry/Vector3d.hpp>
%include <utilities/geometry/Point3d.hpp>
%include <utilities/geometry/PointLatLon.hpp>
%include <utilities/geometry/Plane.hpp>
%include <utilities/geometry/EulerAngles.hpp>
%include <utilities/geometry/Geometry.hpp>
%include <utilities/geometry/Transformation.hpp>
%include <utilities/geometry/BoundingBox.hpp>
%include <utilities/geometry/Intersection.hpp>
%include <utilities/geometry/ThreeJS.hpp>
%include <utilities/geometry/FloorplanJS.hpp>

%extend openstudio::Vector3d{
  std::string __str__() const {
    std::ostringstream os;
    os << *self;
    return os.str();
  }
}

%extend openstudio::Point3d{
  std::string __str__() const {
    std::ostringstream os;
    os << *self;
    return os.str();
  }
}

%extend openstudio::PointLatLon{
  std::string __str__() const {
    std::ostringstream os;
    os << *self;
    return os.str();
  }
}

#endif //UTILITIES_GEOMETRY_GEOMETRY_I 
