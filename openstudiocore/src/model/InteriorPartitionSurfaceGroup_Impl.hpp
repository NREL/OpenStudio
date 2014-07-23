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

#ifndef MODEL_INTERIORPARTITIONSURFACEGROUP_IMPL_HPP
#define MODEL_INTERIORPARTITIONSURFACEGROUP_IMPL_HPP

#include "ModelAPI.hpp"
#include "PlanarSurfaceGroup_Impl.hpp"

namespace openstudio {

class Transformation;

namespace model {

class Space;
class InteriorPartitionSurface;
class InteriorPartitionSurfaceGroup;

namespace detail {

  /** InteriorPartitionSurfaceGroup_Impl is a PlanarSurfaceGroup_Impl that is the implementation class for InteriorPartitionSurfaceGroup.*/
  class MODEL_API InteriorPartitionSurfaceGroup_Impl : public PlanarSurfaceGroup_Impl {
    Q_OBJECT;
    Q_PROPERTY(double directionofRelativeNorth READ directionofRelativeNorth WRITE setDirectionofRelativeNorth RESET resetDirectionofRelativeNorth);
    Q_PROPERTY(bool isDirectionofRelativeNorthDefaulted READ isDirectionofRelativeNorthDefaulted);
    Q_PROPERTY(double xOrigin READ xOrigin WRITE setXOrigin RESET resetXOrigin);
    Q_PROPERTY(bool isXOriginDefaulted READ isXOriginDefaulted);
    Q_PROPERTY(double yOrigin READ yOrigin WRITE setYOrigin RESET resetYOrigin);
    Q_PROPERTY(bool isYOriginDefaulted READ isYOriginDefaulted);
    Q_PROPERTY(double zOrigin READ zOrigin WRITE setZOrigin RESET resetZOrigin);
    Q_PROPERTY(bool isZOriginDefaulted READ isZOriginDefaulted);
    Q_PROPERTY(int multiplier READ multiplier WRITE setMultiplier RESET resetMultiplier);
    Q_PROPERTY(bool isMultiplierDefaulted READ isMultiplierDefaulted);

    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> space READ spaceAsModelObject WRITE setSpaceAsModelObject RESET resetSpace);
    Q_PROPERTY(std::vector<openstudio::model::ModelObject> interiorPartitionSurfaces READ interiorPartitionSurfacesAsModelObjects);
   public:
    /** @name Constructors and Destructors */
    //@{

    InteriorPartitionSurfaceGroup_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    InteriorPartitionSurfaceGroup_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                       Model_Impl* model,
                                       bool keepHandle);

    InteriorPartitionSurfaceGroup_Impl(const InteriorPartitionSurfaceGroup_Impl& other,
                                       Model_Impl* model,
                                       bool keepHandle);

    virtual ~InteriorPartitionSurfaceGroup_Impl() {}

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

    // TODO: Handle Non-Extensible IddField Space Name.

    virtual double directionofRelativeNorth() const;

    virtual bool isDirectionofRelativeNorthDefaulted() const;

    virtual double xOrigin() const;

    virtual bool isXOriginDefaulted() const;

    virtual double yOrigin() const;

    virtual bool isYOriginDefaulted() const;

    virtual double zOrigin() const;

    virtual bool isZOriginDefaulted() const;

    int multiplier() const;

    bool isMultiplierDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    virtual void setDirectionofRelativeNorth(double directionofRelativeNorth, bool driverMethod = true);

    virtual void resetDirectionofRelativeNorth();

    virtual void setXOrigin(double xOrigin, bool driverMethod = true);

    virtual void resetXOrigin();

    virtual void setYOrigin(double yOrigin, bool driverMethod = true);

    virtual void resetYOrigin();

    virtual void setZOrigin(double zOrigin, bool driverMethod = true);

    virtual void resetZOrigin();

    bool setMultiplier(int multiplier);

    void resetMultiplier();

    //@}

    /// Returns the parent Space.
    boost::optional<Space> space() const;

    /// Sets the parent Space.
    bool setSpace(const Space& space);

    void resetSpace();

    /// Returns all \link InteriorPartitionSurface InteriorPartitionSurfaces \endlink in this group.
    std::vector<InteriorPartitionSurface> interiorPartitionSurfaces() const;

   protected:
   private:
    REGISTER_LOGGER("openstudio.model.InteriorPartitionSurfaceGroup");

    boost::optional<ModelObject> spaceAsModelObject() const;
    std::vector<ModelObject> interiorPartitionSurfacesAsModelObjects() const;

    bool setSpaceAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_INTERIORPARTITIONSURFACEGROUP_IMPL_HPP

