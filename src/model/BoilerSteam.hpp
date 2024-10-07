/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_BOILERSTEAM_HPP
#define MODEL_BOILERSTEAM_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class BoilerSteam_Impl;

  }  // namespace detail

  /** BoilerSteam is a StraightComponent that wraps the OpenStudio IDD object 'OS:Boiler:Steam'. */
  class MODEL_API BoilerSteam : public StraightComponent
  {

   public:
    explicit BoilerSteam(const Model& model);

    virtual ~BoilerSteam() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    BoilerSteam(const BoilerSteam& other) = default;
    BoilerSteam(BoilerSteam&& other) = default;
    BoilerSteam& operator=(const BoilerSteam&) = default;
    BoilerSteam& operator=(BoilerSteam&&) = default;

    static IddObjectType iddObjectType();

    static std::vector<std::string> fuelTypeValues();

    /** @name Getters and Setters */
    //@{

    std::string fuelType() const;
    bool setFuelType(const std::string& fuelType);

    boost::optional<double> maximumOperatingPressure() const;
    bool setMaximumOperatingPressure(double maximumOperatingPressure);
    void resetMaximumOperatingPressure();

    boost::optional<double> theoreticalEfficiency() const;
    bool setTheoreticalEfficiency(double theoreticalEfficiency);
    void resetTheoreticalEfficiency();

    boost::optional<double> designOutletSteamTemperature() const;
    bool setDesignOutletSteamTemperature(double designOutletSteamTemperature);
    void resetDesignOutletSteamTemperature();

    boost::optional<double> nominalCapacity() const;
    bool isNominalCapacityAutosized() const;
    bool setNominalCapacity(double nominalCapacity);
    void resetNominalCapacity();
    void autosizeNominalCapacity();

    boost::optional<double> minimumPartLoadRatio() const;
    bool setMinimumPartLoadRatio(double minimumPartLoadRatio);
    void resetMinimumPartLoadRatio();

    boost::optional<double> maximumPartLoadRatio() const;
    bool setMaximumPartLoadRatio(double maximumPartLoadRatio);
    void resetMaximumPartLoadRatio();

    boost::optional<double> optimumPartLoadRatio() const;
    bool setOptimumPartLoadRatio(double optimumPartLoadRatio);
    void resetOptimumPartLoadRatio();

    boost::optional<double> coefficient1ofFuelUseFunctionofPartLoadRatioCurve() const;
    bool setCoefficient1ofFuelUseFunctionofPartLoadRatioCurve(double coefficient1ofFuelUseFunctionofPartLoadRatioCurve);
    void resetCoefficient1ofFuelUseFunctionofPartLoadRatioCurve();

    boost::optional<double> coefficient2ofFuelUseFunctionofPartLoadRatioCurve() const;
    bool setCoefficient2ofFuelUseFunctionofPartLoadRatioCurve(double coefficient2ofFuelUseFunctionofPartLoadRatioCurve);
    void resetCoefficient2ofFuelUseFunctionofPartLoadRatioCurve();

    boost::optional<double> coefficient3ofFuelUseFunctionofPartLoadRatioCurve() const;
    bool setCoefficient3ofFuelUseFunctionofPartLoadRatioCurve(double coefficient3ofFuelUseFunctionofPartLoadRatioCurve);
    void resetCoefficient3ofFuelUseFunctionofPartLoadRatioCurve();

    double sizingFactor() const;
    bool isSizingFactorDefaulted() const;
    bool setSizingFactor(double sizingFactor);
    void resetSizingFactor();

    std::string endUseSubcategory() const;
    bool setEndUseSubcategory(const std::string& endUseSubcategory);

    //@}
    /** @name Other */
    //@{

    //@}

    boost::optional<double> autosizedNominalCapacity() const;

   protected:
    /// @cond
    using ImplType = detail::BoilerSteam_Impl;

    explicit BoilerSteam(std::shared_ptr<detail::BoilerSteam_Impl> impl);

    friend class detail::BoilerSteam_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    /// @endcond

   private:
    REGISTER_LOGGER("openstudio.model.BoilerSteam");
  };

  /** \relates BoilerSteam*/
  using OptionalBoilerSteam = boost::optional<BoilerSteam>;

  /** \relates BoilerSteam*/
  using BoilerSteamVector = std::vector<BoilerSteam>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_BOILERSTEAM_HPP
