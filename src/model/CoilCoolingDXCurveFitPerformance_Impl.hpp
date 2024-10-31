/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILCOOLINGDXCURVEFITPERFORMANCE_IMPL_HPP
#define MODEL_COILCOOLINGDXCURVEFITPERFORMANCE_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "ResourceObject_Impl.hpp"

namespace openstudio {
namespace model {

  class CoilCoolingDX;
  class CoilCoolingDXCurveFitOperatingMode;
  class Curve;
  class Schedule;

  namespace detail {

    /** CoilCoolingDXCurveFitPerformance_Impl is a ResourceObject_Impl that is the implementation class for CoilCoolingDXCurveFitPerformance.*/
    class MODEL_API CoilCoolingDXCurveFitPerformance_Impl : public ResourceObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilCoolingDXCurveFitPerformance_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilCoolingDXCurveFitPerformance_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilCoolingDXCurveFitPerformance_Impl(const CoilCoolingDXCurveFitPerformance_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilCoolingDXCurveFitPerformance_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual ModelObject clone(Model model) const override;

      virtual std::vector<ModelObject> children() const override;

      // If this object is still used by at least one CoilCoolingDX, refuse to remove to avoid putting the CoilCoolingDX(s) in a broken state
      virtual std::vector<IdfObject> remove() override;

      //@}
      /** @name Getters */
      //@{

      double crankcaseHeaterCapacity() const;

      boost::optional<Curve> crankcaseHeaterCapacityFunctionofTemperatureCurve() const;

      double minimumOutdoorDryBulbTemperatureforCompressorOperation() const;

      double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation() const;

      double unitInternalStaticAirPressure() const;

      std::string capacityControlMethod() const;

      double evaporativeCondenserBasinHeaterCapacity() const;

      double evaporativeCondenserBasinHeaterSetpointTemperature() const;

      Schedule evaporativeCondenserBasinHeaterOperatingSchedule() const;

      std::string compressorFuelType() const;

      CoilCoolingDXCurveFitOperatingMode baseOperatingMode() const;

      boost::optional<CoilCoolingDXCurveFitOperatingMode> alternativeOperatingMode1() const;

      boost::optional<CoilCoolingDXCurveFitOperatingMode> alternativeOperatingMode2() const;

      std::vector<CoilCoolingDX> coilCoolingDXs() const;

      //@}
      /** @name Setters */
      //@{

      bool setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity);

      bool setCrankcaseHeaterCapacityFunctionofTemperatureCurve(const Curve& curve);
      void resetCrankcaseHeaterCapacityFunctionofTemperatureCurve();

      bool setMinimumOutdoorDryBulbTemperatureforCompressorOperation(double minimumOutdoorDryBulbTemperatureforCompressorOperation);

      bool setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);

      bool setUnitInternalStaticAirPressure(double unitInternalStaticAirPressure);

      bool setCapacityControlMethod(const std::string& capacityControlMethod);

      bool setEvaporativeCondenserBasinHeaterCapacity(double evaporativeCondenserBasinHeaterCapacity);

      bool setEvaporativeCondenserBasinHeaterSetpointTemperature(double evaporativeCondenserBasinHeaterSetpointTemperature);

      bool setEvaporativeCondenserBasinHeaterOperatingSchedule(Schedule& schedule);

      bool setCompressorFuelType(const std::string& compressorFuelType);

      bool setBaseOperatingMode(const CoilCoolingDXCurveFitOperatingMode& baseOperatingMode);

      bool setAlternativeOperatingMode1(const CoilCoolingDXCurveFitOperatingMode& alternativeOperatingMode1);

      void resetAlternativeOperatingMode1();

      bool setAlternativeOperatingMode2(const CoilCoolingDXCurveFitOperatingMode& alternativeOperatingMode2);

      void resetAlternativeOperatingMode2();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilCoolingDXCurveFitPerformance");

      boost::optional<CoilCoolingDXCurveFitOperatingMode> optionalBaseOperatingMode() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILCOOLINGDXCURVEFITPERFORMANCE_IMPL_HPP
