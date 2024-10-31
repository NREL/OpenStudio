/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SHADINGSURFACEGROUP_IMPL_HPP
#define MODEL_SHADINGSURFACEGROUP_IMPL_HPP

#include "ModelAPI.hpp"
#include "PlanarSurfaceGroup_Impl.hpp"

namespace openstudio {

class Transformation;

namespace model {

  class Space;
  class ShadingSurface;
  class ShadingSurfaceGroup;
  class Surface;
  class SubSurface;

  namespace detail {

    /** ShadingSurfaceGroup_Impl is a PlanarSurfaceGroup_Impl that is the implementation class for ShadingSurfaceGroup.*/
    class MODEL_API ShadingSurfaceGroup_Impl : public PlanarSurfaceGroup_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      ShadingSurfaceGroup_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ShadingSurfaceGroup_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ShadingSurfaceGroup_Impl(const ShadingSurfaceGroup_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ShadingSurfaceGroup_Impl() override = default;

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

      std::string shadingSurfaceType() const;

      virtual double directionofRelativeNorth() const override;

      virtual bool isDirectionofRelativeNorthDefaulted() const override;

      virtual double xOrigin() const override;

      virtual bool isXOriginDefaulted() const override;

      virtual double yOrigin() const override;

      virtual bool isYOriginDefaulted() const override;

      virtual double zOrigin() const override;

      virtual bool isZOriginDefaulted() const override;

      //@}
      /** @name Setters */
      //@{

      bool setShadingSurfaceType(const std::string& shadingSurfaceType);

      virtual bool setDirectionofRelativeNorth(double directionofRelativeNorth, bool driverMethod = true) override;

      virtual void resetDirectionofRelativeNorth() override;

      virtual bool setXOrigin(double xOrigin, bool driverMethod = true) override;

      virtual void resetXOrigin() override;

      virtual bool setYOrigin(double yOrigin, bool driverMethod = true) override;

      virtual void resetYOrigin() override;

      virtual bool setZOrigin(double zOrigin, bool driverMethod = true) override;

      virtual void resetZOrigin() override;

      //@}

      /// Returns the parent Space.
      boost::optional<Space> space() const;

      /// Sets the parent Space.
      bool setSpace(const Space& space);

      /** If (space()), calls setShadingSurfaceType("Building") to remove the relationship. */
      void resetSpace();

      /// Returns child shading surfaces.
      std::vector<ShadingSurface> shadingSurfaces() const;

      /** Returns the specific sub surface shaded by this group if specified. */
      boost::optional<SubSurface> shadedSubSurface() const;

      /** Returns the surface shaded by this group if specified. */
      boost::optional<Surface> shadedSurface() const;

      /** Sets the specifc sub surface shaded by this group, sub surface must be in the same parent space. */
      bool setShadedSubSurface(const SubSurface& subSurface);

      /** Sets the surface shaded by this group, surface must be in the same parent space. */
      bool setShadedSurface(const Surface& surface);

      /** Resets the sub surface shaded by this group. */
      void resetShadedObject();

     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ShadingSurfaceGroup");

      boost::optional<ModelObject> spaceAsModelObject() const;
      std::vector<ModelObject> shadingSurfacesAsModelObjects() const;

      bool setSpaceAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SHADINGSURFACEGROUP_IMPL_HPP
