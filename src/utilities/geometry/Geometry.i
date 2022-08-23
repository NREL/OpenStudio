#ifndef UTILITIES_GEOMETRY_GEOMETRY_I
#define UTILITIES_GEOMETRY_GEOMETRY_I

#if defined(SWIGCSHARP)

  // Avoid triggering a SWIG warning: 'object' is a C# keyword
  %rename(threeSceneObject) openstudio::ThreeScene::object;

#endif

%{
  namespace openstudio{}
  using namespace openstudio;
  #include <utilities/geometry/Vector3d.hpp>
  #include <utilities/geometry/Point3d.hpp>
  #include <utilities/geometry/Polygon3d.hpp>
  #include <utilities/geometry/PointLatLon.hpp>
  #include <utilities/geometry/Plane.hpp>
  #include <utilities/geometry/EulerAngles.hpp>
  #include <utilities/geometry/Geometry.hpp>
  #include <utilities/geometry/Transformation.hpp>
  #include <utilities/geometry/BoundingBox.hpp>
  #include <utilities/geometry/Intersection.hpp>
  #include <utilities/geometry/ThreeJS.hpp>
  #include <utilities/geometry/FloorplanJS.hpp>
  #include <utilities/geometry/RoofGeometry.hpp>
  #include <utilities/geometry/Polyhedron.hpp>

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
%template(OptionalFloorplanObject) boost::optional<openstudio::FloorplanObject>;
%template(OptionalPolygon3d) boost::optional<openstudio::Polygon3d>;
%template(OptionalPolyhedron) boost::optional<openstudio::Polyhedron>;

// create an instantiation of the vector classes
// Note JM 2019-04-16: No need to ignore std::vector<T>::vector/resize when you have a default constructor
%template(Point3dVector) std::vector<openstudio::Point3d>;
%template(Point3dVectorVector) std::vector<std::vector<openstudio::Point3d> >; // for polygon subtraction routines
%template(OptionalPoint3dVector) boost::optional< std::vector<openstudio::Point3d> >; // For openstudio::join (Intersection.hpp)
%template(OptionalPoint3dVectorVector) boost::optional< std::vector< std::vector<openstudio::Point3d> > >; // For openstudio::buffer (Intersection.hpp)
%template(PointLatLonVector) std::vector<openstudio::PointLatLon>;
%template(Vector3dVector) std::vector<openstudio::Vector3d>;
%template(Polygon3dVector) std::vector<openstudio::Polygon3d>;
%template(PolyhedronVector) std::vector<openstudio::Polyhedron>;
%template(Surface3dVector) std::vector<openstudio::Surface3d>;

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

%ignore std::vector<openstudio::ThreeModelObjectMetadata>::vector(size_type);
%ignore std::vector<openstudio::ThreeModelObjectMetadata>::resize(size_type);
%template(ThreeModelObjectMetadataVector) std::vector<openstudio::ThreeModelObjectMetadata>;

%ignore std::vector<openstudio::FloorplanObject>::vector(size_type);
%ignore std::vector<openstudio::FloorplanObject>::resize(size_type);
%template(FloorplanObjectVector) std::vector<openstudio::FloorplanObject>;

%ignore std::vector<openstudio::Surface3dEdge>::vector(size_type);
%ignore std::vector<openstudio::Surface3dEdge>::resize(size_type);
%template(Surface3dEdgeVector) std::vector<openstudio::Surface3dEdge>;

%template(SizeTVector) std::vector<size_t>;
%template(StringStringMap) std::map<std::string, std::string>;

%ignore openstudio::operator<<;

%include <utilities/geometry/Vector3d.hpp>
%include <utilities/geometry/Point3d.hpp>
%include <utilities/geometry/Polygon3d.hpp>
%include <utilities/geometry/PointLatLon.hpp>
%include <utilities/geometry/Plane.hpp>
%include <utilities/geometry/EulerAngles.hpp>
%include <utilities/geometry/Geometry.hpp>
%include <utilities/geometry/Transformation.hpp>
%include <utilities/geometry/BoundingBox.hpp>
%include <utilities/geometry/Intersection.hpp>
%include <utilities/geometry/ThreeJS.hpp>
%include <utilities/geometry/FloorplanJS.hpp>
%include <utilities/geometry/RoofGeometry.hpp>
%include <utilities/geometry/Polyhedron.hpp>

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

%extend openstudio::Plane {
  std::string __str__() const {
    std::ostringstream os;
    os << *self;
    return os.str();
  }
}

%extend openstudio::Surface3dEdge {
  std::string __str__() const {
    std::ostringstream os;
    os << *self;
    return os.str();
  }
}

%extend openstudio::Transformation {

  std::string __str__() const {
    std::ostringstream os;

    typedef Matrix::size_type size_type;
    Matrix m = self->matrix();
    size_type size1 = m.size1();
    size_type size2 = m.size2();

    // Always size (4, 4) really
    os << "Transformation with Matrix";
    if ((size1 == 0) || (size2 == 0)) {
      os << ": Nothing to show, at least one dimension is zero";
      return os.str();
    } else {
      os << ":\n[";
      for (size_type i = 0; i < size1; ++i) {
        os << '[';
        for (size_type j = 0; j < size2; ++j) {
          os << m(i, j);
          if (j != size2 - 1) {
            os << ", ";
          }
        }
        os << ']';
        if (i != size1 - 1) {
          os << ",\n ";
        }
      }
      os << ']';
    }
    return os.str();
  }
}

#endif //UTILITIES_GEOMETRY_GEOMETRY_I
