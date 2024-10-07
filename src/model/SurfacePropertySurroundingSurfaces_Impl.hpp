/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SURFACEPROPERTYSURROUNDINGSURFACES_IMPL_HPP
#define MODEL_SURFACEPROPERTYSURROUNDINGSURFACES_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class SurroundingSurfaceGroup;
  class SurfacePropertyLocalEnvironment;

  namespace detail {

    /** SurfacePropertySurroundingSurfaces_Impl is a ModelObject_Impl that is the implementation class for SurfacePropertySurroundingSurfaces.*/
    class MODEL_API SurfacePropertySurroundingSurfaces_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      SurfacePropertySurroundingSurfaces_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SurfacePropertySurroundingSurfaces_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      SurfacePropertySurroundingSurfaces_Impl(const SurfacePropertySurroundingSurfaces_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~SurfacePropertySurroundingSurfaces_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<double> skyViewFactor() const;
      bool isSkyViewFactorAutocalculated() const;

      boost::optional<Schedule> skyTemperatureSchedule() const;

      boost::optional<double> groundViewFactor() const;
      bool isGroundViewFactorAutocalculated() const;

      boost::optional<Schedule> groundTemperatureSchedule() const;

      //@}
      /** @name Setters */
      //@{

      bool setSkyViewFactor(double skyViewFactor);
      void autocalculateSkyViewFactor();

      bool setSkyTemperatureSchedule(Schedule& schedule);
      void resetSkyTemperatureSchedule();

      bool setGroundViewFactor(double groundViewFactor);
      void autocalculateGroundViewFactor();

      bool setGroundTemperatureSchedule(Schedule& schedule);
      void resetGroundTemperatureSchedule();

      //@}
      /** @name Other */
      //@{

      boost::optional<SurfacePropertyLocalEnvironment> surfacePropertyLocalEnvironment() const;

      std::vector<SurroundingSurfaceGroup> surroundingSurfaceGroups() const;

      unsigned int numberofSurroundingSurfaceGroups() const;

      /** Looks up by Surface Name (case-insensitive)  */
      boost::optional<unsigned> surroundingSurfaceGroupIndex(const SurroundingSurfaceGroup& surroundingSurfaceGroup) const;
      boost::optional<unsigned> surroundingSurfaceGroupIndex(const std::string& surroundingSurfaceName) const;

      boost::optional<SurroundingSurfaceGroup> getSurroundingSurfaceGroup(unsigned groupIndex) const;

      /** If a surroundingSurfaceGroup group is already present (cf `surroundingSurfaceGroupIndex()`), it will Warn and override the surroundingSurfaceGroup value */
      bool addSurroundingSurfaceGroup(const SurroundingSurfaceGroup& surroundingSurfaceGroup);

      // Overloads, it creates a SurroundingSurfaceGroup wrapper, then call `addSurroundingSurfaceGroup(const SurroundingSurfaceGroup& surroundingSurfaceGroup)`
      bool addSurroundingSurfaceGroup(const std::string& surroundingSurfaceName, double viewFactor, const Schedule& temperatureSchedule);

      bool addSurroundingSurfaceGroups(const std::vector<SurroundingSurfaceGroup>& surroundingSurfaceGroups);

      bool removeSurroundingSurfaceGroup(unsigned groupIndex);

      void removeAllSurroundingSurfaceGroups();

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.SurfacePropertySurroundingSurfaces");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SURFACEPROPERTYSURROUNDINGSURFACES_IMPL_HPP
