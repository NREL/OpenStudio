/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILCOOLINGDXCURVEFITPERFORMANCE_HPP
#define MODEL_COILCOOLINGDXCURVEFITPERFORMANCE_HPP

#include <model/ModelAPI.hpp>
#include "ResourceObject.hpp"

namespace openstudio {

namespace model {

  class CoilCoolingDX;
  class CoilCoolingDXCurveFitOperatingMode;
  class Curve;
  class Schedule;

  namespace detail {

    class CoilCoolingDXCurveFitPerformance_Impl;

  }  // namespace detail

  /** CoilCoolingDXCurveFitPerformance is a ResourceObject that wraps the OpenStudio IDD object 'OS:Coil:Cooling:DX:CurveFit:Performance'. */
  class MODEL_API CoilCoolingDXCurveFitPerformance : public ResourceObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CoilCoolingDXCurveFitPerformance(const Model& model, const CoilCoolingDXCurveFitOperatingMode& baseOperatingMode);

    virtual ~CoilCoolingDXCurveFitPerformance() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilCoolingDXCurveFitPerformance(const CoilCoolingDXCurveFitPerformance& other) = default;
    CoilCoolingDXCurveFitPerformance(CoilCoolingDXCurveFitPerformance&& other) = default;
    CoilCoolingDXCurveFitPerformance& operator=(const CoilCoolingDXCurveFitPerformance&) = default;
    CoilCoolingDXCurveFitPerformance& operator=(CoilCoolingDXCurveFitPerformance&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> capacityControlMethodValues();

    static std::vector<std::string> compressorFuelTypeValues();

    static std::vector<std::string> validCapacityControlMethodValues();

    static std::vector<std::string> validCompressorFuelTypeValues();

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
    /// @cond
    using ImplType = detail::CoilCoolingDXCurveFitPerformance_Impl;

    explicit CoilCoolingDXCurveFitPerformance(std::shared_ptr<detail::CoilCoolingDXCurveFitPerformance_Impl> impl);

    friend class detail::CoilCoolingDXCurveFitPerformance_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilCoolingDXCurveFitPerformance");
  };

  /** \relates CoilCoolingDXCurveFitPerformance*/
  using OptionalCoilCoolingDXCurveFitPerformance = boost::optional<CoilCoolingDXCurveFitPerformance>;

  /** \relates CoilCoolingDXCurveFitPerformance*/
  using CoilCoolingDXCurveFitPerformanceVector = std::vector<CoilCoolingDXCurveFitPerformance>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILCOOLINGDXCURVEFITPERFORMANCE_HPP
