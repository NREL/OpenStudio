/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ELECTRICLOADCENTERINVERTERPVWATTS_HPP
#define MODEL_ELECTRICLOADCENTERINVERTERPVWATTS_HPP

#include "ModelAPI.hpp"
#include "Inverter.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class ThermalZone;

  namespace detail {

    class ElectricLoadCenterInverterPVWatts_Impl;

  }  // namespace detail

  /** ElectricLoadCenterInverterPVWatts is a Inverter that wraps the OpenStudio IDD object 'OS:ElectricLoadCenter:Inverter:PVWatts'. */
  class MODEL_API ElectricLoadCenterInverterPVWatts : public Inverter
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ElectricLoadCenterInverterPVWatts(const Model& model);

    virtual ~ElectricLoadCenterInverterPVWatts() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ElectricLoadCenterInverterPVWatts(const ElectricLoadCenterInverterPVWatts& other) = default;
    ElectricLoadCenterInverterPVWatts(ElectricLoadCenterInverterPVWatts&& other) = default;
    ElectricLoadCenterInverterPVWatts& operator=(const ElectricLoadCenterInverterPVWatts&) = default;
    ElectricLoadCenterInverterPVWatts& operator=(ElectricLoadCenterInverterPVWatts&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    double dcToACSizeRatio() const;

    bool isDCToACSizeRatioDefaulted() const;

    double inverterEfficiency() const;

    bool isInverterEfficiencyDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setDCToACSizeRatio(double dcToACSizeRatio);

    void resetDCToACSizeRatio();

    bool setInverterEfficiency(double inverterEfficiency);

    void resetInverterEfficiency();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::ElectricLoadCenterInverterPVWatts_Impl;

    explicit ElectricLoadCenterInverterPVWatts(std::shared_ptr<detail::ElectricLoadCenterInverterPVWatts_Impl> impl);

    friend class detail::ElectricLoadCenterInverterPVWatts_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ElectricLoadCenterInverterPVWatts");
  };

  /** \relates ElectricLoadCenterInverterPVWatts*/
  using OptionalElectricLoadCenterInverterPVWatts = boost::optional<ElectricLoadCenterInverterPVWatts>;

  /** \relates ElectricLoadCenterInverterPVWatts*/
  using ElectricLoadCenterInverterPVWattsVector = std::vector<ElectricLoadCenterInverterPVWatts>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ELECTRICLOADCENTERINVERTERPVWATTS_HPP
