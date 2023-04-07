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

#ifndef MODEL_AIRFLOWNETWORKDETAILEDOPENING_HPP
#define MODEL_AIRFLOWNETWORKDETAILEDOPENING_HPP

#include "ModelAPI.hpp"

#include "AirflowNetworkComponent.hpp"

namespace openstudio {

namespace model {

  class MODEL_API DetailedOpeningFactorData
  {
   public:
    /** @name Constructor */
    //@{
    DetailedOpeningFactorData(double openingFactor, double dischargeCoefficient, double widthFactor, double heightFactor, double startHeightFactor);

    //@}
    /** @name Getters */
    //@{
    /** For a rectangular non-pivoted window or door (LVO Type 1), the opening factor corresponds to the fraction of the
   * window or door that is opened. For rectangular horizontally-pivoted windows (LVO Type 2), the opening factor is
   * the angular fraction that window is open. For example, an opening angle of 45 degrees corresponds to an opening
   * factor of 0.50 since the maximum opening angle is 90 degrees.
   */
    double openingFactor() const;
    /** The discharge coefficient indicates the fractional effectiveness for air flow through a window or door at
   * that Opening Factor.
   */
    double dischargeCoefficient() const;
    /** The width factor is the opening width divided by the window or door width. */
    double widthFactor() const;
    /** The height factor is the opening height divided by the window or door height. */
    double heightFactor() const;
    /** The start height factor is the start height divided by the window or door height.
   * Start height is the distance between the bottom of the window or door and the
   * bottom of the window or door opening. The sum of the height factor and the start height
   * factor must be less than 1.0 in order to have the opening within the window or door dimensions.
   */
    double startHeightFactor() const;

    //@}
    /** @name Setters */
    //@{
    /** Sets the opening factor. */
    bool setOpeningFactor(double openingFactor);
    /** Sets the discharge coefficient. */
    bool setDischargeCoefficient(double dischargeCoefficient);
    /** Sets the width factor. */
    bool setWidthFactor(double widthFactor);
    /** Sets the height factor. */
    bool setHeightFactor(double heightFactor);
    /** Sets the start height factor. */
    bool setStartHeightFactor(double startHeightFactor);
    //@}
   private:
    double m_openingFactor;
    double m_dischargeCoefficient;
    double m_widthFactor;
    double m_heightFactor;
    double m_startHeightFactor;
  };

  namespace detail {

    class AirflowNetworkDetailedOpening_Impl;

  }  // namespace detail

  /** AirflowNetworkDetailedOpening is a ModelObject that wraps the OpenStudio IDD object 'OS:AirflowNetworkDetailedOpening'. */
  class MODEL_API AirflowNetworkDetailedOpening : public AirflowNetworkComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{
    /** Construct a detailed opening object. */
    AirflowNetworkDetailedOpening(const Model& model, double massFlowCoefficientWhenOpeningisClosed, double massFlowExponentWhenOpeningisClosed,
                                  const std::string& typeofRectangularLargeVerticalOpening, double extraCrackLengthorHeightofPivotingAxis,
                                  std::vector<DetailedOpeningFactorData>& openingFactors);
    /** Construct a detailed opening object with defaulted values. */
    AirflowNetworkDetailedOpening(const Model& model, double massFlowCoefficientWhenOpeningisClosed,
                                  std::vector<DetailedOpeningFactorData>& openingFactors);

    virtual ~AirflowNetworkDetailedOpening() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirflowNetworkDetailedOpening(const AirflowNetworkDetailedOpening& other) = default;
    AirflowNetworkDetailedOpening(AirflowNetworkDetailedOpening&& other) = default;
    AirflowNetworkDetailedOpening& operator=(const AirflowNetworkDetailedOpening&) = default;
    AirflowNetworkDetailedOpening& operator=(AirflowNetworkDetailedOpening&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> typeofRectangularLargeVerticalOpeningValues();

    /** @name Getters */
    //@{
    /** Returns the air mass flow coefficient when the opening is closed. */
    double airMassFlowCoefficientWhenOpeningisClosed() const;
    /** Returns the air mass flow exponent when the opening is closed. */
    double airMassFlowExponentWhenOpeningisClosed() const;
    /** Returns true if the air mass flow exponent when the opening is closed is defaulted. */
    bool isAirMassFlowExponentWhenOpeningisClosedDefaulted() const;
    /** Returns the LVO type. */
    std::string typeofRectangularLargeVerticalOpening() const;
    /** Returns true if the LVO type is defaulted. */
    bool isTypeofRectangularLargeVerticalOpeningDefaulted() const;
    /** Returns the extra crack length or height of pivoting axis. */
    double extraCrackLengthorHeightofPivotingAxis() const;
    /** Returns true if the extra crack length or height of pivoting axis is defaulted. */
    bool isExtraCrackLengthorHeightofPivotingAxisDefaulted() const;
    /** Returns the opening factor data. */
    std::vector<DetailedOpeningFactorData> openingFactors() const;

    //@}
    /** @name Setters */
    //@{
    /** Sets the air mass flow coefficient when the opening is closed. */
    bool setAirMassFlowCoefficientWhenOpeningisClosed(double airMassFlowCoefficientWhenOpeningisClosed);
    /** Sets the air mass flow exponent when the opening is closed. */
    bool setAirMassFlowExponentWhenOpeningisClosed(double airMassFlowExponentWhenOpeningisClosed);
    /** Resets the air mass flow exponent when the opening is closed. */
    void resetAirMassFlowExponentWhenOpeningisClosed();
    /** Sets the LVO type. */
    bool setTypeofRectangularLargeVerticalOpening(const std::string& typeofRectangularLargeVerticalOpening);
    /** Resets the LVO type. */
    void resetTypeofRectangularLargeVerticalOpening();
    /** Sets the extra crack length or height of pivoting axis. */
    bool setExtraCrackLengthorHeightofPivotingAxis(double extraCrackLengthorHeightofPivotingAxis);
    /** Resets the extra crack length or height of pivoting axis. */
    void resetExtraCrackLengthorHeightofPivotingAxis();
    /** Sets the opening factor data. */
    bool setOpeningFactors(std::vector<DetailedOpeningFactorData>& factors);

    //@}
   protected:
    /// @cond
    using ImplType = detail::AirflowNetworkDetailedOpening_Impl;

    explicit AirflowNetworkDetailedOpening(std::shared_ptr<detail::AirflowNetworkDetailedOpening_Impl> impl);

    friend class detail::AirflowNetworkDetailedOpening_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirflowNetworkDetailedOpening");
  };

  /** \relates AirflowNetworkDetailedOpening*/
  using OptionalAirflowNetworkDetailedOpening = boost::optional<AirflowNetworkDetailedOpening>;

  /** \relates AirflowNetworkDetailedOpening*/
  using AirflowNetworkDetailedOpeningVector = std::vector<AirflowNetworkDetailedOpening>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKDETAILEDOPENING_HPP
