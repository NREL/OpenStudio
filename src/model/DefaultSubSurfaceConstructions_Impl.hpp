/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_DEFAULTSUBSURFACECONSTRUCTIONS_IMPL_HPP
#define MODEL_DEFAULTSUBSURFACECONSTRUCTIONS_IMPL_HPP

#include "ModelAPI.hpp"
#include "ResourceObject_Impl.hpp"

namespace openstudio {
namespace model {

  class ConstructionBase;
  class DefaultSubSurfaceConstructions;

  namespace detail {

    /** DefaultSubSurfaceConstructions_Impl is a ResourceObject_Impl that is the implementation class for DefaultSubSurfaceConstructions.*/
    class MODEL_API DefaultSubSurfaceConstructions_Impl : public ResourceObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      DefaultSubSurfaceConstructions_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      DefaultSubSurfaceConstructions_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      DefaultSubSurfaceConstructions_Impl(const DefaultSubSurfaceConstructions_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~DefaultSubSurfaceConstructions_Impl() override = default;

      //@}

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      /** @name Getters */
      //@{

      boost::optional<ConstructionBase> fixedWindowConstruction() const;

      boost::optional<ConstructionBase> operableWindowConstruction() const;

      boost::optional<ConstructionBase> doorConstruction() const;

      boost::optional<ConstructionBase> glassDoorConstruction() const;

      boost::optional<ConstructionBase> overheadDoorConstruction() const;

      boost::optional<ConstructionBase> skylightConstruction() const;

      boost::optional<ConstructionBase> tubularDaylightDomeConstruction() const;

      boost::optional<ConstructionBase> tubularDaylightDiffuserConstruction() const;

      //@}
      /** @name Setters */
      //@{

      bool setFixedWindowConstruction(const ConstructionBase& construction);

      void resetFixedWindowConstruction();

      bool setOperableWindowConstruction(const ConstructionBase& construction);

      void resetOperableWindowConstruction();

      bool setDoorConstruction(const ConstructionBase& construction);

      void resetDoorConstruction();

      bool setGlassDoorConstruction(const ConstructionBase& construction);

      void resetGlassDoorConstruction();

      bool setOverheadDoorConstruction(const ConstructionBase& construction);

      void resetOverheadDoorConstruction();

      bool setSkylightConstruction(const ConstructionBase& construction);

      void resetSkylightConstruction();

      bool setTubularDaylightDomeConstruction(const ConstructionBase& construction);

      void resetTubularDaylightDomeConstruction();

      bool setTubularDaylightDiffuserConstruction(const ConstructionBase& construction);

      void resetTubularDaylightDiffuserConstruction();

      //@}

      /// Merge this object with other one, keep fields from this object if set otherwise set to value from other.
      void merge(const DefaultSubSurfaceConstructions& other);

     protected:
     private:
      REGISTER_LOGGER("openstudio.model.DefaultSubSurfaceConstructions");

      boost::optional<ModelObject> fixedWindowConstructionAsModelObject() const;
      boost::optional<ModelObject> operableWindowConstructionAsModelObject() const;
      boost::optional<ModelObject> doorConstructionAsModelObject() const;
      boost::optional<ModelObject> glassDoorConstructionAsModelObject() const;
      boost::optional<ModelObject> overheadDoorConstructionAsModelObject() const;
      boost::optional<ModelObject> skylightConstructionAsModelObject() const;
      boost::optional<ModelObject> tubularDaylightDomeConstructionAsModelObject() const;
      boost::optional<ModelObject> tubularDaylightDiffuserConstructionAsModelObject() const;

      bool setFixedWindowConstructionAsModelObject(boost::optional<ModelObject> modelObject);
      bool setOperableWindowConstructionAsModelObject(boost::optional<ModelObject> modelObject);
      bool setDoorConstructionAsModelObject(boost::optional<ModelObject> modelObject);
      bool setGlassDoorConstructionAsModelObject(boost::optional<ModelObject> modelObject);
      bool setOverheadDoorConstructionAsModelObject(boost::optional<ModelObject> modelObject);
      bool setSkylightConstructionAsModelObject(boost::optional<ModelObject> modelObject);
      bool setTubularDaylightDomeConstructionAsModelObject(boost::optional<ModelObject> modelObject);
      bool setTubularDaylightDiffuserConstructionAsModelObject(boost::optional<ModelObject> modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_DEFAULTSUBSURFACECONSTRUCTIONS_IMPL_HPP
