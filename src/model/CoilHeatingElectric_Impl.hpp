/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILHEATINGELECTRIC_IMPL_HPP
#define MODEL_COILHEATINGELECTRIC_IMPL_HPP

#include "StraightComponent_Impl.hpp"

namespace openstudio {

namespace model {

  class Schedule;

  class Node;

  namespace detail {

    /** CoilHeatingElectric_Impl is a StraightComponent_Impl that is the
   *  implementation class for CoilHeatingElectric.*/
    class MODEL_API CoilHeatingElectric_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilHeatingElectric_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilHeatingElectric_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilHeatingElectric_Impl(const CoilHeatingElectric_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilHeatingElectric_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ModelObject> children() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual bool addToNode(Node& node) override;

      virtual boost::optional<HVACComponent> containingHVACComponent() const override;

      virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      Schedule availabilitySchedule() const;

      double efficiency() const;

      bool isEfficiencyDefaulted() const;

      boost::optional<double> nominalCapacity() const;

      bool isNominalCapacityDefaulted() const;

      bool isNominalCapacityAutosized() const;

      boost::optional<Node> temperatureSetpointNode() const;

      boost::optional<double> autosizedNominalCapacity() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      bool setEfficiency(double efficiency);

      void resetEfficiency();

      bool setNominalCapacity(boost::optional<double> nominalCapacity);

      void resetNominalCapacity();

      void autosizeNominalCapacity();

      bool setTemperatureSetpointNode(Node& temperatureSetpointNode);

      void resetTemperatureSetpointNode();

      //@}

      /** Creates a new equivalent duct object if an object is not already attached. */
      AirflowNetworkEquivalentDuct getAirflowNetworkEquivalentDuct(double length, double diameter);

      /** Returns the attached equivalent duct object, if any. */
      boost::optional<AirflowNetworkEquivalentDuct> airflowNetworkEquivalentDuct() const;

     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilHeatingElectric");

      boost::optional<ModelObject> availabilityScheduleAsModelObject() const;

      bool setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILHEATINGELECTRIC_IMPL_HPP
