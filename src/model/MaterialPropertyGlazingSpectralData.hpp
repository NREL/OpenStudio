/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_MATERIALPROPERTYGLAZINGSPECTRALDATA_HPP
#define MODEL_MATERIALPROPERTYGLAZINGSPECTRALDATA_HPP

#include "ModelAPI.hpp"
#include "ResourceObject.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class MaterialPropertyGlazingSpectralData_Impl;

  }  // namespace detail

  class MODEL_API SpectralDataField
  {
   public:
    SpectralDataField(double wavelength, double transmittance, double frontReflectance, double backReflectance);

    double wavelength() const;
    double transmittance() const;
    double frontReflectance() const;
    double backReflectance() const;

   private:
    double m_wavelength;
    double m_transmittance;
    double m_frontReflectance;
    double m_backReflectance;
  };

  /** MaterialPropertyGlazingSpectralData is a ResourceObject that wraps the OpenStudio IDD object 'OS:MaterialProperty:GlazingSpectralData'. */
  class MODEL_API MaterialPropertyGlazingSpectralData : public ResourceObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit MaterialPropertyGlazingSpectralData(const Model& model);

    virtual ~MaterialPropertyGlazingSpectralData() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    MaterialPropertyGlazingSpectralData(const MaterialPropertyGlazingSpectralData& other) = default;
    MaterialPropertyGlazingSpectralData(MaterialPropertyGlazingSpectralData&& other) = default;
    MaterialPropertyGlazingSpectralData& operator=(const MaterialPropertyGlazingSpectralData&) = default;
    MaterialPropertyGlazingSpectralData& operator=(MaterialPropertyGlazingSpectralData&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    std::vector<SpectralDataField> spectralDataFields() const;

    //@}
    /** @name Setters */
    //@{

    bool setSpectralDataFields(const std::vector<SpectralDataField>& spectralDataFields);

    bool addSpectralDataField(const SpectralDataField& spectralDataField);

    bool addSpectralDataField(double wavelength, double transmittance, double frontReflectance, double backReflectance);

    void resetSpectralDataFields();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::MaterialPropertyGlazingSpectralData_Impl;

    explicit MaterialPropertyGlazingSpectralData(std::shared_ptr<detail::MaterialPropertyGlazingSpectralData_Impl> impl);

    friend class detail::MaterialPropertyGlazingSpectralData_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.MaterialPropertyGlazingSpectralData");
  };

  /** \relates MaterialPropertyGlazingSpectralData*/
  using OptionalMaterialPropertyGlazingSpectralData = boost::optional<MaterialPropertyGlazingSpectralData>;

  /** \relates MaterialPropertyGlazingSpectralData*/
  using MaterialPropertyGlazingSpectralDataVector = std::vector<MaterialPropertyGlazingSpectralData>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_MATERIALPROPERTYGLAZINGSPECTRALDATA_HPP
