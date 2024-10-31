/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILHEATINGLOWTEMPRADIANTVARFLOW_IMPL_HPP
#define MODEL_COILHEATINGLOWTEMPRADIANTVARFLOW_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class ZoneHVACLowTempRadiantVarFlow;

  namespace detail {

    /** CoilHeatingLowTempRadiantVarFlow_Impl is a StraightComponent_Impl that is the implementation class for CoilHeatingLowTempRadiantVarFlow.*/
    class MODEL_API CoilHeatingLowTempRadiantVarFlow_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilHeatingLowTempRadiantVarFlow_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilHeatingLowTempRadiantVarFlow_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilHeatingLowTempRadiantVarFlow_Impl(const CoilHeatingLowTempRadiantVarFlow_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilHeatingLowTempRadiantVarFlow_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual bool addToNode(Node& node) override;

      virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const override;

      //virtual ModelObject clone(Model model) const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<double> maximumHotWaterFlow() const;

      bool isMaximumHotWaterFlowDefaulted() const;

      bool isMaximumHotWaterFlowAutosized() const;

      //Connection heatingWaterInletNode() const;

      //Connection heatingWaterOutletNode() const;

      double heatingControlThrottlingRange() const;

      bool isHeatingControlThrottlingRangeDefaulted() const;

      boost::optional<Schedule> heatingControlTemperatureSchedule() const;

      std::string heatingDesignCapacityMethod() const;

      boost::optional<double> heatingDesignCapacity() const;
      bool isHeatingDesignCapacityAutosized() const;

      double heatingDesignCapacityPerFloorArea() const;

      double fractionofAutosizedHeatingDesignCapacity() const;

      //@}
      /** @name Setters */
      //@{

      bool setMaximumHotWaterFlow(boost::optional<double> maximumHotWaterFlow);

      void resetMaximumHotWaterFlow();

      void autosizeMaximumHotWaterFlow();

      bool setHeatingControlThrottlingRange(double heatingControlThrottlingRange);

      void resetHeatingControlThrottlingRange();

      bool setHeatingControlTemperatureSchedule(Schedule& schedule);

      void resetHeatingControlTemperatureSchedule();

      bool setHeatingDesignCapacityMethod(const std::string& heatingDesignCapacityMethod);

      bool setHeatingDesignCapacity(double heatingDesignCapacity);
      void autosizeHeatingDesignCapacity();

      bool setHeatingDesignCapacityPerFloorArea(double heatingDesignCapacityPerFloorArea);

      bool setFractionofAutosizedHeatingDesignCapacity(double fractionofAutosizedHeatingDesignCapacity);

      //@}
      /** @name Other */
      //@{

      // Used to find the ZoneHVAC that contains this coil
      boost::optional<ZoneHVACLowTempRadiantVarFlow> parentZoneHVAC() const;

      boost::optional<double> autosizedMaximumHotWaterFlow() const;

      boost::optional<double> autosizedHeatingDesignCapacity() const;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilHeatingLowTempRadiantVarFlow");

      boost::optional<ModelObject> heatingControlTemperatureScheduleAsModelObject() const;

      bool setHeatingControlTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILHEATINGLOWTEMPRADIANTVARFLOW_IMPL_HPP
