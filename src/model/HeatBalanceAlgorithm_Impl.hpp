/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_HEATBALANCEALGORITHM_IMPL_HPP
#define MODEL_HEATBALANCEALGORITHM_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** HeatBalanceAlgorithm_Impl is a ModelObject_Impl that is the implementation class for HeatBalanceAlgorithm.*/
    class MODEL_API HeatBalanceAlgorithm_Impl : public ModelObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      // constructor
      HeatBalanceAlgorithm_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // construct from workspace
      HeatBalanceAlgorithm_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // clone copy constructor
      HeatBalanceAlgorithm_Impl(const HeatBalanceAlgorithm_Impl& other, Model_Impl* model, bool keepHandle);

      // virtual destructor
      virtual ~HeatBalanceAlgorithm_Impl() override = default;

      // return the parent object in the hierarchy
      virtual boost::optional<ParentObject> parent() const override;

      // set the parent, child may have to call methods on the parent
      virtual bool setParent(ParentObject& newParent) override;

      //@}

      /** @name Virtual Methods */
      //@{

      // Get all output variable names that could be associated with this object.
      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      std::string algorithm() const;

      bool isAlgorithmDefaulted() const;

      double surfaceTemperatureUpperLimit() const;

      bool isSurfaceTemperatureUpperLimitDefaulted() const;

      double minimumSurfaceConvectionHeatTransferCoefficientValue() const;

      bool isMinimumSurfaceConvectionHeatTransferCoefficientValueDefaulted() const;

      double maximumSurfaceConvectionHeatTransferCoefficientValue() const;

      bool isMaximumSurfaceConvectionHeatTransferCoefficientValueDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      bool setAlgorithm(const std::string& algorithm);

      void resetAlgorithm();

      bool setSurfaceTemperatureUpperLimit(double surfaceTemperatureUpperLimit);

      void resetSurfaceTemperatureUpperLimit();

      bool setMinimumSurfaceConvectionHeatTransferCoefficientValue(double minimumSurfaceConvectionHeatTransferCoefficientValue);

      void resetMinimumSurfaceConvectionHeatTransferCoefficientValue();

      bool setMaximumSurfaceConvectionHeatTransferCoefficientValue(double maximumSurfaceConvectionHeatTransferCoefficientValue);

      void resetMaximumSurfaceConvectionHeatTransferCoefficientValue();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.HeatBalanceAlgorithm");

      std::vector<std::string> algorithmValues() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_HEATBALANCEALGORITHM_IMPL_HPP
