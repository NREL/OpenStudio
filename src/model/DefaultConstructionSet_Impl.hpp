/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

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
    class MODEL_API DefaultConstructionSet_Impl : public ResourceObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      DefaultConstructionSet_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      DefaultConstructionSet_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      DefaultConstructionSet_Impl(const DefaultConstructionSet_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~DefaultConstructionSet_Impl() override = default;

      //@}

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

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

      boost::optional<ConstructionBase> adiabaticSurfaceConstruction() const;

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

      bool setAdiabaticSurfaceConstruction(const ConstructionBase& construction);

      void resetAdiabaticSurfaceConstruction();

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
      boost::optional<ModelObject> adiabaticSurfaceConstructionAsModelObject() const;

      bool setDefaultExteriorSurfaceConstructionsAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setDefaultInteriorSurfaceConstructionsAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setDefaultGroundContactSurfaceConstructionsAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setDefaultExteriorSubSurfaceConstructionsAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setDefaultInteriorSubSurfaceConstructionsAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setInteriorPartitionConstructionAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setSpaceShadingConstructionAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setBuildingShadingConstructionAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setSiteShadingConstructionAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setAdiabaticSurfaceConstructionAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_DEFAULTCONSTRUCTIONSET_IMPL_HPP
