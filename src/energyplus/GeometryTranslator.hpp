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

  // clang-format off

  /** \class StartingVertexPosition
   *
   *  \relates GeometryTranslator */
  OPENSTUDIO_ENUM(StartingVertexPosition,
    ((UpperLeftCorner))
    ((LowerLeftCorner))
    ((UpperRightCorner))
    ((LowerRightCorner))
  );

  /** \class VertexEntryDirection
   *
   *  \relates GeometryTranslator */
  OPENSTUDIO_ENUM(VertexEntryDirection,
    ((Counterclockwise))
    ((Clockwise))
  );

  /** \class CoordinateSystem
   *
   *  \relates GeometryTranslator */
  OPENSTUDIO_ENUM(CoordinateSystem,
    ((Relative))
    ((Absolute)(World))
  );

  /** \class CoordinateChange
   *
   *  \relates GeometryTranslator */
  OPENSTUDIO_ENUM(CoordinateChange,
    ((RelativeToAbsolute))
    ((AbsoluteToRelative))
    ((NoChange))
  );

  // clang-format on

  /// test equality of coordinate systems
  ENERGYPLUS_API bool equal(const CoordinateSystem& left, const CoordinateSystem& right);

  /* Translates EnergyPlus simulation input geometry between various coordinate systems and
*  changes simple geometry to detailed geometry.
*
*  All geometry in the OpenStudio Building Model is expressed in relative coordinates using
*  the UpperLeftCorner, CounterClockWise convention.
*
*/
  class ENERGYPLUS_API GeometryTranslator
  {
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
    struct GlobalGeometryRules
    {
      StartingVertexPosition svp;
      VertexEntryDirection ved;
      CoordinateSystem detailedSystem;
      CoordinateSystem daylightingSystem;
      CoordinateSystem rectangularSystem;
    };

    // get the current GlobalGeometryRules
    GlobalGeometryRules globalGeometryRules() const;

    // set the GlobalGeometryRules, only changes the object does not transform geometry
    bool setGlobalGeometryRules(const StartingVertexPosition& svp, const VertexEntryDirection& ved, const CoordinateSystem& detailedSystem,
                                const CoordinateSystem& daylightingSystem, const CoordinateSystem& rectangularSystem);

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
  ENERGYPLUS_API openstudio::Point3dVector verticesForAzimuthTiltXYZLengthWidthOrHeight(double azimuth, double tilt, double x0, double y0, double z0,
                                                                                        double length, double widthOrHeight);

}  // namespace energyplus
}  // namespace openstudio

#endif  // ENERGYPLUS_GEOMETRYTRANSLATOR_HPP
