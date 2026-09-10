/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILHEATINGSTEAM_HPP
#define MODEL_COILHEATINGSTEAM_HPP

#include "ModelAPI.hpp"
#include "WaterToAirComponent.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class ControllerWaterCoil;

  namespace detail {

    class CoilHeatingSteam_Impl;

  }  // namespace detail

  /** CoilHeatingSteam is a WaterToAirComponent that wraps the IDD object named "OS:Coil:Heating:Steam". */
  class MODEL_API CoilHeatingSteam : public WaterToAirComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    CoilHeatingSteam(const Model& model, Schedule& schedule);

    CoilHeatingSteam(const Model& model);

    virtual ~CoilHeatingSteam() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilHeatingSteam(const CoilHeatingSteam& other) = default;
    CoilHeatingSteam(CoilHeatingSteam&& other) = default;
    CoilHeatingSteam& operator=(const CoilHeatingSteam&) = default;
    CoilHeatingSteam& operator=(CoilHeatingSteam&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

    boost::optional<double> maximumSteamFlowRate();

    bool isMaximumSteamFlowRateAutosized();

    double degreeofSubCooling() const;

    double degreeofLoopSubCooling() const;

    std::string coilControlType() const;

    boost::optional<Node> temperatureSetpointNode() const;

    boost::optional<ControllerWaterCoil> controllerWaterCoil();

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    bool setMaximumSteamFlowRate(double value);

    void autosizeMaximumSteamFlowRate();

    bool setDegreeofSubCooling(double degreeofSubCooling);

    bool setDegreeofLoopSubCooling(double degreeofLoopSubCooling);

    bool setCoilControlType(const std::string& coilControlType);

    bool setTemperatureSetpointNode(Node& temperatureSetpointNode);

    void resetTemperatureSetpointNode();

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedMaximumSteamFlowRate() const;

    //@}
   protected:
    friend class Model;

    friend class openstudio::IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    /// @cond
    using ImplType = detail::CoilHeatingSteam_Impl;

    explicit CoilHeatingSteam(std::shared_ptr<detail::CoilHeatingSteam_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.CoilHeatingSteam");

    CoilHeatingSteam(const Handle& handle, const Model& model);

    /// @endcond
  };  // detail

  using OptionalCoilHeatingSteam = boost::optional<CoilHeatingSteam>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILHEATINGSTEAM_HPP
