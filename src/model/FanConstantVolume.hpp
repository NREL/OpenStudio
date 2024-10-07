/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_FANCONSTANTVOLUME_HPP
#define MODEL_FANCONSTANTVOLUME_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"
#include "StraightComponent.hpp"
#include "Connection.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class AirflowNetworkFan;

  namespace detail {
    class FanConstantVolume_Impl;
  }  // namespace detail

  /** FanConstantVolume is a StraightComponent that wraps the IDD object named
 *  "OS:Fan:ConstantVolume". */
  class MODEL_API FanConstantVolume : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Constructs a new FanConstantVolume object and places it inside the
   *  model.  The object is fully initialized with all companion objects. */
    FanConstantVolume(const Model& model, Schedule& s);

    FanConstantVolume(const Model& model);

    virtual ~FanConstantVolume() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    FanConstantVolume(const FanConstantVolume& other) = default;
    FanConstantVolume(FanConstantVolume&& other) = default;
    FanConstantVolume& operator=(const FanConstantVolume&) = default;
    FanConstantVolume& operator=(FanConstantVolume&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

    /** Returns the value of the Fan Total Efficiency field **/
    double fanTotalEfficiency() const;

    /** Returns the value of the Fan Total Efficiency field. Deprecated, forwards to fanTotalEfficiency **/
    double fanEfficiency() const;

    /** Returns the value of the PressureRise field. **/
    double pressureRise() const;

    /** Returns the value of the MotorEfficiency field. **/
    double motorEfficiency() const;

    /** Returns the value of the MotorInAirStreamFraction field. **/
    double motorInAirstreamFraction() const;

    /** Returns the value of the EndUseSubcategory field. **/
    std::string endUseSubcategory() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& s);

    /** Sets the value of the Fan Total Efficiency field. **/
    bool setFanTotalEfficiency(double value);

    /** Sets the value of the Fan Total Efficiency field. Deprecated, forwards to setFanTotalEfficiency. **/
    bool setFanEfficiency(double value);

    /** Sets the value of the PressureRise field. **/
    bool setPressureRise(double value);

    /** Sets the value of the MotorEfficiency field. **/
    bool setMotorEfficiency(double value);

    /** Sets the value of the MotorInAirStreamFraction field. **/
    bool setMotorInAirstreamFraction(double value);

    /** Sets the value of the EndUseSubcategory field. **/
    bool setEndUseSubcategory(const std::string& value);

    //@}

    boost::optional<double> maximumFlowRate() const;

    bool isMaximumFlowRateAutosized() const;

    bool setMaximumFlowRate(double maximumFlowRate);

    void resetMaximumFlowRate();

    void autosizeMaximumFlowRate();

    AirflowNetworkFan getAirflowNetworkFan();
    boost::optional<AirflowNetworkFan> airflowNetworkFan() const;

    boost::optional<double> autosizedMaximumFlowRate() const;

   protected:
    friend class Model;
    friend class openstudio::IdfObject;

    /// @cond
    using ImplType = detail::FanConstantVolume_Impl;
    explicit FanConstantVolume(std::shared_ptr<detail::FanConstantVolume_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.FanConstantVolume");
    /// @endcond
  };

  /** \relates FanConstantVolume */
  using OptionalFanConstantVolume = boost::optional<FanConstantVolume>;

  /** \relates FanConstantVolume */
  using FanConstantVolumeVector = std::vector<FanConstantVolume>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_FANCONSTANTVOLUME_HPP
