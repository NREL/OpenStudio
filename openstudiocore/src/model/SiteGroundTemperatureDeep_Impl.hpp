/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#ifndef MODEL_SITEGROUNDTEMPERATUREDEEP_IMPL_HPP
#define MODEL_SITEGROUNDTEMPERATUREDEEP_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

  /** SiteGroundTemperatureDeep_Impl is a ModelObject_Impl that is the implementation class for SiteGroundTemperatureDeep.*/
  class MODEL_API SiteGroundTemperatureDeep_Impl : public ModelObject_Impl {
    Q_OBJECT;
    Q_PROPERTY(double januaryDeepGroundTemperature READ januaryDeepGroundTemperature WRITE setJanuaryDeepGroundTemperature RESET resetJanuaryDeepGroundTemperature);
    Q_PROPERTY(bool isJanuaryDeepGroundTemperatureDefaulted READ isJanuaryDeepGroundTemperatureDefaulted);
    Q_PROPERTY(double februaryDeepGroundTemperature READ februaryDeepGroundTemperature WRITE setFebruaryDeepGroundTemperature RESET resetFebruaryDeepGroundTemperature);
    Q_PROPERTY(bool isFebruaryDeepGroundTemperatureDefaulted READ isFebruaryDeepGroundTemperatureDefaulted);
    Q_PROPERTY(double marchDeepGroundTemperature READ marchDeepGroundTemperature WRITE setMarchDeepGroundTemperature RESET resetMarchDeepGroundTemperature);
    Q_PROPERTY(bool isMarchDeepGroundTemperatureDefaulted READ isMarchDeepGroundTemperatureDefaulted);
    Q_PROPERTY(double aprilDeepGroundTemperature READ aprilDeepGroundTemperature WRITE setAprilDeepGroundTemperature RESET resetAprilDeepGroundTemperature);
    Q_PROPERTY(bool isAprilDeepGroundTemperatureDefaulted READ isAprilDeepGroundTemperatureDefaulted);
    Q_PROPERTY(double mayDeepGroundTemperature READ mayDeepGroundTemperature WRITE setMayDeepGroundTemperature RESET resetMayDeepGroundTemperature);
    Q_PROPERTY(bool isMayDeepGroundTemperatureDefaulted READ isMayDeepGroundTemperatureDefaulted);
    Q_PROPERTY(double juneDeepGroundTemperature READ juneDeepGroundTemperature WRITE setJuneDeepGroundTemperature RESET resetJuneDeepGroundTemperature);
    Q_PROPERTY(bool isJuneDeepGroundTemperatureDefaulted READ isJuneDeepGroundTemperatureDefaulted);
    Q_PROPERTY(double julyDeepGroundTemperature READ julyDeepGroundTemperature WRITE setJulyDeepGroundTemperature RESET resetJulyDeepGroundTemperature);
    Q_PROPERTY(bool isJulyDeepGroundTemperatureDefaulted READ isJulyDeepGroundTemperatureDefaulted);
    Q_PROPERTY(double augustDeepGroundTemperature READ augustDeepGroundTemperature WRITE setAugustDeepGroundTemperature RESET resetAugustDeepGroundTemperature);
    Q_PROPERTY(bool isAugustDeepGroundTemperatureDefaulted READ isAugustDeepGroundTemperatureDefaulted);
    Q_PROPERTY(double septemberDeepGroundTemperature READ septemberDeepGroundTemperature WRITE setSeptemberDeepGroundTemperature RESET resetSeptemberDeepGroundTemperature);
    Q_PROPERTY(bool isSeptemberDeepGroundTemperatureDefaulted READ isSeptemberDeepGroundTemperatureDefaulted);
    Q_PROPERTY(double octoberDeepGroundTemperature READ octoberDeepGroundTemperature WRITE setOctoberDeepGroundTemperature RESET resetOctoberDeepGroundTemperature);
    Q_PROPERTY(bool isOctoberDeepGroundTemperatureDefaulted READ isOctoberDeepGroundTemperatureDefaulted);
    Q_PROPERTY(double novemberDeepGroundTemperature READ novemberDeepGroundTemperature WRITE setNovemberDeepGroundTemperature RESET resetNovemberDeepGroundTemperature);
    Q_PROPERTY(bool isNovemberDeepGroundTemperatureDefaulted READ isNovemberDeepGroundTemperatureDefaulted);
    Q_PROPERTY(double decemberDeepGroundTemperature READ decemberDeepGroundTemperature WRITE setDecemberDeepGroundTemperature RESET resetDecemberDeepGroundTemperature);
    Q_PROPERTY(bool isDecemberDeepGroundTemperatureDefaulted READ isDecemberDeepGroundTemperatureDefaulted);
   public:

    /** @name Constructors and Destructors */
    //@{

    SiteGroundTemperatureDeep_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    SiteGroundTemperatureDeep_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                              Model_Impl* model,
                                              bool keepHandle);

    SiteGroundTemperatureDeep_Impl(const SiteGroundTemperatureDeep_Impl& other,
                                              Model_Impl* model,
                                              bool keepHandle);

    virtual ~SiteGroundTemperatureDeep_Impl() {}

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

    //@}
    /** @name Setters */
    //@{

    void setJanuaryDeepGroundTemperature(double januaryDeepGroundTemperature);

    void resetJanuaryDeepGroundTemperature();

    void setFebruaryDeepGroundTemperature(double februaryDeepGroundTemperature);

    void resetFebruaryDeepGroundTemperature();

    void setMarchDeepGroundTemperature(double marchDeepGroundTemperature);

    void resetMarchDeepGroundTemperature();

    void setAprilDeepGroundTemperature(double aprilDeepGroundTemperature);

    void resetAprilDeepGroundTemperature();

    void setMayDeepGroundTemperature(double mayDeepGroundTemperature);

    void resetMayDeepGroundTemperature();

    void setJuneDeepGroundTemperature(double juneDeepGroundTemperature);

    void resetJuneDeepGroundTemperature();

    void setJulyDeepGroundTemperature(double julyDeepGroundTemperature);

    void resetJulyDeepGroundTemperature();

    void setAugustDeepGroundTemperature(double augustDeepGroundTemperature);

    void resetAugustDeepGroundTemperature();

    void setSeptemberDeepGroundTemperature(double septemberDeepGroundTemperature);

    void resetSeptemberDeepGroundTemperature();

    void setOctoberDeepGroundTemperature(double octoberDeepGroundTemperature);

    void resetOctoberDeepGroundTemperature();

    void setNovemberDeepGroundTemperature(double novemberDeepGroundTemperature);

    void resetNovemberDeepGroundTemperature();

    void setDecemberDeepGroundTemperature(double decemberDeepGroundTemperature);

    void resetDecemberDeepGroundTemperature();

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.SiteGroundTemperatureDeep");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_SITEGROUNDTEMPERATUREDEEP_IMPL_HPP
