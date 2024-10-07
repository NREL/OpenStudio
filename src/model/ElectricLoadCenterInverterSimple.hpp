/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ELECTRICLOADCENTERINVERTERSIMPLE_HPP
#define MODEL_ELECTRICLOADCENTERINVERTERSIMPLE_HPP

#include "ModelAPI.hpp"
#include "Inverter.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class ThermalZone;

  namespace detail {

    class ElectricLoadCenterInverterSimple_Impl;

  }  // namespace detail

  /** ElectricLoadCenterInverterSimple is a Inverter that wraps the OpenStudio IDD object 'OS:ElectricLoadCenter:Inverter:Simple'. */
  class MODEL_API ElectricLoadCenterInverterSimple : public Inverter
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ElectricLoadCenterInverterSimple(const Model& model);

    virtual ~ElectricLoadCenterInverterSimple() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ElectricLoadCenterInverterSimple(const ElectricLoadCenterInverterSimple& other) = default;
    ElectricLoadCenterInverterSimple(ElectricLoadCenterInverterSimple&& other) = default;
    ElectricLoadCenterInverterSimple& operator=(const ElectricLoadCenterInverterSimple&) = default;
    ElectricLoadCenterInverterSimple& operator=(ElectricLoadCenterInverterSimple&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    boost::optional<Schedule> availabilitySchedule() const;

    boost::optional<double> radiativeFraction() const;

    boost::optional<double> inverterEfficiency() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    bool setRadiativeFraction(double radiativeFraction);

    void resetRadiativeFraction();

    bool setInverterEfficiency(double inverterEfficiency);

    void resetInverterEfficiency();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::ElectricLoadCenterInverterSimple_Impl;

    explicit ElectricLoadCenterInverterSimple(std::shared_ptr<detail::ElectricLoadCenterInverterSimple_Impl> impl);

    friend class detail::ElectricLoadCenterInverterSimple_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ElectricLoadCenterInverterSimple");
  };

  /** \relates ElectricLoadCenterInverterSimple*/
  using OptionalElectricLoadCenterInverterSimple = boost::optional<ElectricLoadCenterInverterSimple>;

  /** \relates ElectricLoadCenterInverterSimple*/
  using ElectricLoadCenterInverterSimpleVector = std::vector<ElectricLoadCenterInverterSimple>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ELECTRICLOADCENTERINVERTERSIMPLE_HPP
