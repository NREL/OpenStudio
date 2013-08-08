/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#ifndef UTILITIES_DATA_ATTRIBUTE_IMPL_HPP
#define UTILITIES_DATA_ATTRIBUTE_IMPL_HPP

#include "../UtilitiesAPI.hpp"
#include <utilities/data/Attribute.hpp>

#include <utilities/core/Optional.hpp>
#include <utilities/core/String.hpp>
#include <utilities/core/Logger.hpp>
#include <utilities/core/UUID.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <QVariant>
#include <QMetaType>

class QDomElement;
class QDomDocument;

namespace openstudio {
namespace detail {

  class UTILITIES_API Attribute_Impl : public QObject, public boost::enable_shared_from_this<Attribute_Impl>
  {

    Q_OBJECT;

   public:

      /// constructors
      Attribute_Impl(const std::string& name, bool value, const boost::optional<std::string>& units);
      Attribute_Impl(const openstudio::UUID& uuid,
                     const openstudio::UUID& versionUUID,
                     const std::string& name,
                     const boost::optional<std::string>& displayName,
                     bool value,
                     const boost::optional<std::string>& units);

      Attribute_Impl(const std::string& name, double value, const boost::optional<std::string>& units);
      Attribute_Impl(const openstudio::UUID& uuid,
                     const openstudio::UUID& versionUUID,
                     const std::string& name,
                     const boost::optional<std::string>& displayName,
                     double value,
                     const boost::optional<std::string>& units);

      Attribute_Impl(const std::string& name, const OSOptionalQuantity& value);

      Attribute_Impl(const std::string& name, const Quantity& value);
      Attribute_Impl(const openstudio::UUID& uuid,
                     const openstudio::UUID& versionUUID,
                     const std::string& name,
                     const boost::optional<std::string>& displayName,
                     const Quantity& value);

      Attribute_Impl(const std::string& name, const Unit& value);
      Attribute_Impl(const openstudio::UUID& uuid,
                     const openstudio::UUID& versionUUID,
                     const std::string& name,
                     const boost::optional<std::string>& displayName,
                     const Unit& value);

      Attribute_Impl(const std::string& name, int value, const boost::optional<std::string>& units);
      Attribute_Impl(const openstudio::UUID& uuid,
                     const openstudio::UUID& versionUUID,
                     const std::string& name,
                     const boost::optional<std::string>& displayName,
                     int value,
                     const boost::optional<std::string>& units);

      Attribute_Impl(const std::string& name, unsigned value, const boost::optional<std::string>& units);
      Attribute_Impl(const openstudio::UUID& uuid,
                     const openstudio::UUID& versionUUID,
                     const std::string& name,
                     const boost::optional<std::string>& displayName,
                     unsigned value,
                     const boost::optional<std::string>& units);

      Attribute_Impl(const std::string& name, const char* value, const boost::optional<std::string>& units);
      Attribute_Impl(const openstudio::UUID& uuid,
                     const openstudio::UUID& versionUUID,
                     const std::string& name,
                     const boost::optional<std::string>& displayName,
                     const char* value,
                     const boost::optional<std::string>& units);

      Attribute_Impl(const std::string& name, const std::string& value, const boost::optional<std::string>& units);
      Attribute_Impl(const openstudio::UUID& uuid,
                     const openstudio::UUID& versionUUID,
                     const std::string& name,
                     const boost::optional<std::string>& displayName,
                     const std::string& value,
                     const boost::optional<std::string>& units);

      Attribute_Impl(const std::string& name, const std::vector<openstudio::Attribute>& value, const boost::optional<std::string>& units);
      Attribute_Impl(const openstudio::UUID& uuid,
                     const openstudio::UUID& versionUUID,
                     const std::string& name,
                     const boost::optional<std::string>& displayName,
                     const std::vector<openstudio::Attribute>& value,
                     const boost::optional<std::string>& units);

      Attribute_Impl(const QDomElement& element);
      Attribute_Impl(const Attribute_Impl& other);

      openstudio::UUID uuid() const;

      openstudio::UUID versionUUID() const;

      /// get the name
      std::string name() const;

      /// get the display name
      boost::optional<std::string> displayName() const;

      /// set the display name
      bool setDisplayName(const std::string& displayName);

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

      /// set value. throws if wront type.
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
      AttributeValueType m_valueType;
      QVariant m_value;
      boost::optional<std::string> m_units;
  };

} // detail
} // openstudio

#endif // UTILITIES_DATA_ATTRIBUTE_IMPL_HPP
