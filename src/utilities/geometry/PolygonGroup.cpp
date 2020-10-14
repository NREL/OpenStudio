#include "PolygonGroup.hpp"
#include "Point3d.hpp"
#include "../core/LogMessage.hpp"
#include "../core/Logger.hpp"

namespace openstudio {


  void PolygonGroup::intersectSurfaces(PolygonGroup& otherGroup) {

          std::set<std::string> completedIntersections;

  bool anyNewSurfaces = true;
  while (anyNewSurfaces) {

    anyNewSurfaces = false;
    std::vector<Polygon> newSurfaces;
    std::vector<Polygon> newOtherSurfaces;

    for (auto surface : surfaces) {
      std::string surfaceName = surface.getName();
      std::string surfaceHandle = surface.getHandle();

      for (auto otherSurface : otherGroup.surfaces) {
        std::string otherSurfaceName = otherSurface.getName();
        std::string otherSurfaceHandle = otherSurface.getHandle();
        
        // Make sure these two surfaces have not already been intersected
        std::string intersectionKey = surfaceHandle + otherSurfaceHandle;
        if (completedIntersections.find(intersectionKey) != completedIntersections.end()) continue;
        completedIntersections.insert(intersectionKey);
        LOG_FREE(Info, "XXX", "Intersecting surface " << surfaceName << " with " << otherSurfaceName);

        boost::optional<IntersectionResultEx>result = surface.computeIntersection(otherSurface);
        if (result) {
            // Found an intersection
            // The first polygon in matchedPolygons1 replaces the vertices in surface
            // The first polygon in matchedPolygons2 replaces the vertces in otherSurface
            // The remaining polygons in matchedPolygons1 and matchedPolygons2 create new polygons in 
            // this group and otherGroup (but are matched!(
            //
            surface.setPoints(result->getMatchedPolygon1().getPoints());
            surface.reverse();
            otherSurface.setPoints(result->getMatchedPolygon2().getPoints());

            for (int i = 0; i < result->polygons1().size(); i++) {
              Polygon p = result->polygons1()[i];
              p.reverse();
              surfaces.push_back(p);
            }

            for (int i = 0; i < result->polygons2().size(); i++) {
              Polygon p = result->polygons2()[i];
              otherGroup.surfaces.push_back(p);
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

            if (!result->polygons1().empty() || !result->polygons2().empty()) anyNewSurfaces = true;
        }
      }
    }

    
  }
}


}  // namespace openstudio