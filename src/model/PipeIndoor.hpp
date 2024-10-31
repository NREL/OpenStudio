/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PIPEINDOOR_HPP
#define MODEL_PIPEINDOOR_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class Construction;
  class Schedule;
  class ThermalZone;

  namespace detail {

    class PipeIndoor_Impl;

  }  // namespace detail

  /** PipeIndoor is a  StraightComponent that wraps the OpenStudio IDD object 'OS:Pipe:Indoor'. */
  class MODEL_API PipeIndoor : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit PipeIndoor(const Model& model);

    virtual ~PipeIndoor() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    PipeIndoor(const PipeIndoor& other) = default;
    PipeIndoor(PipeIndoor&& other) = default;
    PipeIndoor& operator=(const PipeIndoor&) = default;
    PipeIndoor& operator=(PipeIndoor&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> environmentTypeValues();

    /** @name Getters */
    //@{

    boost::optional<Construction> construction() const;

    std::string environmentType() const;

    boost::optional<ThermalZone> ambientTemperatureZone() const;

    boost::optional<Schedule> ambientTemperatureSchedule() const;

    boost::optional<Schedule> ambientAirVelocitySchedule() const;

    double pipeInsideDiameter() const;

    double pipeLength() const;

    //@}
    /** @name Setters */
    //@{

    bool setConstruction(const Construction& construction);

    void resetConstruction();

    bool setEnvironmentType(const std::string& environmentType);

    bool setAmbientTemperatureZone(const ThermalZone& thermalZone);

    void resetAmbientTemperatureZone();

    bool setAmbientTemperatureSchedule(Schedule& schedule);

    void resetAmbientTemperatureSchedule();

    bool setAmbientAirVelocitySchedule(Schedule& schedule);

    void resetAmbientAirVelocitySchedule();

    bool setPipeInsideDiameter(double pipeInsideDiameter);

    bool setPipeLength(double pipeLength);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::PipeIndoor_Impl;

    explicit PipeIndoor(std::shared_ptr<detail::PipeIndoor_Impl> impl);

    friend class detail::PipeIndoor_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.PipeIndoor");
  };

  /** \relates PipeIndoor*/
  using OptionalPipeIndoor = boost::optional<PipeIndoor>;

  /** \relates PipeIndoor*/
  using PipeIndoorVector = std::vector<PipeIndoor>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PIPEINDOOR_HPP
