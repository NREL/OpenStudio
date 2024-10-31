/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_OUTPUTCONTROLTABLESTYLE_HPP
#define MODEL_OUTPUTCONTROLTABLESTYLE_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class OutputControlTableStyle_Impl;

  }  // namespace detail

  /** OutputControlTableStyle is a ModelObject that wraps the OpenStudio IDD object 'OS:OutputControl:Table:Style'. */
  class MODEL_API OutputControlTableStyle : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~OutputControlTableStyle() override = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> columnSeparatorValues();

    static std::vector<std::string> unitConversionValues();

    /** @name Getters */
    //@{

    std::string columnSeparator() const;

    std::string unitConversion() const;

    //@}
    /** @name Setters */
    //@{

    bool setColumnSeparator(const std::string& columnSeparator);

    bool setUnitConversion(const std::string& unitConversion);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::OutputControlTableStyle_Impl;

    explicit OutputControlTableStyle(std::shared_ptr<detail::OutputControlTableStyle_Impl> impl);

    friend class detail::OutputControlTableStyle_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    explicit OutputControlTableStyle(Model& model);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.OutputControlTableStyle");
  };

  /** \relates OutputControlTableStyle*/
  using OptionalOutputControlTableStyle = boost::optional<OutputControlTableStyle>;

  /** \relates OutputControlTableStyle*/
  using OutputControlTableStyleVector = std::vector<OutputControlTableStyle>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_OUTPUTCONTROLTABLESTYLE_HPP
