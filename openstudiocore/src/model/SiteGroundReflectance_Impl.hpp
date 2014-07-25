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

#ifndef MODEL_SITEGROUNDREFLECTANCE_IMPL_HPP
#define MODEL_SITEGROUNDREFLECTANCE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

  /** SiteGroundReflectance_Impl is a ModelObject_Impl that is the implementation class for SiteGroundReflectance.*/
  class MODEL_API SiteGroundReflectance_Impl : public ModelObject_Impl {
    Q_OBJECT;
    Q_PROPERTY(double januaryGroundReflectance READ januaryGroundReflectance WRITE setJanuaryGroundReflectance RESET resetJanuaryGroundReflectance);
    Q_PROPERTY(bool isJanuaryGroundReflectanceDefaulted READ isJanuaryGroundReflectanceDefaulted);
    Q_PROPERTY(double februaryGroundReflectance READ februaryGroundReflectance WRITE setFebruaryGroundReflectance RESET resetFebruaryGroundReflectance);
    Q_PROPERTY(bool isFebruaryGroundReflectanceDefaulted READ isFebruaryGroundReflectanceDefaulted);
    Q_PROPERTY(double marchGroundReflectance READ marchGroundReflectance WRITE setMarchGroundReflectance RESET resetMarchGroundReflectance);
    Q_PROPERTY(bool isMarchGroundReflectanceDefaulted READ isMarchGroundReflectanceDefaulted);
    Q_PROPERTY(double aprilGroundReflectance READ aprilGroundReflectance WRITE setAprilGroundReflectance RESET resetAprilGroundReflectance);
    Q_PROPERTY(bool isAprilGroundReflectanceDefaulted READ isAprilGroundReflectanceDefaulted);
    Q_PROPERTY(double mayGroundReflectance READ mayGroundReflectance WRITE setMayGroundReflectance RESET resetMayGroundReflectance);
    Q_PROPERTY(bool isMayGroundReflectanceDefaulted READ isMayGroundReflectanceDefaulted);
    Q_PROPERTY(double juneGroundReflectance READ juneGroundReflectance WRITE setJuneGroundReflectance RESET resetJuneGroundReflectance);
    Q_PROPERTY(bool isJuneGroundReflectanceDefaulted READ isJuneGroundReflectanceDefaulted);
    Q_PROPERTY(double julyGroundReflectance READ julyGroundReflectance WRITE setJulyGroundReflectance RESET resetJulyGroundReflectance);
    Q_PROPERTY(bool isJulyGroundReflectanceDefaulted READ isJulyGroundReflectanceDefaulted);
    Q_PROPERTY(double augustGroundReflectance READ augustGroundReflectance WRITE setAugustGroundReflectance RESET resetAugustGroundReflectance);
    Q_PROPERTY(bool isAugustGroundReflectanceDefaulted READ isAugustGroundReflectanceDefaulted);
    Q_PROPERTY(double septemberGroundReflectance READ septemberGroundReflectance WRITE setSeptemberGroundReflectance RESET resetSeptemberGroundReflectance);
    Q_PROPERTY(bool isSeptemberGroundReflectanceDefaulted READ isSeptemberGroundReflectanceDefaulted);
    Q_PROPERTY(double octoberGroundReflectance READ octoberGroundReflectance WRITE setOctoberGroundReflectance RESET resetOctoberGroundReflectance);
    Q_PROPERTY(bool isOctoberGroundReflectanceDefaulted READ isOctoberGroundReflectanceDefaulted);
    Q_PROPERTY(double novemberGroundReflectance READ novemberGroundReflectance WRITE setNovemberGroundReflectance RESET resetNovemberGroundReflectance);
    Q_PROPERTY(bool isNovemberGroundReflectanceDefaulted READ isNovemberGroundReflectanceDefaulted);
    Q_PROPERTY(double decemberGroundReflectance READ decemberGroundReflectance WRITE setDecemberGroundReflectance RESET resetDecemberGroundReflectance);
    Q_PROPERTY(bool isDecemberGroundReflectanceDefaulted READ isDecemberGroundReflectanceDefaulted);
   public:

    /** @name Constructors and Destructors */
    //@{

    SiteGroundReflectance_Impl(const IdfObject& idfObject,
                               Model_Impl* model,
                               bool keepHandle);

    SiteGroundReflectance_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                               Model_Impl* model,
                               bool keepHandle);

    SiteGroundReflectance_Impl(const SiteGroundReflectance_Impl& other,
                               Model_Impl* model,
                               bool keepHandle);

    virtual ~SiteGroundReflectance_Impl() {}

    //@}

    virtual boost::optional<ParentObject> parent() const;

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    /** @name Getters */
    //@{

    double januaryGroundReflectance() const;

    bool isJanuaryGroundReflectanceDefaulted() const;

    double februaryGroundReflectance() const;

    bool isFebruaryGroundReflectanceDefaulted() const;

    double marchGroundReflectance() const;

    bool isMarchGroundReflectanceDefaulted() const;

    double aprilGroundReflectance() const;

    bool isAprilGroundReflectanceDefaulted() const;

    double mayGroundReflectance() const;

    bool isMayGroundReflectanceDefaulted() const;

    double juneGroundReflectance() const;

    bool isJuneGroundReflectanceDefaulted() const;

    double julyGroundReflectance() const;

    bool isJulyGroundReflectanceDefaulted() const;

    double augustGroundReflectance() const;

    bool isAugustGroundReflectanceDefaulted() const;

    double septemberGroundReflectance() const;

    bool isSeptemberGroundReflectanceDefaulted() const;

    double octoberGroundReflectance() const;

    bool isOctoberGroundReflectanceDefaulted() const;

    double novemberGroundReflectance() const;

    bool isNovemberGroundReflectanceDefaulted() const;

    double decemberGroundReflectance() const;

    bool isDecemberGroundReflectanceDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setJanuaryGroundReflectance(double januaryGroundReflectance);

    void resetJanuaryGroundReflectance();

    bool setFebruaryGroundReflectance(double februaryGroundReflectance);

    void resetFebruaryGroundReflectance();

    bool setMarchGroundReflectance(double marchGroundReflectance);

    void resetMarchGroundReflectance();

    bool setAprilGroundReflectance(double aprilGroundReflectance);

    void resetAprilGroundReflectance();

    bool setMayGroundReflectance(double mayGroundReflectance);

    void resetMayGroundReflectance();

    bool setJuneGroundReflectance(double juneGroundReflectance);

    void resetJuneGroundReflectance();

    bool setJulyGroundReflectance(double julyGroundReflectance);

    void resetJulyGroundReflectance();

    bool setAugustGroundReflectance(double augustGroundReflectance);

    void resetAugustGroundReflectance();

    bool setSeptemberGroundReflectance(double septemberGroundReflectance);

    void resetSeptemberGroundReflectance();

    bool setOctoberGroundReflectance(double octoberGroundReflectance);

    void resetOctoberGroundReflectance();

    bool setNovemberGroundReflectance(double novemberGroundReflectance);

    void resetNovemberGroundReflectance();

    bool setDecemberGroundReflectance(double decemberGroundReflectance);

    void resetDecemberGroundReflectance();

    //@}

   protected:
   private:
    REGISTER_LOGGER("openstudio.model.SiteGroundReflectance");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_SITEGROUNDREFLECTANCE_IMPL_HPP

