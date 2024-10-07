/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

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
    class MODEL_API InteriorPartitionSurface_Impl : public PlanarSurface_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      InteriorPartitionSurface_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      InteriorPartitionSurface_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      InteriorPartitionSurface_Impl(const InteriorPartitionSurface_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~InteriorPartitionSurface_Impl() override = default;

      //@}

      // return the parent object in the hierarchy
      virtual boost::optional<ParentObject> parent() const override;

      // return any children objects in the hierarchy
      virtual std::vector<ModelObject> children() const override;

      /// set the parent, child may have to call methods on the parent
      bool setParent(ParentObject& newParent) override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      /// should subtract this surface from parent's gross area for net area
      virtual bool subtractFromGrossArea() const override;

      /// get the construction object and search distance
      virtual boost::optional<std::pair<ConstructionBase, int>> constructionWithSearchDistance() const override;

      /// Returns true if the construction is not directly referenced by this surface .
      virtual bool isConstructionDefaulted() const override;

      /// set the construction object
      virtual bool setConstruction(const ConstructionBase& construction) override;

      /// Resets the construction object.
      virtual void resetConstruction() override;

      /// Returns the containing PlanarSurfaceGroup if available.
      virtual boost::optional<PlanarSurfaceGroup> planarSurfaceGroup() const override;

      /// Returns the containing Space if available.
      virtual boost::optional<Space> space() const override;

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

      bool setConverttoInternalMass(bool converttoInternalMass);

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

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_INTERIORPARTITIONSURFACE_IMPL_HPP
