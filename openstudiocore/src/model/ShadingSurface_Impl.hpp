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

#ifndef MODEL_SHADINGSURFACE_IMPL_HPP
#define MODEL_SHADINGSURFACE_IMPL_HPP

#include "ModelAPI.hpp"
#include "PlanarSurface_Impl.hpp"

namespace openstudio {
namespace model {

class ShadingSurfaceGroup;
class Schedule;
class ShadingSurface;
class DaylightingDeviceShelf;

namespace detail {

  /** ShadingSurface_Impl is a PlanarSurface_Impl that is the implementation class for ShadingSurface.*/
  class MODEL_API ShadingSurface_Impl : public PlanarSurface_Impl {
    Q_OBJECT;
    Q_PROPERTY(boost::optional<double> numberofVertices READ numberofVertices WRITE setNumberofVertices RESET resetNumberofVertices);
    Q_PROPERTY(bool isNumberofVerticesDefaulted READ isNumberofVerticesDefaulted);
    Q_PROPERTY(bool isNumberofVerticesAutocalculated READ isNumberofVerticesAutocalculated);

    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> shadingSurfaceGroup READ shadingSurfaceGroupAsModelObject WRITE setShadingSurfaceGroupAsModelObject RESET resetShadingSurfaceGroup);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> transmittanceSchedule READ transmittanceScheduleAsModelObject WRITE setTransmittanceScheduleAsModelObject RESET resetTransmittanceSchedule);
   public:
    /** @name Constructors and Destructors */
    //@{

    ShadingSurface_Impl(const IdfObject& idfObject, 
                        Model_Impl* model,
                        bool keepHandle);

    ShadingSurface_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                        Model_Impl* model,
                        bool keepHandle);

    ShadingSurface_Impl(const ShadingSurface_Impl& other,
                        Model_Impl* model,
                        bool keepHandle);

    virtual ~ShadingSurface_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    // return the parent object in the hierarchy
    virtual boost::optional<ParentObject> parent() const;

    // return any children objects in the hierarchy
    virtual std::vector<ModelObject> children() const;

    /// set the parent, child may have to call methods on the parent
    virtual bool setParent(ParentObject& newParent);

    virtual const std::vector<std::string>& outputVariableNames() const;
    
    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

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

    //@}
    /** @name Getters */
    //@{

    /// get the shading group
    boost::optional<ShadingSurfaceGroup> shadingSurfaceGroup() const;

    // get the transmittance schedule
    boost::optional<Schedule> transmittanceSchedule() const;

    boost::optional<double> numberofVertices() const;

    bool isNumberofVerticesDefaulted() const;

    bool isNumberofVerticesAutocalculated() const;

    //@}
    /** @name Setters */
    //@{

    /// set the exterior shading group
    bool setShadingSurfaceGroup(const ShadingSurfaceGroup& shadingSurfaceGroup);

    void resetShadingSurfaceGroup();

    /// set the transmittance schedule
    bool setTransmittanceSchedule(Schedule& transmittanceSchedule);

    /// reset the transmittance schedule
    void resetTransmittanceSchedule();

    bool setNumberofVertices(boost::optional<double> numberofVertices);

    bool setNumberofVertices(double numberofVertices);

    void resetNumberofVertices();

    void autocalculateNumberofVertices();

    //@}

    boost::optional<DaylightingDeviceShelf> daylightingDeviceShelf() const;

   protected:
   private:
    REGISTER_LOGGER("openstudio.model.ShadingSurface");

    boost::optional<ModelObject> shadingSurfaceGroupAsModelObject() const;
    boost::optional<ModelObject> transmittanceScheduleAsModelObject() const;

    bool setShadingSurfaceGroupAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setTransmittanceScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_SHADINGSURFACE_IMPL_HPP

