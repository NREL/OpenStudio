/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "IddObject.hpp"
#include "IddObject_Impl.hpp"

#include "ExtensibleIndex.hpp"
#include "IddRegex.hpp"
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "IddKey.hpp"
#include "CommentRegex.hpp"

#include "../core/Assert.hpp"
#include "../core/ASCIIStrings.hpp"

#include <boost/lexical_cast.hpp>

using std::string;
using std::vector;
using boost::regex;
using boost::smatch;
using boost::regex_replace;
using boost::replace_all;

namespace openstudio {

namespace detail {

  // CONSTRUCTORS

  /// default constructor for serialization
  IddObject_Impl::IddObject_Impl() : m_name("Catchall"), m_type(IddObjectType::Catchall) {
    m_properties.extensible = true;
    m_properties.numExtensible = 1;
    m_properties.hasURL = false;
    OptionalIddField oField = IddField::load("Object Type", "A1, \\field Object Type \n \\type alpha", m_name);
    m_fields.push_back(*oField);
    oField = IddField::load("Generic Data Field", "A2; \\field Generic Data Field \n \\type alpha \n \\begin-extensible", m_name);
    OS_ASSERT(oField);
    m_extensibleFields.push_back(*oField);
  }

  // GETTERS

  std::string IddObject_Impl::name() const {
    return m_name;
  }

  IddObjectType IddObject_Impl::type() const {
    return m_type;
  }

  std::string IddObject_Impl::group() const {
    return m_group;
  }

  const IddObjectProperties& IddObject_Impl::properties() const {
    return m_properties;
  }

  const IddFieldVector& IddObject_Impl::nonextensibleFields() const {
    return m_fields;
  }

  const IddFieldVector& IddObject_Impl::extensibleGroup() const {
    return m_extensibleFields;
  }

  boost::optional<IddField> IddObject_Impl::getField(unsigned index) const {
    OptionalIddField field;

    // is index in the regular fields
    if (index < m_fields.size()) {
      field = m_fields[index];
    } else if (!m_extensibleFields.empty()) {
      // if not subtract out fields size and mod by number of extensible fields
      index = index - m_fields.size();
      index = index % m_extensibleFields.size();
      field = m_extensibleFields[index];
    }

    return field;
  }

  boost::optional<IddField> IddObject_Impl::getField(const std::string& fieldName) const {
    OptionalIddField result;

    // look in fields
    for (const IddField& field : m_fields) {
      if (boost::iequals(field.name(), fieldName)) {
        result = field;
        break;
      }
    }
    // look in extensible fields
    if (!result) {
      for (const IddField& field : m_extensibleFields) {
        if (boost::iequals(field.name(), fieldName)) {
          result = field;
          break;
        }
      }
    }

    return result;
  }

  boost::optional<int> IddObject_Impl::getFieldIndex(const std::string& fieldName) const {
    OptionalInt result;
    int index = 0;

    // look in fields
    for (const IddField& field : m_fields) {
      if (boost::iequals(field.name(), fieldName)) {
        result = index;
        break;
      }
      ++index;
    }

    // look in extensible fields
    if (!result) {
      for (const IddField& field : m_extensibleFields) {
        if (boost::iequals(field.name(), fieldName)) {
          result = index;
          break;
        }
        ++index;
      }
    }

    return result;
  }

  // SETTERS

  void IddObject_Impl::insertHandleField() {
    if (!hasHandleField()) {
      std::stringstream fieldText;
      fieldText << "  A1,  \\field Handle" << '\n' << "       \\type handle" << '\n' << "       \\required-field";
      IddField handleField = IddField::load("Handle", fieldText.str(), m_name).get();
      auto it = m_fields.insert(m_fields.begin(), handleField);
      ++it;
      for (auto itEnd = m_fields.end(); it != itEnd; ++it) {
        it->incrementFieldId();  // by default, applies only to 'A'-type fields
      }
      it = m_extensibleFields.begin();
      for (auto itEnd = m_extensibleFields.end(); it != itEnd; ++it) {
        it->incrementFieldId();  // by default, applies only to 'A'-type fields
      }
      ++m_properties.minFields;
      if (m_properties.maxFields) {
        unsigned newMaxFields = m_properties.maxFields.get() + 1;
        m_properties.maxFields = newMaxFields;
      }
    }
  }

  // QUERIES

  unsigned IddObject_Impl::numFields() const {
    return m_fields.size();
  }

  unsigned IddObject_Impl::numFieldsInDefaultObject() const {
    unsigned result = m_properties.minFields;

    // check for non-extensible required fields
    unsigned index = result;
    unsigned n = numFields();
    while (index < n) {
      if (m_fields[index].properties().required) {
        result = index + 1;
      }
      ++index;
    }

    // do not pre-create extensible groups, so user can directly call
    // .pushExtensibleGroup, without preceding it with a .clearExtensibleGroups.
    if (result > n) {
      result = n;
    }

    return result;
  }

  bool IddObject_Impl::isVersionObject() const {
    return (type() == IddObjectType::Version) || (type() == IddObjectType::OS_Version) || (boost::regex_match(name(), iddRegex::versionObjectName()));
  }

  bool IddObject_Impl::isNonextensibleField(unsigned index) const {
    if (index < m_fields.size()) {
      return true;
    }
    return false;
  }

  bool IddObject_Impl::isExtensibleField(unsigned index) const {
    if ((index >= m_fields.size()) && (m_properties.extensible)) {
      return true;
    }
    return false;
  }

  bool IddObject_Impl::hasHandleField() const {
    return ((!m_fields.empty()) && (m_fields[0].properties().type == IddFieldType::HandleType));
  }

  bool IddObject_Impl::hasNameField() const {
    if (m_nameFieldCache) {
      return m_nameFieldCache->first;
    }

    unsigned index = 0;
    if (hasHandleField()) {
      index = 1;
    }
    bool result = ((m_fields.size() > index) && (m_fields[index].isNameField()));
    m_nameFieldCache = std::pair<bool, unsigned>(result, index);
    return result;
  }

  boost::optional<unsigned> IddObject_Impl::nameFieldIndex() const {
    if (hasNameField()) {
      return m_nameFieldCache->second;
    }
    return boost::none;
  }

  bool IddObject_Impl::isRequiredField(unsigned index) const {
    OptionalIddField iddField = getField(index);
    if (iddField) {
      return iddField->properties().required;
    }
    return false;
  }

  bool IddObject_Impl::hasURL() const {
    return m_properties.hasURL;
  }

  ExtensibleIndex IddObject_Impl::extensibleIndex(unsigned index) const {
    if (!isExtensibleField(index)) {
      LOG_AND_THROW("Field " << index << " is not an extensible field in IddObject " << name() << ".");
    }
    ExtensibleIndex result(0, 0);
    result.field = (index - m_fields.size()) % m_properties.numExtensible;
    result.group = (index - result.field - m_fields.size()) / m_properties.numExtensible;
    return result;
  }

  unsigned IddObject_Impl::index(ExtensibleIndex extensibleIndex) const {
    if (!m_properties.extensible) {
      LOG_AND_THROW("IddObject " << name() << " does not have extensible fields.");
    }
    if (extensibleIndex.field >= m_properties.numExtensible) {
      LOG_AND_THROW("IddObject " << name() << " only has " << m_properties.numExtensible
                                 << " fields in its extensible group. Therefore, a ExtensibleIndex.field of " << extensibleIndex.field
                                 << " is invalid.");
    }
    return m_fields.size() + extensibleIndex.group * m_properties.numExtensible + extensibleIndex.field;
  }

  std::vector<std::string> IddObject_Impl::references() const {
    std::vector<std::string> result;
    if (OptionalUnsigned index = nameFieldIndex()) {
      result = m_fields[*index].properties().references;
      // To ensure uniqueness of name within a given class, we add a fake reference by class
      // https://github.com/NREL/OpenStudio/issues/3079
      //if (result.empty()) {
      std::string ref = name();
      result.push_back(ref + "UniqueNames");
      //}
    } else {
      std::string n = name();
      if (n == "OS:PortList") {
        result.push_back("ConnectionObject");
        result.push_back("PortLists");
      } else if (n == "OS:Connection") {
        result.push_back("ConnectionNames");
      }
    }

    return result;
  }

  std::set<std::string> IddObject_Impl::objectLists() const {
    std::set<std::string> result;
    for (const IddField& field : m_fields) {
      const IddFieldProperties& properties = field.properties();
      result.insert(properties.objectLists.begin(), properties.objectLists.end());
    }
    for (const IddField& field : m_extensibleFields) {
      const IddFieldProperties& properties = field.properties();
      result.insert(properties.objectLists.begin(), properties.objectLists.end());
    }
    return result;
  }

  std::set<std::string> IddObject_Impl::objectLists(unsigned index) const {
    std::set<std::string> result;

    if (isNonextensibleField(index)) {
      const IddFieldProperties& properties = m_fields[index].properties();
      result.insert(properties.objectLists.begin(), properties.objectLists.end());
    } else if (isExtensibleField(index)) {
      ExtensibleIndex eIndex = extensibleIndex(index);
      const IddFieldProperties& properties = m_extensibleFields[eIndex.field].properties();
      result.insert(properties.objectLists.begin(), properties.objectLists.end());
    }

    return result;
  }

  UnsignedVector IddObject_Impl::objectListFields() const {

    UnsignedVector result;

    for (unsigned index = 0; index < m_fields.size(); ++index) {
      if (m_fields[index].isObjectListField()) {
        result.push_back(index);
      }
    }
    for (unsigned index = 0; index < m_extensibleFields.size(); ++index) {
      if (m_extensibleFields[index].isObjectListField()) {
        result.push_back(m_fields.size() + index);
      }
    }

    return result;
  }

  std::vector<unsigned> IddObject_Impl::urlFields() const {
    UnsignedVector result;

    for (unsigned index = 0; index < m_fields.size(); ++index) {
      if (m_fields[index].properties().type == IddFieldType::URLType) {
        result.push_back(index);
      }
    }
    for (unsigned index = 0; index < m_extensibleFields.size(); ++index) {
      if (m_extensibleFields[index].properties().type == IddFieldType::URLType) {
        result.push_back(m_fields.size() + index);
      }
    }

    return result;
  }

  bool IddObject_Impl::operator==(const IddObject_Impl& other) const {
    if (this == &other) {
      return true;
    }

    if (m_type != other.m_type) {
      return false;
    }
    if (m_name != other.m_name) {
      return false;
    }
    if (m_group != other.m_group) {
      return false;
    }
    if (m_properties != other.m_properties) {
      return false;
    }
    if (m_fields != other.m_fields) {
      return false;
    }
    if (m_extensibleFields != other.m_extensibleFields) {
      return false;
    }

    return true;
  }

  // SERIALIZATION

  std::shared_ptr<IddObject_Impl> IddObject_Impl::load(const std::string& name, const std::string& group, const std::string& text,
                                                       IddObjectType type) {
    std::shared_ptr<IddObject_Impl> result;
    result = std::shared_ptr<IddObject_Impl>(new IddObject_Impl(name, group, type));

    try {
      result->parse(text);
    } catch (...) {
      return {};
    }

    return result;
  }

  /// print
  std::ostream& IddObject_Impl::print(std::ostream& os) const {
    if (m_fields.empty() && m_extensibleFields.empty()) {

      os << m_name << ";" << '\n';
      m_properties.print(os);
      os << '\n';

    } else {

      os << m_name << "," << '\n';
      m_properties.print(os);

      bool extensibleFields = !m_extensibleFields.empty();
      for (auto it = m_fields.begin(), itend = m_fields.end(); it != itend; ++it) {
        if (extensibleFields) {
          it->print(os, false);  // don't print ; just yet
        } else {
          it->print(os, (it == itend - 1));
        }
      }
      for (auto it = m_extensibleFields.begin(), itend = m_extensibleFields.end(); it != itend; ++it) {
        it->print(os, (it == itend - 1));
      }

      os << '\n';
    }

    return os;
  }

  // PRIVATE

  IddObject_Impl::IddObject_Impl(const string& name, const string& group, IddObjectType type) : m_name(name), m_group(group), m_type(type) {}

  void IddObject_Impl::parse(const std::string& text) {
    smatch matches;
    if (boost::regex_search(text, matches, iddRegex::objectAndFields())) {
      // find and parse the object text
      string objectText(matches[1].first, matches[1].second);
      parseObject(objectText);

      // find and parse the fields text
      string fieldsText(matches[2].first, matches[2].second);
      parseFields(fieldsText);

    } else if (boost::regex_match(text, iddRegex::objectNoFields())) {
      // there are no fields in this object, it is all object text
      parseObject(text);

    } else {
      // error
      LOG_AND_THROW("Unexpected pattern '" << text << "' found in object '" << m_name << "'");
    }

    // remove existing extensible fields and add them the the extensible list
    if (m_properties.extensible) {
      makeExtensible();
    }
  }

  void IddObject_Impl::makeExtensible() {
    // number of fields in extensible group
    unsigned numExtensible = m_properties.numExtensible;

    // check that numExtensible > 0
    if (numExtensible == 0) {
      LOG(Error, "Extensible length 0 in object '" << m_name << "'");
      return;
    }

    // find the begin extensible field, there should be only one
    auto extensibleBegin = m_fields.end();
    for (auto it = m_fields.begin(), itend = m_fields.end(); it != itend; ++it) {
      if (it->properties().beginExtensible) {
        extensibleBegin = it;
        break;
      }
    }

    // no extensible begin found
    if (extensibleBegin == m_fields.end()) {
      LOG(Error, "No begin-extensible field detected in object '" << m_name << "'");
      return;
    }

    // extensible begin is too close to the end of the field list
    if ((extensibleBegin + numExtensible) > m_fields.end()) {
      LOG(Error, "Extensible fields begin too close to end of fields in object '" << m_name << "'");
      return;
    }

    // copy extensible fields
    m_extensibleFields = IddFieldVector(extensibleBegin, extensibleBegin + numExtensible);

    // remove all the extensible fields from the field list
    m_fields.resize(extensibleBegin - m_fields.begin());

    // regexs that match extensible fields
    std::string replace;

    // replace names of extensible fields so they do not contain numbers
    // e.g. "Vertex 1 X-coordinate" -> "Vertex X-coordinate"
    for (IddField& extensibleField : m_extensibleFields) {
      std::string extensibleFieldName = extensibleField.name();
      extensibleFieldName = regex_replace(extensibleFieldName, iddRegex::numberAndPrecedingSpace(), replace, boost::format_first_only);
      openstudio::ascii_trim(extensibleFieldName);
      extensibleField.setName(extensibleFieldName);
    }

    // figure out numExtensibleGroupsRequired
    if (m_properties.minFields > 0) {
      unsigned minFields = m_properties.minFields;
      if (minFields > m_fields.size()) {
        double numerator(minFields - (unsigned)m_fields.size());
        double denominator(numExtensible);
        m_properties.numExtensibleGroupsRequired = unsigned(std::ceil(numerator / denominator));
      }
    }
  }

  void IddObject_Impl::parseObject(const std::string& text) {
    // find the object name and the property text
    smatch matches;
    string propertiesText;
    if (boost::regex_search(text, matches, iddRegex::line())) {
      std::string objectName = string(matches[1].first, matches[1].second);
      openstudio::ascii_trim(objectName);
      if (!boost::equals(m_name, objectName)) {
        LOG_AND_THROW("Object name '" << objectName << "' does not match expected '" << m_name << "'");
      }

      propertiesText = string(matches[2].first, matches[2].second);
      openstudio::ascii_trim(propertiesText);
    } else {
      LOG_AND_THROW("Could not determine object name from text '" << text << "'");
    }

    while (boost::regex_search(propertiesText, matches, iddRegex::metaDataComment())) {
      string thisProperty(matches[1].first, matches[1].second);
      openstudio::ascii_trim(thisProperty);
      parseProperty(thisProperty);

      propertiesText = string(matches[2].first, matches[2].second);
      openstudio::ascii_trim(propertiesText);
    }
    if (!((boost::regex_match(propertiesText, commentRegex::whitespaceOnlyBlock()))
          || (boost::regex_match(propertiesText, iddRegex::commentOnlyLine())))) {
      LOG_AND_THROW("Could not process properties text '" << propertiesText << "' in object '" << m_name << "'");
    }
  }

  void IddObject_Impl::parseProperty(const std::string& text) {
    smatch matches;
    if (boost::regex_search(text, matches, iddRegex::memoProperty())) {
      string memo(matches[1].first, matches[1].second);
      openstudio::ascii_trim(memo);
      if (m_properties.memo.empty()) {
        m_properties.memo = memo;
      } else {
        m_properties.memo += "\n" + memo;
      }

    } else if (boost::regex_match(text, iddRegex::uniqueProperty())) {
      m_properties.unique = true;

    } else if (boost::regex_match(text, iddRegex::requiredObjectProperty())) {
      m_properties.required = true;

    } else if (boost::regex_match(text, iddRegex::obsoleteProperty())) {
      m_properties.obsolete = true;

    } else if (boost::regex_match(text, iddRegex::hasurlProperty())) {
      m_properties.hasURL = true;

    } else if (boost::regex_search(text, matches, iddRegex::extensibleProperty())) {
      m_properties.extensible = true;

      string numExtensible(matches[1].first, matches[1].second);
      m_properties.numExtensible = boost::lexical_cast<unsigned>(numExtensible);

    } else if (boost::regex_search(text, matches, iddRegex::formatProperty())) {
      string format(matches[1].first, matches[1].second);
      openstudio::ascii_trim(format);
      m_properties.format = format;

    } else if (boost::regex_search(text, matches, iddRegex::minFieldsProperty())) {
      string minFields(matches[1].first, matches[1].second);
      m_properties.minFields = boost::lexical_cast<unsigned>(minFields);

    } else if (boost::regex_search(text, matches, iddRegex::maxFieldsProperty())) {
      string maxFields(matches[1].first, matches[1].second);
      m_properties.maxFields = boost::lexical_cast<unsigned>(maxFields);
    } else {
      // error, unknown property
      LOG_AND_THROW("Unknown property text '" << text << "' in object '" << m_name << "'");
    }
  }

  void IddObject_Impl::parseFields(const std::string& text) {
    static const boost::regex field_start("[AN][0-9]+[\\s]*[,;]");

    auto begin = text.begin();
    const auto end = text.end();

    boost::match_results<std::string::const_iterator> matches;
    if (boost::regex_search(begin, end, matches, field_start)) {
      begin = matches[0].first;
      if (begin != text.begin()) {
        LOG_AND_THROW("Could not process field text '" << text << "' in object ', start is not where expected" << m_name << "'");
      }
    } else {
      return;
    }

    std::string::const_iterator field_end;

    while (begin != end) {
      if (boost::regex_search(begin + 1, end, matches, field_start)) {
        field_end = matches[0].first;
      } else {
        field_end = end;
      }

      // take the text of the last field
      string fieldText(begin, field_end);
      begin = field_end;

      string fieldName;

      // peak ahead to find the field name for indexing in map
      smatch nameMatches;
      if (boost::regex_search(fieldText, nameMatches, iddRegex::name())) {
        fieldName = string(nameMatches[1].first, nameMatches[1].second);
        openstudio::ascii_trim(fieldName);
      } else if (boost::regex_search(fieldText, nameMatches, iddRegex::field())) {
        // if no explicit field name, use the type and number
        string fieldTypeChar(nameMatches[1].first, nameMatches[1].second);
        openstudio::ascii_trim(fieldTypeChar);
        string fieldTypeNumber(nameMatches[2].first, nameMatches[2].second);
        openstudio::ascii_trim(fieldTypeNumber);
        fieldName = fieldTypeChar + fieldTypeNumber;
      } else {
        // cannot find the field name
        LOG_AND_THROW("Cannot determine field name from text '" << fieldText << "'");
      }

      // construct the field
      OptionalIddField oField = IddField::load(fieldName, fieldText, m_name);
      if (!oField) {
        LOG_AND_THROW("Cannot parse IddField text '" << fieldText << "'.");
      }

      // construct a new object and put it in the object list and object map
      m_fields.push_back(*oField);
    }
  }

}  // namespace detail

// CONSTRUCTORS

IddObject::IddObject() : m_impl(std::shared_ptr<detail::IddObject_Impl>(new detail::IddObject_Impl())) {}

IddObject::IddObject(const IddObject& other) = default;

// GETTERS

std::string IddObject::name() const {
  return m_impl->name();
}

IddObjectType IddObject::type() const {
  return m_impl->type();
}

std::string IddObject::group() const {
  return m_impl->group();
}

const IddObjectProperties& IddObject::properties() const {
  return m_impl->properties();
}

const IddFieldVector& IddObject::nonextensibleFields() const {
  return m_impl->nonextensibleFields();
}

const IddFieldVector& IddObject::extensibleGroup() const {
  return m_impl->extensibleGroup();
}

boost::optional<IddField> IddObject::getField(unsigned index) const {
  return m_impl->getField(index);
}

boost::optional<IddField> IddObject::getField(const std::string& fieldName) const {
  return m_impl->getField(fieldName);
}

boost::optional<int> IddObject::getFieldIndex(const std::string& fieldName) const {
  return m_impl->getFieldIndex(fieldName);
}

// SETTERS

void IddObject::insertHandleField() {
  return m_impl->insertHandleField();
}

// QUERIES

unsigned IddObject::numFields() const {
  return m_impl->numFields();
}

unsigned IddObject::numFieldsInDefaultObject() const {
  return m_impl->numFieldsInDefaultObject();
}

bool IddObject::isVersionObject() const {
  return m_impl->isVersionObject();
}

bool IddObject::isNonextensibleField(unsigned index) const {
  return m_impl->isNonextensibleField(index);
}

bool IddObject::isExtensibleField(unsigned index) const {
  return m_impl->isExtensibleField(index);
}

bool IddObject::hasHandleField() const {
  return m_impl->hasHandleField();
}

bool IddObject::hasNameField() const {
  return m_impl->hasNameField();
}

boost::optional<unsigned> IddObject::nameFieldIndex() const {
  return m_impl->nameFieldIndex();
}

bool IddObject::isRequiredField(unsigned index) const {
  return m_impl->isRequiredField(index);
}

bool IddObject::hasURL() const {
  return m_impl->hasURL();
}

ExtensibleIndex IddObject::extensibleIndex(unsigned index) const {
  return m_impl->extensibleIndex(index);
}

unsigned IddObject::index(ExtensibleIndex extensibleIndex) const {
  return m_impl->index(extensibleIndex);
}

std::vector<std::string> IddObject::references() const {
  return m_impl->references();
}

std::set<std::string> IddObject::objectLists() const {
  return m_impl->objectLists();
}

std::set<std::string> IddObject::objectLists(unsigned index) const {
  return m_impl->objectLists(index);
}

std::vector<unsigned> IddObject::objectListFields() const {
  return m_impl->objectListFields();
}

std::vector<unsigned> IddObject::urlFields() const {
  return m_impl->urlFields();
}

bool IddObject::operator==(const IddObject& other) const {
  return (*m_impl == *(other.m_impl));
}

bool IddObject::operator!=(const IddObject& other) const {
  return !(*this == other);
}

// SERIALIZATION

boost::optional<IddObject> IddObject::load(const std::string& name, const std::string& group, const std::string& text, IddObjectType type) {
  std::shared_ptr<detail::IddObject_Impl> p = detail::IddObject_Impl::load(name, group, text, type);
  if (p) {
    return IddObject(p);
  } else {
    return boost::none;
  }
}

boost::optional<IddObject> IddObject::load(const std::string& name, const std::string& group, const std::string& text) {
  return load(name, group, text, IddObjectType(IddObjectType::UserCustom));
}

std::ostream& IddObject::print(std::ostream& os) const {
  return m_impl->print(os);
}

// PRIVATE

IddObject::IddObject(const std::shared_ptr<detail::IddObject_Impl>& impl) : m_impl(impl) {}

// NON-MEMBER FUNCTIONS

std::ostream& operator<<(std::ostream& os, const IddObject& iddObject) {
  return iddObject.print(os);
}

IddObjectTypeVector getIddObjectTypeVector(const IddObjectVector& objects) {
  IddObjectTypeVector result;
  for (const IddObject& object : objects) {
    result.push_back(object.type());
  }
  return result;
}

IddObjectTypeSet getIddObjectTypeSet(const IddObjectVector& objects) {
  IddObjectTypeSet result;
  for (const IddObject& object : objects) {
    result.insert(object.type());
  }
  return result;
}

std::vector<std::string> getIddKeyNames(const IddObject& object, unsigned index) {
  StringVector result;
  OptionalIddField oIddField = object.getField(index);
  if (!oIddField) {
    return result;
  }
  IddField iddField = *oIddField;
  IddKeyVector keys = iddField.keys();
  for (const IddKey& key : keys) {
    result.push_back(key.name());
  }
  return result;
}

}  // namespace openstudio
