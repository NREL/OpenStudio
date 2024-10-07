/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SURFACEPROPERTYGROUNDSURFACES_IMPL_HPP
#define MODEL_SURFACEPROPERTYGROUNDSURFACES_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject_Impl.hpp"
#include "Schedule.hpp"

namespace openstudio {
namespace model {

  class GroundSurfaceGroup;
  class SurfacePropertyLocalEnvironment;

  namespace detail {

    /** SurfacePropertyGroundSurfaces_Impl is a ModelObject_Impl that is the implementation class for SurfacePropertyGroundSurfaces.*/
    class MODEL_API SurfacePropertyGroundSurfaces_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      SurfacePropertyGroundSurfaces_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SurfacePropertyGroundSurfaces_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      SurfacePropertyGroundSurfaces_Impl(const SurfacePropertyGroundSurfaces_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~SurfacePropertyGroundSurfaces_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      //@}
      /** @name Getters */
      //@{

      //@}
      /** @name Setters */
      //@{

      //@}
      /** @name Other */
      //@{

      boost::optional<SurfacePropertyLocalEnvironment> surfacePropertyLocalEnvironment() const;

      std::vector<GroundSurfaceGroup> groundSurfaceGroups() const;

      unsigned int numberofGroundSurfaceGroups() const;

      /** Looks up by Surface Name (case-insensitive)  */
      boost::optional<unsigned> groundSurfaceGroupIndex(const GroundSurfaceGroup& groundSurfaceGroup) const;
      boost::optional<unsigned> groundSurfaceGroupIndex(const std::string& groundSurfaceName) const;

      boost::optional<GroundSurfaceGroup> getGroundSurfaceGroup(unsigned groupIndex) const;

      /** If a groundSurfaceGroup group is already present (cf `groundSurfaceGroupIndex()`), it will Warn and override the groundSurfaceGroup value */
      bool addGroundSurfaceGroup(const GroundSurfaceGroup& groundSurfaceGroup);

      // Overloads, it creates a GroundSurfaceGroup wrapper, then call `addGroundSurfaceGroup(const GroundSurfaceGroup& groundSurfaceGroup)`
      bool addGroundSurfaceGroup(const std::string& groundSurfaceName, double viewFactor, boost::optional<Schedule> temperatureSchedule = boost::none,
                                 boost::optional<Schedule> reflectanceSchedule = boost::none);

      bool addGroundSurfaceGroups(const std::vector<GroundSurfaceGroup>& groundSurfaceGroups);

      bool removeGroundSurfaceGroup(unsigned groupIndex);

      void removeAllGroundSurfaceGroups();

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.SurfacePropertyGroundSurfaces");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SURFACEPROPERTYGROUNDSURFACES_IMPL_HPP
