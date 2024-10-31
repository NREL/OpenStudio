/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRFLOWNETWORKFAN_IMPL_HPP
#define MODEL_AIRFLOWNETWORKFAN_IMPL_HPP

#include "ModelAPI.hpp"
#include "AirflowNetworkComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class StraightComponent;
  class FanConstantVolume;
  class FanVariableVolume;
  class FanOnOff;
  class FanSystemModel;

  namespace detail {

    /** AirflowNetworkFan_Impl is a ModelObject_Impl that is the implementation class for AirflowNetworkFan.*/
    class MODEL_API AirflowNetworkFan_Impl : public AirflowNetworkComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      AirflowNetworkFan_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AirflowNetworkFan_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AirflowNetworkFan_Impl(const AirflowNetworkFan_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AirflowNetworkFan_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual boost::optional<ModelObject> componentModelObject() const override;

      //@}
      /** @name Getters */
      //@{

      /*
    template <typename T> boost::optional<T> fan() const
    {
      return getObject<ModelObject>().getModelObjectTarget<T>(OS_AirflowNetworkFanFields::FanName);
    }
    */

      boost::optional<FanConstantVolume> fanConstantVolume() const;
      boost::optional<FanVariableVolume> fanVariableVolume() const;
      boost::optional<FanOnOff> fanOnOff() const;
      boost::optional<FanSystemModel> fanSystemModel() const;

      //@}
      /** @name Setters */
      //@{

      /*
    template <typename T> bool setFan(const typename std::enable_if<
      std::is_same<FanConstantVolume, T>::value ||
      std::is_same<FanVariableVolume, T>::value ||
      std::is_same<FanOnOff, T>::value, T>::type
      &fan)
    {
      return setPointer(OS_AirflowNetworkFanFields::FanName, fan.handle());
    }
    */

      void resetFan();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.AirflowNetworkFan");

      // TODO: Check the return types of these methods.
      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      //boost::optional<FansCVandOnOff> optionalFan() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKFAN_IMPL_HPP
