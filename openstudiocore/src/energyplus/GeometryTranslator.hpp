/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
*  All rights reserved.
*
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#ifndef ENERGYPLUS_GEOMETRYTRANSLATOR_HPP
#define ENERGYPLUS_GEOMETRYTRANSLATOR_HPP

#include "EnergyPlusAPI.hpp"

#include "../utilities/idf/Workspace.hpp"
#include "../utilities/core/Logger.hpp"
#include "../utilities/core/Enum.hpp"
#include "../utilities/geometry/Transformation.hpp"
#include "../utilities/geometry/Point3d.hpp"

namespace openstudio {
namespace energyplus {

/** \class StartingVertexPosition 
 *
 *  \relates GeometryTranslator */
OPENSTUDIO_ENUM(StartingVertexPosition,
  ((UpperLeftCorner))
  ((LowerLeftCorner))
  ((UpperRightCorner))
  ((LowerRightCorner)));

/** \class VertexEntryDirection 
 *
 *  \relates GeometryTranslator */
OPENSTUDIO_ENUM(VertexEntryDirection,
  ((Counterclockwise))
  ((Clockwise)));

/** \class CoordinateSystem 
 *
 *  \relates GeometryTranslator */
OPENSTUDIO_ENUM(CoordinateSystem,
  ((Relative))
  ((World))
  ((Absolute)));

/** \class CoordinateChange 
 *
 *  \relates GeometryTranslator */
OPENSTUDIO_ENUM(CoordinateChange,
  ((RelativeToAbsolute))
  ((AbsoluteToRelative))
  ((NoChange)));

/// test equality of coordinate systems
ENERGYPLUS_API bool equal(const CoordinateSystem& left, const CoordinateSystem& right);

/* Translates EnergyPlus simulation input geometry between various coordinate systems and
*  changes simple geometry to detailed geometry.
*
*  All geometry in the OpenStudio Building Model is expressed in relative coordinates using
*  the UpperLeftCorner, CounterClockWise convention.
*
*/
class ENERGYPLUS_API GeometryTranslator {
 public:

  /// constructor with an EnergyPlus Workspace
  GeometryTranslator(const openstudio::Workspace& workspace);

  /// convert workspace to given coordinate systems
  /// all geometry will be converted to upper-left-corner, counterclockwise
  /// all simple geometry will be converted to detailed geometry
  bool convert(const CoordinateSystem& detailedSystem, const CoordinateSystem& daylightingSystem);

 private:

   REGISTER_LOGGER("openstudio.energyplus.GeometryTranslator");

  // private struct to hold GlobalGeometryRules
  struct GlobalGeometryRules{
    StartingVertexPosition svp;
    VertexEntryDirection ved;
    CoordinateSystem detailedSystem;
    CoordinateSystem daylightingSystem;
    CoordinateSystem rectangularSystem;
  };

  // get the current GlobalGeometryRules
  GlobalGeometryRules globalGeometryRules() const;

  // set the GlobalGeometryRules, only changes the object does not transform geometry
  bool setGlobalGeometryRules(const StartingVertexPosition& svp, const VertexEntryDirection& ved,
                              const CoordinateSystem& detailedSystem,
                              const CoordinateSystem& daylightingSystem,
                              const CoordinateSystem& rectangularSystem);

  // get the transformation from building to world
  Transformation buildingTransformation() const;

  // get the transformation from zone to building
  Transformation zoneTransformation(const WorkspaceObject& zone) const;

  // convert simple shading to detailed in the current system
  bool convertSimpleShading(const CoordinateChange& coordChange);

  // convert simple subsurfaces to detailed in the current system
  bool convertSimpleSubSurfaces();

  // convert simple surfaces to detailed in the current system
  bool convertSimpleSurfaces(const CoordinateChange& coordChange);

  // convert daylighting geometry from the current system to the new system
  bool convertDaylightingGeometry(const CoordinateChange& daylightingCoordChange);

  // convert detailed geometry from the current system to the new system
  bool convertDetailedGeometry(const CoordinateChange& detailedCoordChange);

  // reverse all detailed vertices
  bool reverseAllDetailedVertices();

  // apply upper left corner rule to all detailed vertices
  bool applyUpperLeftCornerRule();

  openstudio::Workspace m_workspace;

};

/** Get vertices for a surface. \relates GeometryTranslator */
ENERGYPLUS_API openstudio::Point3dVector getVertices(unsigned firstVertex, const IdfObject& surface);

/** Set vertices for a surface, only detailed surfaces are supported. \relates GeometryTranslator */
ENERGYPLUS_API bool setVertices(unsigned firstVertex, WorkspaceObject& surface, const openstudio::Point3dVector& vertices);

/** Convert azimuth, tilt, starting x, y, z, length, and width to vertices
 *  azimith and tilt are in degrees. \relates GeometryTranslator */
ENERGYPLUS_API openstudio::Point3dVector verticesForAzimuthTiltXYZLengthWidthOrHeight(
    double azimuth, double tilt, double x0, double y0, double z0, double length, double widthOrHeight);

} // energyplus
} // openstudio

#endif // ENERGYPLUS_GEOMETRYTRANSLATOR_HPP
