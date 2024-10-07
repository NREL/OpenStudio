/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SITEGROUNDREFLECTANCE_IMPL_HPP
#define MODEL_SITEGROUNDREFLECTANCE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** SiteGroundReflectance_Impl is a ModelObject_Impl that is the implementation class for SiteGroundReflectance.*/
    class MODEL_API SiteGroundReflectance_Impl : public ModelObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      SiteGroundReflectance_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SiteGroundReflectance_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      SiteGroundReflectance_Impl(const SiteGroundReflectance_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~SiteGroundReflectance_Impl() override = default;

      //@}

      virtual boost::optional<ParentObject> parent() const override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

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

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SITEGROUNDREFLECTANCE_IMPL_HPP
