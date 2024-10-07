/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SITEGROUNDTEMPERATUREFCFACTORMETHOD_IMPL_HPP
#define MODEL_SITEGROUNDTEMPERATUREFCFACTORMETHOD_IMPL_HPP

#include <vector>
#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"
#include "../utilities/time/Date.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** SiteGroundTemperatureFCfactorMethod_Impl is a ModelObject_Impl that is the implementation class for SiteGroundTemperatureFCfactorMethod.*/
    class MODEL_API SiteGroundTemperatureFCfactorMethod_Impl : public ModelObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      SiteGroundTemperatureFCfactorMethod_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SiteGroundTemperatureFCfactorMethod_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      SiteGroundTemperatureFCfactorMethod_Impl(const SiteGroundTemperatureFCfactorMethod_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~SiteGroundTemperatureFCfactorMethod_Impl() override = default;

      //@}

      virtual boost::optional<ParentObject> parent() const override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      /** @name Getters */
      //@{

      double januaryGroundTemperature() const;

      bool isJanuaryGroundTemperatureDefaulted() const;

      double februaryGroundTemperature() const;

      bool isFebruaryGroundTemperatureDefaulted() const;

      double marchGroundTemperature() const;

      bool isMarchGroundTemperatureDefaulted() const;

      double aprilGroundTemperature() const;

      bool isAprilGroundTemperatureDefaulted() const;

      double mayGroundTemperature() const;

      bool isMayGroundTemperatureDefaulted() const;

      double juneGroundTemperature() const;

      bool isJuneGroundTemperatureDefaulted() const;

      double julyGroundTemperature() const;

      bool isJulyGroundTemperatureDefaulted() const;

      double augustGroundTemperature() const;

      bool isAugustGroundTemperatureDefaulted() const;

      double septemberGroundTemperature() const;

      bool isSeptemberGroundTemperatureDefaulted() const;

      double octoberGroundTemperature() const;

      bool isOctoberGroundTemperatureDefaulted() const;

      double novemberGroundTemperature() const;

      bool isNovemberGroundTemperatureDefaulted() const;

      double decemberGroundTemperature() const;

      bool isDecemberGroundTemperatureDefaulted() const;

      double getTemperatureByMonth(int month) const;

      double getTemperatureByMonth(const openstudio::MonthOfYear& month) const;

      bool isMonthDefaulted(int month) const;

      bool isMonthDefaulted(const openstudio::MonthOfYear& month) const;

      std::vector<double> getAllMonthlyTemperatures() const;

      //@}
      /** @name Setters */
      //@{

      bool setJanuaryGroundTemperature(double januaryGroundTemperature);

      void resetJanuaryGroundTemperature();

      bool setFebruaryGroundTemperature(double februaryGroundTemperature);

      void resetFebruaryGroundTemperature();

      bool setMarchGroundTemperature(double marchGroundTemperature);

      void resetMarchGroundTemperature();

      bool setAprilGroundTemperature(double aprilGroundTemperature);

      void resetAprilGroundTemperature();

      bool setMayGroundTemperature(double mayGroundTemperature);

      void resetMayGroundTemperature();

      bool setJuneGroundTemperature(double juneGroundTemperature);

      void resetJuneGroundTemperature();

      bool setJulyGroundTemperature(double julyGroundTemperature);

      void resetJulyGroundTemperature();

      bool setAugustGroundTemperature(double augustGroundTemperature);

      void resetAugustGroundTemperature();

      bool setSeptemberGroundTemperature(double septemberGroundTemperature);

      void resetSeptemberGroundTemperature();

      bool setOctoberGroundTemperature(double octoberGroundTemperature);

      void resetOctoberGroundTemperature();

      bool setNovemberGroundTemperature(double novemberGroundTemperature);

      void resetNovemberGroundTemperature();

      bool setDecemberGroundTemperature(double decemberGroundTemperature);

      void resetDecemberGroundTemperature();

      bool setTemperatureByMonth(int month, double temperature);

      bool setTemperatureByMonth(const openstudio::MonthOfYear& month, double temperature);

      void resetTemperatureByMonth(int month);

      void resetTemperatureByMonth(const openstudio::MonthOfYear& month);

      void resetAllMonths();

      bool setAllMonthlyTemperatures(const std::vector<double>& monthly_temperatures);

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.SiteGroundTemperatureFCfactorMethod");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SITEGROUNDTEMPERATUREFCFACTORMETHOD_IMPL_HPP
