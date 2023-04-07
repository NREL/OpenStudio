/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

      virtual ~SiteGroundTemperatureFCfactorMethod_Impl() = default;

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
