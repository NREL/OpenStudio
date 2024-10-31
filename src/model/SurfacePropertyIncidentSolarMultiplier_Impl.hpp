/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SURFACEPROPERTYINCIDENTSOLARMULTIPLIER_IMPL_HPP
#define MODEL_SURFACEPROPERTYINCIDENTSOLARMULTIPLIER_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class SubSurface;
  class Schedule;

  namespace detail {

    /** SurfacePropertyIncidentSolarMultiplier_Impl is a ModelObject_Impl that is the implementation class for SurfacePropertyIncidentSolarMultiplier.*/
    class MODEL_API SurfacePropertyIncidentSolarMultiplier_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      SurfacePropertyIncidentSolarMultiplier_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SurfacePropertyIncidentSolarMultiplier_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      SurfacePropertyIncidentSolarMultiplier_Impl(const SurfacePropertyIncidentSolarMultiplier_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~SurfacePropertyIncidentSolarMultiplier_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      //@}
      /** @name Getters */
      //@{

      SubSurface subSurface() const;

      double incidentSolarMultiplier() const;

      boost::optional<Schedule> incidentSolarMultiplierSchedule() const;

      //@}
      /** @name Setters */
      //@{

      bool setSubSurface(const SubSurface& subSurface);

      bool setIncidentSolarMultiplier(double incidentSolarMultiplier);

      bool setIncidentSolarMultiplierSchedule(Schedule& schedule);

      void resetIncidentSolarMultiplierSchedule();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.SurfacePropertyIncidentSolarMultiplier");

      boost::optional<SubSurface> optionalSubSurface() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SURFACEPROPERTYINCIDENTSOLARMULTIPLIER_IMPL_HPP
