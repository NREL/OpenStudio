/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_REFRIGERATIONCONDENSERCASCADE_IMPL_HPP
#define MODEL_REFRIGERATIONCONDENSERCASCADE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class RefrigerationSystem;

  namespace detail {

    /** RefrigerationCondenserCascade_Impl is a ModelObject_Impl that is the implementation class for RefrigerationCondenserCascade.*/
    class MODEL_API RefrigerationCondenserCascade_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      RefrigerationCondenserCascade_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      RefrigerationCondenserCascade_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      RefrigerationCondenserCascade_Impl(const RefrigerationCondenserCascade_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~RefrigerationCondenserCascade_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<IdfObject> remove() override;

      //@}
      /** @name Getters */
      //@{

      double ratedCondensingTemperature() const;

      double ratedApproachTemperatureDifference() const;

      bool isRatedApproachTemperatureDifferenceDefaulted() const;

      double ratedEffectiveTotalHeatRejectionRate() const;

      std::string condensingTemperatureControlType() const;

      bool isCondensingTemperatureControlTypeDefaulted() const;

      boost::optional<double> condenserRefrigerantOperatingChargeInventory() const;

      boost::optional<double> condensateReceiverRefrigerantInventory() const;

      boost::optional<double> condensatePipingRefrigerantInventory() const;

      //@}
      /** @name Setters */
      //@{

      bool setRatedCondensingTemperature(double ratedCondensingTemperature);

      bool setRatedApproachTemperatureDifference(double ratedApproachTemperatureDifference);

      void resetRatedApproachTemperatureDifference();

      bool setRatedEffectiveTotalHeatRejectionRate(double ratedEffectiveTotalHeatRejectionRate);

      bool setCondensingTemperatureControlType(const std::string& condensingTemperatureControlType);

      void resetCondensingTemperatureControlType();

      bool setCondenserRefrigerantOperatingChargeInventory(boost::optional<double> condenserRefrigerantOperatingChargeInventory);

      void resetCondenserRefrigerantOperatingChargeInventory();

      bool setCondensateReceiverRefrigerantInventory(boost::optional<double> condensateReceiverRefrigerantInventory);

      void resetCondensateReceiverRefrigerantInventory();

      bool setCondensatePipingRefrigerantInventory(boost::optional<double> condensatePipingRefrigerantInventory);

      void resetCondensatePipingRefrigerantInventory();

      //@}
      /** @name Other */
      //@{

      boost::optional<RefrigerationSystem> system() const;
      void removeFromSystem();

      boost::optional<RefrigerationSystem> heatRejectingSystem() const;
      void removeFromHeatRejectingSystem();

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.RefrigerationCondenserCascade");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_REFRIGERATIONCONDENSERCASCADE_IMPL_HPP
