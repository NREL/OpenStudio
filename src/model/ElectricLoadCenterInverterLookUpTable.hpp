/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ELECTRICLOADCENTERINVERTERLOOKUPTABLE_HPP
#define MODEL_ELECTRICLOADCENTERINVERTERLOOKUPTABLE_HPP

#include "ModelAPI.hpp"
#include "Inverter.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class ThermalZone;

  namespace detail {

    class ElectricLoadCenterInverterLookUpTable_Impl;

  }  // namespace detail

  /** ElectricLoadCenterInverterLookUpTable is a Inverter that wraps the OpenStudio IDD object 'OS:ElectricLoadCenter:Inverter:LookUpTable'. */
  class MODEL_API ElectricLoadCenterInverterLookUpTable : public Inverter
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ElectricLoadCenterInverterLookUpTable(const Model& model);

    virtual ~ElectricLoadCenterInverterLookUpTable() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ElectricLoadCenterInverterLookUpTable(const ElectricLoadCenterInverterLookUpTable& other) = default;
    ElectricLoadCenterInverterLookUpTable(ElectricLoadCenterInverterLookUpTable&& other) = default;
    ElectricLoadCenterInverterLookUpTable& operator=(const ElectricLoadCenterInverterLookUpTable&) = default;
    ElectricLoadCenterInverterLookUpTable& operator=(ElectricLoadCenterInverterLookUpTable&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    boost::optional<Schedule> availabilitySchedule() const;

    boost::optional<double> radiativeFraction() const;

    boost::optional<double> ratedMaximumContinuousOutputPower() const;

    boost::optional<double> nightTareLossPower() const;

    boost::optional<double> nominalVoltageInput() const;

    // DLM: the IDD for these fields looks weird, there is no default and the field is not required but I don't
    // see how the object would work without values for these fields

    boost::optional<double> efficiencyAt10PowerAndNominalVoltage() const;

    boost::optional<double> efficiencyAt20PowerAndNominalVoltage() const;

    boost::optional<double> efficiencyAt30PowerAndNominalVoltage() const;

    boost::optional<double> efficiencyAt50PowerAndNominalVoltage() const;

    boost::optional<double> efficiencyAt75PowerAndNominalVoltage() const;

    boost::optional<double> efficiencyAt100PowerAndNominalVoltage() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    bool setRadiativeFraction(double radiativeFraction);

    void resetRadiativeFraction();

    bool setRatedMaximumContinuousOutputPower(double ratedMaximumContinuousOutputPower);

    void resetRatedMaximumContinuousOutputPower();

    bool setNightTareLossPower(double nightTareLossPower);

    void resetNightTareLossPower();

    bool setNominalVoltageInput(double nominalVoltageInput);

    void resetNominalVoltageInput();

    bool setEfficiencyAt10PowerAndNominalVoltage(double efficiencyAt10PowerAndNominalVoltage);

    void resetEfficiencyAt10PowerAndNominalVoltage();

    bool setEfficiencyAt20PowerAndNominalVoltage(double efficiencyAt20PowerAndNominalVoltage);

    void resetEfficiencyAt20PowerAndNominalVoltage();

    bool setEfficiencyAt30PowerAndNominalVoltage(double efficiencyAt30PowerAndNominalVoltage);

    void resetEfficiencyAt30PowerAndNominalVoltage();

    bool setEfficiencyAt50PowerAndNominalVoltage(double efficiencyAt50PowerAndNominalVoltage);

    void resetEfficiencyAt50PowerAndNominalVoltage();

    bool setEfficiencyAt75PowerAndNominalVoltage(double efficiencyAt75PowerAndNominalVoltage);

    void resetEfficiencyAt75PowerAndNominalVoltage();

    bool setEfficiencyAt100PowerAndNominalVoltage(double efficiencyAt100PowerAndNominalVoltage);

    void resetEfficiencyAt100PowerAndNominalVoltage();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::ElectricLoadCenterInverterLookUpTable_Impl;

    explicit ElectricLoadCenterInverterLookUpTable(std::shared_ptr<detail::ElectricLoadCenterInverterLookUpTable_Impl> impl);

    friend class detail::ElectricLoadCenterInverterLookUpTable_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ElectricLoadCenterInverterLookUpTable");
  };

  /** \relates ElectricLoadCenterInverterLookUpTable*/
  using OptionalElectricLoadCenterInverterLookUpTable = boost::optional<ElectricLoadCenterInverterLookUpTable>;

  /** \relates ElectricLoadCenterInverterLookUpTable*/
  using ElectricLoadCenterInverterLookUpTableVector = std::vector<ElectricLoadCenterInverterLookUpTable>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ELECTRICLOADCENTERINVERTERLOOKUPTABLE_HPP
