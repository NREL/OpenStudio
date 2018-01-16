/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef UTILITIES_DATA_ATTRIBUTE_IMPL_HPP
#define UTILITIES_DATA_ATTRIBUTE_IMPL_HPP

#include "../UtilitiesAPI.hpp"
#include "Attribute.hpp"

#include "../core/Optional.hpp"
#include "../core/String.hpp"
#include "../core/Logger.hpp"
#include "../core/UUID.hpp"

#include <QVariant>
#include <QMetaType>

class QDomElement;
class QDomDocument;

namespace openstudio {
namespace detail {

  class UTILITIES_API Attribute_Impl : public std::enable_shared_from_this<Attribute_Impl>
  {

   public:

      /// constructors
      Attribute_Impl(const std::string& name, bool value, const boost::optional<std::string>& units);
      Attribute_Impl(const openstudio::UUID& uuid,
                     const openstudio::UUID& versionUUID,
                     const std::string& name,
                     const boost::optional<std::string>& displayName,
                     bool value,
                     const boost::optional<std::string>& units,
                     const std::string& source = std::string());

      Attribute_Impl(const std::string& name, double value, const boost::optional<std::string>& units);
      Attribute_Impl(const openstudio::UUID& uuid,
                     const openstudio::UUID& versionUUID,
                     const std::string& name,
                     const boost::optional<std::string>& displayName,
                     double value,
                     const boost::optional<std::string>& units,
                     const std::string& source = std::string());

      Attribute_Impl(const std::string& name, const OSOptionalQuantity& value);

      Attribute_Impl(const std::string& name, const Quantity& value);
      Attribute_Impl(const openstudio::UUID& uuid,
                     const openstudio::UUID& versionUUID,
                     const std::string& name,
                     const boost::optional<std::string>& displayName,
                     const Quantity& value,
                     const std::string& source = std::string());

      Attribute_Impl(const std::string& name, const Unit& value);
      Attribute_Impl(const openstudio::UUID& uuid,
                     const openstudio::UUID& versionUUID,
                     const std::string& name,
                     const boost::optional<std::string>& displayName,
                     const Unit& value,
                     const std::string& source = std::string());

      Attribute_Impl(const std::string& name, int value, const boost::optional<std::string>& units);
      Attribute_Impl(const openstudio::UUID& uuid,
                     const openstudio::UUID& versionUUID,
                     const std::string& name,
                     const boost::optional<std::string>& displayName,
                     int value,
                     const boost::optional<std::string>& units,
                     const std::string& source = std::string());

      Attribute_Impl(const std::string& name, unsigned value, const boost::optional<std::string>& units);
      Attribute_Impl(const openstudio::UUID& uuid,
                     const openstudio::UUID& versionUUID,
                     const std::string& name,
                     const boost::optional<std::string>& displayName,
                     unsigned value,
                     const boost::optional<std::string>& units,
                     const std::string& source = std::string());

      Attribute_Impl(const std::string& name, const char* value, const boost::optional<std::string>& units);
      Attribute_Impl(const openstudio::UUID& uuid,
                     const openstudio::UUID& versionUUID,
                     const std::string& name,
                     const boost::optional<std::string>& displayName,
                     const char* value,
                     const boost::optional<std::string>& units,
                     const std::string& source = std::string());

      Attribute_Impl(const std::string& name, const std::string& value, const boost::optional<std::string>& units);
      Attribute_Impl(const openstudio::UUID& uuid,
                     const openstudio::UUID& versionUUID,
                     const std::string& name,
                     const boost::optional<std::string>& displayName,
                     const std::string& value,
                     const boost::optional<std::string>& units,
                     const std::string& source = std::string());

      Attribute_Impl(const std::string& name, const std::vector<openstudio::Attribute>& value, const boost::optional<std::string>& units);
      Attribute_Impl(const openstudio::UUID& uuid,
                     const openstudio::UUID& versionUUID,
                     const std::string& name,
                     const boost::optional<std::string>& displayName,
                     const std::vector<openstudio::Attribute>& value,
                     const boost::optional<std::string>& units,
                     const std::string& source = std::string());

      Attribute_Impl(const QDomElement& element);
      Attribute_Impl(const Attribute_Impl& other);

      // Destructor
      virtual ~Attribute_Impl() {}

      openstudio::UUID uuid() const;

      openstudio::UUID versionUUID() const;

      /// get the name
      std::string name() const;

      // ETH@20140414 - displayName should return a std::string (with returnName=true behavior),
      // and the return type of setDisplayName should be void.

      /// get the display name. if returnName and the display name is empty, will return
      /// name() instead.
      boost::optional<std::string> displayName(bool returnName=false) const;

      /// set the display name
      bool setDisplayName(const std::string& displayName);

      /// clear the display name
      void clearDisplayName();

      /// get the (optional) data source
      std::string source() const;

      /// set the data source
      void setSource(const std::string& source);

      /// clear the data source
      void clearSource();

      /// get the attribute value type
      AttributeValueType valueType() const;

      /// get value as a bool
      bool valueAsBoolean() const;

      /// set value. throws if wrong type.
      void setValue(bool value);

      /// get value as int
      int valueAsInteger() const;

      /// set value. throws if wrong type.
      void setValue(int value);

      /// get value as unsigned
      unsigned valueAsUnsigned() const;

      /// set value. throws if wrong type.
      void setValue(unsigned value);

      /// get value as double
      double valueAsDouble() const;

      /// set value. throws if wrong type.
      void setValue(double value);

      /// get value as Quantity
      Quantity valueAsQuantity() const;

      /// set value. throws if wrong type.
      void setValue(const Quantity& value);

      /// get value as Unit
      Unit valueAsUnit() const;

      /// set value. throws if wrong type.
      void setValue(const Unit& value);

      /// get value as string
      std::string valueAsString() const;

      /// set value. throws if wrong type.
      void setValue(const char* value);

      /// set value. throws if wrong type.
      void setValue(const std::string& value);

      /// get value as attribute vector
      std::vector<Attribute> valueAsAttributeVector() const;

      /// set value. throws if wrong type.
      void setValue(const std::vector<Attribute>& value);

      /// get value as qvariant
      QVariant valueAsQVariant() const;

      /// find child attribute by name
      boost::optional<Attribute> findChildByName(const std::string& name) const;

      /// get the units
      boost::optional<std::string> units() const;

      /// set the units
      bool setUnits(const std::string& units);

      /** If attribute is a simple type (non-vector), prints attribute's value to std::string as
      *  simply as possible. Numeric types print in high precision. Attribute vectors are printed
      *  as XML. */
      std::string toString() const;

      /// write object and all children to xml
      QDomDocument toXml() const;

      /// comparison
      bool operator==(const Attribute& other) const;

    protected:

      friend class openstudio::Attribute;

      // for setting after construction
      void setValue(const QVariant& value, bool check);

      /// write values to an xml element
      /// override in derived classes
      virtual void writeValues(QDomDocument& doc, QDomElement& element) const;

    private:

      REGISTER_LOGGER("openstudio.Attribute");

      openstudio::UUID m_uuid;
      openstudio::UUID m_versionUUID;
      std::string m_name;
      boost::optional<std::string> m_displayName;
      std::string m_source;
      AttributeValueType m_valueType;
      QVariant m_value;
      boost::optional<std::string> m_units;
  };

} // detail
} // openstudio

#endif // UTILITIES_DATA_ATTRIBUTE_IMPL_HPP
