/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "Polyhedron.hpp"
#include "Polygon3d.hpp"
#include "Vector3d.hpp"
#include "Point3d.hpp"
#include "Geometry.hpp"
#include "Plane.hpp"
#include "Intersection.hpp"
#include <utilities/geometry/Transformation.hpp>

#include <algorithm>
#include <utility>
#include <vector>

namespace openstudio {

Surface3dEdge::Surface3dEdge(Point3d start, Point3d end, Surface3d firstSurface, size_t firstSurfNum)
  : m_start(std::move(start)), m_end(std::move(end)), m_firstSurfNum(firstSurfNum) {
  m_allSurfaces.emplace_back(std::move(firstSurface));
}

/// check equality
bool Surface3dEdge::operator==(const Surface3dEdge& other) const {
  return ((isAlmostEqual3dPt(m_start, other.m_start) && isAlmostEqual3dPt(m_end, other.m_end))
          || (isAlmostEqual3dPt(m_start, other.m_end) && isAlmostEqual3dPt(m_end, other.m_start)));
}

/// check inequality
bool Surface3dEdge::operator!=(const Surface3dEdge& other) const {
  return !(*this == other);
}

bool Surface3dEdge::reverseEqual(const Surface3dEdge& other) const {
  return isAlmostEqual3dPt(m_start, other.m_end) && isAlmostEqual3dPt(m_end, other.m_start);
}

Point3d Surface3dEdge::start() const {
  return m_start;
}
Point3d Surface3dEdge::end() const {
  return m_end;
}

size_t Surface3dEdge::count() const {
  return m_allSurfaces.size();
}

size_t Surface3dEdge::firstSurfNum() const {
  return m_firstSurfNum;
}

std::vector<Surface3d> Surface3dEdge::allSurfaces() const {
  return m_allSurfaces;
}

void Surface3dEdge::appendSurface(Surface3d surface) {
  m_allSurfaces.emplace_back(std::move(surface));
}

bool Surface3dEdge::containsPoint(const Point3d& testVertex) {
  return !isAlmostEqual3dPt(m_start, testVertex) && !isAlmostEqual3dPt(m_end, testVertex) && isPointOnLineBetweenPoints(m_start, m_end, testVertex);
}

std::ostream& operator<<(std::ostream& os, const Surface3dEdge& edge) {
  os << "Surface3dEdge: start=" << edge.start() << ", end=" << edge.end() << ", count=" << edge.count()
     << ", firstSurface=" << edge.allSurfaces()[0].name;
  return os;
}

Surface3d::Surface3d(std::vector<Point3d> t_vertices, std::string t_name) : vertices(std::move(t_vertices)), name(std::move(t_name)) {}

Polyhedron::Polyhedron(std::vector<Surface3d> surfaces) : m_surfaces(std::move(surfaces)){};

size_t Polyhedron::numVertices() const {
  size_t count = 0;
  for (const auto& surface : m_surfaces) {
    count += surface.vertices.size();
  }
  return count;
}

std::vector<Point3d> Polyhedron::uniqueVertices() const {

  std::vector<Point3d> uniqVertices;
  uniqVertices.reserve(numVertices());

  for (const auto& surface : m_surfaces) {
    for (const auto& pt : surface.vertices) {
      if (std::find_if(uniqVertices.cbegin(), uniqVertices.cend(), [&pt](const auto& unqV) { return isAlmostEqual3dPt(pt, unqV); })
          == uniqVertices.cend()) {
        uniqVertices.push_back(pt);
      }
    }
  }
  return uniqVertices;
}

std::vector<Surface3dEdge> Polyhedron::edgesNotTwoForEnclosedVolumeTest() const {

  std::vector<Surface3dEdge> uniqueSurface3dEdges;
  uniqueSurface3dEdges.reserve(numVertices());

  // construct list of unique edges
  int surfNum = 0;
  for (const auto& surface : m_surfaces) {
    LOG(Debug, "Surface: " << surface.name);
    const auto& vertices = surface.vertices;
    for (auto it = vertices.begin(); it != vertices.end(); ++it) {
      auto itnext = std::next(it);
      if (itnext == std::end(vertices)) {
        itnext = std::begin(vertices);
      }
      Surface3dEdge thisSurface3dEdge(*it, *itnext, surface, surfNum);
      auto itFound = std::find(uniqueSurface3dEdges.begin(), uniqueSurface3dEdges.end(), thisSurface3dEdge);
      if (itFound == uniqueSurface3dEdges.end()) {
        LOG(Debug, "NOT FOUND: " << thisSurface3dEdge);
        uniqueSurface3dEdges.emplace_back(std::move(thisSurface3dEdge));
      } else {
        LOG(Debug, "    FOUND: " << thisSurface3dEdge);
        itFound->appendSurface(surface);
      }
    }
    ++surfNum;
  }

  for (auto& edge : uniqueSurface3dEdges) {
    LOG(Debug, edge);
  }
  // All edges for an enclosed polyhedron should be shared by two (and only two) sides.
  // So if the count is not two for all edges, the polyhedron is not enclosed, so erase all that are 2
  uniqueSurface3dEdges.erase(
    std::remove_if(uniqueSurface3dEdges.begin(), uniqueSurface3dEdges.end(), [](const auto& edge) { return edge.count() == 2; }),
    uniqueSurface3dEdges.end());

  return uniqueSurface3dEdges;
}

Polyhedron Polyhedron::updateZonePolygonsForMissingColinearPoints() const {
  // Make a copy, we don't want to mutate in place
  Polyhedron updZonePoly(*this);

  std::vector<Point3d> uniqVertices = uniqueVertices();

  for (auto& surface : updZonePoly.m_surfaces) {
    // for (int iterationLimiter = 0; iterationLimiter < 20; ++iterationLimiter) {  // could probably be while loop but want to make sure it does not get stuck
    LOG(Debug, surface.name)
    bool insertedVertext = true;
    while (insertedVertext) {
      insertedVertext = false;
      auto& vertices = surface.vertices;
      for (auto it = vertices.begin(); it != vertices.end(); ++it) {

        auto itnext = std::next(it);
        if (itnext == std::end(vertices)) {
          itnext = std::begin(vertices);
        }

        // Don't care about surfNum
        Surface3dEdge thisSurface3dEdge(*it, *itnext, surface, 0);

        // now go through all the vertices and see if they are colinear with start and end vertices
        for (const auto& testVertex : uniqVertices) {
          if (thisSurface3dEdge.containsPoint(testVertex)) {
            LOG(Debug, testVertex << " is on " << thisSurface3dEdge);
            vertices.insert(itnext, testVertex);
            insertedVertext = true;
            break;
          }
        }
        // Break out of the surface loop too, and start again at the while
        if (insertedVertext) {
          break;
        }
      }
    }
  }
  return updZonePoly;
}

std::vector<Surface3dEdge> edgesInBoth(const std::vector<Surface3dEdge>& edges1, const std::vector<Surface3dEdge>& edges2) {
  // this is not optimized but the number of edges for a typical polyhedron is 12 and is probably rarely bigger than 20.

  // (std::set_union could be used, but that requires sorting...)
  std::vector<Surface3dEdge> inBoth;
  for (const auto& e1 : edges1) {
    for (const auto& e2 : edges2) {
      // If they have the same surface and are the same (within tolerance)
      if ((e1.firstSurfNum() == e2.firstSurfNum()) && (e1 == e2)) {
        inBoth.push_back(e1);
        break;
      }
    }
  }
  return inBoth;
}

VolumeEnclosedReturnType Polyhedron::isEnclosedVolume() const {

  VolumeEnclosedReturnType result;

  std::vector<Surface3dEdge> edgeNot2orig = this->edgesNotTwoForEnclosedVolumeTest();
  // if all edges had two counts then it is fully enclosed
  if (edgeNot2orig.empty()) {
    result.isEnclosedVolume = true;
    return result;
  } else {  // if the count is three or greater it is likely that a vertex that is colinear was counted on the faces on one edge and not
            // on the "other side" of the edge Go through all the points looking for the number that are colinear and see if that is
            // consistent with the number of edges found that didn't have a count of two
    Polyhedron updatedZonePoly =
      updateZonePolygonsForMissingColinearPoints();  // this is done after initial test since it is computationally intensive.
    std::vector<Surface3dEdge> edgeNot2again = updatedZonePoly.edgesNotTwoForEnclosedVolumeTest();
    if (edgeNot2again.empty()) {
      result.isEnclosedVolume = true;
      return result;
    }
    // only return a list of those edges that appear in both the original edge and the revised edges:
    // this eliminates added edges that will confuse users (edges that were caught by the updateZonePolygonsForMissingColinearPoints routine)
    result.isEnclosedVolume = false;
    result.edgesNot2 = edgesInBoth(edgeNot2orig, edgeNot2again);
    return result;
  }
}

// boost::optional<double> Polyhedron::calculatedVolume() const {
//   auto [isVolEnclosed, edgesNot2] = isEnclosedVolume();
//   if (isVolEnclosed) {
//     return calcPolyhedronVolume();
//   }
//   return boost::none;
// }

double Polyhedron::calcPolyhedronVolume() const {

  double volume = 0.0;
  for (const auto& surface : m_surfaces) {
    const std::vector<Point3d>& vertices = surface.vertices;
    Vector3d p3FaceOrigin = vertices[1] - Point3d{};
    boost::optional<Vector3d> newellAreaVector = getNewellVector(vertices);
    if (!newellAreaVector) {
      LOG_AND_THROW("Cannot compute newellAreaVector");
    }
    double pyramidVolume = newellAreaVector->dot(p3FaceOrigin);
    volume += pyramidVolume;
  }
  // for (int NFace = 1; NFace <= Poly.NumSurfaceFaces; ++NFace) {
  //     p3FaceOrigin = Poly.SurfaceFace(NFace).FacePoints(2);
  //     double pyramidVolume = dot(Poly.SurfaceFace(NFace).NewellAreaVector, (p3FaceOrigin - state.dataVectors->p0));
  //     Volume += PyramidVolume / 3.0;
  // }
  return volume / 6.0;  // Our newellArea vector has twice the length
}

double Polyhedron::calcDivergenceTheoremVolume() const {
  double volume = 0.0;
  for (const auto& surface : m_surfaces) {
    const std::vector<Point3d>& vertices = surface.vertices;
    Plane plane(vertices);
    boost::optional<double> area = getArea(vertices);
    if (!area) {
      LOG_AND_THROW("Cannot compute area for " << surface.name);
    }
    volume -= plane.d() * area.get() / 3.0;
  }
  return volume;
}

}  // namespace openstudio
