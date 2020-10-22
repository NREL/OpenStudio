#include "PolygonGroup.hpp"
#include "Point3d.hpp"
#include "../core/LogMessage.hpp"
#include "../core/Logger.hpp"
#include "BoundingBox.hpp"

namespace openstudio {


  void PolygonGroup::intersectSurfaces(PolygonGroup& otherGroup) {

  std::set<std::string> completedIntersections;

  bool anyNewSurfaces = true;
  while (anyNewSurfaces) {

    anyNewSurfaces = false;
    std::vector<Polygon> newSurfaces;
    std::vector<Polygon> newOtherSurfaces;

    for (Polygon& surface : surfaces) {
      std::string surfaceName = surface.getName();
      std::string surfaceHandle = surface.getHandle();

      for (Polygon& otherSurface : otherGroup.surfaces) {
        std::string otherSurfaceName = otherSurface.getName();
        std::string otherSurfaceHandle = otherSurface.getHandle();

        // Make sure these two surfaces have not already been intersected
        std::string intersectionKey = surfaceHandle + otherSurfaceHandle;
        if (completedIntersections.find(intersectionKey) != completedIntersections.end()) continue;
        completedIntersections.insert(intersectionKey);

        boost::optional<IntersectionResultEx> result = surface.computeIntersection(otherSurface);
        if (result) {
          LOG_FREE(Info, "XXX", "Intersection found " << surfaceName << " with " << otherSurfaceName);
          LOG_FREE(Info, "XXX", surfaceName << " " << surface.getPoints());
          LOG_FREE(Info, "XXX", otherSurfaceName << " " << otherSurface.getPoints());

          // Found an intersection
          // The first polygon in matchedPolygons1 replaces the vertices in surface
          // The first polygon in matchedPolygons2 replaces the vertces in otherSurface
          // The remaining polygons in matchedPolygons1 and matchedPolygons2 create new polygons in
          // this group and otherGroup (but are matched!(
          LOG_FREE(Info, "XXX", "Common area " << result->getMatchedPolygon1().getPoints());
          surface.setPoints(result->getMatchedPolygon1().getPoints());
          surface.reverse();
          otherSurface.setPoints(result->getMatchedPolygon2().getPoints());

          for (int i = 0; i < result->polygons1().size(); i++) {
            Polygon p = result->polygons1()[i];
            p.reverse();
            newSurfaces.push_back(p);
            //surfaces.push_back(p);
            LOG_FREE(Info, "XXX", "Added new polygon to this polygon group " << p.getPoints());
          }

          for (int i = 0; i < result->polygons2().size(); i++) {
            Polygon p = result->polygons2()[i];
            newOtherSurfaces.push_back(p);
            //otherGroup.surfaces.push_back(p);
            LOG_FREE(Info, "XXX", "Added new polygon to other polygon group " << p.getPoints());
          }

          // I'm really not getting what this is doing
          std::vector<Polygon> ineligibleSurfaces;
          ineligibleSurfaces.push_back(surface);
          ineligibleSurfaces.insert(ineligibleSurfaces.end(), result->polygons1().begin(), result->polygons1().end());

          std::vector<Polygon> ineligibleOtherSurfaces;
          ineligibleOtherSurfaces.push_back(otherSurface);
          ineligibleOtherSurfaces.insert(ineligibleOtherSurfaces.end(), result->polygons2().begin(), result->polygons2().end());

          for (Polygon ineligibleSurface : ineligibleSurfaces) {
            for (Polygon ineligibleOtherSurface : ineligibleOtherSurfaces) {
              std::string ineligibleIntersectionKey = toString(ineligibleSurface.getHandle()) + toString(ineligibleOtherSurface.getHandle());
              completedIntersections.insert(ineligibleIntersectionKey);
            }
          }
        } 
      }
    }

    if (!newSurfaces.empty()) {
      surfaces.insert(surfaces.end(), newSurfaces.begin(), newSurfaces.end());
      anyNewSurfaces = true;
    }
    if (!newOtherSurfaces.empty()) {
      otherGroup.surfaces.insert(otherGroup.surfaces.end(), newOtherSurfaces.begin(), newOtherSurfaces.end());
      anyNewSurfaces = true;
    }
  }
}

  BoundingBox PolygonGroup::boundingBox() const {
    BoundingBox result;

    for (Polygon surface : surfaces) {
      result.addPoints(surface.getPoints());
    }

    return result;
  }
  }  // namespace openstudio