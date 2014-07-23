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

#ifndef MODEL_DEFAULTCONSTRUCTIONSET_IMPL_HPP
#define MODEL_DEFAULTCONSTRUCTIONSET_IMPL_HPP

#include "ModelAPI.hpp"
#include "ResourceObject_Impl.hpp"

namespace openstudio {
namespace model {

class PlanarSurface;
class ConstructionBase;
class DefaultSubSurfaceConstructions;
class DefaultSurfaceConstructions;
class DefaultConstructionSet;

namespace detail {

  /** DefaultConstructionSet_Impl is a ResourceObject_Impl that is the implementation class for DefaultConstructionSet.*/
  class MODEL_API DefaultConstructionSet_Impl : public ResourceObject_Impl {
    Q_OBJECT;

    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> defaultExteriorSurfaceConstructions READ defaultExteriorSurfaceConstructionsAsModelObject WRITE setDefaultExteriorSurfaceConstructionsAsModelObject RESET resetDefaultExteriorSurfaceConstructions);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> defaultInteriorSurfaceConstructions READ defaultInteriorSurfaceConstructionsAsModelObject WRITE setDefaultInteriorSurfaceConstructionsAsModelObject RESET resetDefaultInteriorSurfaceConstructions);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> defaultGroundContactSurfaceConstructions READ defaultGroundContactSurfaceConstructionsAsModelObject WRITE setDefaultGroundContactSurfaceConstructionsAsModelObject RESET resetDefaultGroundContactSurfaceConstructions);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> defaultExteriorSubSurfaceConstructions READ defaultExteriorSubSurfaceConstructionsAsModelObject WRITE setDefaultExteriorSubSurfaceConstructionsAsModelObject RESET resetDefaultExteriorSubSurfaceConstructions);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> defaultInteriorSubSurfaceConstructions READ defaultInteriorSubSurfaceConstructionsAsModelObject WRITE setDefaultInteriorSubSurfaceConstructionsAsModelObject RESET resetDefaultInteriorSubSurfaceConstructions);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> interiorPartitionConstruction READ interiorPartitionConstructionAsModelObject WRITE setInteriorPartitionConstructionAsModelObject RESET resetInteriorPartitionConstruction);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> spaceShadingConstruction READ spaceShadingConstructionAsModelObject WRITE setSpaceShadingConstructionAsModelObject RESET resetSpaceShadingConstruction);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> buildingShadingConstruction READ buildingShadingConstructionAsModelObject WRITE setBuildingShadingConstructionAsModelObject RESET resetBuildingShadingConstruction);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> siteShadingConstruction READ siteShadingConstructionAsModelObject WRITE setSiteShadingConstructionAsModelObject RESET resetSiteShadingConstruction);
   public:
    /** @name Constructors and Destructors */
    //@{

    DefaultConstructionSet_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    DefaultConstructionSet_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                Model_Impl* model,
                                bool keepHandle);

    DefaultConstructionSet_Impl(const DefaultConstructionSet_Impl& other,
                                Model_Impl* model,
                                bool keepHandle);

    virtual ~DefaultConstructionSet_Impl() {}

    //@}
    
    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    /** @name Getters */
    //@{

    boost::optional<DefaultSurfaceConstructions> defaultExteriorSurfaceConstructions() const;

    boost::optional<DefaultSurfaceConstructions> defaultInteriorSurfaceConstructions() const;

    boost::optional<DefaultSurfaceConstructions> defaultGroundContactSurfaceConstructions() const;

    boost::optional<DefaultSubSurfaceConstructions> defaultExteriorSubSurfaceConstructions() const;

    boost::optional<DefaultSubSurfaceConstructions> defaultInteriorSubSurfaceConstructions() const;

    boost::optional<ConstructionBase> interiorPartitionConstruction() const;

    boost::optional<ConstructionBase> spaceShadingConstruction() const;

    boost::optional<ConstructionBase> buildingShadingConstruction() const;

    boost::optional<ConstructionBase> siteShadingConstruction() const;

    //@}
    /** @name Setters */
    //@{

    bool setDefaultExteriorSurfaceConstructions(const DefaultSurfaceConstructions& defaultSurfaceConstructions);

    void resetDefaultExteriorSurfaceConstructions();

    bool setDefaultInteriorSurfaceConstructions(const DefaultSurfaceConstructions& defaultSurfaceConstructions);

    void resetDefaultInteriorSurfaceConstructions();

    bool setDefaultGroundContactSurfaceConstructions(const DefaultSurfaceConstructions& defaultSurfaceConstructions);

    void resetDefaultGroundContactSurfaceConstructions();

    bool setDefaultExteriorSubSurfaceConstructions(const DefaultSubSurfaceConstructions& defaultSubSurfaceConstructions);

    void resetDefaultExteriorSubSurfaceConstructions();

    bool setDefaultInteriorSubSurfaceConstructions(const DefaultSubSurfaceConstructions& defaultSubSurfaceConstructions);

    void resetDefaultInteriorSubSurfaceConstructions();

    bool setInteriorPartitionConstruction(const ConstructionBase& construction);

    void resetInteriorPartitionConstruction();

    bool setSpaceShadingConstruction(const ConstructionBase& construction);

    void resetSpaceShadingConstruction();

    bool setBuildingShadingConstruction(const ConstructionBase& construction);

    void resetBuildingShadingConstruction();

    bool setSiteShadingConstruction(const ConstructionBase& construction);

    void resetSiteShadingConstruction();

    //@}

    /// Returns the default construction for this planar surface if available.
    boost::optional<ConstructionBase> getDefaultConstruction(const PlanarSurface& planarSurface) const;
  
    /// Merge this object with other one, keep fields from this object if set otherwise set to value from other.
    void merge(const DefaultConstructionSet& other);

   protected:
   private:
    REGISTER_LOGGER("openstudio.model.DefaultConstructionSet");

    boost::optional<ModelObject> defaultExteriorSurfaceConstructionsAsModelObject() const;
    boost::optional<ModelObject> defaultInteriorSurfaceConstructionsAsModelObject() const;
    boost::optional<ModelObject> defaultGroundContactSurfaceConstructionsAsModelObject() const;
    boost::optional<ModelObject> defaultExteriorSubSurfaceConstructionsAsModelObject() const;
    boost::optional<ModelObject> defaultInteriorSubSurfaceConstructionsAsModelObject() const;
    boost::optional<ModelObject> interiorPartitionConstructionAsModelObject() const;
    boost::optional<ModelObject> spaceShadingConstructionAsModelObject() const;
    boost::optional<ModelObject> buildingShadingConstructionAsModelObject() const;
    boost::optional<ModelObject> siteShadingConstructionAsModelObject() const;

    bool setDefaultExteriorSurfaceConstructionsAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setDefaultInteriorSurfaceConstructionsAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setDefaultGroundContactSurfaceConstructionsAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setDefaultExteriorSubSurfaceConstructionsAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setDefaultInteriorSubSurfaceConstructionsAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setInteriorPartitionConstructionAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setSpaceShadingConstructionAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setBuildingShadingConstructionAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setSiteShadingConstructionAsModelObject(const boost::optional<ModelObject>& modelObject);

  };

} // detail

} // model
} // openstudio

#endif // MODEL_DEFAULTCONSTRUCTIONSET_IMPL_HPP

