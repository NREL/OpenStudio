/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PHOTOVOLTAICPERFORMANCESANDIA_HPP
#define MODEL_PHOTOVOLTAICPERFORMANCESANDIA_HPP

#include <model/ModelAPI.hpp>
#include "PhotovoltaicPerformance.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class PhotovoltaicPerformanceSandia_Impl;

  }  // namespace detail

  /** PhotovoltaicPerformanceSandia is a PhotovoltaicPerformance that wraps the OpenStudio IDD object 'OS:PhotovoltaicPerformance:Sandia'. */
  class MODEL_API PhotovoltaicPerformanceSandia : public PhotovoltaicPerformance
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    // Defaults all characteristics to one random entry in the Sandia Database
    explicit PhotovoltaicPerformanceSandia(const Model& model);

    explicit PhotovoltaicPerformanceSandia(
      const Model& model, double activeArea, int numberofCellsinSeries, int numberofCellsinParallel, double shortCircuitCurrent,
      double openCircuitVoltage, double currentatMaximumPowerPoint, double voltageatMaximumPowerPoint, double sandiaDatabaseParameteraIsc,
      double sandiaDatabaseParameteraImp, double sandiaDatabaseParameterc0, double sandiaDatabaseParameterc1, double sandiaDatabaseParameterBVoc0,
      double sandiaDatabaseParametermBVoc, double sandiaDatabaseParameterBVmp0, double sandiaDatabaseParametermBVmp, double diodeFactor,
      double sandiaDatabaseParameterc2, double sandiaDatabaseParameterc3, double sandiaDatabaseParametera0, double sandiaDatabaseParametera1,
      double sandiaDatabaseParametera2, double sandiaDatabaseParametera3, double sandiaDatabaseParametera4, double sandiaDatabaseParameterb0,
      double sandiaDatabaseParameterb1, double sandiaDatabaseParameterb2, double sandiaDatabaseParameterb3, double sandiaDatabaseParameterb4,
      double sandiaDatabaseParameterb5, double sandiaDatabaseParameterDeltaTc, double sandiaDatabaseParameterfd, double sandiaDatabaseParametera,
      double sandiaDatabaseParameterb, double sandiaDatabaseParameterc4, double sandiaDatabaseParameterc5, double sandiaDatabaseParameterIx0,
      double sandiaDatabaseParameterIxx0, double sandiaDatabaseParameterc6, double sandiaDatabaseParameterc7);

    /// Factory method to creates a PhotovoltaicPerformanceSandia by looking up characteristics in the embedded Sandia database by its name
    // Please use the PhotovoltaicPerformanceSandia::sandiaModulePerformanceNames() static method to look up the valid names
    // as it will throw if it cannot find it
    static PhotovoltaicPerformanceSandia fromSandiaDatabase(const Model& model, const std::string& sandiaModulePerformanceName);

    virtual ~PhotovoltaicPerformanceSandia() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    PhotovoltaicPerformanceSandia(const PhotovoltaicPerformanceSandia& other) = default;
    PhotovoltaicPerformanceSandia(PhotovoltaicPerformanceSandia&& other) = default;
    PhotovoltaicPerformanceSandia& operator=(const PhotovoltaicPerformanceSandia&) = default;
    PhotovoltaicPerformanceSandia& operator=(PhotovoltaicPerformanceSandia&&) = default;

    //@}

    static std::vector<std::string> sandiaModulePerformanceNames();

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    double activeArea() const;

    int numberofCellsinSeries() const;

    int numberofCellsinParallel() const;

    double shortCircuitCurrent() const;

    double openCircuitVoltage() const;

    double currentatMaximumPowerPoint() const;

    double voltageatMaximumPowerPoint() const;

    double sandiaDatabaseParameteraIsc() const;

    double sandiaDatabaseParameteraImp() const;

    double sandiaDatabaseParameterc0() const;

    double sandiaDatabaseParameterc1() const;

    double sandiaDatabaseParameterBVoc0() const;

    double sandiaDatabaseParametermBVoc() const;

    double sandiaDatabaseParameterBVmp0() const;

    double sandiaDatabaseParametermBVmp() const;

    double diodeFactor() const;

    double sandiaDatabaseParameterc2() const;

    double sandiaDatabaseParameterc3() const;

    double sandiaDatabaseParametera0() const;

    double sandiaDatabaseParametera1() const;

    double sandiaDatabaseParametera2() const;

    double sandiaDatabaseParametera3() const;

    double sandiaDatabaseParametera4() const;

    double sandiaDatabaseParameterb0() const;

    double sandiaDatabaseParameterb1() const;

    double sandiaDatabaseParameterb2() const;

    double sandiaDatabaseParameterb3() const;

    double sandiaDatabaseParameterb4() const;

    double sandiaDatabaseParameterb5() const;

    double sandiaDatabaseParameterDeltaTc() const;

    double sandiaDatabaseParameterfd() const;

    double sandiaDatabaseParametera() const;

    double sandiaDatabaseParameterb() const;

    double sandiaDatabaseParameterc4() const;

    double sandiaDatabaseParameterc5() const;

    double sandiaDatabaseParameterIx0() const;

    double sandiaDatabaseParameterIxx0() const;

    double sandiaDatabaseParameterc6() const;

    double sandiaDatabaseParameterc7() const;

    //@}
    /** @name Setters */
    //@{

    bool setActiveArea(double activeArea);

    bool setNumberofCellsinSeries(int numberofCellsinSeries);

    bool setNumberofCellsinParallel(int numberofCellsinParallel);

    bool setShortCircuitCurrent(double shortCircuitCurrent);

    bool setOpenCircuitVoltage(double openCircuitVoltage);

    bool setCurrentatMaximumPowerPoint(double currentatMaximumPowerPoint);

    bool setVoltageatMaximumPowerPoint(double voltageatMaximumPowerPoint);

    bool setSandiaDatabaseParameteraIsc(double sandiaDatabaseParameteraIsc);

    bool setSandiaDatabaseParameteraImp(double sandiaDatabaseParameteraImp);

    bool setSandiaDatabaseParameterc0(double sandiaDatabaseParameterc0);

    bool setSandiaDatabaseParameterc1(double sandiaDatabaseParameterc1);

    bool setSandiaDatabaseParameterBVoc0(double sandiaDatabaseParameterBVoc0);

    bool setSandiaDatabaseParametermBVoc(double sandiaDatabaseParametermBVoc);

    bool setSandiaDatabaseParameterBVmp0(double sandiaDatabaseParameterBVmp0);

    bool setSandiaDatabaseParametermBVmp(double sandiaDatabaseParametermBVmp);

    bool setDiodeFactor(double diodeFactor);

    bool setSandiaDatabaseParameterc2(double sandiaDatabaseParameterc2);

    bool setSandiaDatabaseParameterc3(double sandiaDatabaseParameterc3);

    bool setSandiaDatabaseParametera0(double sandiaDatabaseParametera0);

    bool setSandiaDatabaseParametera1(double sandiaDatabaseParametera1);

    bool setSandiaDatabaseParametera2(double sandiaDatabaseParametera2);

    bool setSandiaDatabaseParametera3(double sandiaDatabaseParametera3);

    bool setSandiaDatabaseParametera4(double sandiaDatabaseParametera4);

    bool setSandiaDatabaseParameterb0(double sandiaDatabaseParameterb0);

    bool setSandiaDatabaseParameterb1(double sandiaDatabaseParameterb1);

    bool setSandiaDatabaseParameterb2(double sandiaDatabaseParameterb2);

    bool setSandiaDatabaseParameterb3(double sandiaDatabaseParameterb3);

    bool setSandiaDatabaseParameterb4(double sandiaDatabaseParameterb4);

    bool setSandiaDatabaseParameterb5(double sandiaDatabaseParameterb5);

    bool setSandiaDatabaseParameterDeltaTc(double sandiaDatabaseParameterDeltaTc);

    bool setSandiaDatabaseParameterfd(double sandiaDatabaseParameterfd);

    bool setSandiaDatabaseParametera(double sandiaDatabaseParametera);

    bool setSandiaDatabaseParameterb(double sandiaDatabaseParameterb);

    bool setSandiaDatabaseParameterc4(double sandiaDatabaseParameterc4);

    bool setSandiaDatabaseParameterc5(double sandiaDatabaseParameterc5);

    bool setSandiaDatabaseParameterIx0(double sandiaDatabaseParameterIx0);

    bool setSandiaDatabaseParameterIxx0(double sandiaDatabaseParameterIxx0);

    bool setSandiaDatabaseParameterc6(double sandiaDatabaseParameterc6);

    bool setSandiaDatabaseParameterc7(double sandiaDatabaseParameterc7);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::PhotovoltaicPerformanceSandia_Impl;

    explicit PhotovoltaicPerformanceSandia(std::shared_ptr<detail::PhotovoltaicPerformanceSandia_Impl> impl);

    friend class detail::PhotovoltaicPerformanceSandia_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.PhotovoltaicPerformanceSandia");
  };

  /** \relates PhotovoltaicPerformanceSandia*/
  using OptionalPhotovoltaicPerformanceSandia = boost::optional<PhotovoltaicPerformanceSandia>;

  /** \relates PhotovoltaicPerformanceSandia*/
  using PhotovoltaicPerformanceSandiaVector = std::vector<PhotovoltaicPerformanceSandia>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PHOTOVOLTAICPERFORMANCESANDIA_HPP
