/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILCOOLINGLOWTEMPRADIANTVARFLOW_IMPL_HPP
#define MODEL_COILCOOLINGLOWTEMPRADIANTVARFLOW_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class ZoneHVACLowTempRadiantVarFlow;

  namespace detail {

    /** CoilCoolingLowTempRadiantVarFlow_Impl is a StraightComponent_Impl that is the implementation class for CoilCoolingLowTempRadiantVarFlow.*/
    class MODEL_API CoilCoolingLowTempRadiantVarFlow_Impl : public StraightComponent_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      CoilCoolingLowTempRadiantVarFlow_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilCoolingLowTempRadiantVarFlow_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilCoolingLowTempRadiantVarFlow_Impl(const CoilCoolingLowTempRadiantVarFlow_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilCoolingLowTempRadiantVarFlow_Impl() override = default;

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

      virtual void autosize() override;

      virtual void applySizingValues() override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<double> maximumColdWaterFlow() const;

      bool isMaximumColdWaterFlowDefaulted() const;

      bool isMaximumColdWaterFlowAutosized() const;

      double coolingControlThrottlingRange() const;

      bool isCoolingControlThrottlingRangeDefaulted() const;

      boost::optional<Schedule> coolingControlTemperatureSchedule() const;

      std::string condensationControlType() const;

      bool isCondensationControlTypeDefaulted() const;

      double condensationControlDewpointOffset() const;

      bool isCondensationControlDewpointOffsetDefaulted() const;

      std::string coolingDesignCapacityMethod() const;

      boost::optional<double> coolingDesignCapacity() const;
      bool isCoolingDesignCapacityAutosized() const;

      double coolingDesignCapacityPerFloorArea() const;

      double fractionofAutosizedCoolingDesignCapacity() const;

      //@}
      /** @name Setters */
      //@{

      bool setMaximumColdWaterFlow(boost::optional<double> maximumColdWaterFlow);

      void resetMaximumColdWaterFlow();

      void autosizeMaximumColdWaterFlow();

      bool setCoolingControlThrottlingRange(double coolingControlThrottlingRange);

      void resetCoolingControlThrottlingRange();

      bool setCoolingControlTemperatureSchedule(Schedule& schedule);

      void resetCoolingControlTemperatureSchedule();

      bool setCondensationControlType(const std::string& condensationControlType);

      void resetCondensationControlType();

      bool setCondensationControlDewpointOffset(double condensationControlDewpointOffset);

      void resetCondensationControlDewpointOffset();

      bool setCoolingDesignCapacityMethod(const std::string& coolingDesignCapacityMethod);

      bool setCoolingDesignCapacity(double coolingDesignCapacity);
      void autosizeCoolingDesignCapacity();

      bool setCoolingDesignCapacityPerFloorArea(double coolingDesignCapacityPerFloorArea);

      bool setFractionofAutosizedCoolingDesignCapacity(double fractionofAutosizedCoolingDesignCapacity);

      //@}
      /** @name Other */
      //@{

      // Used to find the ZoneHVAC that contains this coil
      boost::optional<ZoneHVACLowTempRadiantVarFlow> parentZoneHVAC() const;

      boost::optional<double> autosizedMaximumColdWaterFlow() const;

      boost::optional<double> autosizedCoolingDesignCapacity() const;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilCoolingLowTempRadiantVarFlow");

      std::vector<std::string> condensationControlTypeValues() const;

      //boost::optional<ModelObject> coolingWaterInletNodeAsModelObject() const;
      //boost::optional<ModelObject> coolingWaterOutletNodeAsModelObject() const;
      boost::optional<ModelObject> coolingControlTemperatureScheduleAsModelObject() const;

      //bool setCoolingWaterInletNodeAsModelObject(const boost::optional<ModelObject>& modelObject);
      //bool setCoolingWaterOutletNodeAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setCoolingControlTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILCOOLINGLOWTEMPRADIANTVARFLOW_IMPL_HPP
