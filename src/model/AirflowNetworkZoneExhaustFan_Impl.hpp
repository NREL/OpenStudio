/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRFLOWNETWORKZONEEXHAUSTFAN_IMPL_HPP
#define MODEL_AIRFLOWNETWORKZONEEXHAUSTFAN_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class OutdoorAirController;
  class Crack;

  namespace detail {

    /** AirflowNetworkZoneExhaustFan_Impl is a ModelObject_Impl that is the implementation class for AirflowNetworkZoneExhaustFan.*/
    class MODEL_API AirflowNetworkZoneExhaustFan_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      AirflowNetworkZoneExhaustFan_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AirflowNetworkZoneExhaustFan_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AirflowNetworkZoneExhaustFan_Impl(const AirflowNetworkZoneExhaustFan_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AirflowNetworkZoneExhaustFan_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<FanZoneExhaust> fanZoneExhaust() const;
      boost::optional<AirflowNetworkCrack> crack() const;

      //@}
      /** @name Setters */
      //@{

      bool setCrack(const AirflowNetworkCrack& crack);
      void resetCrack();

      //@}
      /** @name Other */
      //@{

      void resetFanZoneExhaust();

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.AirflowNetworkZoneExhaustFan");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKZONEEXHAUSTFAN_IMPL_HPP
