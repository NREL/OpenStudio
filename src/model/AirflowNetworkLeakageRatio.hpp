/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRFLOWNETWORKLEAKAGERATIO_HPP
#define MODEL_AIRFLOWNETWORKLEAKAGERATIO_HPP

#include "ModelAPI.hpp"
#include "AirflowNetworkComponent.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class AirflowNetworkLeakageRatio_Impl;

  }  // namespace detail

  /** AirflowNetworkLeakageRatio is a AirflowNetworkComponent that wraps the OpenStudio IDD object 'OS:AirflowNetwork:LeakageRatio'. */
  class MODEL_API AirflowNetworkLeakageRatio : public AirflowNetworkComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit AirflowNetworkLeakageRatio(const Model& model);

    virtual ~AirflowNetworkLeakageRatio() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirflowNetworkLeakageRatio(const AirflowNetworkLeakageRatio& other) = default;
    AirflowNetworkLeakageRatio(AirflowNetworkLeakageRatio&& other) = default;
    AirflowNetworkLeakageRatio& operator=(const AirflowNetworkLeakageRatio&) = default;
    AirflowNetworkLeakageRatio& operator=(AirflowNetworkLeakageRatio&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    boost::optional<double> effectiveLeakageRatio() const;

    double maximumFlowRate() const;

    double referencePressureDifference() const;

    double airMassFlowExponent() const;

    bool isAirMassFlowExponentDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setEffectiveLeakageRatio(double effectiveLeakageRatio);

    void resetEffectiveLeakageRatio();

    bool setMaximumFlowRate(double maximumFlowRate);

    bool setReferencePressureDifference(double referencePressureDifference);

    bool setAirMassFlowExponent(double airMassFlowExponent);

    void resetAirMassFlowExponent();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::AirflowNetworkLeakageRatio_Impl;

    explicit AirflowNetworkLeakageRatio(std::shared_ptr<detail::AirflowNetworkLeakageRatio_Impl> impl);

    friend class detail::AirflowNetworkLeakageRatio_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirflowNetworkLeakageRatio");
  };

  /** \relates AirflowNetworkLeakageRatio*/
  using OptionalAirflowNetworkLeakageRatio = boost::optional<AirflowNetworkLeakageRatio>;

  /** \relates AirflowNetworkLeakageRatio*/
  using AirflowNetworkLeakageRatioVector = std::vector<AirflowNetworkLeakageRatio>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKLEAKAGERATIO_HPP
