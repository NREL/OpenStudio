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

#ifndef MODEL_PLANARSURFACEGROUP_HPP
#define MODEL_PLANARSURFACEGROUP_HPP

#include "ModelAPI.hpp"
#include "OpaqueMaterial.hpp"

namespace openstudio {

class Transformation;
class Point3d;
class BoundingBox;

namespace model {

  namespace detail {
    class PlanarSurfaceGroup_Impl;
  }

  /** PlanarSurfaceGroup is an abstract class representing a group that holds many PlanarSurfaces.
 */
  class MODEL_API PlanarSurfaceGroup : public ParentObject
  {
   public:
    virtual ~PlanarSurfaceGroup() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    PlanarSurfaceGroup(const PlanarSurfaceGroup& other) = default;
    PlanarSurfaceGroup(PlanarSurfaceGroup&& other) = default;
    PlanarSurfaceGroup& operator=(const PlanarSurfaceGroup&) = default;
    PlanarSurfaceGroup& operator=(PlanarSurfaceGroup&&) = default;

    /** @name Getters */
    //@{

    double directionofRelativeNorth() const;

    bool isDirectionofRelativeNorthDefaulted() const;

    double xOrigin() const;

    bool isXOriginDefaulted() const;

    double yOrigin() const;

    bool isYOriginDefaulted() const;

    double zOrigin() const;

    bool isZOriginDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setDirectionofRelativeNorth(double directionofRelativeNorth);

    void resetDirectionofRelativeNorth();

    bool setXOrigin(double xOrigin);

    void resetXOrigin();

    bool setYOrigin(double yOrigin);

    void resetYOrigin();

    bool setZOrigin(double zOrigin);

    void resetZOrigin();

    //@}

    /** Returns the transformation from local coordinates to parent coordinates. */
    openstudio::Transformation transformation() const;

    /** Returns the transformation from local coordinates to building coordinates. */
    openstudio::Transformation buildingTransformation() const;

    /** Returns the transformation from local coordinates to site coordinates. */
    openstudio::Transformation siteTransformation() const;

    /** Sets the transformation from local coordinates to parent coordinates,
   *  this method can be used to move the group. */
    bool setTransformation(const openstudio::Transformation& transformation);

    /** Changes the transformation from local coordinates to parent coordinates,
   *  this method alter geometry of children relative to the
   *  group so that it stays in the same place with the new transformation. */
    bool changeTransformation(const openstudio::Transformation& transformation);

    /** Get the BoundingBox in local coordinates. */
    openstudio::BoundingBox boundingBox() const;

    /** Get the BoundingBox in parent coordinates. */
    openstudio::BoundingBox boundingBoxParentCoordinates() const;

    /** Get the BoundingBox in building coordinates. */
    openstudio::BoundingBox boundingBoxBuildingCoordinates() const;

    /** Get the BoundingBox in site coordinates. */
    openstudio::BoundingBox boundingBoxSiteCoordinates() const;

   protected:
    /** @name Constructors and Destructors */
    //@{

    explicit PlanarSurfaceGroup(IddObjectType iddObjectType, const Model& model);

    //@}

    /// @cond
    using ImplType = detail::PlanarSurfaceGroup_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class detail::PlanarSurfaceGroup_Impl;

    explicit PlanarSurfaceGroup(std::shared_ptr<detail::PlanarSurfaceGroup_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.PlanarSurfaceGroup");
  };

  /** \relates PlanarSurfaceGroup */
  using OptionalPlanarSurfaceGroup = boost::optional<PlanarSurfaceGroup>;

  /** \relates PlanarSurfaceGroup */
  using PlanarSurfaceGroupVector = std::vector<PlanarSurfaceGroup>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PLANARSURFACEGROUP_HPP
