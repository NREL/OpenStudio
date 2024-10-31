/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

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
    class MODEL_API ShadingSurface_Impl : public PlanarSurface_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      ShadingSurface_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ShadingSurface_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ShadingSurface_Impl(const ShadingSurface_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ShadingSurface_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      // return the parent object in the hierarchy
      virtual boost::optional<ParentObject> parent() const override;

      // return any children objects in the hierarchy
      virtual std::vector<ModelObject> children() const override;

      /// set the parent, child may have to call methods on the parent
      virtual bool setParent(ParentObject& newParent) override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

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

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SHADINGSURFACE_IMPL_HPP
