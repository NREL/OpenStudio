/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRFLOWNETWORKDUCT_IMPL_HPP
#define MODEL_AIRFLOWNETWORKDUCT_IMPL_HPP

#include "ModelAPI.hpp"
#include "AirflowNetworkComponent_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** AirflowNetworkDuct_Impl is a ModelObject_Impl that is the implementation class for AirflowNetworkDuct.*/
    class MODEL_API AirflowNetworkDuct_Impl : public AirflowNetworkComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      AirflowNetworkDuct_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AirflowNetworkDuct_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AirflowNetworkDuct_Impl(const AirflowNetworkDuct_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AirflowNetworkDuct_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
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
     private:
      REGISTER_LOGGER("openstudio.model.AirflowNetworkDuct");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKDuCT_IMPL_HPP
