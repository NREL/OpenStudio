/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_LOADINGINDEX_IMPL_HPP
#define MODEL_LOADINGINDEX_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "ParentObject_Impl.hpp"

namespace openstudio {
namespace model {

  class Curve;

  namespace detail {

    /** LoadingIndex_Impl is a ParentObject_Impl that is the implementation class for LoadingIndex.*/
    class MODEL_API LoadingIndex_Impl : public ParentObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      LoadingIndex_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      LoadingIndex_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      LoadingIndex_Impl(const LoadingIndex_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~LoadingIndex_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      // Overriding clone and children here because we want to try to remove the Curves (if they aren't used by something else)
      // So we list them as children. But ParentObject_Impl::clone would also clone them, so we override clone to call ModelObject_Impl::clone
      virtual ModelObject clone(Model model) const override;

      virtual std::vector<ModelObject> children() const override;

      // No need to override, it bears only ResourceObjects
      // virtual std::vector<IddObjectType> allowableChildTypes() const override;

      //@}
      /** @name Getters */
      //@{

      double compressorSpeed() const;

      Curve evaporativeCapacityMultiplierFunctionofTemperatureCurve() const;

      Curve compressorPowerMultiplierFunctionofTemperatureCurve() const;

      //@}
      /** @name Setters */
      //@{

      bool setCompressorSpeed(double compressorSpeed);

      bool setEvaporativeCapacityMultiplierFunctionofTemperatureCurve(const Curve& curve);

      bool setCompressorPowerMultiplierFunctionofTemperatureCurve(const Curve& curve);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.LoadingIndex");

      boost::optional<Curve> optionalEvaporativeCapacityMultiplierFunctionofTemperatureCurve() const;
      boost::optional<Curve> optionalCompressorPowerMultiplierFunctionofTemperatureCurve() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_LOADINGINDEX_IMPL_HPP
