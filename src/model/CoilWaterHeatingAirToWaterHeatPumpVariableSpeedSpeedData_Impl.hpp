/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILWATERHEATINGAIRTOWATERHEATPUMPVARIABLESPEEDSPEEDDATA_IMPL_HPP
#define MODEL_COILWATERHEATINGAIRTOWATERHEATPUMPVARIABLESPEEDSPEEDDATA_IMPL_HPP

#include "ModelAPI.hpp"
#include "ParentObject_Impl.hpp"

namespace openstudio {
namespace model {

  class Curve;

  namespace detail {

    /** CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl is a ParentObject_Impl that is the implementation class for CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData.*/
    class MODEL_API CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl : public ParentObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                    bool keepHandle);

      CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl(const CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl& other,
                                                                    Model_Impl* model, bool keepHandle);

      virtual ~CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual ModelObject clone(Model model) const override;

      virtual std::vector<ModelObject> children() const override;

      //@}
      /** @name Getters */
      //@{

      double ratedWaterHeatingCapacity() const;

      double ratedWaterHeatingCOP() const;

      double ratedSensibleHeatRatio() const;

      double referenceUnitRatedAirFlowRate() const;

      double referenceUnitRatedWaterFlowRate() const;

      double referenceUnitWaterPumpInputPowerAtRatedConditions() const;

      Curve totalWaterHeatingCapacityFunctionofTemperatureCurve() const;

      Curve totalWaterHeatingCapacityFunctionofAirFlowFractionCurve() const;

      Curve totalWaterHeatingCapacityFunctionofWaterFlowFractionCurve() const;

      Curve copFunctionofTemperatureCurve() const;

      Curve copFunctionofAirFlowFractionCurve() const;

      Curve copFunctionofWaterFlowFractionCurve() const;

      //@}
      /** @name Setters */
      //@{

      bool setRatedWaterHeatingCapacity(double ratedWaterHeatingCapacity);

      bool setRatedWaterHeatingCOP(double ratedWaterHeatingCOP);

      bool setRatedSensibleHeatRatio(double ratedSensibleHeatRatio);

      bool setReferenceUnitRatedAirFlowRate(double referenceUnitRatedAirFlowRate);

      bool setReferenceUnitRatedWaterFlowRate(double referenceUnitRatedWaterFlowRate);

      bool setReferenceUnitWaterPumpInputPowerAtRatedConditions(double referenceUnitWaterPumpInputPowerAtRatedConditions);

      bool setTotalWaterHeatingCapacityFunctionofTemperatureCurve(const Curve& curve);

      bool setTotalWaterHeatingCapacityFunctionofAirFlowFractionCurve(const Curve& curve);

      bool setTotalWaterHeatingCapacityFunctionofWaterFlowFractionCurve(const Curve& curve);

      bool setCOPFunctionofTemperatureCurve(const Curve& curve);

      bool setCOPFunctionofAirFlowFractionCurve(const Curve& curve);

      bool setCOPFunctionofWaterFlowFractionCurve(const Curve& curve);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData");

      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<Curve> optionalTotalWaterHeatingCapacityFunctionofTemperatureCurve() const;
      boost::optional<Curve> optionalTotalWaterHeatingCapacityFunctionofAirFlowFractionCurve() const;
      boost::optional<Curve> optionalTotalWaterHeatingCapacityFunctionofWaterFlowFractionCurve() const;
      boost::optional<Curve> optionalCOPFunctionofTemperatureCurve() const;
      boost::optional<Curve> optionalCOPFunctionofAirFlowFractionCurve() const;
      boost::optional<Curve> optionalCOPFunctionofWaterFlowFractionCurve() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILWATERHEATINGAIRTOWATERHEATPUMPVARIABLESPEEDSPEEDDATA_IMPL_HPP
