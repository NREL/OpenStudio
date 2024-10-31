/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "Polyhedron.hpp"
#include "Polygon3d.hpp"

#include "Geometry.hpp"
#include "Intersection.hpp"
#include "Plane.hpp"
#include "Point3d.hpp"
#include "Transformation.hpp"
#include "Vector3d.hpp"

#include <fmt/core.h>

#include <algorithm>
#include <numeric>
#include <utility>
#include <vector>

namespace openstudio {

Surface3dEdge::Surface3dEdge(Point3d start, Point3d end, const Surface3d& firstSurface)
  : m_start(std::move(start)), m_end(std::move(end)), m_firstSurfaceName(firstSurface.name) {
  m_allSurfNums.push_back(firstSurface.surfNum);
}

Surface3dEdge::Surface3dEdge(Point3d start, Point3d end, std::string t_name, size_t t_surfNum)
  : m_start(std::move(start)), m_end(std::move(end)), m_firstSurfaceName(std::move(t_name)) {
  m_allSurfNums.push_back(t_surfNum);
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

const Point3d& Surface3dEdge::start() const {
  return m_start;
}
const Point3d& Surface3dEdge::end() const {
  return m_end;
}

size_t Surface3dEdge::count() const {
  return m_allSurfNums.size();
}

size_t Surface3dEdge::firstSurfNum() const {
  return m_allSurfNums.front();
}

std::string Surface3dEdge::firstSurfaceName() const {
  return m_firstSurfaceName;
}

const std::vector<size_t>& Surface3dEdge::allSurfNums() const {
  return m_allSurfNums;
}

void Surface3dEdge::markConflictedOrientation() {
  m_conflictedOrientation = true;
}

bool Surface3dEdge::hasConflictedOrientation() const {
  return m_conflictedOrientation;
}

void Surface3dEdge::markCreated() {
  m_hasBeenCreated = true;
}

bool Surface3dEdge::hasBeenCreated() const {
  return m_hasBeenCreated;
}

void Surface3dEdge::appendSurface(const Surface3d& surface) {
  m_allSurfNums.emplace_back(surface.surfNum);
}

void Surface3dEdge::resetEdgeMatching() {
  m_allSurfNums.erase(std::next(m_allSurfNums.begin()), m_allSurfNums.end());
  m_conflictedOrientation = false;
}

bool Surface3dEdge::containsPoint(const Point3d& testVertex) const {
  return !isAlmostEqual3dPt(m_start, testVertex) && !isAlmostEqual3dPt(m_end, testVertex) && isPointOnLineBetweenPoints(m_start, m_end, testVertex);
}

Vector3d Surface3dEdge::asVector() const {
  return m_end - m_start;
}

std::ostream& operator<<(std::ostream& os, const Surface3dEdge& edge) {
  os << "Surface3dEdge: start=" << edge.start() << ", end=" << edge.end() << ", count=" << edge.count() << ", firstSurface='"
     << edge.firstSurfaceName() << "'";
  return os;
}

Surface3d::Surface3d(std::vector<Point3d> t_vertices, std::string t_name, size_t t_surfNum)
  : vertices(std::move(t_vertices)), name(std::move(t_name)), surfNum(t_surfNum) {

  edges.reserve(vertices.size());
  for (auto it = vertices.begin(); it != vertices.end(); ++it) {

    auto itnext = std::next(it);
    if (itnext == std::end(vertices)) {
      itnext = std::begin(vertices);
    }

    edges.emplace_back(*it, *itnext, name, surfNum);
  }
}

std::ostream& operator<<(std::ostream& os, const Surface3d& surface3d) {
  os << "Surface3d ";
  if (!surface3d.name.empty()) {
    os << "'" << surface3d.name << "' ";
  }
  os << "= [\n";
  for (const auto& pt : surface3d.vertices) {
    os << "  " << pt << ",\n";
  }
  os << "]";
  return os;
}

bool Surface3d::operator<(const Surface3d& rhs) const {
  return this->name < rhs.name;
}

size_t Surface3d::numConflictedEdges() const {
  return std::count_if(edges.cbegin(), edges.cend(), [](const auto& edge) { return edge.hasConflictedOrientation(); });
}

double Surface3d::ratioOfConflictedEdges() const {
  return static_cast<double>(numConflictedEdges()) / static_cast<double>(edges.size());
}

void Surface3d::resetEdgeMatching() {
  for (auto& edge : edges) {
    edge.resetEdgeMatching();
  }
}

bool Surface3d::isConvex() const {

  // for each triangle of 3 points, we compute the triangleNormal by taking the cross product of AB x AC.
  // We then take the dot product with the initial outwardNormal. If the dot product is negative, this means the triangleNomal is pointing in the
  // opposite direction and we therefore have a non convex surface

  // const auto& a = edges.front().start();
  // const auto& b = edges.front().end();
  // const auto& c = edges.at(1).end();

  auto ab1 = edges.front().asVector();
  auto bc1 = edges[1].asVector();
  auto outwardNormal = ab1.cross(bc1);
  outwardNormal.normalize();

  for (auto it = std::next(edges.begin()); it != edges.end(); ++it) {
    auto itnext = std::next(it);
    if (itnext == std::end(edges)) {
      itnext = std::begin(edges);
    }
    const Point3d& a = it->start();
    const Point3d& b = it->end();
    const Point3d& c = itnext->end();

    auto ab = b - a;
    auto ac = c - a;

    auto triangleNormal = ab.cross(ac);
    auto d = outwardNormal.dot(triangleNormal);
    if (d < 0) {
      return false;
    }
  }
  return true;
}

Polyhedron::Polyhedron(std::vector<Surface3d> surfaces) : m_surfaces(std::move(surfaces)) {
  performEdgeMatching();

  // if all edges had two counts then it is fully enclosed
  if (!hasEdgesNot2()) {
    m_isEnclosedVolume = true;
  } else {
    LOG(Warn, "Polyhedron is not enclosed in original testing. Trying to add missing colinear points.");
    updateZonePolygonsForMissingColinearPoints();
  }
  if (!m_isEnclosedVolume) {
    LOG(Warn, "Polyhedron is not enclosed.");
  } else {
    // We call the volume calculation... if this ends up being negative, we're in the case where ALL surfaces are in the wrong orientation
    if (!m_hasAnySurfaceWithIncorrectOrientation) {
      m_polyhedronVolume = calcPolyhedronVolume();
      if (m_polyhedronVolume < 0) {
        LOG(Error, "It seems that ALL surfaces are reversed");
        m_hasAnySurfaceWithIncorrectOrientation = true;
        m_isCompletelyInsideOut = true;
      }
    }
  }
};

bool Polyhedron::hasEdgesNot2() const {
  return std::any_of(m_surfaces.cbegin(), m_surfaces.cend(), [](const auto& surface) {
    return std::any_of(surface.edges.cbegin(), surface.edges.cend(), [](const auto& edge) { return edge.count() != 2; });
  });
}

bool Polyhedron::hasAnySurfaceWithIncorrectOrientation() const {
  return m_hasAnySurfaceWithIncorrectOrientation;
}

bool Polyhedron::hasAddedColinearPoints() const {
  return m_hasAddedColinearPoints;
}

bool Polyhedron::isEnclosedVolume() const {
  return m_isEnclosedVolume;
}

bool Polyhedron::isCompletelyInsideOut() const {
  return m_isCompletelyInsideOut;
}

void Polyhedron::performEdgeMatching() {

  m_hasAnySurfaceWithIncorrectOrientation = false;

  // We use **Combinations** (rather than Permutations) to avoid traversing unnecessarily
  for (size_t i = 0; i < m_surfaces.size(); ++i) {
    for (size_t j = i + 1; j < m_surfaces.size(); ++j) {
      auto& surface1 = m_surfaces[i];
      auto& surface2 = m_surfaces[j];
      for (Surface3dEdge& edge1 : surface1.edges) {
        for (Surface3dEdge& edge2 : surface2.edges) {
          if (edge1 == edge2) {
            if (std::find(edge1.allSurfNums().begin(), edge1.allSurfNums().cend(), edge2.firstSurfNum()) == edge1.allSurfNums().end()) {
              // appendSurface will allow use to check edge.count() later to check if count == 2.
              // All edges must be count == 2 in an Enclosed Polyhedron
              edge1.appendSurface(surface2);
              edge2.appendSurface(surface1);
              // In a Polyhedron that has a consistent orientation (typically all faces are in counter clockwise order),
              // each edge must be matched by an edge in the **opposite** direction. If not, mark conflicted
              if (!edge1.reverseEqual(edge2)) {
                edge1.markConflictedOrientation();
                edge2.markConflictedOrientation();
                m_hasAnySurfaceWithIncorrectOrientation = true;
              }
            }
          }
        }
      }
    }
  }

  // #5002 - special case to find edges that are used to "cut" in to a surface to remove an interior hole
  // we allow these edges to double count the first surface since they bound the same surface on two sides
  for (auto& surface : m_surfaces) {
    auto& edges = surface.edges;
    for (size_t i = 0; i < edges.size(); ++i) {
      for (size_t j = i + 1; j < edges.size(); ++j) {
        if ((edges[i].count() == 1) && (edges[j].count() == 1) && (edges[i] == edges[j]) && edges[i].reverseEqual(edges[j])) {
          // appendSurface will allow use to check edge.count() later to check if count == 2.
          // All edges must be count == 2 in an Enclosed Polyhedron
          edges[i].appendSurface(surface);
          edges[j].appendSurface(surface);
        }
      }
    }
  }
}

void Polyhedron::resetEdgeMatching() {
  for (auto& surface : m_surfaces) {
    surface.resetEdgeMatching();
  }
  m_hasAnySurfaceWithIncorrectOrientation = false;
}

void Polyhedron::updateZonePolygonsForMissingColinearPoints() {
  const std::vector<Point3d> uniqVertices = uniqueVertices();

  bool anyInserted = false;

  for (auto& surface : m_surfaces) {
    LOG(Debug, surface.name)
    bool insertedVertext = true;
    while (insertedVertext) {
      insertedVertext = false;

      for (auto it = surface.edges.begin(); it != surface.edges.end(); ++it) {

        // now go through all the vertices and see if they are colinear with start and end vertices
        for (const auto& testVertex : uniqVertices) {
          if (const boost::optional<Surface3dEdge> newEdge = it->splitEdge(testVertex)) {
            LOG(Debug, testVertex << " is on " << *it);
            auto itnext = std::next(it);
            if (itnext == std::end(surface.edges)) {
              itnext = std::begin(surface.edges);
            }
            auto before_pos = std::distance(surface.edges.begin(), itnext);
            surface.edges.insert(itnext, newEdge.get());
            surface.vertices.insert(std::next(surface.vertices.begin(), before_pos), testVertex);

            insertedVertext = true;
            anyInserted = true;
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

  if (anyInserted) {
    resetEdgeMatching();
    performEdgeMatching();
    m_hasAddedColinearPoints = true;
    m_isEnclosedVolume = !hasEdgesNot2();
  }
}

size_t Polyhedron::numVertices() const {
  return std::accumulate(m_surfaces.cbegin(), m_surfaces.cend(), 0, [](size_t sum, const auto& surface) { return sum + surface.edges.size(); });
}

std::vector<Point3d> Polyhedron::uniqueVertices() const {

  std::vector<Point3d> uniqVertices;
  uniqVertices.reserve(numVertices());

  for (const auto& surface : m_surfaces) {
    for (const auto& edge : surface.edges) {
      const auto& pt = edge.start();
      if (std::find_if(uniqVertices.cbegin(), uniqVertices.cend(), [&pt](const auto& unqV) { return isAlmostEqual3dPt(pt, unqV); })
          == uniqVertices.cend()) {
        uniqVertices.push_back(pt);
      }
    }
  }
  return uniqVertices;
}

std::vector<Surface3dEdge> Polyhedron::uniqueEdges() const {

  std::vector<Surface3dEdge> uniqueSurface3dEdges;
  uniqueSurface3dEdges.reserve(numVertices());

  // construct list of unique edges
  for (const auto& surface : m_surfaces) {
    for (const Surface3dEdge& thisSurface3dEdge : surface.edges) {
      auto itFound = std::find(uniqueSurface3dEdges.begin(), uniqueSurface3dEdges.end(), thisSurface3dEdge);
      if (itFound == uniqueSurface3dEdges.end()) {
        uniqueSurface3dEdges.push_back(thisSurface3dEdge);
      }
    }
  }

  return uniqueSurface3dEdges;
}

std::vector<Surface3dEdge> Polyhedron::edgesNotTwo(bool includeCreatedEdges) const {

  std::vector<Surface3dEdge> edgesNotTwo;
  edgesNotTwo.reserve(numVertices());

  // All edges for an enclosed polyhedron should be shared by two (and only two) side
  for (const auto& surface : m_surfaces) {
    for (const Surface3dEdge& thisSurface3dEdge : surface.edges) {
      // only return a list of those edges that appear in both the original edge and the revised edges:
      // this eliminates added edges that will confuse users (edges that were caught by the updateZonePolygonsForMissingColinearPoints routine)
      if ((thisSurface3dEdge.count() != 2) && (includeCreatedEdges || !thisSurface3dEdge.hasBeenCreated())) {
        auto itFound = std::find(edgesNotTwo.begin(), edgesNotTwo.end(), thisSurface3dEdge);
        if (itFound == edgesNotTwo.end()) {
          edgesNotTwo.push_back(thisSurface3dEdge);
        }
      }
    }
  }
  for (const auto& edge : edgesNotTwo) {
    LOG(Debug, edge);
  }

  return edgesNotTwo;
}

boost::optional<Surface3dEdge> Surface3dEdge::splitEdge(Point3d testVertex) {
  if (containsPoint(testVertex)) {
    Surface3dEdge newEdge(*this);
    this->m_end = testVertex;
    newEdge.m_start = std::move(testVertex);
    this->markCreated();
    newEdge.markCreated();
    return newEdge;
  }

  return boost::none;
}

std::vector<Surface3d> Polyhedron::findSurfacesWithIncorrectOrientation() const {

  if (!m_hasAnySurfaceWithIncorrectOrientation) {
    return {};
  }

  if (m_isCompletelyInsideOut) {
    LOG(Error, "It seems that ALL surfaces are reversed");
    return m_surfaces;
  }

  if (!m_isEnclosedVolume) {
    LOG(Warn, "Polyhedron is not enclosed. Looking surfaces with incorrect orientations can return false negatives.");
    //  return {};
  }

  std::vector<Surface3dEdge> uniqueSurface3dEdges = uniqueEdges();

  // Remove non-conflicted edges (Note: this also removes any edge that doesn't have at least one match... since these can't be conflicted)
  uniqueSurface3dEdges.erase(
    std::remove_if(uniqueSurface3dEdges.begin(), uniqueSurface3dEdges.end(), [](const auto& edge) { return !edge.hasConflictedOrientation(); }),
    uniqueSurface3dEdges.end());
  if (uniqueSurface3dEdges.empty()) {
    return {};
  }

  std::set<Surface3d> conflictedSurfaces;
  for (const auto& edge : uniqueSurface3dEdges) {
    const auto& surfNums = edge.allSurfNums();
    const size_t sfIndex1 = surfNums.front();
    const size_t sfIndex2 = surfNums.back();

    const auto& sf1 = m_surfaces.at(sfIndex1);
    const auto& sf2 = m_surfaces.at(sfIndex2);

    const auto c1 = sf1.ratioOfConflictedEdges();
    const auto c2 = sf2.ratioOfConflictedEdges();

    LOG(Trace, fmt::format("'{}' has {:.2f}% conflicted edges ({}/{}), while '{}' has {:.2f}% conflicted edges ({}/{})", sf1.name, c1,
                           sf1.numConflictedEdges(), sf1.edges.size(), sf2.name, c2, sf2.numConflictedEdges(), sf2.edges.size()));
    if (c1 > c2) {
      conflictedSurfaces.insert(sf1);
    } else {
      conflictedSurfaces.insert(sf2);
    }
  }

  return {conflictedSurfaces.begin(), conflictedSurfaces.end()};
}

// boost::optional<double> Polyhedron::calculatedVolume() const {
//   if (m_m_isEnclosedVolume) {
//     return calcPolyhedronVolume();
//   }
//   return boost::none;
// }

double Polyhedron::calcPolyhedronVolume() const {

  const Point3d p0{};
  double volume = 0.0;
  for (const auto& surface : m_surfaces) {
    const std::vector<Point3d>& vertices = surface.vertices;
    const Vector3d p3FaceOrigin = vertices[1] - p0;
    boost::optional<Vector3d> newellAreaVector = getNewellVector(vertices);
    if (!newellAreaVector) {
      LOG_AND_THROW("Cannot compute newellAreaVector");
    }
    const double pyramidVolume = newellAreaVector->dot(p3FaceOrigin);
    volume += pyramidVolume;
  }
  // for (int NFace = 1; NFace <= Poly.NumSurfaceFaces; ++NFace) {
  //     p3FaceOrigin = Poly.SurfaceFace(NFace).FacePoints(2);
  //     double pyramidVolume = dot(Poly.SurfaceFace(NFace).NewellAreaVector, (p3FaceOrigin - state.dataVectors->p0));
  //     Volume += PyramidVolume / 3.0;
  // }
  return volume / 6.0;  // Our newellArea vector has twice the length
}

double Polyhedron::polyhedronVolume() const {
  if (m_isEnclosedVolume && (m_isCompletelyInsideOut || !m_hasAnySurfaceWithIncorrectOrientation)) {
    return m_polyhedronVolume;
  }
  if (!m_isEnclosedVolume) {
    LOG(Error, "Polyhedron volume calculation for a non-enclosed Polyhedron will return bogus values");
  } else if (m_hasAnySurfaceWithIncorrectOrientation && !m_isCompletelyInsideOut) {
    LOG(Error, "Polyhedron volume calculation for an enclosed Polyhedron but with incorrectly oriented surfaces will return bogus values");
  }
  return calcPolyhedronVolume();
}

double Polyhedron::calcDivergenceTheoremVolume() const {
  double volume = 0.0;
  for (const auto& surface : m_surfaces) {
    const std::vector<Point3d>& vertices = surface.vertices;
    const Plane plane(vertices);
    boost::optional<double> area = getArea(vertices);
    if (!area) {
      LOG_AND_THROW("Cannot compute area for " << surface.name);
    }
    volume -= plane.d() * area.get() / 3.0;
  }
  return volume;
}

std::vector<Surface3d> Polyhedron::surface3ds() const {
  return m_surfaces;
}

}  // namespace openstudio
