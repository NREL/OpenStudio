/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_FUELFACTORS_HPP
#define MODEL_FUELFACTORS_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  class Schedule;

  namespace detail {

    class FuelFactors_Impl;

  }  // namespace detail

  /** FuelFactors is a ModelObject that wraps the OpenStudio IDD object 'OS:FuelFactors'. */
  class MODEL_API FuelFactors : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit FuelFactors(const Model& model);

    virtual ~FuelFactors() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    FuelFactors(const FuelFactors& other) = default;
    FuelFactors(FuelFactors&& other) = default;
    FuelFactors& operator=(const FuelFactors&) = default;
    FuelFactors& operator=(FuelFactors&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> existingFuelResourceNameValues();

    /** @name Getters */
    //@{

    std::string existingFuelResourceName() const;

    double sourceEnergyFactor() const;

    boost::optional<Schedule> sourceEnergySchedule() const;

    double cO2EmissionFactor() const;

    boost::optional<Schedule> cO2EmissionFactorSchedule() const;

    double cOEmissionFactor() const;

    boost::optional<Schedule> cOEmissionFactorSchedule() const;

    double cH4EmissionFactor() const;

    boost::optional<Schedule> cH4EmissionFactorSchedule() const;

    double nOxEmissionFactor() const;

    boost::optional<Schedule> nOxEmissionFactorSchedule() const;

    double n2OEmissionFactor() const;

    boost::optional<Schedule> n2OEmissionFactorSchedule() const;

    double sO2EmissionFactor() const;

    boost::optional<Schedule> sO2EmissionFactorSchedule() const;

    double pMEmissionFactor() const;

    boost::optional<Schedule> pMEmissionFactorSchedule() const;

    double pM10EmissionFactor() const;

    boost::optional<Schedule> pM10EmissionFactorSchedule() const;

    double pM25EmissionFactor() const;

    boost::optional<Schedule> pM25EmissionFactorSchedule() const;

    double nH3EmissionFactor() const;

    boost::optional<Schedule> nH3EmissionFactorSchedule() const;

    double nMVOCEmissionFactor() const;

    boost::optional<Schedule> nMVOCEmissionFactorSchedule() const;

    double hgEmissionFactor() const;

    boost::optional<Schedule> hgEmissionFactorSchedule() const;

    double pbEmissionFactor() const;

    boost::optional<Schedule> pbEmissionFactorSchedule() const;

    double waterEmissionFactor() const;

    boost::optional<Schedule> waterEmissionFactorSchedule() const;

    double nuclearHighLevelEmissionFactor() const;

    boost::optional<Schedule> nuclearHighLevelEmissionFactorSchedule() const;

    double nuclearLowLevelEmissionFactor() const;

    boost::optional<Schedule> nuclearLowLevelEmissionFactorSchedule() const;

    //@}
    /** @name Setters */
    //@{

    bool setExistingFuelResourceName(const std::string& existingFuelResourceName);

    bool setSourceEnergyFactor(double sourceEnergyFactor);

    bool setSourceEnergySchedule(Schedule& schedule);
    void resetSourceEnergySchedule();

    bool setCO2EmissionFactor(double cO2EmissionFactor);

    bool setCO2EmissionFactorSchedule(Schedule& schedule);
    void resetCO2EmissionFactorSchedule();

    bool setCOEmissionFactor(double cOEmissionFactor);

    bool setCOEmissionFactorSchedule(Schedule& schedule);
    void resetCOEmissionFactorSchedule();

    bool setCH4EmissionFactor(double cH4EmissionFactor);

    bool setCH4EmissionFactorSchedule(Schedule& schedule);
    void resetCH4EmissionFactorSchedule();

    bool setNOxEmissionFactor(double nOxEmissionFactor);

    bool setNOxEmissionFactorSchedule(Schedule& schedule);

    void resetNOxEmissionFactorSchedule();

    bool setN2OEmissionFactor(double n2OEmissionFactor);

    bool setN2OEmissionFactorSchedule(Schedule& schedule);
    void resetN2OEmissionFactorSchedule();

    bool setSO2EmissionFactor(double sO2EmissionFactor);

    bool setSO2EmissionFactorSchedule(Schedule& schedule);
    void resetSO2EmissionFactorSchedule();

    bool setPMEmissionFactor(double pMEmissionFactor);

    bool setPMEmissionFactorSchedule(Schedule& schedule);
    void resetPMEmissionFactorSchedule();

    bool setPM10EmissionFactor(double pM10EmissionFactor);

    bool setPM10EmissionFactorSchedule(Schedule& schedule);
    void resetPM10EmissionFactorSchedule();

    bool setPM25EmissionFactor(double pM25EmissionFactor);

    bool setPM25EmissionFactorSchedule(Schedule& schedule);
    void resetPM25EmissionFactorSchedule();

    bool setNH3EmissionFactor(double nH3EmissionFactor);

    bool setNH3EmissionFactorSchedule(Schedule& schedule);
    void resetNH3EmissionFactorSchedule();

    bool setNMVOCEmissionFactor(double nMVOCEmissionFactor);

    bool setNMVOCEmissionFactorSchedule(Schedule& schedule);
    void resetNMVOCEmissionFactorSchedule();

    bool setHgEmissionFactor(double hgEmissionFactor);

    bool setHgEmissionFactorSchedule(Schedule& schedule);
    void resetHgEmissionFactorSchedule();

    bool setPbEmissionFactor(double pbEmissionFactor);

    bool setPbEmissionFactorSchedule(Schedule& schedule);
    void resetPbEmissionFactorSchedule();

    bool setWaterEmissionFactor(double waterEmissionFactor);

    bool setWaterEmissionFactorSchedule(Schedule& schedule);
    void resetWaterEmissionFactorSchedule();

    bool setNuclearHighLevelEmissionFactor(double nuclearHighLevelEmissionFactor);

    bool setNuclearHighLevelEmissionFactorSchedule(Schedule& schedule);
    void resetNuclearHighLevelEmissionFactorSchedule();

    bool setNuclearLowLevelEmissionFactor(double nuclearLowLevelEmissionFactor);

    bool setNuclearLowLevelEmissionFactorSchedule(Schedule& schedule);
    void resetNuclearLowLevelEmissionFactorSchedule();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::FuelFactors_Impl;

    explicit FuelFactors(std::shared_ptr<detail::FuelFactors_Impl> impl);

    friend class detail::FuelFactors_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.FuelFactors");
  };

  /** \relates FuelFactors*/
  using OptionalFuelFactors = boost::optional<FuelFactors>;

  /** \relates FuelFactors*/
  using FuelFactorsVector = std::vector<FuelFactors>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_FUELFACTORS_HPP
