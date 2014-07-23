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

#ifndef MODEL_SHADINGSURFACEGROUP_IMPL_HPP
#define MODEL_SHADINGSURFACEGROUP_IMPL_HPP

#include "ModelAPI.hpp"
#include "PlanarSurfaceGroup_Impl.hpp"

namespace openstudio {

class Transformation;

namespace model {

class Space;
class ShadingSurface;
class ShadingSurfaceGroup;
class Surface;
class SubSurface;

namespace detail {

  /** ShadingSurfaceGroup_Impl is a PlanarSurfaceGroup_Impl that is the implementation class for ShadingSurfaceGroup.*/
  class MODEL_API ShadingSurfaceGroup_Impl : public PlanarSurfaceGroup_Impl {
    Q_OBJECT;
    Q_PROPERTY(std::string shadingSurfaceType READ shadingSurfaceType WRITE setShadingSurfaceType);
    Q_PROPERTY(double directionofRelativeNorth READ directionofRelativeNorth WRITE setDirectionofRelativeNorth RESET resetDirectionofRelativeNorth);
    Q_PROPERTY(bool isDirectionofRelativeNorthDefaulted READ isDirectionofRelativeNorthDefaulted);
    Q_PROPERTY(double xOrigin READ xOrigin WRITE setXOrigin RESET resetXOrigin);
    Q_PROPERTY(bool isXOriginDefaulted READ isXOriginDefaulted);
    Q_PROPERTY(double yOrigin READ yOrigin WRITE setYOrigin RESET resetYOrigin);
    Q_PROPERTY(bool isYOriginDefaulted READ isYOriginDefaulted);
    Q_PROPERTY(double zOrigin READ zOrigin WRITE setZOrigin RESET resetZOrigin);
    Q_PROPERTY(bool isZOriginDefaulted READ isZOriginDefaulted);

    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> space READ spaceAsModelObject WRITE setSpaceAsModelObject RESET resetSpace);
    Q_PROPERTY(std::vector<openstudio::model::ModelObject> shadingSurfaces READ shadingSurfacesAsModelObjects);
   public:
    /** @name Constructors and Destructors */
    //@{

    ShadingSurfaceGroup_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    ShadingSurfaceGroup_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                             Model_Impl* model,
                             bool keepHandle);

    ShadingSurfaceGroup_Impl(const ShadingSurfaceGroup_Impl& other,
                             Model_Impl* model,
                             bool keepHandle);

    virtual ~ShadingSurfaceGroup_Impl() {}

    //@}

    // return the parent object in the hierarchy
    virtual boost::optional<ParentObject> parent() const;

    // set the parent, child may have to call methods on the parent
    virtual bool setParent(ParentObject& newParent);

    // return any children objects in the hierarchy
    virtual std::vector<ModelObject> children() const;

    /// get a vector of allowable children types
    virtual std::vector<IddObjectType> allowableChildTypes() const;

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual openstudio::Transformation buildingTransformation() const;

    virtual bool changeTransformation(const openstudio::Transformation& transformation);

    virtual openstudio::BoundingBox boundingBox() const;

    /** @name Getters */
    //@{

    std::string shadingSurfaceType() const;

    virtual double directionofRelativeNorth() const;

    virtual bool isDirectionofRelativeNorthDefaulted() const;

    virtual double xOrigin() const;

    virtual bool isXOriginDefaulted() const;

    virtual double yOrigin() const;

    virtual bool isYOriginDefaulted() const;

    virtual double zOrigin() const;

    virtual bool isZOriginDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setShadingSurfaceType(std::string shadingSurfaceType);

    virtual void setDirectionofRelativeNorth(double directionofRelativeNorth, bool driverMethod = true);

    virtual void resetDirectionofRelativeNorth();

    virtual void setXOrigin(double xOrigin, bool driverMethod = true);

    virtual void resetXOrigin();

    virtual void setYOrigin(double yOrigin, bool driverMethod = true);

    virtual void resetYOrigin();

    virtual void setZOrigin(double zOrigin, bool driverMethod = true);

    virtual void resetZOrigin();

    //@}

    /// Returns the parent Space.
    boost::optional<Space> space() const;

    /// Sets the parent Space.
    bool setSpace(const Space& space);

    /** If (space()), calls setShadingSurfaceType("Building") to remove the relationship. */
    void resetSpace();

    /// Returns child shading surfaces.
    std::vector<ShadingSurface> shadingSurfaces() const;

    /** Returns the specific sub surface shaded by this group if specified. */
    boost::optional<SubSurface> shadedSubSurface() const;

    /** Returns the surface shaded by this group if specified. */
    boost::optional<Surface> shadedSurface() const;

    /** Sets the specifc sub surface shaded by this group, sub surface must be in the same parent space. */
    bool setShadedSubSurface(const SubSurface& subSurface);

    /** Sets the surface shaded by this group, surface must be in the same parent space. */
    bool setShadedSurface(const Surface& surface);

    /** Resets the sub surface shaded by this group. */
    void resetShadedObject();

   protected:

   private:
    REGISTER_LOGGER("openstudio.model.ShadingSurfaceGroup");

    boost::optional<ModelObject> spaceAsModelObject() const;
    std::vector<ModelObject> shadingSurfacesAsModelObjects() const;

    bool setSpaceAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_SHADINGSURFACEGROUP_IMPL_HPP

