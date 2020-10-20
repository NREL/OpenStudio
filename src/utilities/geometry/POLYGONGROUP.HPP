#ifndef UTILITIES_GEOMETRY_POLYGONGROUP_HPP
#define UTILITIES_GEOMETRY_POLYGONGROUP_HPP

#include "../UtilitiesAPI.hpp"
#include <vector>
#include "Polygon.hpp"
#include "transformation.hpp"
#include "boundingBox.hpp"

namespace openstudio {

/// <summary>
/// For prototyping this is the equivalent of a space in the sense that a space is a collection of surfaces
/// a polygon group is a collection of polygons.
/// The PolygonGroup shares the name of the space and carries a reference to the space as a void*  because the
/// Space is not accesible at this level.
/// </summary>
class UTILITIES_API PolygonGroup
{
  public:
  std::vector<Polygon> getSurfaces() {
    return surfaces;
  }
  void setSurfaces(std::vector<Polygon> polygons) {
    surfaces = polygons;
  }
  void addSurface(const Polygon& surface) {
    surfaces.push_back(surface);
  }
  std::string getName() {
    return name;
  }
  void setName(std::string value) {
    name = value;
  }
  void setReference(void* value) {
    reference = value;
  }
  void* getReference() {
    return reference;
  }
  void setTransformation(const Transformation& value) {
    transform = value;
  }
  Transformation getTransformation() {
    return transform;
  }
  void intersectSurfaces(PolygonGroup& otherGroup);

  BoundingBox boundingBox() const;

  private:
  void* reference;
  std::string name;
  std::vector<Polygon> surfaces;
  Transformation transform;
};
}  // namespace openstudio
#endif