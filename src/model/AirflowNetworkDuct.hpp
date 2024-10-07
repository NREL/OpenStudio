/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRFLOWNETWORKDUCT_HPP
#define MODEL_AIRFLOWNETWORKDUCT_HPP

#include "ModelAPI.hpp"
#include "AirflowNetworkComponent.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class AirflowNetworkDuct_Impl;

  }  // namespace detail

  /** AirflowNetworkDuct is a ModelObject that wraps the OpenStudio IDD object 'OS:AirflowNetwork:Duct'. */
  class MODEL_API AirflowNetworkDuct : public AirflowNetworkComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit AirflowNetworkDuct(const Model& model);

    virtual ~AirflowNetworkDuct() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirflowNetworkDuct(const AirflowNetworkDuct& other) = default;
    AirflowNetworkDuct(AirflowNetworkDuct&& other) = default;
    AirflowNetworkDuct& operator=(const AirflowNetworkDuct&) = default;
    AirflowNetworkDuct& operator=(AirflowNetworkDuct&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    double ductLength() const;

    double hydraulicDiameter() const;

    double crossSectionArea() const;

    double surfaceRoughness() const;

    bool isSurfaceRoughnessDefaulted() const;

    double coefficientforLocalDynamicLossDuetoFitting() const;

    bool isCoefficientforLocalDynamicLossDuetoFittingDefaulted() const;

    double ductWallHeatTransmittanceCoefficient() const;

    bool isDuctWallHeatTransmittanceCoefficientDefaulted() const;

    double insideConvectionCoefficient() const;

    bool isInsideConvectionCoefficientDefaulted() const;

    double outsideConvectionCoefficient() const;

    bool isOutsideConvectionCoefficientDefaulted() const;

    double overallMoistureTransmittanceCoefficientfromAirtoAir() const;

    bool isOverallMoistureTransmittanceCoefficientfromAirtoAirDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setDuctLength(double ductLength);

    bool setHydraulicDiameter(double hydraulicDiameter);

    bool setCrossSectionArea(double crossSectionArea);

    bool setSurfaceRoughness(double surfaceRoughness);

    void resetSurfaceRoughness();

    bool setCoefficientforLocalDynamicLossDuetoFitting(double coefficientforLocalDynamicLossDuetoFitting);

    void resetCoefficientforLocalDynamicLossDuetoFitting();

    bool setDuctWallHeatTransmittanceCoefficient(double coefficient);

    void resetDuctWallHeatTransmittanceCoefficient();

    bool setInsideConvectionCoefficient(double coefficient);

    void resetInsideConvectionCoefficient();

    bool setOutsideConvectionCoefficient(double coefficient);

    void resetOutsideConvectionCoefficient();

    bool setOverallMoistureTransmittanceCoefficientfromAirtoAir(double overallMoistureTransmittanceCoefficientfromAirtoAir);

    void resetOverallMoistureTransmittanceCoefficientfromAirtoAir();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::AirflowNetworkDuct_Impl;

    explicit AirflowNetworkDuct(std::shared_ptr<detail::AirflowNetworkDuct_Impl> impl);

    friend class detail::AirflowNetworkDuct_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirflowNetworkDuct");
  };

  /** \relates AirflowNetworkDuct*/
  using OptionalAirflowNetworkDuct = boost::optional<AirflowNetworkDuct>;

  /** \relates AirflowNetworkDuct*/
  using AirflowNetworkDuctVector = std::vector<AirflowNetworkDuct>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKDUCT_HPP
