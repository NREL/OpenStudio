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

#ifndef MODEL_DEFAULTSURFACECONSTRUCTIONS_IMPL_HPP
#define MODEL_DEFAULTSURFACECONSTRUCTIONS_IMPL_HPP

#include "ModelAPI.hpp"
#include "ResourceObject_Impl.hpp"

namespace openstudio {
namespace model {

class ConstructionBase;
class DefaultSurfaceConstructions;

namespace detail {

  /** DefaultSurfaceConstructions_Impl is a ResourceObject_Impl that is the implementation class for DefaultSurfaceConstructions.*/
  class MODEL_API DefaultSurfaceConstructions_Impl : public ResourceObject_Impl {
    Q_OBJECT;

    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> floorConstruction READ floorConstructionAsModelObject WRITE setFloorConstructionAsModelObject RESET resetFloorConstruction);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> wallConstruction READ wallConstructionAsModelObject WRITE setWallConstructionAsModelObject RESET resetWallConstruction);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> roofCeilingConstruction READ roofCeilingConstructionAsModelObject WRITE setRoofCeilingConstructionAsModelObject RESET resetRoofCeilingConstruction);
   public:
    /** @name Constructors and Destructors */
    //@{

    DefaultSurfaceConstructions_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    DefaultSurfaceConstructions_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                     Model_Impl* model,
                                     bool keepHandle);

    DefaultSurfaceConstructions_Impl(const DefaultSurfaceConstructions_Impl& other,
                                     Model_Impl* model,
                                     bool keepHandle);

    virtual ~DefaultSurfaceConstructions_Impl() {}

    //@}
   
    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    /** @name Getters */
    //@{

    boost::optional<ConstructionBase> floorConstruction() const;

    boost::optional<ConstructionBase> wallConstruction() const;

    boost::optional<ConstructionBase> roofCeilingConstruction() const;

    //@}
    /** @name Setters */
    //@{

    bool setFloorConstruction(const ConstructionBase& construction);

    void resetFloorConstruction();

    bool setWallConstruction(const ConstructionBase& construction);

    void resetWallConstruction();

    bool setRoofCeilingConstruction(const ConstructionBase& construction);

    void resetRoofCeilingConstruction();

    //@}

    /// Merge this object with other one, keep fields from this object if set otherwise set to value from other.
    void merge(const DefaultSurfaceConstructions& other);

   protected:
   private:
    REGISTER_LOGGER("openstudio.model.DefaultSurfaceConstructions");

    boost::optional<ModelObject> floorConstructionAsModelObject() const;
    boost::optional<ModelObject> wallConstructionAsModelObject() const;
    boost::optional<ModelObject> roofCeilingConstructionAsModelObject() const;

    bool setFloorConstructionAsModelObject(boost::optional<ModelObject> modelObject);
    bool setWallConstructionAsModelObject(boost::optional<ModelObject> modelObject);
    bool setRoofCeilingConstructionAsModelObject(boost::optional<ModelObject> modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_DEFAULTSURFACECONSTRUCTIONS_IMPL_HPP

