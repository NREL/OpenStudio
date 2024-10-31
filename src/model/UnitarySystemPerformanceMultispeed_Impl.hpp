/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_UNITARYSYSTEMPERFORMANCEMULTISPEED_IMPL_HPP
#define MODEL_UNITARYSYSTEMPERFORMANCEMULTISPEED_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class SupplyAirflowRatioField;
  class ModelExtensibleGroup;

  namespace detail {

    /** UnitarySystemPerformanceMultispeed_Impl is a ModelObject_Impl that is the implementation class for UnitarySystemPerformanceMultispeed.*/
    class MODEL_API UnitarySystemPerformanceMultispeed_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{
      UnitarySystemPerformanceMultispeed_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      UnitarySystemPerformanceMultispeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      UnitarySystemPerformanceMultispeed_Impl(const UnitarySystemPerformanceMultispeed_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~UnitarySystemPerformanceMultispeed_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{
      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;
      //@}
      /** @name Getters */
      //@{
      bool singleModeOperation() const;

      double noLoadSupplyAirflowRateRatio() const;

      std::vector<SupplyAirflowRatioField> supplyAirflowRatioFields() const;

      //@}
      /** @name Setters */
      //@{
      bool setSingleModeOperation(bool singleMode);

      void resetSingleModeOperation();

      bool setNoLoadSupplyAirflowRateRatio(double noLoadSupplyAirflowRateRatio);

      bool setSupplyAirflowRatioFields(const std::vector<SupplyAirflowRatioField>& airflowRatioFields);

      bool addSupplyAirflowRatioField(const SupplyAirflowRatioField& airflowRatio);

      bool addSupplyAirflowRatioField(double heatingRatio, double coolingRatio);

      void resetSupplyAirflowRatioFields();

      //@}

     protected:
     private:
      REGISTER_LOGGER("openstudio.model.UnitarySystemPerformanceMultispeed");
      boost::optional<double> readDoubleFieldOrAutosize(unsigned fieldNum, const ModelExtensibleGroup& group) const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_UNITARYSYSTEMPERFORMANCEMULTISPEED_IMPL_HPP
