/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#ifndef MODEL_SITEGROUNDTEMPERATUREBUILDINGSURFACE_IMPL_HPP
#define MODEL_SITEGROUNDTEMPERATUREBUILDINGSURFACE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

  /** SiteGroundTemperatureBuildingSurface_Impl is a ModelObject_Impl that is the implementation class for SiteGroundTemperatureBuildingSurface.*/
  class MODEL_API SiteGroundTemperatureBuildingSurface_Impl : public ModelObject_Impl {
    Q_OBJECT;
    Q_PROPERTY(double januaryGroundTemperature READ januaryGroundTemperature WRITE setJanuaryGroundTemperature RESET resetJanuaryGroundTemperature);
    Q_PROPERTY(bool isJanuaryGroundTemperatureDefaulted READ isJanuaryGroundTemperatureDefaulted);
    Q_PROPERTY(double februaryGroundTemperature READ februaryGroundTemperature WRITE setFebruaryGroundTemperature RESET resetFebruaryGroundTemperature);
    Q_PROPERTY(bool isFebruaryGroundTemperatureDefaulted READ isFebruaryGroundTemperatureDefaulted);
    Q_PROPERTY(double marchGroundTemperature READ marchGroundTemperature WRITE setMarchGroundTemperature RESET resetMarchGroundTemperature);
    Q_PROPERTY(bool isMarchGroundTemperatureDefaulted READ isMarchGroundTemperatureDefaulted);
    Q_PROPERTY(double aprilGroundTemperature READ aprilGroundTemperature WRITE setAprilGroundTemperature RESET resetAprilGroundTemperature);
    Q_PROPERTY(bool isAprilGroundTemperatureDefaulted READ isAprilGroundTemperatureDefaulted);
    Q_PROPERTY(double mayGroundTemperature READ mayGroundTemperature WRITE setMayGroundTemperature RESET resetMayGroundTemperature);
    Q_PROPERTY(bool isMayGroundTemperatureDefaulted READ isMayGroundTemperatureDefaulted);
    Q_PROPERTY(double juneGroundTemperature READ juneGroundTemperature WRITE setJuneGroundTemperature RESET resetJuneGroundTemperature);
    Q_PROPERTY(bool isJuneGroundTemperatureDefaulted READ isJuneGroundTemperatureDefaulted);
    Q_PROPERTY(double julyGroundTemperature READ julyGroundTemperature WRITE setJulyGroundTemperature RESET resetJulyGroundTemperature);
    Q_PROPERTY(bool isJulyGroundTemperatureDefaulted READ isJulyGroundTemperatureDefaulted);
    Q_PROPERTY(double augustGroundTemperature READ augustGroundTemperature WRITE setAugustGroundTemperature RESET resetAugustGroundTemperature);
    Q_PROPERTY(bool isAugustGroundTemperatureDefaulted READ isAugustGroundTemperatureDefaulted);
    Q_PROPERTY(double septemberGroundTemperature READ septemberGroundTemperature WRITE setSeptemberGroundTemperature RESET resetSeptemberGroundTemperature);
    Q_PROPERTY(bool isSeptemberGroundTemperatureDefaulted READ isSeptemberGroundTemperatureDefaulted);
    Q_PROPERTY(double octoberGroundTemperature READ octoberGroundTemperature WRITE setOctoberGroundTemperature RESET resetOctoberGroundTemperature);
    Q_PROPERTY(bool isOctoberGroundTemperatureDefaulted READ isOctoberGroundTemperatureDefaulted);
    Q_PROPERTY(double novemberGroundTemperature READ novemberGroundTemperature WRITE setNovemberGroundTemperature RESET resetNovemberGroundTemperature);
    Q_PROPERTY(bool isNovemberGroundTemperatureDefaulted READ isNovemberGroundTemperatureDefaulted);
    Q_PROPERTY(double decemberGroundTemperature READ decemberGroundTemperature WRITE setDecemberGroundTemperature RESET resetDecemberGroundTemperature);
    Q_PROPERTY(bool isDecemberGroundTemperatureDefaulted READ isDecemberGroundTemperatureDefaulted);
   public:

    /** @name Constructors and Destructors */
    //@{

    SiteGroundTemperatureBuildingSurface_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    SiteGroundTemperatureBuildingSurface_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                              Model_Impl* model,
                                              bool keepHandle);

    SiteGroundTemperatureBuildingSurface_Impl(const SiteGroundTemperatureBuildingSurface_Impl& other,
                                              Model_Impl* model,
                                              bool keepHandle);

    virtual ~SiteGroundTemperatureBuildingSurface_Impl() {}

    //@}

    virtual boost::optional<ParentObject> parent() const;

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

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

    //@}
    /** @name Setters */
    //@{

    void setJanuaryGroundTemperature(double januaryGroundTemperature);

    void resetJanuaryGroundTemperature();

    void setFebruaryGroundTemperature(double februaryGroundTemperature);

    void resetFebruaryGroundTemperature();

    void setMarchGroundTemperature(double marchGroundTemperature);

    void resetMarchGroundTemperature();

    void setAprilGroundTemperature(double aprilGroundTemperature);

    void resetAprilGroundTemperature();

    void setMayGroundTemperature(double mayGroundTemperature);

    void resetMayGroundTemperature();

    void setJuneGroundTemperature(double juneGroundTemperature);

    void resetJuneGroundTemperature();

    void setJulyGroundTemperature(double julyGroundTemperature);

    void resetJulyGroundTemperature();

    void setAugustGroundTemperature(double augustGroundTemperature);

    void resetAugustGroundTemperature();

    void setSeptemberGroundTemperature(double septemberGroundTemperature);

    void resetSeptemberGroundTemperature();

    void setOctoberGroundTemperature(double octoberGroundTemperature);

    void resetOctoberGroundTemperature();

    void setNovemberGroundTemperature(double novemberGroundTemperature);

    void resetNovemberGroundTemperature();

    void setDecemberGroundTemperature(double decemberGroundTemperature);

    void resetDecemberGroundTemperature();

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.SiteGroundTemperatureBuildingSurface");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_SITEGROUNDTEMPERATUREBUILDINGSURFACE_IMPL_HPP

