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

#ifndef MODEL_INTERIORPARTITIONSURFACE_IMPL_HPP
#define MODEL_INTERIORPARTITIONSURFACE_IMPL_HPP

#include "ModelAPI.hpp"
#include "PlanarSurface_Impl.hpp"

namespace openstudio {
namespace model {

class InteriorPartitionSurfaceGroup;
class InteriorPartitionSurface;
class DaylightingDeviceShelf;

namespace detail {

  /** InteriorPartitionSurface_Impl is a PlanarSurface_Impl that is the implementation class for InteriorPartitionSurface.*/
  class MODEL_API InteriorPartitionSurface_Impl : public PlanarSurface_Impl {
    Q_OBJECT;
    Q_PROPERTY(bool converttoInternalMass READ converttoInternalMass WRITE setConverttoInternalMass RESET resetConverttoInternalMass);
    Q_PROPERTY(bool isConverttoInternalMassDefaulted READ isConverttoInternalMassDefaulted);
    Q_PROPERTY(boost::optional<double> surfaceArea READ surfaceArea WRITE setSurfaceArea RESET resetSurfaceArea);
    Q_PROPERTY(boost::optional<double> numberofVertices READ numberofVertices WRITE setNumberofVertices RESET resetNumberofVertices);
    Q_PROPERTY(bool isNumberofVerticesDefaulted READ isNumberofVerticesDefaulted);
    Q_PROPERTY(bool isNumberofVerticesAutocalculated READ isNumberofVerticesAutocalculated);

    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> interiorPartitionSurfaceGroup READ interiorPartitionSurfaceGroupAsModelObject WRITE setInteriorPartitionSurfaceGroupAsModelObject RESET resetInteriorPartitionSurfaceGroup);
   public:
    /** @name Constructors and Destructors */
    //@{

    InteriorPartitionSurface_Impl(const IdfObject& idfObject,
                                  Model_Impl* model,
                                  bool keepHandle);

    InteriorPartitionSurface_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                  Model_Impl* model,
                                  bool keepHandle);

    InteriorPartitionSurface_Impl(const InteriorPartitionSurface_Impl& other,
                                  Model_Impl* model,
                                  bool keepHandle);

    virtual ~InteriorPartitionSurface_Impl() {}

    //@}

    // return the parent object in the hierarchy
    virtual boost::optional<ParentObject> parent() const;

    // return any children objects in the hierarchy
    virtual std::vector<ModelObject> children() const;

    /// set the parent, child may have to call methods on the parent
    bool setParent(ParentObject& newParent);

    virtual const std::vector<std::string>& outputVariableNames() const;
    
    virtual IddObjectType iddObjectType() const;

    /// should subtract this surface from parent's gross area for net area
    virtual bool subtractFromGrossArea() const;

    /// get the construction object and search distance
    virtual boost::optional<std::pair<ConstructionBase, int> > constructionWithSearchDistance() const;

    /// Returns true if the construction is not directly referenced by this surface .
    virtual bool isConstructionDefaulted() const;

    /// set the construction object
    virtual bool setConstruction(const ConstructionBase& construction);

    /// Resets the construction object.
    virtual void resetConstruction();

    /// Returns the containing PlanarSurfaceGroup if available.
    virtual boost::optional<PlanarSurfaceGroup> planarSurfaceGroup() const;

    /// Returns the containing Space if available.
    virtual boost::optional<Space> space() const;

    /** @name Getters */
    //@{

    bool converttoInternalMass() const;

    bool isConverttoInternalMassDefaulted() const;

    boost::optional<double> surfaceArea() const;

    boost::optional<double> numberofVertices() const;

    bool isNumberofVerticesDefaulted() const;

    bool isNumberofVerticesAutocalculated() const;

    // TODO: Handle this object's extensible fields.

    //@}
    /** @name Setters */
    //@{

    void setConverttoInternalMass(bool converttoInternalMass);

    void resetConverttoInternalMass();

    bool setSurfaceArea(boost::optional<double> surfaceArea);

    bool setSurfaceArea(double surfaceArea);

    void resetSurfaceArea();

    bool setNumberofVertices(boost::optional<double> numberofVertices);

    bool setNumberofVertices(double numberofVertices);

    void resetNumberofVertices();

    void autocalculateNumberofVertices();

    // TODO: Handle this object's extensible fields.

    //@}

    /// get the interior partition surface group
    boost::optional<InteriorPartitionSurfaceGroup> interiorPartitionSurfaceGroup() const;
    
    /// set the interior partition surface group
    bool setInteriorPartitionSurfaceGroup(const InteriorPartitionSurfaceGroup& interiorPartitionSurfaceGroup);

    void resetInteriorPartitionSurfaceGroup();

    boost::optional<DaylightingDeviceShelf> daylightingDeviceShelf() const;

   protected:
   private:
    REGISTER_LOGGER("openstudio.model.InteriorPartitionSurface");

    boost::optional<ModelObject> interiorPartitionSurfaceGroupAsModelObject() const;

    bool setInteriorPartitionSurfaceGroupAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_INTERIORPARTITIONSURFACE_IMPL_HPP

