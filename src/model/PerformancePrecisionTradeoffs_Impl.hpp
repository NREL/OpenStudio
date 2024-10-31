/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PERFORMANCEPRECISIONTRADEOFFS_IMPL_HPP
#define MODEL_PERFORMANCEPRECISIONTRADEOFFS_IMPL_HPP

#include <vector>
#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** PerformancePrecisionTradeoffs_Impl is a ModelObject_Impl that is the implementation class for PerformancePrecisionTradeoffs.*/
    class MODEL_API PerformancePrecisionTradeoffs_Impl : public ModelObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      PerformancePrecisionTradeoffs_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      PerformancePrecisionTradeoffs_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      PerformancePrecisionTradeoffs_Impl(const PerformancePrecisionTradeoffs_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~PerformancePrecisionTradeoffs_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      bool useCoilDirectSolutions() const;
      bool isUseCoilDirectSolutionsDefaulted() const;

      std::string zoneRadiantExchangeAlgorithm() const;
      bool isZoneRadiantExchangeAlgorithmDefaulted() const;

      std::string overrideMode() const;
      bool isOverrideModeDefaulted() const;

      double maxZoneTempDiff() const;
      bool isMaxZoneTempDiffDefaulted() const;

      double maxAllowedDelTemp() const;
      bool isMaxAllowedDelTempDefaulted() const;

      bool useRepresentativeSurfacesforCalculations() const;
      bool isUseRepresentativeSurfacesforCalculationsDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      bool setUseCoilDirectSolutions(bool useCoilDirectSolutions);
      void resetUseCoilDirectSolutions();

      bool setZoneRadiantExchangeAlgorithm(const std::string& zoneRadiantExchangeAlgorithm);
      void resetZoneRadiantExchangeAlgorithm();

      bool setOverrideMode(const std::string& overrideMode);
      void resetOverrideMode();

      bool setMaxZoneTempDiff(double maxZoneTempDiff);
      void resetMaxZoneTempDiff();

      bool setMaxAllowedDelTemp(double maxAllowedDelTemp);
      void resetMaxAllowedDelTemp();

      bool setUseRepresentativeSurfacesforCalculations(bool useRepresentativeSurfacesforCalculations);
      void resetUseRepresentativeSurfacesforCalculations();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.PerformancePrecisionTradeoffs");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PERFORMANCEPRECISIONTRADEOFFS_IMPL_HPP
