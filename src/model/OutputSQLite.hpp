/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_OUTPUTSQLITE_HPP
#define MODEL_OUTPUTSQLITE_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class OutputSQLite_Impl;

  }  // namespace detail

  /** OutputSQLite is a ModelObject that wraps the OpenStudio IDD object 'OS:Output:SQLite'. */
  class MODEL_API OutputSQLite : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~OutputSQLite() override = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> optionTypeValues();

    static std::vector<std::string> unitConversionforTabularDataValues();

    /** @name Getters */
    //@{

    std::string optionType() const;

    std::string unitConversionforTabularData() const;

    //@}
    /** @name Setters */
    //@{

    bool setOptionType(const std::string& optionType);

    bool setUnitConversionforTabularData(const std::string& unitConversionforTabularData);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::OutputSQLite_Impl;

    explicit OutputSQLite(std::shared_ptr<detail::OutputSQLite_Impl> impl);

    friend class detail::OutputSQLite_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    explicit OutputSQLite(Model& model);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.OutputSQLite");
  };

  /** \relates OutputSQLite*/
  using OptionalOutputSQLite = boost::optional<OutputSQLite>;

  /** \relates OutputSQLite*/
  using OutputSQLiteVector = std::vector<OutputSQLite>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_OUTPUTSQLITE_HPP
