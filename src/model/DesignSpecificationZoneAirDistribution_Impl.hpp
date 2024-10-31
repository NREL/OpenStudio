/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_DESIGNSPECIFICATIONZONEAIRDISTRIBUTION_IMPL_HPP
#define MODEL_DESIGNSPECIFICATIONZONEAIRDISTRIBUTION_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;

  namespace detail {

    /** DesignSpecificationZoneAirDistribution_Impl is a ModelObject_Impl that is the implementation class for DesignSpecificationZoneAirDistribution.*/
    class MODEL_API DesignSpecificationZoneAirDistribution_Impl : public ModelObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      DesignSpecificationZoneAirDistribution_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      DesignSpecificationZoneAirDistribution_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      DesignSpecificationZoneAirDistribution_Impl(const DesignSpecificationZoneAirDistribution_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~DesignSpecificationZoneAirDistribution_Impl() override = default;

      //@}

      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      //@}
      /** @name Getters */
      //@{

      double zoneAirDistributionEffectivenessinCoolingMode() const;

      bool isZoneAirDistributionEffectivenessinCoolingModeDefaulted() const;

      double zoneAirDistributionEffectivenessinHeatingMode() const;

      bool isZoneAirDistributionEffectivenessinHeatingModeDefaulted() const;

      boost::optional<Schedule> zoneAirDistributionEffectivenessSchedule() const;

      double zoneSecondaryRecirculationFraction() const;

      bool isZoneSecondaryRecirculationFractionDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      bool setZoneAirDistributionEffectivenessinCoolingMode(double zoneAirDistributionEffectivenessinCoolingMode);

      void resetZoneAirDistributionEffectivenessinCoolingMode();

      bool setZoneAirDistributionEffectivenessinHeatingMode(double zoneAirDistributionEffectivenessinHeatingMode);

      void resetZoneAirDistributionEffectivenessinHeatingMode();

      bool setZoneAirDistributionEffectivenessSchedule(Schedule& schedule);

      void resetZoneAirDistributionEffectivenessSchedule();

      bool setZoneSecondaryRecirculationFraction(double zoneSecondaryRecirculationFraction);

      void resetZoneSecondaryRecirculationFraction();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.DesignSpecificationZoneAirDistribution");

      boost::optional<ModelObject> zoneAirDistributionEffectivenessScheduleAsModelObject() const;

      bool setZoneAirDistributionEffectivenessScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_DESIGNSPECIFICATIONZONEAIRDISTRIBUTION_IMPL_HPP
