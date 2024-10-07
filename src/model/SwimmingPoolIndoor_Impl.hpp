/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SWIMMINGPOOLINDOOR_IMPL_HPP
#define MODEL_SWIMMINGPOOLINDOOR_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Surface;
  class Schedule;
  class Node;

  namespace detail {

    /** SwimmingPoolIndoor_Impl is a StraightComponent_Impl that is the implementation class for SwimmingPoolIndoor.*/
    class MODEL_API SwimmingPoolIndoor_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      SwimmingPoolIndoor_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SwimmingPoolIndoor_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      SwimmingPoolIndoor_Impl(const SwimmingPoolIndoor_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~SwimmingPoolIndoor_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual bool addToNode(Node& node) override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      Surface surface() const;

      double averageDepth() const;

      Schedule activityFactorSchedule() const;

      Schedule makeupWaterSupplySchedule() const;

      Schedule coverSchedule() const;

      double coverEvaporationFactor() const;

      double coverConvectionFactor() const;

      double coverShortWavelengthRadiationFactor() const;

      double coverLongWavelengthRadiationFactor() const;

      double poolHeatingSystemMaximumWaterFlowRate() const;

      double poolMiscellaneousEquipmentPower() const;

      Schedule setpointTemperatureSchedule() const;

      double maximumNumberofPeople() const;

      Schedule peopleSchedule() const;

      Schedule peopleHeatGainSchedule() const;

      //@}
      /** @name Setters */
      //@{

      bool setSurface(const Surface& floorSurface);

      bool setAverageDepth(double averageDepth);

      bool setActivityFactorSchedule(Schedule& schedule);

      bool setMakeupWaterSupplySchedule(Schedule& schedule);

      bool setCoverSchedule(Schedule& schedule);

      bool setCoverEvaporationFactor(double coverEvaporationFactor);

      bool setCoverConvectionFactor(double coverConvectionFactor);

      bool setCoverShortWavelengthRadiationFactor(double coverShortWavelengthRadiationFactor);

      bool setCoverLongWavelengthRadiationFactor(double coverLongWavelengthRadiationFactor);

      bool setPoolWaterInletNode(const Connection& connection);

      bool setPoolWaterOutletNode(const Connection& connection);

      bool setPoolHeatingSystemMaximumWaterFlowRate(double poolHeatingSystemMaximumWaterFlowRate);

      bool setPoolMiscellaneousEquipmentPower(double poolMiscellaneousEquipmentPower);

      bool setSetpointTemperatureSchedule(Schedule& schedule);

      bool setMaximumNumberofPeople(double maximumNumberofPeople);

      bool setPeopleSchedule(Schedule& schedule);

      bool setPeopleHeatGainSchedule(Schedule& schedule);

      //@}
      /** @name Other */
      //@{

      // Convenience function to return the inletNode of the Pool object
      boost::optional<Node> poolWaterInletNode() const;

      // Convenience function to return the Outlet Node of the Pool object
      boost::optional<Node> poolWaterOutletNode() const;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.SwimmingPoolIndoor");

      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<Surface> optionalSurface() const;
      boost::optional<Schedule> optionalActivityFactorSchedule() const;
      boost::optional<Schedule> optionalMakeupWaterSupplySchedule() const;
      boost::optional<Schedule> optionalCoverSchedule() const;
      boost::optional<Schedule> optionalSetpointTemperatureSchedule() const;
      boost::optional<Schedule> optionalPeopleSchedule() const;
      boost::optional<Schedule> optionalPeopleHeatGainSchedule() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SWIMMINGPOOLINDOOR_IMPL_HPP
