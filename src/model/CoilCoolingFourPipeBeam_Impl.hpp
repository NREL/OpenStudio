/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILCOOLINGFOURPIPEBEAM_IMPL_HPP
#define MODEL_COILCOOLINGFOURPIPEBEAM_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Node;
  class Curve;
  class AirTerminalSingleDuctConstantVolumeFourPipeBeam;

  namespace detail {

    /** CoilCoolingFourPipeBeam_Impl is a StraightComponent_Impl that is the implementation class for CoilCoolingFourPipeBeam.*/
    class MODEL_API CoilCoolingFourPipeBeam_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilCoolingFourPipeBeam_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilCoolingFourPipeBeam_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilCoolingFourPipeBeam_Impl(const CoilCoolingFourPipeBeam_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilCoolingFourPipeBeam_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual boost::optional<HVACComponent> containingHVACComponent() const override;

      virtual boost::optional<StraightComponent> containingStraightComponent() const override;

      virtual bool addToNode(Node& node) override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<Node> chilledWaterInletNode() const;

      boost::optional<Node> chilledWaterOutletNode() const;

      double beamRatedCoolingCapacityperBeamLength() const;
      bool isBeamRatedCoolingCapacityperBeamLengthDefaulted() const;

      double beamRatedCoolingRoomAirChilledWaterTemperatureDifference() const;
      bool isBeamRatedCoolingRoomAirChilledWaterTemperatureDifferenceDefaulted() const;

      double beamRatedChilledWaterVolumeFlowRateperBeamLength() const;
      bool isBeamRatedChilledWaterVolumeFlowRateperBeamLengthDefaulted() const;

      Curve beamCoolingCapacityTemperatureDifferenceModificationFactorCurve() const;

      Curve beamCoolingCapacityAirFlowModificationFactorCurve() const;

      Curve beamCoolingCapacityChilledWaterFlowModificationFactorCurve() const;

      //@}
      /** @name Setters */
      //@{

      bool setChilledWaterInletNode(const Node& connection);
      void resetChilledWaterInletNode();

      bool setChilledWaterOutletNode(const Node& connection);
      void resetChilledWaterOutletNode();

      bool setBeamRatedCoolingCapacityperBeamLength(double beamRatedCoolingCapacityperBeamLength);
      void resetBeamRatedCoolingCapacityperBeamLength();

      bool setBeamRatedCoolingRoomAirChilledWaterTemperatureDifference(double beamRatedCoolingRoomAirChilledWaterTemperatureDifference);
      void resetBeamRatedCoolingRoomAirChilledWaterTemperatureDifference();

      bool setBeamRatedChilledWaterVolumeFlowRateperBeamLength(double beamRatedChilledWaterVolumeFlowRateperBeamLength);
      void resetBeamRatedChilledWaterVolumeFlowRateperBeamLength();

      bool setBeamCoolingCapacityTemperatureDifferenceModificationFactorCurve(const Curve& curve);

      bool setBeamCoolingCapacityAirFlowModificationFactorCurve(const Curve& curve);

      bool setBeamCoolingCapacityChilledWaterFlowModificationFactorCurve(const Curve& curve);

      //@}
      /** @name Other */
      //@{

      /* Convenience function to return the parent AirTerminal:SingleDuct:ConstantVolume:FourPipeBeam */
      boost::optional<AirTerminalSingleDuctConstantVolumeFourPipeBeam> airTerminalSingleDuctConstantVolumeFourPipeBeam() const;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilCoolingFourPipeBeam");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILCOOLINGFOURPIPEBEAM_IMPL_HPP
