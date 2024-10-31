/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PLANTCOMPONENTTEMPERATURESOURCE_HPP
#define MODEL_PLANTCOMPONENTTEMPERATURESOURCE_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;

  namespace detail {

    class PlantComponentTemperatureSource_Impl;

  }  // namespace detail

  /** PlantComponentTemperatureSource is a StraightComponent that wraps the OpenStudio IDD object 'OS:PlantComponent:TemperatureSource'. */
  class MODEL_API PlantComponentTemperatureSource : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit PlantComponentTemperatureSource(const Model& model);

    virtual ~PlantComponentTemperatureSource() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    PlantComponentTemperatureSource(const PlantComponentTemperatureSource& other) = default;
    PlantComponentTemperatureSource(PlantComponentTemperatureSource&& other) = default;
    PlantComponentTemperatureSource& operator=(const PlantComponentTemperatureSource&) = default;
    PlantComponentTemperatureSource& operator=(PlantComponentTemperatureSource&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> temperatureSpecificationTypeValues();

    /** @name Getters */
    //@{

    boost::optional<double> designVolumeFlowRate() const;

    bool isDesignVolumeFlowRateAutosized() const;

    std::string temperatureSpecificationType() const;

    boost::optional<double> sourceTemperature() const;

    boost::optional<Schedule> sourceTemperatureSchedule() const;

    //@}
    /** @name Setters */
    //@{

    bool setDesignVolumeFlowRate(double designVolumeFlowRate);

    void autosizeDesignVolumeFlowRate();

    bool setTemperatureSpecificationType(const std::string& temperatureSpecificationType);

    bool setSourceTemperature(double sourceTemperature);

    void resetSourceTemperature();

    bool setSourceTemperatureSchedule(Schedule& schedule);

    void resetSourceTemperatureSchedule();

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedDesignVolumeFlowRate() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::PlantComponentTemperatureSource_Impl;

    explicit PlantComponentTemperatureSource(std::shared_ptr<detail::PlantComponentTemperatureSource_Impl> impl);

    friend class detail::PlantComponentTemperatureSource_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.PlantComponentTemperatureSource");
  };

  /** \relates PlantComponentTemperatureSource*/
  using OptionalPlantComponentTemperatureSource = boost::optional<PlantComponentTemperatureSource>;

  /** \relates PlantComponentTemperatureSource*/
  using PlantComponentTemperatureSourceVector = std::vector<PlantComponentTemperatureSource>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PLANTCOMPONENTTEMPERATURESOURCE_HPP
