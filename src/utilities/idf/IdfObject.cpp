/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "IdfObject.hpp"
#include "IdfObject_Impl.hpp"

#include "IdfExtensibleGroup.hpp"
#include "IdfRegex.hpp"
#include "ValidityReport.hpp"

#include "../idd/IddKey.hpp"
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../idd/IddRegex.hpp"
#include "../idd/CommentRegex.hpp"
#include "../idd/Comments.hpp"

#include "../math/FloatCompare.hpp"
#include "../core/Finder.hpp"
#include "../core/Assert.hpp"

#include "../units/Quantity.hpp"
#include "../units/OSOptionalQuantity.hpp"
#include "../units/QuantityConverter.hpp"

#include <boost/lexical_cast.hpp>

#include <iomanip>

namespace openstudio {

namespace detail {

  // CONSTRUCTORS

  IdfObject_Impl::IdfObject_Impl(const IdfObject_Impl& other, bool keepHandle)
    : m_comment(other.comment()), m_iddObject(other.iddObject()), m_fields(other.fields()), m_fieldComments(other.fieldComments()) {
    if (keepHandle) {
      OS_ASSERT(!other.handle().isNull());
      m_handle = other.handle();
    } else {
      m_handle = openstudio::createUUID();
      if (m_iddObject.hasHandleField()) {
        bool ok = setString(0, toString(m_handle));
        OS_ASSERT(ok);
      }
    }
  }

  IdfObject_Impl::IdfObject_Impl(IddObjectType type, bool fastName) : m_handle(openstudio::createUUID()) {
    OptionalIddObject candidate = IddFactory::instance().getObject(type);
    OS_ASSERT(candidate);
    m_iddObject = *candidate;
    if (m_iddObject.hasHandleField()) {
      bool ok = setString(0, toString(m_handle));
      OS_ASSERT(ok);
    }
    if (fastName) {
      boost::optional<unsigned> nameFieldIndex = this->m_iddObject.nameFieldIndex();
      if (nameFieldIndex) {
        bool ok = setString(*nameFieldIndex, toString(createUUID()));
        OS_ASSERT(ok);
      } else {
        LOG(Error, "Requested setting name field to UUID but object of type '" << this->m_iddObject.name() << "' has no name field");
      }
    }
    resizeToMinFields();
  }

  IdfObject_Impl::IdfObject_Impl(const IddObject& iddObject, bool fastName) : m_handle(openstudio::createUUID()), m_iddObject(iddObject) {
    if (this->m_iddObject.hasHandleField()) {
      bool ok = setString(0, toString(m_handle));
      OS_ASSERT(ok);
    }
    if (fastName) {
      boost::optional<unsigned> nameFieldIndex = this->m_iddObject.nameFieldIndex();
      if (nameFieldIndex) {
        bool ok = setString(*nameFieldIndex, toString(createUUID()));
        OS_ASSERT(ok);
      } else {
        LOG(Error, "Requested setting name field to UUID but object of type '" << this->m_iddObject.name() << "' has no name field");
      }
    }
    resizeToMinFields();
  }

  IdfObject_Impl::IdfObject_Impl(const IddObject& iddObject, bool fastName, bool minimal) : m_iddObject(iddObject) {
    OS_ASSERT(!fastName);
    OS_ASSERT(minimal);
  }

  IdfObject_Impl::IdfObject_Impl(const Handle& handle, const std::string& comment, const IddObject& iddObject, const StringVector& fields,
                                 const StringVector& fieldComments)
    : m_handle(handle), m_comment(comment), m_iddObject(iddObject), m_fields(fields), m_fieldComments(fieldComments) {
    resizeToMinFields();
  }

  // GETTERS

  Handle IdfObject_Impl::handle() const {
    return m_handle;
  }

  IddObject IdfObject_Impl::iddObject() const {
    return m_iddObject;
  }

  std::string IdfObject_Impl::comment() const {
    return m_comment;
  }

  boost::optional<std::string> IdfObject_Impl::fieldComment(unsigned index, bool returnDefault) const {
    if (index >= numFields()) {
      return boost::none;
    }

    std::string result;
    if (index < m_fieldComments.size()) {
      result = m_fieldComments[index];
    }

    if (returnDefault && result.empty()) {
      if (OptionalIddField iddField = m_iddObject.getField(index)) {
        std::stringstream ss;
        ss << makeIdfEditorComment(iddField->name());
        if (m_iddObject.isExtensibleField(index)) {
          ExtensibleIndex ei = m_iddObject.extensibleIndex(index);
          ss << " " << ei.group + 1;
        }
        if (OptionalString units = iddField->properties().units) {
          if (units) {
            ss << " {" << *units << "}";
          }
        }
        result = ss.str();
      }
    }
    return result;
  }

  boost::optional<std::string> IdfObject_Impl::name(bool returnDefault) const {
    if (OptionalUnsigned oi = m_iddObject.nameFieldIndex()) {
      unsigned index = *oi;
      bool validIndex = m_fields.size() > index;
      if (returnDefault && validIndex && m_fields[index].empty()) {
        if (OptionalString stringDefault = m_iddObject.nonextensibleFields()[index].properties().stringDefault) {
          if (stringDefault) {
            return decodeString(*stringDefault);
          } else {
            return boost::none;
          }
        }
        return decodeString(m_fields[index]);
      } else if (validIndex) {
        return decodeString(m_fields[index]);
      }
    }
    return boost::none;
  }

  std::string IdfObject_Impl::nameString(bool returnDefault) const {
    boost::optional<std::string> temp = name(returnDefault);
    if (temp) {
      return temp.get();
    }
    return "";
  }

  std::string IdfObject_Impl::briefDescription() const {
    std::stringstream ss;
    ss << "Object of type '" << m_iddObject.type().valueDescription() << "'";
    OptionalString oName = name(true);
    if (oName && (!oName->empty())) {
      ss << " and named '" << *oName << "'";
    }
    return ss.str();
  }

  bool IdfObject_Impl::isEmpty(unsigned index) const {
    bool result = true;
    if (index < m_fields.size()) {
      OptionalString string = getString(index, false, true);
      if (string) {
        result = false;
      }
    }
    return result;
  }

  boost::optional<std::string> IdfObject_Impl::getString(unsigned index, bool returnDefault, bool returnUninitializedEmpty) const {
    OptionalString result;
    if (index < m_fields.size()) {
      result = m_fields[index];
    }
    if (returnDefault && ((result && result->empty()) || (!result))) {
      OptionalIddField iddField = m_iddObject.getField(index);
      if (iddField && iddField->properties().stringDefault) {
        result = *(iddField->properties().stringDefault);
      }
    }
    if (returnUninitializedEmpty && (result && result->empty())) {
      result.reset();
    }
    if (result) {
      return decodeString(*result);
    }
    return boost::none;
  }

  boost::optional<std::string> IdfObject_Impl::getField(unsigned index, bool returnDefault) const {
    return IdfObject_Impl::getString(index, returnDefault, false);
  }

  boost::optional<double> IdfObject_Impl::getDouble(unsigned index, bool returnDefault) const {
    OptionalDouble result;
    OptionalString value = getString(index, returnDefault, false);
    if (value) {
      if (!(istringEqual(*value, "") || istringEqual(*value, "autosize") || istringEqual(*value, "autocalculate"))) {
        try {
          result = boost::lexical_cast<double>(*value);
        } catch (const std::exception&) {
          LOG(Error, "Could not convert '" << *value << "' to double");
        }
      }
    }
    return result;
  }

  OSOptionalQuantity IdfObject_Impl::getQuantity(unsigned index, bool returnDefault, bool returnIP) const {
    OptionalDouble value = getDouble(index, returnDefault);
    return getQuantityFromDouble(index, value, returnIP);
  }

  boost::optional<unsigned> IdfObject_Impl::getUnsigned(unsigned index, bool returnDefault) const {
    OptionalUnsigned result;
    OptionalString value = getString(index, returnDefault, false);
    if (value) {
      if (!(istringEqual(*value, "") || istringEqual(*value, "autosize") || istringEqual(*value, "autocalculate"))) {
        try {
          auto temp = boost::lexical_cast<double>(*value);
          result = boost::numeric_cast<unsigned>(temp);
        } catch (const std::exception&) {
          LOG(Error, "Could not convert '" << *value << "' to unsigned");
        }
      }
    }
    return result;
  }

  boost::optional<int> IdfObject_Impl::getInt(unsigned index, bool returnDefault) const {
    OptionalInt result;
    OptionalString value = getString(index, returnDefault, false);
    if (value) {
      if (!(istringEqual(*value, "") || istringEqual(*value, "autosize") || istringEqual(*value, "autocalculate"))) {
        try {
          auto temp = boost::lexical_cast<double>(*value);
          result = boost::numeric_cast<int>(temp);
        } catch (const std::exception&) {
          LOG(Error, "Could not convert '" << *value << "' to int");
        }
      }
    }
    return result;
  }

  IdfExtensibleGroup IdfObject_Impl::getExtensibleGroup(unsigned groupIndex) const {
    unsigned n = numFields();
    unsigned i = n;
    std::shared_ptr<detail::IdfObject_Impl> p;
    IdfExtensibleGroup eg(p, i);
    if (m_iddObject.properties().extensible) {
      if (m_iddObject.index(ExtensibleIndex(groupIndex, m_iddObject.properties().numExtensible - 1)) < n) {
        i = m_iddObject.index(ExtensibleIndex(groupIndex, 0));
        eg = IdfExtensibleGroup(std::const_pointer_cast<IdfObject_Impl>(shared_from_this()), i);
      }
    }
    return eg;
  }

  std::vector<IdfExtensibleGroup> IdfObject_Impl::extensibleGroups() const {
    IdfExtensibleGroupVector result;
    for (unsigned i = 0, n = numExtensibleGroups(); i < n; ++i) {
      result.push_back(getExtensibleGroup(i));
    }
    return result;
  }

  // SETTERS

  void IdfObject_Impl::setComment(const std::string& comment) {
    setComment(comment, true);
    this->emitChangeSignals();
  }

  void IdfObject_Impl::setComment(const std::string& comment, bool /*checkValidity*/) {
    m_comment = makeComment(comment);
    m_diffs.push_back(IdfObjectDiff(boost::none, boost::none, boost::none));
  }

  bool IdfObject_Impl::setFieldComment(unsigned index, const std::string& cmnt) {
    bool result = setFieldComment(index, cmnt, true);
    if (result) {
      this->emitChangeSignals();
    }
    return result;
  }

  bool IdfObject_Impl::setFieldComment(unsigned index, const std::string& cmnt, bool /*checkValidity*/) {
    if (index < m_fields.size()) {
      if (index >= m_fieldComments.size()) {
        m_fieldComments.resize(index + 1);
      }

      m_fieldComments[index] = makeComment(cmnt);

      m_diffs.push_back(IdfObjectDiff(index, m_fields[index], m_fields[index]));

      return true;
    }
    return false;
  }

  boost::optional<std::string> IdfObject_Impl::setName(const std::string& newName) {
    OptionalString result = IdfObject_Impl::setName(newName, true);
    if (result) {
      this->emitChangeSignals();
    }
    return result;
  }

  boost::optional<std::string> IdfObject_Impl::setName(const std::string& _newName, bool /*checkValidity*/) {
    std::string newName = encodeString(_newName);

    switch (m_iddObject.type().value()) {
      // get all EMS idd object types in both E+ and OS IDD
      case openstudio::IddObjectType::EnergyManagementSystem_Actuator:;                      // deliberate fall through
      case openstudio::IddObjectType::OS_EnergyManagementSystem_Actuator:;                   // deliberate fall through
      case openstudio::IddObjectType::EnergyManagementSystem_ConstructionIndexVariable:;     // TODO uncomment when wrapped
      case openstudio::IddObjectType::OS_EnergyManagementSystem_ConstructionIndexVariable:;  // deliberate fall through
      case openstudio::IddObjectType::EnergyManagementSystem_CurveOrTableIndexVariable:;     // TODO uncomment when wrapped
      case openstudio::IddObjectType::OS_EnergyManagementSystem_CurveOrTableIndexVariable:;  // deliberate fall through
      case openstudio::IddObjectType::EnergyManagementSystem_GlobalVariable:;                // deliberate fall through
      case openstudio::IddObjectType::OS_EnergyManagementSystem_GlobalVariable:;             // deliberate fall through
      case openstudio::IddObjectType::EnergyManagementSystem_InternalVariable:;              // deliberate fall through
      case openstudio::IddObjectType::OS_EnergyManagementSystem_InternalVariable:;           // deliberate fall through
      case openstudio::IddObjectType::EnergyManagementSystem_Program:;                       // deliberate fall through
      case openstudio::IddObjectType::OS_EnergyManagementSystem_Program:;                    // deliberate fall through
      case openstudio::IddObjectType::EnergyManagementSystem_Sensor:;                        // deliberate fall through
      case openstudio::IddObjectType::OS_EnergyManagementSystem_Sensor:;                     // deliberate fall through
      case openstudio::IddObjectType::EnergyManagementSystem_Subroutine:;                    // deliberate fall through
      case openstudio::IddObjectType::OS_EnergyManagementSystem_Subroutine:;                 // deliberate fall through
      case openstudio::IddObjectType::EnergyManagementSystem_TrendVariable:;                 // deliberate fall through
      case openstudio::IddObjectType::OS_EnergyManagementSystem_TrendVariable:;              // deliberate fall through
        // replace � � with �_�
        std::replace(newName.begin(), newName.end(), ' ', '_');
        break;
      //case openstudio::IddObjectType::EnergyManagementSystem_OutputVariable:; // deliberate fall through
      //case openstudio::IddObjectType::OS_EnergyManagementSystem_OutputVariable:; // deliberate fall through
      //TODO Enforce Title-Case
      default:
        // no-op
        break;
    }

    // check Idd to see if this object has a name
    if (OptionalUnsigned index = m_iddObject.nameFieldIndex()) {
      // if so, change the name, or create it
      unsigned n = numFields();
      unsigned i = *index;
      OS_ASSERT(i < 2u);
      if (n == 0 && i == 1) {
        OS_ASSERT(!m_handle.isNull());
        m_fields.push_back(toString(m_handle));
        m_diffs.push_back(IdfObjectDiff(0u, boost::none, m_fields.back()));
      }
      n = numFields();
      if (i < n) {
        std::string oldName = m_fields[i];
        m_fields[i] = newName;
        m_diffs.push_back(IdfObjectDiff(i, oldName, newName));
      } else {
        m_fields.push_back(newName);
        m_diffs.push_back(IdfObjectDiff(i, boost::none, newName));
      }
      //return decoded string since we might have made changes to it if its an EMS object.
      newName = decodeString(newName);
      return newName;  // success!
    }
    return boost::none;  // no name
  }

  boost::optional<std::string> IdfObject_Impl::createName() {
    return IdfObject_Impl::createName(true);
  }

  boost::optional<std::string> IdfObject_Impl::createName(bool overwrite) {
    if (OptionalUnsigned index = m_iddObject.nameFieldIndex()) {
      OptionalString oName = name();
      if (!oName || oName->empty() || overwrite) {
        std::string newName = createUniqueName(m_iddObject.name());
        return setName(newName);
      }
    }
    return boost::none;
  }

  bool IdfObject_Impl::setString(unsigned index, const std::string& value) {
    bool result = IdfObject_Impl::setString(index, value, true);
    if (result) {
      this->emitChangeSignals();
    }
    return result;
  }

  bool IdfObject_Impl::setString(unsigned index, const std::string& _value, bool checkValidity) {
    std::string value = encodeString(_value);

    if (m_iddObject.hasNameField() && (index == m_iddObject.nameFieldIndex().get())) {
      return IdfObject_Impl::setName(value, checkValidity).has_value();
    }

    // push fields and groups if necessary and possible
    if (m_iddObject.isNonextensibleField(index) || m_iddObject.isExtensibleField(index)) {
      bool result = true;
      boost::optional<std::string> oldValue;
      unsigned n = m_fields.size();
      unsigned nn = n;
      unsigned diffSize = m_diffs.size();
      unsigned iddn = m_iddObject.numFields();

      if (index >= m_fields.size()) {
        while (result && (index >= nn) && (nn < iddn)) {
          // push regular fields
          result = result && this->pushString(checkValidity);
          nn = m_fields.size();
        }
        while (result && (index >= nn)) {
          // push extensible fields
          result = result && !this->pushExtensibleGroup(StringVector(), checkValidity).empty();
          nn = m_fields.size();
        }
      } else {
        oldValue = m_fields[index];
      }

      if (!result) {
        // remove diffs
        m_diffs.resize(diffSize);

        // resize fields
        m_fields.resize(n);
        if (m_fieldComments.size() > n) {
          m_fieldComments.resize(n);
        }

        return false;
      }

      OS_ASSERT(index < m_fields.size());

      m_fields[index] = value;
      m_diffs.emplace_back(index, oldValue, value);
      return result;
    }
    return false;
  }

  bool IdfObject_Impl::setDouble(unsigned index, double value) {
    bool result = setDouble(index, value, true);
    if (result) {
      this->emitChangeSignals();
    }
    return result;
  }

  bool IdfObject_Impl::setDouble(unsigned index, double value, bool checkValidity) {
    try {
      return setString(index, toString(value), checkValidity);
    } catch (...) {
      return false;
    }
  }

  bool IdfObject_Impl::setQuantity(unsigned index, const Quantity& q) {
    bool result = setQuantity(index, q, true);
    if (result) {
      this->emitChangeSignals();
    }
    return result;
  }

  bool IdfObject_Impl::setQuantity(unsigned index, const Quantity& q, bool checkValidity) {
    OptionalDouble value = getDoubleFromQuantity(index, q);
    if (!value) {
      return false;
    }
    return setDouble(index, *value, checkValidity);
  }

  bool IdfObject_Impl::setUnsigned(unsigned index, unsigned value) {
    bool result = setUnsigned(index, value, true);
    if (result) {
      this->emitChangeSignals();
    }
    return result;
  }

  bool IdfObject_Impl::setUnsigned(unsigned index, unsigned value, bool checkValidity) {
    try {
      auto str = boost::lexical_cast<std::string>(value);
      return setString(index, str, checkValidity);
    } catch (...) {
      return false;
    }
  }

  bool IdfObject_Impl::setInt(unsigned index, int value) {
    bool result = setInt(index, value, true);
    if (result) {
      this->emitChangeSignals();
    }
    return result;
  }

  bool IdfObject_Impl::setInt(unsigned index, int value, bool checkValidity) {
    try {
      auto str = boost::lexical_cast<std::string>(value);
      return setString(index, str, checkValidity);
    } catch (...) {
      return false;
    }
  }

  bool IdfObject_Impl::pushString() {
    bool result = IdfObject_Impl::pushString(std::string(), true);
    if (result) {
      this->emitChangeSignals();
    }
    return result;
  }

  bool IdfObject_Impl::pushString(bool checkValidity) {
    return IdfObject_Impl::pushString(std::string(), checkValidity);
  }

  bool IdfObject_Impl::pushString(const std::string& value) {
    bool result = IdfObject_Impl::pushString(value, true);
    if (result) {
      this->emitChangeSignals();
    }
    return result;
  }

  bool IdfObject_Impl::pushString(const std::string& value, bool checkValidity) {
    // get new index
    unsigned index = m_fields.size();
    if (m_iddObject.hasNameField() && (index == m_iddObject.nameFieldIndex().get())) {
      return IdfObject_Impl::setName(value, checkValidity).has_value();
    }

    // ok if nonextensible, or extensible w/ group size 1
    if (m_iddObject.isNonextensibleField(index) || (m_iddObject.isExtensibleField(index) && (m_iddObject.properties().numExtensible == 1))) {
      m_fields.push_back(value);
      m_diffs.push_back(IdfObjectDiff(index, boost::none, value));
      return true;
    }
    return false;
  }

  IdfExtensibleGroup IdfObject_Impl::pushExtensibleGroup(const std::vector<std::string>& values) {
    IdfExtensibleGroup result = IdfObject_Impl::pushExtensibleGroup(values, true);
    if (!result.empty()) {
      this->emitChangeSignals();
    }
    return result;
  }

  IdfExtensibleGroup IdfObject_Impl::pushExtensibleGroup(const std::vector<std::string>& values, bool checkValidity) {
    unsigned groupSize = m_iddObject.properties().numExtensible;
    unsigned n = numFields();
    IdfObject_ImplPtr p = nullptr;
    IdfExtensibleGroup result(p, n);

    if (!values.empty() && (values.size() != groupSize)) {
      return result;
    }

    StringVector wValues = values;  // copy so can resize empty vector
    OptionalUnsigned mf = maxFields();
    unsigned diffSize = m_diffs.size();

    // push fields as needed
    unsigned iddn = m_iddObject.numFields();
    if (n < iddn) {
      bool ok = this->setString(iddn - 1, "", checkValidity);
      if (!ok) {
        // remove the diffs
        m_diffs.resize(diffSize);

        // resize the fields
        m_fields.resize(n);
        if (m_fieldComments.size() > n) {
          m_fieldComments.resize(n);
        }
        return result;
      }
    }

    n = numFields();
    OS_ASSERT(n >= iddn);

    // needs to be extensible, have all non-extensible fields defined, and not violate maxFields.
    if ((groupSize > 0) && (!mf || (n + groupSize <= *mf))) {

      OS_ASSERT(m_iddObject.properties().extensible);

      if (wValues.empty()) {
        wValues.resize(groupSize);
      }

      m_fields.resize(n + groupSize);

      for (unsigned i = 0; i < groupSize; ++i) {

        bool ok = setString(n + i, wValues[i], checkValidity);
        if (!ok) {
          // remove the diffs
          m_diffs.resize(diffSize);

          // resize the fields
          m_fields.resize(n);
          if (m_fieldComments.size() > n) {
            m_fieldComments.resize(n);
          }
          return result;
        }
      }

      result = getExtensibleGroup(numExtensibleGroups() - 1);
      OS_ASSERT(!result.empty());
      OS_ASSERT(result.groupIndex() == m_iddObject.extensibleIndex(n).group);
    }

    return result;
  }

  IdfExtensibleGroup IdfObject_Impl::insertExtensibleGroup(unsigned groupIndex, const std::vector<std::string>& values) {
    IdfExtensibleGroup result = insertExtensibleGroup(groupIndex, values, true);
    if (!result.empty()) {
      this->emitChangeSignals();
    }
    return result;
  }

  IdfExtensibleGroup IdfObject_Impl::insertExtensibleGroup(unsigned groupIndex, const std::vector<std::string>& values, bool checkValidity) {
    // if really a push request, send it there
    if (groupIndex == numExtensibleGroups()) {
      return pushExtensibleGroup(values, checkValidity);
    }

    // now have groupIndex != numExtensibleGroups()
    unsigned groupSize = m_iddObject.properties().numExtensible;
    unsigned n = numFields();
    IdfObject_ImplPtr p;
    IdfExtensibleGroup result(p, n);
    // nothing to do if not extensible, groupIndex invalid, or values invalid
    if ((groupSize == 0) || (groupIndex > numExtensibleGroups())) {
      return result;
    }
    if (!values.empty() && (values.size() != groupSize)) {
      return result;
    }

    // record diffs at start
    unsigned diffSize = m_diffs.size();

    // from now on, groupIndex < numExtensibleGroups(), and numExtensibleGroups() > 0
    OptionalUnsigned mf = maxFields();
    // needs to have all non-extensible fields defined, and not violate maxFields.
    if ((n >= m_iddObject.numFields()) && (!mf || (n + groupSize <= *mf))) {
      OS_ASSERT(m_iddObject.properties().extensible);

      StringVector wValues = values;
      if (wValues.empty()) {
        wValues.resize(groupSize);
      }

      // push a new group on the end
      unsigned i = numExtensibleGroups() - 1;
      IdfExtensibleGroup eg = getExtensibleGroup(i);
      OS_ASSERT(!eg.empty());
      IdfExtensibleGroup temp = pushExtensibleGroup(eg.fieldsWithHandles(), checkValidity);
      if (temp.empty()) {
        OS_ASSERT(numFields() == n);
        OS_ASSERT(m_diffs.size() == diffSize);

        return result;
      }

      // copy values from group i to i+1 down to and including groupIndex
      bool ok = true;
      while (i > groupIndex) {
        --i;
        IdfExtensibleGroup peg = getExtensibleGroup(i);
        OS_ASSERT(!peg.empty());
        ok = eg.setFields(peg.fieldsWithHandles(), false);
        if (!ok) {
          // roll back
          i += 2;
          while (i < numExtensibleGroups() - 1) {
            peg = getExtensibleGroup(i);
            OS_ASSERT(!peg.empty());
            eg = getExtensibleGroup(i + 1);
            OS_ASSERT(!eg.empty());
            peg.setFields(eg.fieldsWithHandles(), false);
            ++i;
          }
          popExtensibleGroup(false);

          // remove the diffs
          m_diffs.resize(diffSize);

          return result;
        }
        eg = peg;
      }

      // now set the values at this group index
      ok = eg.setFields(wValues, checkValidity);

      if (!ok) {
        i = groupIndex + 1;
        IdfExtensibleGroup peg = getExtensibleGroup(i);
        OS_ASSERT(!peg.empty());
        while (i < numExtensibleGroups() - 1) {
          ++i;
          eg = getExtensibleGroup(i);
          OS_ASSERT(!eg.empty());
          peg.setFields(eg.fieldsWithHandles(), false);
          peg = eg;
        }
        popExtensibleGroup(false);

        // remove the diffs
        m_diffs.resize(diffSize);

        return result;
      }

      result = eg;
      OS_ASSERT(!result.empty());
      OS_ASSERT(numFields() == n + groupSize);
    }

    return result;
  }

  std::vector<std::string> IdfObject_Impl::popExtensibleGroup() {
    StringVector result = IdfObject_Impl::popExtensibleGroup(true);
    if (!result.empty()) {
      this->emitChangeSignals();
    }
    return result;
  }

  /** Pops the final extensible group from the object, if possible. Returns the popped data if
   *  successful. Otherwise, the returned vector will be empty. */
  std::vector<std::string> IdfObject_Impl::popExtensibleGroup(bool /*checkValidity*/) {

    unsigned groupSize = m_iddObject.properties().numExtensible;
    unsigned numBeforePop = numFields();
    unsigned numAfterPop = numBeforePop - groupSize;
    StringVector result;
    // must be extensible object, with some extensible fields
    if ((groupSize > 0) && (numAfterPop >= m_iddObject.numFields())) {
      IdfExtensibleGroup egToPop = getExtensibleGroup(numExtensibleGroups() - 1);
      OS_ASSERT(!egToPop.empty());
      UnsignedVector indices = egToPop.mf_indices();
      result = egToPop.fieldsWithHandles();
      OS_ASSERT(result.size() == groupSize);

      // record diffs for each field going backwards
      for (unsigned i = 0; i < groupSize; ++i) {
        m_diffs.push_back(IdfObjectDiff(numBeforePop - 1 - i, result[i], boost::none));
      }

      m_fields.resize(numAfterPop);
      if (m_fieldComments.size() > m_fields.size()) {
        m_fieldComments.resize(numAfterPop);
      }
      OS_ASSERT(egToPop.empty());
    }

    return result;
  }

  /** Erases the extensible group at groupIndex, if possible. Returns the erased data, and
   *  preserves the relative order of the remaining data if successful. Otherwise, the returned
   *  vector will be empty. */
  std::vector<std::string> IdfObject_Impl::eraseExtensibleGroup(unsigned groupIndex) {
    StringVector result = eraseExtensibleGroup(groupIndex, true);
    if (!result.empty()) {
      this->emitChangeSignals();
    }
    return result;
  }

  std::vector<std::string> IdfObject_Impl::eraseExtensibleGroup(unsigned groupIndex, bool checkValidity) {
    StringVector result;
    if (groupIndex >= numExtensibleGroups()) {
      return result;
    }
    UnsignedVector indices = getExtensibleGroup(groupIndex).mf_indices();

    // start at bottom
    result = popExtensibleGroup(false);
    if (result.empty()) {
      return result;
    }

    // record diffs at start
    unsigned diffSize = m_diffs.size();

    bool ok = true;
    // pop was successful. roll up until overwrite groupIndex
    for (int i = numExtensibleGroups() - 1; i >= static_cast<int>(groupIndex); --i) {
      StringVector temp = result;
      IdfExtensibleGroup eg = getExtensibleGroup(i);
      OS_ASSERT(!eg.empty());
      result = eg.fieldsWithHandles();
      ok = eg.setFields(temp, checkValidity);
      if (!ok) {
        // roll back changes and return
        for (unsigned j = i + 1, n = numExtensibleGroups(); j < n; ++j) {
          eg = getExtensibleGroup(j);
          OS_ASSERT(!eg.empty());
          result = eg.fieldsWithHandles();
          OS_ASSERT(result.size() == temp.size());
          eg.setFields(temp, false);
          temp = result;
        }
        eg = pushExtensibleGroup(temp, false);
        OS_ASSERT(!eg.empty());

        // remove the diffs
        m_diffs.resize(diffSize);

        return {};
      }
    }

    OS_ASSERT(ok);
    return result;
  }

  /** Pops all \link IdfExtensibleGroup IdfExtensibleGroups\endlink from the object, if possible.
   *  Returns popped data if successful. Otherwise, the returned vector will be empty. */
  std::vector<std::vector<std::string>> IdfObject_Impl::clearExtensibleGroups() {
    std::vector<StringVector> result = clearExtensibleGroups(true);
    if (!result.empty()) {
      this->emitChangeSignals();
    }
    return result;
  }

  std::vector<std::vector<std::string>> IdfObject_Impl::clearExtensibleGroups(bool checkValidity) {

    // data to restore if cannot delete all groups
    std::vector<StringVector> rollbackValues;
    std::vector<StringVector> rollbackComments;

    unsigned gn = numExtensibleGroups();
    if (gn == 0) {
      return rollbackValues;
    }

    // record diffs at start
    unsigned diffSize = m_diffs.size();

    // loop through groups
    UnsignedVector indices;
    while (gn > 0) {
      // get this group's data
      IdfExtensibleGroup eg = getExtensibleGroup(gn - 1);
      OS_ASSERT(!eg.empty());
      if (indices.empty()) {
        indices = eg.mf_indices();
      }
      rollbackValues.push_back(eg.fieldsWithHandles());
      rollbackComments.push_back(eg.fieldComments());
      // try to pop
      StringVector result = popExtensibleGroup(checkValidity);
      if (result.empty()) {
        // unsuccessful--restore cleared data
        rollbackValues.pop_back();
        rollbackComments.pop_back();
        OS_ASSERT(gn == numExtensibleGroups());
        // add back groups already popped
        while (!rollbackValues.empty()) {
          IdfExtensibleGroup pushed = pushExtensibleGroup(rollbackValues.back(), false);
          unsigned fieldIndex = 0;
          for (const std::string& cmnt : rollbackComments.back()) {
            pushed.setFieldComment(fieldIndex, cmnt);
            ++fieldIndex;
          }
          rollbackValues.pop_back();
          rollbackComments.pop_back();
        }

        // remove the diffs
        m_diffs.resize(diffSize);

        return rollbackValues;
      }
      --gn;
      OS_ASSERT(gn == numExtensibleGroups());
    }

    OS_ASSERT(!rollbackValues.empty());
    OS_ASSERT(!indices.empty());

    return rollbackValues;
  }

  // QUERIES

  unsigned IdfObject_Impl::numFields() const {
    return m_fields.size();
  }

  unsigned IdfObject_Impl::numNonextensibleFields() const {
    unsigned n = numFields();
    unsigned iddN = m_iddObject.numFields();
    if (iddN < n) {
      return iddN;
    }
    return n;
  }

  unsigned IdfObject_Impl::minFields() const {
    return m_iddObject.properties().minFields;
  }

  OptionalUnsigned IdfObject_Impl::maxFields() const {
    OptionalUnsigned result;
    if (!m_iddObject.properties().extensible) {
      result = m_iddObject.nonextensibleFields().size();
    } else {
      result = m_iddObject.properties().maxFields;
    }
    return result;
  }

  unsigned IdfObject_Impl::numExtensibleGroups() const {
    unsigned nExtFields = numFields() - numNonextensibleFields();
    if (nExtFields == 0) {
      return 0;
    }
    OS_ASSERT(m_iddObject.properties().extensible);
    unsigned groupSize = m_iddObject.properties().numExtensible;
    OS_ASSERT(nExtFields % groupSize == 0);
    return nExtFields / groupSize;
  }

  unsigned IdfObject_Impl::minExtensibleGroups() const {
    return m_iddObject.properties().numExtensibleGroupsRequired;
  }

  boost::optional<unsigned> IdfObject_Impl::maxExtensibleGroups() const {
    OptionalUnsigned result = static_cast<unsigned>(0);
    if (!m_iddObject.properties().extensible) {
      return result;
    }
    OptionalUnsigned mf = maxFields();
    if (!mf) {
      return boost::none;
    }
    unsigned groupSize = m_iddObject.properties().numExtensible;
    unsigned maxExtFields = *mf - m_iddObject.numFields();
    OS_ASSERT(maxExtFields % groupSize == 0);
    OS_ASSERT(groupSize > 0);
    result = maxExtFields / groupSize;
    return result;
  }

  bool IdfObject_Impl::isObjectListField(unsigned index) const {
    if (index >= numFields()) {
      return false;
    }
    OptionalIddField oIddField = m_iddObject.getField(index);
    if (oIddField) {
      return oIddField->isObjectListField();
    }
    return false;
  }

  std::vector<unsigned> IdfObject_Impl::objectListFields() const {
    UnsignedVector result = m_iddObject.objectListFields();
    result = trimFieldIndices(result);
    result = repeatExtensibleIndices(result);
    return result;
  }

  bool IdfObject_Impl::isDataField(unsigned index) const {
    if (index >= numFields()) {
      return false;
    }
    return (!isObjectListField(index) && (index > 0 || !m_iddObject.hasHandleField()));
  }

  std::vector<unsigned> IdfObject_Impl::dataFields() const {
    UnsignedVector result;
    for (unsigned i = 0, n = numFields(); i < n; ++i) {
      if (isDataField(i)) {
        result.push_back(i);
      }
    }
    return result;
  }

  UnsignedVector IdfObject_Impl::requiredFields() const {
    UnsignedVector result;
    for (unsigned index = 0; index < m_fields.size(); ++index) {
      OptionalIddField field = m_iddObject.getField(index);
      if (field && field->properties().required) {
        result.push_back(index);
      }
    }
    return result;
  }

  bool IdfObject_Impl::isValid(StrictnessLevel level) const {
    return isValid(level, false);
  }

  bool IdfObject_Impl::isValid(StrictnessLevel level, bool checkNames) const {
    return (validityReport(level, checkNames).numErrors() == 0);
  }

  ValidityReport IdfObject_Impl::validityReport(StrictnessLevel level, bool checkNames) const {
    ValidityReport report(level, getObject<IdfObject>());
    this->populateValidityReport(report, checkNames);
    return report;
  }

  bool IdfObject_Impl::dataFieldsEqual(const IdfObject& other) const {
    if (m_iddObject != other.iddObject()) {
      return false;
    }
    UnsignedVector myFields = dataFields();
    UnsignedVector otherFields = other.dataFields();
    if (myFields != otherFields) {
      return false;
    }

    OptionalUnsigned iName = m_iddObject.nameFieldIndex();

    // iddObject() same, field indices same--compare data
    for (unsigned i : myFields) {
      bool compareStrings = true;
      OptionalIddField oIddField = m_iddObject.getField(i);

      // integers
      if (oIddField && (oIddField->properties().type == IddFieldType::IntegerType)) {
        OptionalInt oMyIntValue = getInt(i);
        OptionalInt oOtherIntValue = other.getInt(i);
        if (oMyIntValue || oOtherIntValue) {
          if (!(oMyIntValue && oOtherIntValue)) {
            return false;
          }
          if (oMyIntValue.get() != oOtherIntValue.get()) {
            return false;
          }
          compareStrings = false;
        }
      }

      // doubles
      if (oIddField && (oIddField->properties().type == IddFieldType::RealType)) {
        OptionalDouble oMyDoubleValue = getDouble(i);
        OptionalDouble oOtherDoubleValue = other.getDouble(i);
        if (oMyDoubleValue || oOtherDoubleValue) {
          if (!(oMyDoubleValue && oOtherDoubleValue)) {
            return false;
          }
          if (!equal(oMyDoubleValue.get(), oOtherDoubleValue.get())) {
            return false;
          }
          compareStrings = false;
        }
      }

      // urls
      // DLM: just treat these as strings
      //      if (oIddField && (oIddField->properties().type == IddFieldType::URLType)) {
      //        OptionalUrl oMyUrlValue = getURL(i);
      //        OptionalUrl oOtherUrlValue = other.getURL(i);
      //        if (oMyUrlValue || oOtherUrlValue) {
      //          if (!(oMyUrlValue && oOtherUrlValue)) { return false; }
      //          if (oMyUrlValue.get() != oOtherUrlValue.get()) { return false; }
      //          compareStrings = false;
      //        }
      //      }

      // strings (case-insensitive)
      if (compareStrings) {
        OptionalString oMyStringValue = getString(i);
        OptionalString oOtherStringValue = other.getString(i);
        OS_ASSERT(oMyStringValue);
        OS_ASSERT(oOtherStringValue);
        if (!istringEqual(*oMyStringValue, *oOtherStringValue)) {
          if (iName && (i == iName.get()) && (handle() == other.handle())) {
            continue;
          }
          return false;
        }
      }
    }

    return true;
  }

  bool IdfObject_Impl::objectListFieldsEqual(const IdfObject& other) const {
    if (m_iddObject != other.iddObject()) {
      return false;
    }
    UnsignedVector myFields = objectListFields();
    UnsignedVector otherFields = other.objectListFields();
    if (myFields != otherFields) {
      return false;
    }

    // iddObject() same, field indices same--compare data
    for (unsigned i : myFields) {
      // in idf, just comparing strings
      OptionalString oMyStringValue = getString(i);
      OptionalString oOtherStringValue = other.getString(i);
      OS_ASSERT(oMyStringValue);
      OS_ASSERT(oOtherStringValue);
      if (!istringEqual(*oMyStringValue, *oOtherStringValue)) {
        return false;
      }
    }

    return true;
  }

  bool IdfObject_Impl::objectListFieldsNonConflicting(const IdfObject& other) const {
    if (m_iddObject != other.iddObject()) {
      return false;
    }
    UnsignedVector myFields = objectListFields();
    UnsignedVector otherFields = other.objectListFields();
    // require same managedObjectLists fields to at least exist
    if (myFields != otherFields) {
      return false;
    }

    // iddObject() same--compare data
    for (unsigned i : myFields) {
      // in idf, just comparing strings
      OptionalString oMyStringValue = getString(i);
      OS_ASSERT(oMyStringValue);
      // empty() == null pointer == ok
      if (oMyStringValue->empty()) {
        continue;
      }

      OptionalString oOtherStringValue = other.getString(i);
      OS_ASSERT(oOtherStringValue);
      // empty() == null pointer == ok
      if (oOtherStringValue->empty()) {
        continue;
      }
      if (!istringEqual(*oMyStringValue, *oOtherStringValue)) {
        return false;
      }
    }

    return true;
  }

  // SERIALIZATION

  std::shared_ptr<IdfObject_Impl> IdfObject_Impl::load(const std::string& text) {
    std::shared_ptr<IdfObject_Impl> result;
    IdfObject_Impl idfObjectImpl;

    try {
      idfObjectImpl.parse(text, true);
      idfObjectImpl.resizeToMinFields();
    } catch (...) {
      return result;
    }

    bool keepHandle = idfObjectImpl.iddObject().hasHandleField();
    result = std::shared_ptr<IdfObject_Impl>(new IdfObject_Impl(idfObjectImpl, keepHandle));
    return result;
  }

  std::shared_ptr<IdfObject_Impl> IdfObject_Impl::load(const std::string& text, const IddObject& iddObject) {
    std::shared_ptr<IdfObject_Impl> result;
    IdfObject_Impl idfObjectImpl(iddObject, false, true);

    try {
      idfObjectImpl.parse(text, false);
      idfObjectImpl.resizeToMinFields();
    } catch (...) {
      return result;
    }

    bool keepHandle = idfObjectImpl.iddObject().hasHandleField();
    result = std::shared_ptr<IdfObject_Impl>(new IdfObject_Impl(idfObjectImpl, keepHandle));
    return result;
  }

  std::ostream& IdfObject_Impl::print(std::ostream& os) const {
    unsigned n = numFields();
    if (n == 0) {
      printName(os, false);
    } else {
      printName(os, true);
    }

    for (unsigned i = 0; i < n; ++i) {
      if (i < n - 1) {
        printField(os, i);
      } else {
        printField(os, i, true);
      }
    }

    os << '\n';

    return os;
  }

  std::ostream& IdfObject_Impl::printName(std::ostream& os, bool hasFields) const {
    // print comment, if any
    if (!m_comment.empty()) {
      os << m_comment << '\n';
    }

    // if this is a comment only object, return
    // todo, tighten up handling of comments with comment only object type
    if (boost::iequals(m_iddObject.name(), iddRegex::commentOnlyObjectName())) {
      return os;
    }

    os << m_iddObject.name();

    if (hasFields) {
      os << "," << '\n';
    } else {
      os << ";" << '\n';
    }

    return os;
  }

  std::ostream& IdfObject_Impl::printField(std::ostream& os, unsigned index, bool isLastField) const {
    if (index < numFields()) {
      // different formatting for vertices
      if ((m_iddObject.properties().format == "vertices") && (m_iddObject.isExtensibleField(index))) {
        ExtensibleIndex eIndex = m_iddObject.extensibleIndex(index);
        // making this static is a bad idea,
        // it makes no sense in a threaded environment
        static int textWidth(0);
        if (eIndex.field == 0) {
          os << "  ";
          textWidth = 0;
        } else {
          os << " ";
        }
        // field value
        os << m_fields[index];
        // delimiter
        if (isLastField) {
          os << ";";
        } else {
          os << ",";
        }
        textWidth += m_fields[index].size();
        // comment
        if (eIndex.field == m_iddObject.properties().numExtensible - 1) {
          int numSpaces = IdfObject::printedFieldSpace() - textWidth - 4;
          if (numSpaces > 0) {
            os << std::setw(numSpaces) << " ";
          }
          os << " !- X,Y,Z Vertex " << eIndex.group + 1;
          IddField iddField = m_iddObject.getField(index).get();
          if (OptionalString units = iddField.properties().units) {
            os << " {" << *units << "}";
          }
          os << '\n';
        }
      } else {
        // field value
        os << "  " << m_fields[index];
        // delimiter
        if (isLastField) {
          os << ";";
        } else {
          os << ",";
        }
        // field comment
        int numSpaces = IdfObject::printedFieldSpace() - int(m_fields[index].size());
        if (numSpaces > 0) {
          os << std::setw(numSpaces) << " ";
        }
        os << " " << fieldComment(index, true) << '\n';
      }
    }  // if index < numFields()
    return os;
  }

  void IdfObject_Impl::emitChangeSignals() {
    if (m_diffs.empty()) {
      return;
    }

    bool nameChange = false;
    bool dataChange = false;

    for (const IdfObjectDiff& diff : m_diffs) {

      if (diff.isNull()) {
        continue;
      }

      boost::optional<unsigned> index = diff.index();
      if (index) {

        OptionalIddField oIddField = m_iddObject.getField(*index);

        if (oIddField && oIddField->isNameField()) {
          nameChange = true;
        } else {
          dataChange = true;
        }
      }
    }

    if (nameChange) {
      this->onNameChange.nano_emit();
    }

    if (dataChange) {
      this->onDataChange.nano_emit();
    }

    this->onChange.nano_emit();

    m_diffs.clear();
  }

  // PRIVATE

  void IdfObject_Impl::resizeToMinFields() {
    unsigned min_n = m_iddObject.numFieldsInDefaultObject();
    unsigned n = numFields();
    if (n < min_n) {
      m_fields.resize(min_n);
      n = min_n;
    }
    // also make sure extensible groups are whole
    if (m_iddObject.properties().extensible) {
      int nExtFields = n - m_iddObject.numFields();
      if (nExtFields > 0) {
        int groupSize = m_iddObject.properties().numExtensible;
        int modulo = nExtFields % groupSize;
        if (modulo > 0) {
          m_fields.resize(n + (groupSize - modulo));
        }
      }
    }
  }

  void IdfObject_Impl::parse(const std::string& text, bool getIddFromFactory) {
    std::string objectType;

    // cut down on this text as we parse
    std::string parsedText(text);

    // get preceding comments
    boost::smatch matches;
    while (boost::regex_match(parsedText, idfRegex::commentOnlyLine()) && boost::regex_search(parsedText, matches, idfRegex::commentOnlyLine())) {
      std::string comment(matches[1].first, matches[1].second);
      std::string otherText(matches[2].first, matches[2].second);
      boost::trim_left(otherText);

      // append the comment
      if (!comment.empty()) {
        m_comment += "!" + comment + idfRegex::newLinestring();
      }

      // reduce the parsed text
      parsedText = otherText;
    }

    // the first entry will be the object type
    if (boost::regex_search(parsedText, matches, idfRegex::line())) {
      objectType = std::string(matches[1].first, matches[1].second);
      boost::trim(objectType);
      std::string commentOrOtherText(matches[2].first, matches[2].second);
      boost::trim_left(commentOrOtherText);
      std::string otherText(matches[3].first, matches[3].second);

      if (getIddFromFactory) {
        // find appropriate IddObject in IddFactory
        OptionalIddObject candidate = IddFactory::instance().getObject(objectType);
        if (candidate) {
          m_iddObject = *candidate;
        } else {
          LOG(Warn, "IddObject type '" << objectType << "' not found in IddFactory. " << "Reverting to default Catchall object.");
          OS_ASSERT(m_iddObject.name() == "Catchall");
          m_fields.push_back(objectType);
          objectType = "Catchall";
        }
      } else {
        if (!boost::iequals(objectType, m_iddObject.name())) {
          if (m_iddObject.type() != IddObjectType::Catchall) {
            LOG(Error, "IdfObject type '" << objectType << "', does not equal its IddObject name '" << m_iddObject.name()
                                          << "'. Reverting to default Catchall IddObject.");
          }
          m_iddObject = IddObject();
          m_fields.push_back(objectType);
          objectType = "Catchall";
        }
      }

      if (boost::regex_match(commentOrOtherText, idfRegex::commentOnlyLine())
          || boost::regex_match(commentOrOtherText, commentRegex::whitespaceOnlyBlock())) {

        // set comment
        m_comment += commentOrOtherText;

        // reduce the parsed text
        parsedText = otherText;
      } else {
        // reduce the parsed text
        parsedText = commentOrOtherText + otherText;
      }

    } else {
      LOG_AND_THROW("Cannot extract an IdfObject type from text '" << parsedText << "'");
    }

    // get trailing comments
    while (boost::regex_match(parsedText, idfRegex::commentOnlyLine()) && boost::regex_search(parsedText, matches, idfRegex::commentOnlyLine())) {
      std::string comment(matches[1].first, matches[1].second);
      std::string otherText(matches[2].first, matches[2].second);
      boost::trim_left(otherText);

      // append the comment
      if (!comment.empty()) {
        m_comment += "!" + comment + idfRegex::newLinestring();
      }

      // reduce the parsed text
      parsedText = otherText;
    }

    // remove trailing whitespace and new lines
    boost::trim_right(m_comment);

    // parse the fields
    parseFields(parsedText);
  }

  void IdfObject_Impl::parseFields(const std::string& text) {
    // match variables
    boost::match_results<std::string::const_iterator> matches;

    // cut down on this text as we parse
    std::string::const_iterator start = text.begin();
    std::string::const_iterator stop = text.end();

    // current idd field index
    unsigned iddFieldIndex = 0;

    // parse all the fields
    while (boost::regex_search(start, stop, matches, idfRegex::line())) {
      std::string fieldText(matches[1].first, matches[1].second);
      boost::trim(fieldText);
      std::string commentOrOtherText(matches[2].first, matches[2].second);
      boost::trim(commentOrOtherText);

      if (commentOrOtherText.empty() || boost::regex_match(commentOrOtherText, idfRegex::commentOnlyLine())) {
        // reduce the text
        start = matches[3].first;
        stop = matches[3].second;
      } else {
        // reduce the text; there may be multiple fields on this line
        start = matches[2].first;
        stop = matches[3].second;

        // matches[2] is not a comment
        commentOrOtherText.clear();
      }

      // get the idd field
      OptionalIddField iddField = m_iddObject.getField(iddFieldIndex);

      if (iddField) {

        // add this to our fields
        m_fields.push_back(fieldText);

        if (!commentOrOtherText.empty()) {
          // drop default comments
          if (!boost::regex_match(commentOrOtherText, commentRegex::editorCommentWhitespaceOnlyLine())) {
            m_fieldComments.resize(m_fields.size());
            m_fieldComments.back() = commentOrOtherText;
          }
        }

        // keep handle if this is a handle field
        if (iddField->properties().type == IddFieldType::HandleType) {
          Handle candidate = toUUID(fieldText);
          if (!candidate.isNull()) {
            m_handle = candidate;
          }
        }

      } else {
        LOG(Error, "IdfObject of type '" << m_iddObject.name() << "' " << "cannot have field index of " << iddFieldIndex << ". "
                                         << "Cutting off IdfObject field parsing here, with the following text " << "remaining: " << '\n'
                                         << fieldText << '\n'
                                         << std::string(start, stop));
        return;
      }

      // increment current idd field index
      ++iddFieldIndex;
    }  // while line matches

    std::string unparsedText(start, stop);
    boost::trim(unparsedText);
    if (!unparsedText.empty()) {
      LOG(Warn, "After parsing IdfObject fields, the following text remains unprocessed: " << '\n' << unparsedText);
    }
  }

  // GETTER AND SETTER HELPERS

  bool IdfObject_Impl::setIddObject(const IddObject& iddObject) {
    m_iddObject = iddObject;
    if (m_fields.size() < minFields()) {
      m_fields.resize(minFields());
    } else {
      // pop any fields that the IddObject does not recognize
      for (unsigned i = 0, n = numFields(); i < n; ++i) {
        if (!(m_iddObject.isNonextensibleField(i) || m_iddObject.isExtensibleField(i))) {
          m_fields.resize(i);
          if (m_fieldComments.size() > m_fields.size()) {
            m_fieldComments.resize(i);
          }
          break;
        }
      }
    }
    return true;
  }

  UnsignedVector IdfObject_Impl::trimFieldIndices(const UnsignedVector& indices) const {
    unsigned n = m_fields.size();  // number of fields
    UnsignedVector result = indices;
    // quick check to see if action is necessary
    if (!result.empty() && (result.back() >= n)) {
      // assume indices are in order and find first that is out of range
      for (auto it = result.begin(), itEnd = result.end(); it != itEnd; ++it) {
        if (*it >= n) {
          result.erase(it, result.end());
          break;
        }
      }
    }
    return result;
  }

  UnsignedVector IdfObject_Impl::repeatExtensibleIndices(const UnsignedVector& indices) const {

    // assume indices is in order, and any extensible field indices are from the first
    // extensible group
    UnsignedVector result = indices;

    // nothing to do if not extensible
    if (!m_iddObject.properties().extensible) {
      return result;
    }

    // nothing to do if last index is not in extensible group
    if (!result.empty() && !m_iddObject.isExtensibleField(result.back())) {
      return result;
    }

    // otherwise, find group indices of extensible field indices in result
    UnsignedVector groupIndices;
    for (unsigned index : result) {
      if (m_iddObject.isExtensibleField(index)) {
        groupIndices.push_back(m_iddObject.extensibleIndex(index).field);
      }
    }

    unsigned n = m_fields.size();
    unsigned nn = m_iddObject.numFields();
    unsigned ne = m_iddObject.properties().numExtensible;
    unsigned groupIndex = 1;
    while (nn + groupIndex * ne < n) {
      for (unsigned gi : groupIndices) {
        unsigned index = nn + groupIndex * ne + gi;
        if (index < n) {
          result.push_back(index);
        }
      }
      ++groupIndex;
    }

    return result;
  }

  // QUERY HELPERS

  void IdfObject_Impl::populateValidityReport(ValidityReport& report, bool /*checkNames*/) const {
    // field-level errors
    if (report.level() > StrictnessLevel::None) {
      for (unsigned index = 0; index < m_fields.size(); ++index) {
        DataErrorVector fieldErrors = fieldDataIsValid(index, report.level());
        for (const DataError& error : fieldErrors) {
          report.insertError(error);
        }
      }
    }

    // StrictnessLevel::Final
    if (report.level() > StrictnessLevel::Draft) {

      // DataErrorType::NumberOfFields
      // object-level
      if ((numFields() < minFields()) || (maxFields() && (numFields() > maxFields().get()))) {
        report.insertError(DataError(getObject<IdfObject>(), DataErrorType(DataErrorType::NumberOfFields)));
      }

    }  // end StrictnessLevel::Final
  }

  std::vector<DataError> IdfObject_Impl::fieldDataIsValid(unsigned index, const StrictnessLevel& level) const {
    DataErrorVector result;

    // StrictnessLevel::Minimal
    if (level > StrictnessLevel::None) {

      // DataErrorType::NoIdd
      // field-level
      if (!m_iddObject.getField(index)) {
        result.push_back(DataError(index, getObject<IdfObject>(), DataErrorType(DataErrorType::NoIdd)));
        // no other checks will work
        return result;
      }
    }

    // StrictnessLevel::Draft
    if (level > StrictnessLevel::Minimal) {

      // DataErrorType::DataType
      // DataErrorType::NumericBound
      // field-level
      if (!fieldDataIsCorrectType(index)) {
        result.push_back(DataError(index, getObject<IdfObject>(), DataErrorType(DataErrorType::DataType)));
      }
      if (!fieldDataIsWithinBounds(index)) {
        result.push_back(DataError(index, getObject<IdfObject>(), DataErrorType(DataErrorType::NumericBound)));
      }

    }  // end StrictnessLevel::Draft

    // StrictnessLevel::Final
    if (level > StrictnessLevel::Draft) {

      // DataErrorType::NullAndRequired
      // field-level
      if (!fieldIsNonnullIfRequired(index)) {
        result.push_back(DataError(index, getObject<IdfObject>(), DataErrorType(DataErrorType::NullAndRequired)));
      }
    }  // end StrictnessLevel::Final

    return result;
  }

  bool IdfObject_Impl::fieldDataIsCorrectType(unsigned index) const {
    OptionalIddField oIddField = m_iddObject.getField(index);
    if (!oIddField) {
      return true;
    }

    IddField iddField = *oIddField;
    IddFieldType fieldType = iddField.properties().type;
    OS_ASSERT(m_fields.size() > index);

    if ((fieldType == IddFieldType::IntegerType) && (!m_fields[index].empty())) {
      OptionalInt value = getInt(index);
      if (!value) {
        // ok if autosize or autocalculate
        if (iddField.properties().autosizable && istringEqual(m_fields[index], "autosize")) {
        } else if (iddField.properties().autocalculatable && istringEqual(m_fields[index], "autocalculate")) {
        } else if (iddField.properties().autosizable && istringEqual(m_fields[index], "autocalculate")) {
          LOG(Info, "Field " << index << ", '" << iddField.name() << "', of an object of type " << m_iddObject.name()
                             << " has 'autocalculate' as its value even though it is autosizable.");
        } else if (iddField.properties().autocalculatable && istringEqual(m_fields[index], "autosize")) {
          LOG(Info, "Field " << index << ", '" << iddField.name() << "', of an object of type " << m_iddObject.name()
                             << " has 'autosize' as its value even though it is autocalculable.");
        } else {
          return false;
        }
      } else {
        // integers can't be NaN or Infinity
      }
    }

    if ((fieldType == IddFieldType::RealType) && (!m_fields[index].empty())) {
      OptionalDouble value = getDouble(index);
      if (!value) {
        // ok if autosize or autocalculate
        if (iddField.properties().autosizable && istringEqual(m_fields[index], "autosize")) {
        } else if (iddField.properties().autocalculatable && istringEqual(m_fields[index], "autocalculate")) {
        } else if (iddField.properties().autosizable && istringEqual(m_fields[index], "autocalculate")) {
          LOG(Info, "Field " << index << ", '" << iddField.name() << "', of an object of type " << m_iddObject.name()
                             << " has 'autocalculate' as its value even though it is autosizable.");
        } else if (iddField.properties().autocalculatable && istringEqual(m_fields[index], "autosize")) {
          LOG(Info, "Field " << index << ", '" << iddField.name() << "', of an object of type " << m_iddObject.name()
                             << " has 'autosize' as its value even though it is autocalculable.");
        } else {
          return false;
        }
      } else {
        if (std::isnan(*value)) {
          LOG(Warn, "Cannot set field " << index << ", '" << iddField.name() << "', an object of type " << m_iddObject.name() << " to NaN.");
          return false;
        } else if (std::isinf(*value)) {
          LOG(Warn, "Cannot set field " << index << ", '" << iddField.name() << "', an object of type " << m_iddObject.name() << " to Infinity.");
          return false;
        }
      }
    }

    if ((fieldType == IddFieldType::ChoiceType) && (!m_fields[index].empty())) {
      // value should iequal one of the keys
      IddKeyVector keys = iddField.keys();
      NameFinder<IddKey> finder(m_fields[index]);
      auto loc = std::find_if(keys.begin(), keys.end(), finder);
      if (loc == keys.end()) {
        return false;
      }
    }

    return true;
  }

  bool IdfObject_Impl::fieldDataIsWithinBounds(unsigned index) const {
    OptionalIddField oIddField = m_iddObject.getField(index);
    if (!oIddField) {
      return true;
    }  // default to true

    IddField iddField = *oIddField;
    IddFieldType fieldType = iddField.properties().type;
    OS_ASSERT(m_fields.size() > index);

    if (fieldType == IddFieldType::IntegerType) {
      OptionalInt value = getInt(index);
      if (value) {
        auto fieldValue = static_cast<double>(*value);
        return withinBounds(fieldValue, iddField);
      }
    }
    if (fieldType == IddFieldType::RealType) {
      OptionalDouble value = getDouble(index);
      if (value) {
        return withinBounds(*value, iddField);
      }
    }

    return true;
  }

  bool IdfObject_Impl::fieldIsNonnullIfRequired(unsigned index) const {
    OptionalIddField oIddField = m_iddObject.getField(index);
    if (!oIddField) {
      return true;
    }  // default to true

    IddField iddField = *oIddField;
    OS_ASSERT(m_fields.size() > index);

    if (iddField.properties().required && (!iddField.isObjectListField()) && m_fields[index].empty()) {
      return false;
    }
    return true;
  }

  bool IdfObject_Impl::withinBounds(double fieldValue, const IddField& iddField) const {

    // minimum bounds
    OptionalDouble boundValue = iddField.properties().minBoundValue;
    IddFieldProperties::BoundTypes boundType = iddField.properties().minBoundType;
    if (boundType != IddFieldProperties::Unbounded) {
      OS_ASSERT(boundValue);
      // check min bound
      if (boundType == IddFieldProperties::InclusiveBound) {
        if (fieldValue < *boundValue) {
          return false;
        }
      } else if (fieldValue <= *boundValue) {
        return false;
      }
    }

    // maximum bounds
    boundValue = iddField.properties().maxBoundValue;
    boundType = iddField.properties().maxBoundType;
    if (boundType != IddFieldProperties::Unbounded) {
      OS_ASSERT(boundValue);
      if (boundType == IddFieldProperties::InclusiveBound) {
        if (fieldValue > *boundValue) {
          return false;
        }
      } else if (fieldValue >= *boundValue) {
        return false;
      }
    }

    return true;
  }

  OSOptionalQuantity IdfObject_Impl::getQuantityFromDouble(unsigned index, boost::optional<double> value, bool returnIP) const {
    OptionalIddField iddField = m_iddObject.getField(index);
    if (!iddField) {
      LOG_AND_THROW("get/setQuantity not available without an IddField. Asked to getQuantity at "
                    "field "
                    << index << "for IdfObject with Idd:\n"
                    << m_iddObject);
    }

    OptionalUnit siUnit = iddField->getUnits(false);
    OptionalUnit units = siUnit;
    OptionalUnit ipUnit;
    if (returnIP) {
      ipUnit = iddField->getUnits(true);
      units = ipUnit;
    }

    if (!units) {
      LOG_AND_THROW("Unable to construct a unit for field " << index << " for IdfObject with " << "Idd:\n" << m_iddObject);
    }
    OS_ASSERT(siUnit);

    if (value) {
      Quantity result(*value, *siUnit);
      if (returnIP) {
        OptionalQuantity temp = convert(result, *ipUnit);
        if (temp) {
          result = *temp;
        }
      }
      return OSOptionalQuantity(result);
    }

    return OSOptionalQuantity(*units);
  }

  boost::optional<double> IdfObject_Impl::getDoubleFromQuantity(unsigned index, const Quantity& q) const {
    OptionalDouble result;

    OptionalIddField iddField = m_iddObject.getField(index);
    if (!iddField) {
      LOG(Error, "get/setQuantity not available without an IddField. Asked to setQuantity at field " << index << "for IdfObject with Idd:\n"
                                                                                                     << m_iddObject);
      return result;
    }

    Quantity wq(q);
    OptionalUnit unit = iddField->getUnits(false);

    // dimensionless
    if (!unit) {
      wq.setScale(0);
      if (wq.standardUnitsString(true).empty()) {
        result = wq.value();
      }
      return result;
    }

    // has unit, convert to it
    // (QuantityConverter checks for equality first, sets scale.)
    OptionalQuantity candidate = convert(wq, *unit);
    if (!candidate) {
      return result;
    }
    wq = *candidate;

    OS_ASSERT(wq.units() == *unit);

    LOG(Trace, "Converted " << q << " to " << wq << "to set field with units " << *unit << ".");

    result = wq.value();
    return result;
  }

  std::vector<std::string> IdfObject_Impl::fields() const {
    return m_fields;
  }

  std::vector<std::string> IdfObject_Impl::fieldComments() const {
    return m_fieldComments;
  }

  std::string IdfObject_Impl::encodeString(const std::string& value) const {
    std::string result;
    for (auto const& s : value) {
      switch (s) {
        case '\n':
          result.append("&#10");
          break;
        case '\r':
          result.append("&#13");
          break;
        case ',':
          result.append("&#44");
          break;
        case ';':
          result.append("&#59");
          break;
        case '!':
          result.append("&#33");
          break;
        default:
          result.push_back(s);
          break;
      }
    }
    return result;
  }

  std::string IdfObject_Impl::decodeString(const std::string& value) const {
    std::string result;
    char c;
    bool special_char = false;
    auto const max_length = value.size();
    auto const length = max_length - 3;
    // auto const * str = value.c_str();
    result.reserve(max_length);

    for (size_t i = 0; i < max_length; ++i) {
      if (i < length && value[i] == '&' && value[i + 1] == '#') {
        if (value[i + 2] == '1') {
          if (value[i + 3] == '0') {
            special_char = true;
            c = '\n';  // "&#10"
          } else if (value[i + 3] == '3') {
            special_char = true;
            c = '\r';  // "&#13"
          }
        } else if (value[i + 2] == '3' && value[i + 3] == '3') {
          special_char = true;
          c = '!';  // "&#33"
        } else if (value[i + 2] == '4' && value[i + 3] == '4') {
          special_char = true;
          c = ',';  // "&#44"
        } else if (value[i + 2] == '5' && value[i + 3] == '9') {
          special_char = true;
          c = ';';  // "&#59"
        }
      }

      if (special_char) {
        special_char = false;
        i += 3;
        result.push_back(c);
      } else {
        result.push_back(value[i]);
      }
    }
    return result;
  }

}  // namespace detail

// CONSTRUCTORS

IdfObject::IdfObject(IddObjectType type, bool fastName) : m_impl(std::make_shared<detail::IdfObject_Impl>(type, fastName)) {
  OS_ASSERT(m_impl);
}

IdfObject::IdfObject(const IddObject& iddObject, bool fastName) : m_impl(std::make_shared<detail::IdfObject_Impl>(iddObject, fastName)) {
  OS_ASSERT(m_impl);
}

IdfObject::IdfObject(std::shared_ptr<detail::IdfObject_Impl> impl) : m_impl(std::move(impl)) {
  OS_ASSERT(m_impl);
}

IdfObject::IdfObject(const IdfObject& other) : m_impl(other.m_impl) {
  OS_ASSERT(m_impl);
}

IdfObject& IdfObject::operator=(const IdfObject& other) {
  m_impl = other.m_impl;
  OS_ASSERT(m_impl);
  return *this;
}

IdfObject::IdfObject(IdfObject&& other) noexcept : m_impl(std::move(other.m_impl)) {}

IdfObject& IdfObject::operator=(IdfObject&& other) noexcept {
  m_impl = std::move(other.m_impl);
  return *this;
}

IdfObject IdfObject::clone(bool keepHandle) const {
  IdfObject copy(std::make_shared<detail::IdfObject_Impl>(*m_impl, keepHandle));
  return copy;
}

// GETTERS

Handle IdfObject::handle() const {
  return m_impl->handle();
}

IddObject IdfObject::iddObject() const {
  return m_impl->iddObject();
}

std::string IdfObject::comment() const {
  return m_impl->comment();
}

boost::optional<std::string> IdfObject::fieldComment(unsigned index, bool returnDefault) const {
  return m_impl->fieldComment(index, returnDefault);
}

boost::optional<std::string> IdfObject::name(bool returnDefault) const {
  return m_impl->name(returnDefault);
}

std::string IdfObject::nameString(bool returnDefault) const {
  return m_impl->nameString(returnDefault);
}

std::string IdfObject::briefDescription() const {
  return m_impl->briefDescription();
}

bool IdfObject::isEmpty(unsigned index) const {
  return m_impl->isEmpty(index);
}

boost::optional<std::string> IdfObject::getString(unsigned index, bool returnDefault, bool returnUninitializedEmpty) const {
  return m_impl->getString(index, returnDefault, returnUninitializedEmpty);
}

boost::optional<std::string> IdfObject::getField(unsigned index, bool returnDefault) const {
  return m_impl->getField(index, returnDefault);
}

boost::optional<double> IdfObject::getDouble(unsigned index, bool returnDefault) const {
  return m_impl->getDouble(index, returnDefault);
}

OSOptionalQuantity IdfObject::getQuantity(unsigned index, bool returnDefault, bool returnIP) const {
  return m_impl->getQuantity(index, returnDefault, returnIP);
}

OptionalUnsigned IdfObject::getUnsigned(unsigned index, bool returnDefault) const {
  return m_impl->getUnsigned(index, returnDefault);
}

OptionalInt IdfObject::getInt(unsigned index, bool returnDefault) const {
  return m_impl->getInt(index, returnDefault);
}

IdfExtensibleGroup IdfObject::getExtensibleGroup(unsigned groupIndex) const {
  return m_impl->getExtensibleGroup(groupIndex);
}

std::vector<IdfExtensibleGroup> IdfObject::extensibleGroups() const {
  return m_impl->extensibleGroups();
}

// SETTERS

void IdfObject::setComment(const std::string& msg) {
  return m_impl->setComment(msg);
}

bool IdfObject::setFieldComment(unsigned index, const std::string& cmnt) {
  return m_impl->setFieldComment(index, cmnt);
}

boost::optional<std::string> IdfObject::setName(const std::string& newName) {
  return m_impl->setName(newName);
}

boost::optional<std::string> IdfObject::createName(bool overwrite) {
  return m_impl->createName(overwrite);
}

bool IdfObject::setString(unsigned index, const std::string& value) {
  return m_impl->setString(index, value);
}

bool IdfObject::setDouble(unsigned index, double value) {
  return m_impl->setDouble(index, value);
}

bool IdfObject::setQuantity(unsigned index, const Quantity& q) {
  return m_impl->setQuantity(index, q);
}

bool IdfObject::setUnsigned(unsigned index, unsigned value) {
  return m_impl->setUnsigned(index, value);
}

bool IdfObject::setInt(unsigned index, int value) {
  return m_impl->setInt(index, value);
}

IdfExtensibleGroup IdfObject::pushExtensibleGroup() {
  return pushExtensibleGroup(StringVector());
}

IdfExtensibleGroup IdfObject::pushExtensibleGroup(const StringVector& values) {
  return m_impl->pushExtensibleGroup(values);
}

IdfExtensibleGroup IdfObject::insertExtensibleGroup(unsigned groupIndex) {
  return insertExtensibleGroup(groupIndex, StringVector());
}

IdfExtensibleGroup IdfObject::insertExtensibleGroup(unsigned groupIndex, const std::vector<std::string>& values) {
  return m_impl->insertExtensibleGroup(groupIndex, values);
}

std::vector<std::string> IdfObject::popExtensibleGroup() {
  return m_impl->popExtensibleGroup();
}

std::vector<std::string> IdfObject::eraseExtensibleGroup(unsigned groupIndex) {
  return m_impl->eraseExtensibleGroup(groupIndex);
}

std::vector<std::vector<std::string>> IdfObject::clearExtensibleGroups() {
  return m_impl->clearExtensibleGroups();
}

// QUERIES

unsigned IdfObject::numFields() const {
  return m_impl->numFields();
}

unsigned IdfObject::numNonextensibleFields() const {
  return m_impl->numNonextensibleFields();
}

unsigned IdfObject::minFields() const {
  return m_impl->minFields();
}

boost::optional<unsigned> IdfObject::maxFields() const {
  return m_impl->maxFields();
}

unsigned IdfObject::numExtensibleGroups() const {
  return m_impl->numExtensibleGroups();
}

unsigned IdfObject::minExtensibleGroups() const {
  return m_impl->minExtensibleGroups();
}

boost::optional<unsigned> IdfObject::maxExtensibleGroups() const {
  return m_impl->maxExtensibleGroups();
}

bool IdfObject::isObjectListField(unsigned index) const {
  return m_impl->isObjectListField(index);
}

std::vector<unsigned> IdfObject::objectListFields() const {
  return m_impl->objectListFields();
}

bool IdfObject::isDataField(unsigned index) const {
  return m_impl->isDataField(index);
}

std::vector<unsigned> IdfObject::dataFields() const {
  return m_impl->dataFields();
}

std::vector<unsigned> IdfObject::requiredFields() const {
  return m_impl->requiredFields();
}

bool IdfObject::isValid(StrictnessLevel level, bool checkNames) const {
  return m_impl->isValid(level, checkNames);
}

ValidityReport IdfObject::validityReport(StrictnessLevel level, bool checkNames) const {
  return m_impl->validityReport(level, checkNames);
}

bool IdfObject::dataFieldsEqual(const IdfObject& other) const {
  return m_impl->dataFieldsEqual(other);
}

bool IdfObject::objectListFieldsEqual(const IdfObject& other) const {
  return m_impl->objectListFieldsEqual(other);
}

bool IdfObject::objectListFieldsNonConflicting(const IdfObject& other) const {
  return m_impl->objectListFieldsNonConflicting(other);
}

bool IdfObject::operator==(const IdfObject& other) const {
  return (m_impl == other.m_impl);
}

bool IdfObject::operator!=(const IdfObject& other) const {
  return !(operator==(other));
}

// SERIALIZATION

OptionalIdfObject IdfObject::load(const std::string& text) {
  std::shared_ptr<detail::IdfObject_Impl> p = detail::IdfObject_Impl::load(text);
  if (p) {
    return IdfObject(p);
  }
  return boost::none;
}

OptionalIdfObject IdfObject::load(const std::string& text, const IddObject& iddObject) {
  std::shared_ptr<detail::IdfObject_Impl> p = detail::IdfObject_Impl::load(text, iddObject);
  if (p) {
    return IdfObject(p);
  }
  return boost::none;
}

int IdfObject::printedFieldSpace() {
  return 38;
}

std::ostream& IdfObject::print(std::ostream& os) const {
  return m_impl->print(os);
}

std::ostream& IdfObject::printName(std::ostream& os, bool hasFields) const {
  return m_impl->printName(os, hasFields);
}

std::ostream& IdfObject::printField(std::ostream& os, unsigned index, bool isLastField) const {
  return m_impl->printField(os, index, isLastField);
}

/** Function object for sorting by name. */
bool IdfObjectNameLess::operator()(const IdfObject& left, const IdfObject& right) const {
  boost::optional<std::string> leftName = left.name();
  boost::optional<std::string> rightName = right.name();

  bool result = false;
  if (leftName && rightName) {
    // a < b
    result = istringLess(*leftName, *rightName);
  } else if (!leftName && rightName) {
    // nil < !nil
    result = true;
  }
  return result;
}

/** Function object for sorting by impl. */
bool IdfObjectImplLess::operator()(const IdfObject& left, const IdfObject& right) const {
  return (left.getImpl<detail::IdfObject_Impl>() < right.getImpl<detail::IdfObject_Impl>());
}

/** Function object for sorting by IddObjectType. */
bool IdfObjectTypeLess::operator()(const IdfObject& left, const IdfObject& right) const {
  return (left.iddObject().type() < right.iddObject().type());
}

// ostream operator
std::ostream& operator<<(std::ostream& os, const IdfObject& idfObject) {
  return idfObject.print(os);
}

std::string objectName(const IdfObject& obj) {
  std::string result;
  if (obj.name()) {
    result = obj.name().get();
  } else {
    result = obj.iddObject().name();
  }
  return result;
}

}  // namespace openstudio
