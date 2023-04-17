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

#ifndef UTILITIES_GEOMETRY_POLYHEDRON_HPP
#define UTILITIES_GEOMETRY_POLYHEDRON_HPP

#include "../UtilitiesAPI.hpp"
#include "Point3d.hpp"

#include <vector>
#include <boost/optional.hpp>

namespace openstudio {

namespace model {
  class Space;
  class ThermalZone;
}  // namespace model

// A thin wrapper to improve reporting, by attaching a name to the vertices
class UTILITIES_API Surface3d
{
 public:
  Surface3d() = default;
  Surface3d(std::vector<Point3d> t_vertices, std::string t_name = "None");
  std::vector<Point3d> vertices;
  std::string name;
};

class UTILITIES_API Surface3dEdge
{
 public:
  Surface3dEdge(Point3d start, Point3d end, Surface3d firstSurface, size_t firstSurfNum);

  Point3d start() const;
  Point3d end() const;

  /// check equality: this uses a tolerance
  bool operator==(const Surface3dEdge& other) const;

  /// check inequality
  bool operator!=(const Surface3dEdge& other) const;

  size_t count() const;

  size_t firstSurfNum() const;
  void appendSurface(Surface3d surface);

  // Checks whether a Point: is not almost equal to the start and end points, and that isPointOnLineBetweenPoints(start, end, testVertex) is true
  bool containsPoint(const Point3d& testVertex);

  std::vector<Surface3d> allSurfaces() const;

 private:
  Point3d m_start;
  Point3d m_end;
  size_t m_firstSurfNum;
  std::vector<Surface3d> m_allSurfaces;
};

using Surface3dEdgeVector = std::vector<Surface3dEdge>;

class VolumeEnclosedReturnType
{
 public:
  VolumeEnclosedReturnType() = default;

  bool isEnclosedVolume = false;
  Surface3dEdgeVector edgesNot2;
};

class UTILITIES_API Polyhedron
{
 public:
  Polyhedron() = default;  // Default ctor, for swig

  Polyhedron(std::vector<Surface3d> surfaces);

  // test if the volume described by the polyhedron if full enclosed (would not leak)
  // This is done by checking that every edge is used exactly TWICE.
  VolumeEnclosedReturnType isEnclosedVolume() const;

  // Maybe unused
  std::vector<Point3d> uniqueVertices() const;

  static std::vector<Surface3dEdge> edgesNotTwoForEnclosedVolumeTest(const Polyhedron& volumePoly);

  Polyhedron updateZonePolygonsForMissingColinearPoints() const;

  size_t numVertices() const;

  // Call calculatedVolume as it will handle the logic to dispatch to inidivual methods based on checks
  // Note JM 2022-05-23: Actually a lot of the needed logic is in PlanarSurface,
  // so I don't think it's worth trying to fallback to the other calculation modes E+ does (like checking if floor and roof are same and use that etc)
  // double calculatedVolume() const;

  // Polyhedron MUST be enclosed
  double calcPolyhedronVolume() const;
  // Polyhedron MUST be enclosed
  double calcDivergenceTheoremVolume() const;

 private:
  REGISTER_LOGGER("utilities.Polyhedron");

  std::vector<Surface3d> m_surfaces;
};

using PolyhedronVector = std::vector<Polyhedron>;
using Surface3dVector = std::vector<Surface3d>;

/// ostream operator
UTILITIES_API std::ostream& operator<<(std::ostream& os, const Surface3dEdge& edge);

}  // namespace openstudio

#endif  // UTILITIES_GEOMETRY_POLYHEDRON_HPP
