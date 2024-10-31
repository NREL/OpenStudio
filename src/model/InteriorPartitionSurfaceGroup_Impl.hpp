/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

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
    class MODEL_API InteriorPartitionSurfaceGroup_Impl : public PlanarSurfaceGroup_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      InteriorPartitionSurfaceGroup_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      InteriorPartitionSurfaceGroup_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      InteriorPartitionSurfaceGroup_Impl(const InteriorPartitionSurfaceGroup_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~InteriorPartitionSurfaceGroup_Impl() override = default;

      //@}

      // return the parent object in the hierarchy
      virtual boost::optional<ParentObject> parent() const override;

      // set the parent, child may have to call methods on the parent
      virtual bool setParent(ParentObject& newParent) override;

      // return any children objects in the hierarchy
      virtual std::vector<ModelObject> children() const override;

      /// get a vector of allowable children types
      virtual std::vector<IddObjectType> allowableChildTypes() const override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual openstudio::Transformation buildingTransformation() const override;

      virtual bool changeTransformation(const openstudio::Transformation& transformation) override;

      virtual openstudio::BoundingBox boundingBox() const override;

      /** @name Getters */
      //@{

      // TODO: Handle Non-Extensible IddField Space Name.

      virtual double directionofRelativeNorth() const override;

      virtual bool isDirectionofRelativeNorthDefaulted() const override;

      virtual double xOrigin() const override;

      virtual bool isXOriginDefaulted() const override;

      virtual double yOrigin() const override;

      virtual bool isYOriginDefaulted() const override;

      virtual double zOrigin() const override;

      virtual bool isZOriginDefaulted() const override;

      int multiplier() const;

      bool isMultiplierDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      virtual bool setDirectionofRelativeNorth(double directionofRelativeNorth, bool driverMethod = true) override;

      virtual void resetDirectionofRelativeNorth() override;

      virtual bool setXOrigin(double xOrigin, bool driverMethod = true) override;

      virtual void resetXOrigin() override;

      virtual bool setYOrigin(double yOrigin, bool driverMethod = true) override;

      virtual void resetYOrigin() override;

      virtual bool setZOrigin(double zOrigin, bool driverMethod = true) override;

      virtual void resetZOrigin() override;

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

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_INTERIORPARTITIONSURFACEGROUP_IMPL_HPP
