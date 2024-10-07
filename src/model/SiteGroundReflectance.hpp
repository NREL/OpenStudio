/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SITEGROUNDREFLECTANCE_HPP
#define MODEL_SITEGROUNDREFLECTANCE_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class SiteGroundReflectance_Impl;

  }  // namespace detail

  /** SiteGroundReflectance is a ModelObject that wraps the OpenStudio IDD object 'OS:Site:GroundReflectance'. */

  class MODEL_API SiteGroundReflectance : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~SiteGroundReflectance() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SiteGroundReflectance(const SiteGroundReflectance& other) = default;
    SiteGroundReflectance(SiteGroundReflectance&& other) = default;
    SiteGroundReflectance& operator=(const SiteGroundReflectance&) = default;
    SiteGroundReflectance& operator=(SiteGroundReflectance&&) = default;

    //@}
    /** @name Static Methods */
    //@{

    static IddObjectType iddObjectType();

    //@}
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
    /// @cond
    using ImplType = detail::SiteGroundReflectance_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class detail::SiteGroundReflectance_Impl;

    explicit SiteGroundReflectance(std::shared_ptr<detail::SiteGroundReflectance_Impl> impl);

    explicit SiteGroundReflectance(Model& model);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SiteGroundReflectance");
  };

  /** \relates SiteGroundReflectance*/
  using OptionalSiteGroundReflectance = boost::optional<SiteGroundReflectance>;

  /** \relates SiteGroundReflectance*/
  using SiteGroundReflectanceVector = std::vector<SiteGroundReflectance>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SITEGROUNDREFLECTANCE_HPP
