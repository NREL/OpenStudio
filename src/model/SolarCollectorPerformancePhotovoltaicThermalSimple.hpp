/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SOLARCOLLECTORPERFORMANCEPHOTOVOLTAICTHERMALSIMPLE_HPP
#define MODEL_SOLARCOLLECTORPERFORMANCEPHOTOVOLTAICTHERMALSIMPLE_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  class SolarCollectorFlatPlatePhotovoltaicThermal;
  class Schedule;

  namespace detail {

    class SolarCollectorFlatPlatePhotovoltaicThermal_Impl;
    class SolarCollectorPerformancePhotovoltaicThermalSimple_Impl;

  }  // namespace detail

  /** SolarCollectorPerformancePhotovoltaicThermalSimple is a ModelObject that wraps the OpenStudio IDD object 'OS:SolarCollectorPerformance:PhotovoltaicThermal:Simple'. */
  class MODEL_API SolarCollectorPerformancePhotovoltaicThermalSimple : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SolarCollectorPerformancePhotovoltaicThermalSimple(const Model& model);

    virtual ~SolarCollectorPerformancePhotovoltaicThermalSimple() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SolarCollectorPerformancePhotovoltaicThermalSimple(const SolarCollectorPerformancePhotovoltaicThermalSimple& other) = default;
    SolarCollectorPerformancePhotovoltaicThermalSimple(SolarCollectorPerformancePhotovoltaicThermalSimple&& other) = default;
    SolarCollectorPerformancePhotovoltaicThermalSimple& operator=(const SolarCollectorPerformancePhotovoltaicThermalSimple&) = default;
    SolarCollectorPerformancePhotovoltaicThermalSimple& operator=(SolarCollectorPerformancePhotovoltaicThermalSimple&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> thermalConversionEfficiencyInputModeTypeValues();

    /** @name Getters */
    //@{

    double fractionOfSurfaceAreaWithActiveThermalCollector() const;

    std::string thermalConversionEfficiencyInputModeType() const;

    boost::optional<double> thermalConversionEfficiency() const;

    boost::optional<Schedule> thermalConversionEfficiencySchedule() const;

    double frontSurfaceEmittance() const;

    bool isFrontSurfaceEmittanceDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setFractionOfSurfaceAreaWithActiveThermalCollector(double fractionofSurfaceAreawithActiveThermalCollector);

    bool setThermalConversionEfficiency(double valueforThermalConversionEfficiencyifFixed);

    void resetThermalConversionEfficiency();

    bool setThermalConversionEfficiencySchedule(Schedule& schedule);

    void resetThermalConversionEfficiencySchedule();

    bool setFrontSurfaceEmittance(double frontSurfaceEmittance);

    void resetFrontSurfaceEmittance();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::SolarCollectorPerformancePhotovoltaicThermalSimple_Impl;

    explicit SolarCollectorPerformancePhotovoltaicThermalSimple(
      std::shared_ptr<detail::SolarCollectorPerformancePhotovoltaicThermalSimple_Impl> impl);

    friend class detail::SolarCollectorPerformancePhotovoltaicThermalSimple_Impl;
    friend class detail::SolarCollectorFlatPlatePhotovoltaicThermal_Impl;
    friend class SolarCollectorFlatPlatePhotovoltaicThermal;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SolarCollectorPerformancePhotovoltaicThermalSimple");
  };

  /** \relates SolarCollectorPerformancePhotovoltaicThermalSimple*/
  using OptionalSolarCollectorPerformancePhotovoltaicThermalSimple = boost::optional<SolarCollectorPerformancePhotovoltaicThermalSimple>;

  /** \relates SolarCollectorPerformancePhotovoltaicThermalSimple*/
  using SolarCollectorPerformancePhotovoltaicThermalSimpleVector = std::vector<SolarCollectorPerformancePhotovoltaicThermalSimple>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SOLARCOLLECTORPERFORMANCEPHOTOVOLTAICTHERMALSIMPLE_HPP
