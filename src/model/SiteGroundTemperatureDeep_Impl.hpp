/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SITEGROUNDTEMPERATUREDEEP_IMPL_HPP
#define MODEL_SITEGROUNDTEMPERATUREDEEP_IMPL_HPP

#include <vector>
#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"
#include "../utilities/time/Date.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** SiteGroundTemperatureDeep_Impl is a ModelObject_Impl that is the implementation class for SiteGroundTemperatureDeep.*/
    class MODEL_API SiteGroundTemperatureDeep_Impl : public ModelObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      SiteGroundTemperatureDeep_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SiteGroundTemperatureDeep_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      SiteGroundTemperatureDeep_Impl(const SiteGroundTemperatureDeep_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~SiteGroundTemperatureDeep_Impl() override = default;

      //@}

      virtual boost::optional<ParentObject> parent() const override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      /** @name Getters */
      //@{

      double januaryDeepGroundTemperature() const;

      bool isJanuaryDeepGroundTemperatureDefaulted() const;

      double februaryDeepGroundTemperature() const;

      bool isFebruaryDeepGroundTemperatureDefaulted() const;

      double marchDeepGroundTemperature() const;

      bool isMarchDeepGroundTemperatureDefaulted() const;

      double aprilDeepGroundTemperature() const;

      bool isAprilDeepGroundTemperatureDefaulted() const;

      double mayDeepGroundTemperature() const;

      bool isMayDeepGroundTemperatureDefaulted() const;

      double juneDeepGroundTemperature() const;

      bool isJuneDeepGroundTemperatureDefaulted() const;

      double julyDeepGroundTemperature() const;

      bool isJulyDeepGroundTemperatureDefaulted() const;

      double augustDeepGroundTemperature() const;

      bool isAugustDeepGroundTemperatureDefaulted() const;

      double septemberDeepGroundTemperature() const;

      bool isSeptemberDeepGroundTemperatureDefaulted() const;

      double octoberDeepGroundTemperature() const;

      bool isOctoberDeepGroundTemperatureDefaulted() const;

      double novemberDeepGroundTemperature() const;

      bool isNovemberDeepGroundTemperatureDefaulted() const;

      double decemberDeepGroundTemperature() const;

      bool isDecemberDeepGroundTemperatureDefaulted() const;

      double getTemperatureByMonth(int month) const;

      double getTemperatureByMonth(const openstudio::MonthOfYear& month) const;

      bool isMonthDefaulted(int month) const;

      bool isMonthDefaulted(const openstudio::MonthOfYear& month) const;

      std::vector<double> getAllMonthlyTemperatures() const;

      //@}
      /** @name Setters */
      //@{

      bool setJanuaryDeepGroundTemperature(double januaryDeepGroundTemperature);

      void resetJanuaryDeepGroundTemperature();

      bool setFebruaryDeepGroundTemperature(double februaryDeepGroundTemperature);

      void resetFebruaryDeepGroundTemperature();

      bool setMarchDeepGroundTemperature(double marchDeepGroundTemperature);

      void resetMarchDeepGroundTemperature();

      bool setAprilDeepGroundTemperature(double aprilDeepGroundTemperature);

      void resetAprilDeepGroundTemperature();

      bool setMayDeepGroundTemperature(double mayDeepGroundTemperature);

      void resetMayDeepGroundTemperature();

      bool setJuneDeepGroundTemperature(double juneDeepGroundTemperature);

      void resetJuneDeepGroundTemperature();

      bool setJulyDeepGroundTemperature(double julyDeepGroundTemperature);

      void resetJulyDeepGroundTemperature();

      bool setAugustDeepGroundTemperature(double augustDeepGroundTemperature);

      void resetAugustDeepGroundTemperature();

      bool setSeptemberDeepGroundTemperature(double septemberDeepGroundTemperature);

      void resetSeptemberDeepGroundTemperature();

      bool setOctoberDeepGroundTemperature(double octoberDeepGroundTemperature);

      void resetOctoberDeepGroundTemperature();

      bool setNovemberDeepGroundTemperature(double novemberDeepGroundTemperature);

      void resetNovemberDeepGroundTemperature();

      bool setDecemberDeepGroundTemperature(double decemberDeepGroundTemperature);

      void resetDecemberDeepGroundTemperature();

      bool setTemperatureByMonth(int month, double temperature);

      bool setTemperatureByMonth(const openstudio::MonthOfYear& month, double temperature);

      void resetTemperatureByMonth(int month);

      void resetTemperatureByMonth(const openstudio::MonthOfYear& month);

      void resetAllMonths();

      bool setAllMonthlyTemperatures(const std::vector<double>& monthly_temperatures);

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.SiteGroundTemperatureDeep");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SITEGROUNDTEMPERATUREDEEP_IMPL_HPP
