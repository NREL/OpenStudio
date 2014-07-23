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

#ifndef MODEL_PLANARSURFACEGROUP_IMPL_HPP
#define MODEL_PLANARSURFACEGROUP_IMPL_HPP

#include "ParentObject_Impl.hpp"

#include "../utilities/geometry/Transformation.hpp"

namespace openstudio {

class Point3d;
class BoundingBox;

namespace model {
namespace detail {

  class MODEL_API PlanarSurfaceGroup_Impl : public ParentObject_Impl {
    Q_OBJECT;
   public:

    /** @name Constructors and Destructors */
    //@{

    // constructor
    PlanarSurfaceGroup_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    // construct from workspace
    PlanarSurfaceGroup_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                Model_Impl* model,
                bool keepHandle);

    PlanarSurfaceGroup_Impl(const PlanarSurfaceGroup_Impl& other, Model_Impl* model,bool keepHandle);

    // virtual destructor
    virtual ~PlanarSurfaceGroup_Impl(){}

    virtual double directionofRelativeNorth() const = 0;

    virtual bool isDirectionofRelativeNorthDefaulted() const = 0;

    virtual double xOrigin() const = 0;

    virtual bool isXOriginDefaulted() const = 0;

    virtual double yOrigin() const = 0;

    virtual bool isYOriginDefaulted() const = 0;

    virtual double zOrigin() const = 0;

    virtual bool isZOriginDefaulted() const = 0;
  
    virtual void setDirectionofRelativeNorth(double directionofRelativeNorth, bool driverMethod = true) = 0;

    virtual void resetDirectionofRelativeNorth() = 0;

    virtual void setXOrigin(double xOrigin, bool driverMethod = true) = 0;

    virtual void resetXOrigin() = 0;

    virtual void setYOrigin(double yOrigin, bool driverMethod = true) = 0;

    virtual void resetYOrigin() = 0;

    virtual void setZOrigin(double zOrigin, bool driverMethod = true) = 0;

    virtual void resetZOrigin() = 0;

    /** Returns the transformation from local coordinates to parent coordinates. */
    openstudio::Transformation transformation() const;

    /** Returns the transformation from local coordinates to site coordinates. */
    openstudio::Transformation siteTransformation() const;

    /** Returns the transformation from local coordinates to building coordinates. */
    virtual openstudio::Transformation buildingTransformation() const = 0;

    /** Sets the transformation from local coordinates to parent coordinates,
     *  this method can be used to move the group. */
    bool setTransformation(const openstudio::Transformation& transformation);

    /** Changes the transformation from local coordinates to parent coordinates,
     *  this method alter geometry of children relative to the
     *  group so that it stays in the same place with the new transformation. */
    virtual bool changeTransformation(const openstudio::Transformation& transformation) = 0;

    /** Get the BoundingBox in local coordinates. */
    virtual openstudio::BoundingBox boundingBox() const = 0;

    //@}
   private slots:

    void clearCachedVariables();

   private:
    REGISTER_LOGGER("openstudio.model.PlanarSurfaceGroup");

    mutable boost::optional<openstudio::Transformation> m_cachedTransformation;

  };

} // detail
} // model
} // openstudio

#endif // MODEL_PLANARSURFACEGROUP_IMPL_HPP
