/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRFLOWNETWORKCONSTANTPRESSUREDROP_IMPL_HPP
#define MODEL_AIRFLOWNETWORKCONSTANTPRESSUREDROP_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** AirflowNetworkConstantPressureDrop_Impl is a ModelObject_Impl that is the implementation class for AirflowNetworkConstantPressureDrop.*/
    class MODEL_API AirflowNetworkConstantPressureDrop_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      AirflowNetworkConstantPressureDrop_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AirflowNetworkConstantPressureDrop_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AirflowNetworkConstantPressureDrop_Impl(const AirflowNetworkConstantPressureDrop_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AirflowNetworkConstantPressureDrop_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      double pressureDrop() const;

      //@}
      /** @name Setters */
      //@{

      bool setPressureDrop(double pressureDrop);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.AirflowNetworkConstantPressureDrop");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKCONSTANTPRESSUREDROP_IMPL_HPP
