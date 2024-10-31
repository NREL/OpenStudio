/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SURFACEPROPERTYLOCALENVIRONMENT_IMPL_HPP
#define MODEL_SURFACEPROPERTYLOCALENVIRONMENT_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class PlanarSurface;
  class Surface;
  class SubSurface;
  class Schedule;
  class SurfacePropertySurroundingSurfaces;
  class SurfacePropertyGroundSurfaces;
  // class OutdoorAirNode;

  namespace detail {

    /** SurfacePropertyLocalEnvironment_Impl is a ModelObject_Impl that is the implementation class for SurfacePropertyLocalEnvironment.*/
    class MODEL_API SurfacePropertyLocalEnvironment_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      SurfacePropertyLocalEnvironment_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SurfacePropertyLocalEnvironment_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      SurfacePropertyLocalEnvironment_Impl(const SurfacePropertyLocalEnvironment_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~SurfacePropertyLocalEnvironment_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      //@}
      /** @name Getters */
      //@{

      PlanarSurface exteriorSurface() const;
      boost::optional<Surface> exteriorSurfaceAsSurface() const;
      boost::optional<SubSurface> exteriorSurfaceAsSubSurface() const;

      boost::optional<Schedule> externalShadingFractionSchedule() const;

      boost::optional<SurfacePropertySurroundingSurfaces> surfacePropertySurroundingSurfaces() const;

      boost::optional<SurfacePropertyGroundSurfaces> surfacePropertyGroundSurfaces() const;

      // boost::optional<OutdoorAirNode> outdoorAirNode() const;

      //@}
      /** @name Setters */
      //@{

      bool setExteriorSurface(const PlanarSurface& surface);

      bool setExternalShadingFractionSchedule(Schedule& schedule);
      void resetExternalShadingFractionSchedule();

      bool setSurfacePropertySurroundingSurfaces(const SurfacePropertySurroundingSurfaces& surfacePropertySurroundingSurfaces);
      void resetSurfacePropertySurroundingSurfaces();

      bool setSurfacePropertyGroundSurfaces(const SurfacePropertyGroundSurfaces& surfacePropertyGroundSurfaces);
      void resetSurfacePropertyGroundSurfaces();

      // bool setOutdoorAirNode(const OutdoorAirNode& outdoorAirNode);
      // void resetOutdoorAirNode();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.SurfacePropertyLocalEnvironment");

      boost::optional<PlanarSurface> optionalExteriorSurface() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SURFACEPROPERTYLOCALENVIRONMENT_IMPL_HPP
