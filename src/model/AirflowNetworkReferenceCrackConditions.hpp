/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRFLOWNETWORKREFERENCECRACKCONDITIONS_HPP
#define MODEL_AIRFLOWNETWORKREFERENCECRACKCONDITIONS_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class AirflowNetworkReferenceCrackConditions_Impl;

  }  // namespace detail

  /** AirflowNetworkReferenceCrackConditions is a ModelObject that wraps the OpenStudio IDD object 'OS:AirflowNetworkReferenceCrackConditions'. */
  class MODEL_API AirflowNetworkReferenceCrackConditions : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{
    /** Construct a reference crack conditions object with default values. */
    explicit AirflowNetworkReferenceCrackConditions(const Model& model);
    /** Construct a reference crack conditions object with specified temperature, pressure, and barometric pressure. */
    AirflowNetworkReferenceCrackConditions(const Model& model, double temperature, double barometricPressure, double humidityRatio);

    virtual ~AirflowNetworkReferenceCrackConditions() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirflowNetworkReferenceCrackConditions(const AirflowNetworkReferenceCrackConditions& other) = default;
    AirflowNetworkReferenceCrackConditions(AirflowNetworkReferenceCrackConditions&& other) = default;
    AirflowNetworkReferenceCrackConditions& operator=(const AirflowNetworkReferenceCrackConditions&) = default;
    AirflowNetworkReferenceCrackConditions& operator=(AirflowNetworkReferenceCrackConditions&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{
    /** Returns the reference temperature in degrees C. */
    double temperature() const;
    /** Returns true if the reference temperature is defaulted. */
    bool isTemperatureDefaulted() const;
    /** Returns the reference barometric pressure in Pa. */
    double barometricPressure() const;
    /** Returns true if the reference barometric pressure is defaulted. */
    bool isBarometricPressureDefaulted() const;
    /** Returns the reference humidity ratio in kgWater/kgDryAir. */
    double humidityRatio() const;
    /** Returns true if the reference humidity ratio is defaulted. */
    bool isHumidityRatioDefaulted() const;

    //@}
    /** @name Setters */
    //@{
    /** Sets the reference temperature in degrees C. */
    void setTemperature(double referenceTemperature);
    /** Resets the reference temperature to the default. */
    void resetTemperature();
    /** Sets the the reference barometric pressure in Pa. */
    bool setBarometricPressure(double referenceBarometricPressure);
    /** Resets the reference barometric pressure to the default. */
    void resetBarometricPressure();
    /** Sets the reference humidity ratio in kgWater/kgDryAir. */
    void setHumidityRatio(double referenceHumidityRatio);
    /** Resets the reference humidity ratio to the default. */
    void resetHumidityRatio();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::AirflowNetworkReferenceCrackConditions_Impl;

    explicit AirflowNetworkReferenceCrackConditions(std::shared_ptr<detail::AirflowNetworkReferenceCrackConditions_Impl> impl);

    friend class detail::AirflowNetworkReferenceCrackConditions_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirflowNetworkReferenceCrackConditions");
  };

  /** \relates AirflowNetworkReferenceCrackConditions*/
  using OptionalAirflowNetworkReferenceCrackConditions = boost::optional<AirflowNetworkReferenceCrackConditions>;

  /** \relates AirflowNetworkReferenceCrackConditions*/
  using AirflowNetworkReferenceCrackConditionsVector = std::vector<AirflowNetworkReferenceCrackConditions>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKREFERENCECRACKCONDITIONS_HPP
