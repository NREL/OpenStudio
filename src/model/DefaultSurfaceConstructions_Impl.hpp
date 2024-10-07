/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

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
    class MODEL_API DefaultSurfaceConstructions_Impl : public ResourceObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      DefaultSurfaceConstructions_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      DefaultSurfaceConstructions_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      DefaultSurfaceConstructions_Impl(const DefaultSurfaceConstructions_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~DefaultSurfaceConstructions_Impl() override = default;

      //@}

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

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

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_DEFAULTSURFACECONSTRUCTIONS_IMPL_HPP
