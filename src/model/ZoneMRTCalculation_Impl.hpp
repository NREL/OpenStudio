/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Energy Innovation, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONEMRTCALCULATION_IMPL_HPP
#define MODEL_ZONEMRTCALCULATION_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class MRTWeightingFactor;
  class People;
  class ThermalZone;

  namespace detail {

    /** ZoneMRTCalculation_Impl is a ModelObject_Impl that is the implementation class for ZoneMRTCalculation.*/
    class MODEL_API ZoneMRTCalculation_Impl : public ModelObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      ZoneMRTCalculation_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ZoneMRTCalculation_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ZoneMRTCalculation_Impl(const ZoneMRTCalculation_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ZoneMRTCalculation_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual ModelObject clone(Model model) const override;

      //@}
      /** @name Getters */
      //@{

      ThermalZone thermalZone() const;

      std::vector<MRTWeightingFactor> mrtWeightingFactors() const;

      unsigned int numberofMRTWeightingFactors() const;

      boost::optional<unsigned> mrtWeightingFactorIndex(const People& people) const;

      boost::optional<MRTWeightingFactor> getMRTWeightingFactor(unsigned groupIndex) const;

      //@}
      /** @name Setters */
      //@{

      bool addMRTWeightingFactor(const MRTWeightingFactor& mrtWeightingFactor);

      bool addMRTWeightingFactor(const People& people, double mrtWeightingFactor);

      bool addMRTWeightingFactors(const std::vector<MRTWeightingFactor>& mrtWeightingFactors);

      bool removeMRTWeightingFactor(unsigned groupIndex);

      void removeAllMRTWeightingFactors();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ZoneMRTCalculation");

      boost::optional<People> getPeople(unsigned groupIndex) const;

      boost::optional<double> getMRTWeightingFactorValue(unsigned groupIndex) const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEMRTCALCULATION_IMPL_HPP