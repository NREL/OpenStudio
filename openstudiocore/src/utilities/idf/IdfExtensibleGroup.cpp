/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "IdfExtensibleGroup.hpp"
#include "IdfObject.hpp"
#include "IdfObject_Impl.hpp"

#include "../idd/IddObjectProperties.hpp"
#include "../idd/IddFieldProperties.hpp"

#include "../core/Assert.hpp"

#include <boost/lexical_cast.hpp>

namespace openstudio {

// GETTERS

StringVector IdfExtensibleGroup::fields(bool returnDefault) const {
  StringVector result;
  for (unsigned i = 0, n = numFields(); i < n; ++i) {
    OptionalString str = getString(i,returnDefault);
    OS_ASSERT(str);
    result.push_back(*str);
  }
  return result;
}

StringVector IdfExtensibleGroup::fieldComments(bool returnDefault) const {
  StringVector result;
  for (unsigned i = 0, n = numFields(); i < n; ++i) {
    OptionalString str = fieldComment(i,returnDefault);
    if (str) { result.push_back(*str); }
    else { break; }
  }
  return result;
}

OptionalString IdfExtensibleGroup::fieldComment(unsigned fieldIndex, bool returnDefault) const {
  if (!isValid(fieldIndex)) { return boost::none; }
  return m_impl->fieldComment(mf_toIndex(fieldIndex),returnDefault);
}

bool IdfExtensibleGroup::isEmpty(unsigned fieldIndex) const
{
  OptionalString test = this->getString(fieldIndex, false);
  if (!test || test.get() == ""){
    return true;
  }
  return false;
}

OptionalString IdfExtensibleGroup::getString(unsigned fieldIndex, bool returnDefault) const {
  if (!isValid(fieldIndex)) { return boost::none; }
  return m_impl->getString(mf_toIndex(fieldIndex),returnDefault);
}

OptionalDouble IdfExtensibleGroup::getDouble(unsigned fieldIndex, bool returnDefault) const {
  if (!isValid(fieldIndex)) { return boost::none; }
  return m_impl->getDouble(mf_toIndex(fieldIndex),returnDefault);
}

OptionalUnsigned IdfExtensibleGroup::getUnsigned(unsigned fieldIndex, bool returnDefault) const {
  if (!isValid(fieldIndex)) { return boost::none; }
  return m_impl->getUnsigned(mf_toIndex(fieldIndex),returnDefault);
}

OptionalInt IdfExtensibleGroup::getInt(unsigned fieldIndex, bool returnDefault) const {
  if (!isValid(fieldIndex)) { return boost::none; }
  return m_impl->getInt(mf_toIndex(fieldIndex),returnDefault);
}

// SETTERS

bool IdfExtensibleGroup::setFieldComment(unsigned fieldIndex, const std::string& cmnt) {
  return isValid(fieldIndex) && m_impl->setFieldComment(mf_toIndex(fieldIndex),cmnt);
}

bool IdfExtensibleGroup::setFields(const std::vector<std::string>& values) {
  bool result = setFields(values,true);
  if (result) {
    m_impl->emitChangeSignals();
  }
  return result;
}

bool IdfExtensibleGroup::setFields(const std::vector<std::string>& values, bool checkValidity) {
  if ((values.size() == 0) || (values.size() != numFields())) { return false; }

  unsigned n = numFields();
  // save oldValues for rollback
  StringVector oldValues;
  for (unsigned i = 0; i < n; ++i) {
    OptionalString oValue = getString(i);
    OS_ASSERT(oValue);
    oldValues.push_back(*oValue);
  }

  // try to set values
  bool ok = true;
  for (unsigned i = 0; i < n; ++i) {
    ok = setString(i,values[i],checkValidity);
    if (!ok) { break; }
  }

  if (!ok) {
    // rollback
    for (unsigned i = 0; i < n; ++i) {
      bool rollbackOk = setString(i,oldValues[i],false);
      OS_ASSERT(rollbackOk);
    }
  }

  return ok;
}

bool IdfExtensibleGroup::setString(unsigned fieldIndex, const std::string& value) {
  bool result = setString(fieldIndex,value,true);
  if (result) {
    m_impl->emitChangeSignals();
  }
  return result;
}

bool IdfExtensibleGroup::setString(unsigned fieldIndex, const std::string& value, bool checkValidity) {
  // private method for internal use
  if (!isValid(fieldIndex)) { return false; }
  return m_impl->setString(mf_toIndex(fieldIndex),value,checkValidity);
}

bool IdfExtensibleGroup::setDouble(unsigned fieldIndex, double value) {
  if (!isValid(fieldIndex)) { return false; }
  return m_impl->setDouble(mf_toIndex(fieldIndex),value);
}

bool IdfExtensibleGroup::setUnsigned(unsigned fieldIndex, unsigned value) {
  if (!isValid(fieldIndex)) { return false; }
  return m_impl->setUnsigned(mf_toIndex(fieldIndex),value);
}

bool IdfExtensibleGroup::setInt(unsigned fieldIndex, int value) {
  if (!isValid(fieldIndex)) { return false; }
  return m_impl->setInt(mf_toIndex(fieldIndex),value);
}

IdfExtensibleGroup IdfExtensibleGroup::pushClone() const {
  if (empty()) {
    detail::IdfObject_ImplPtr p;
    return IdfExtensibleGroup(p,0);
  }

  StringVector values = fields();
  OS_ASSERT(values.size() == numFields());
  return m_impl->pushExtensibleGroup(values);
}

IdfExtensibleGroup IdfExtensibleGroup::insertClone(unsigned groupIndex) const {
  if (empty()) {
    detail::IdfObject_ImplPtr p;
    return IdfExtensibleGroup(p,0);
  }

  StringVector values = fields();
  OS_ASSERT(values.size() == numFields());
  return m_impl->insertExtensibleGroup(groupIndex,values);
}

// QUERIES

bool IdfExtensibleGroup::empty() const {
  if (!m_impl || (m_index >= m_impl->numFields())) { return true; }
  return false;
}

unsigned IdfExtensibleGroup::groupIndex() const {
  if (empty()) {
    LOG_AND_THROW("This extensible group is invalid, and so does not have an index.");
  }
  return m_impl->iddObject().extensibleIndex(m_index).group;
};

unsigned IdfExtensibleGroup::numFields() const {
  if (empty()) { return 0; }
  unsigned n = m_impl->iddObject().properties().numExtensible;
  if (m_index + n > m_impl->numFields()) {
    OS_ASSERT(false);
    return 0;
  }
  return n;
}

bool IdfExtensibleGroup::isValid(unsigned fieldIndex) const {
  if (empty()) { return false; }
  if (fieldIndex >= m_impl->iddObject().properties().numExtensible) { return false; }
  OS_ASSERT(m_index + fieldIndex < m_impl->numFields());
  return true;
}

std::vector<unsigned> IdfExtensibleGroup::objectListFields() const {
  UnsignedVector result;
  if (!empty()) {
    result = mf_subsetAndToFieldIndices(m_impl->objectListFields());
  }
  return result;
}

std::vector<unsigned> IdfExtensibleGroup::dataFields() const {
  UnsignedVector result;
  if (!empty()) {
    result = mf_subsetAndToFieldIndices(m_impl->dataFields());
  }
  return result;
}

std::vector<unsigned> IdfExtensibleGroup::requiredFields() const {
  UnsignedVector result;
  if (!empty()) {
    IddFieldVector iddExtFields = m_impl->iddObject().extensibleGroup();
    for (unsigned fieldIndex = 0, n = numFields(); fieldIndex < n; ++fieldIndex) {
      if (iddExtFields[fieldIndex].properties().required) {
        result.push_back(fieldIndex);
      }
    }
  }
  return result;
}

bool IdfExtensibleGroup::operator==(const IdfExtensibleGroup& other) const {
  return (m_impl == other.m_impl) && (m_index == other.m_index);
}

bool IdfExtensibleGroup::operator!=(const IdfExtensibleGroup& other) const {
  return !(*this == other);
}

// PRIVATE

IdfExtensibleGroup::IdfExtensibleGroup(std::shared_ptr<detail::IdfObject_Impl> impl,unsigned index)
    : m_impl(impl), m_index(index) 
{
  // impl is passed in as null in many cases
  //OS_ASSERT(m_impl);
}

unsigned IdfExtensibleGroup::mf_toIndex(unsigned fieldIndex) const {
  // only call after checking isValid(fieldIndex)
  try {
    return m_impl->iddObject().index(ExtensibleIndex(groupIndex(),fieldIndex));
  }
  catch (...) { }
  OS_ASSERT(false);
  return 0;
}

std::vector<unsigned> IdfExtensibleGroup::mf_indices() const {
  UnsignedVector result;
  if (!empty()) {
    for (unsigned i = 0, n = numFields(); i < n; ++i) {
      result.push_back(mf_toIndex(i));
    }
  }
  return result;
}

UnsignedVector IdfExtensibleGroup::mf_subsetAndToFieldIndices(UnsignedVector indices) const {
  UnsignedVector result;
  OS_ASSERT(!empty());
  // downselect and convert to only this ExtensibleGroup
  for (unsigned index : indices) {
    if (index >= m_index) {
      unsigned fieldIndex = index - m_index;
      if (isValid(fieldIndex)) { result.push_back(fieldIndex); }
    }
  }
  return result;
}

} // openstudio

