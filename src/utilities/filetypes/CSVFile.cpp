/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "CSVFile.hpp"
#include "CSVFile_Impl.hpp"

#include "../core/Assert.hpp"
#include "../core/PathHelpers.hpp"
#include "../core/Checksum.hpp"
#include "../data/Variant.hpp"
#include "../data/Vector.hpp"
#include "../time/DateTime.hpp"

#include <iostream>

#include <boost/regex.hpp>

namespace openstudio {
namespace detail {

  CSVFile_Impl::CSVFile_Impl() : m_numColumns(0) {}

  CSVFile_Impl::CSVFile_Impl(const std::string& s) {
    std::istringstream ss(s);

    // will throw on error
    m_rows = parseRows(ss);

    assignNumColumns();
    padRows();
  }

  CSVFile_Impl::CSVFile_Impl(const openstudio::path& p) {
    if (!boost::filesystem::exists(p) || !boost::filesystem::is_regular_file(p)) {
      LOG_AND_THROW("Path '" << p << "' is not a CSVFile file");
    }

    // open file
    std::ifstream ifs(openstudio::toSystemFilename(p));

    // will throw on error
    m_rows = parseRows(ifs);

    m_path = p;
    assignNumColumns();
    padRows();
  }

  CSVFile CSVFile_Impl::clone() const {
    CSVFile result(this->string());
    if (m_path) {
      result.setPath(*m_path);
    }

    return result;
  }

  std::string CSVFile_Impl::string() const {
    static const boost::regex escapeItRegex(",");

    std::string s;
    std::stringstream result;
    for (const auto& row : m_rows) {
      OS_ASSERT(row.size() == m_numColumns);
      for (size_t i = 0; i < m_numColumns; ++i) {

        switch (row[i].variantType().value()) {
          case VariantType::Integer:
            result << row[i].valueAsInteger();
            break;
          case VariantType::Double:
            result << row[i].valueAsDouble();
            break;
          case VariantType::String:
            s = row[i].valueAsString();
            if (boost::regex_match(s, escapeItRegex)) {
              result << "\"" << s << "\"";
            } else {
              result << s;
            }
            break;
          default:
            break;
        }

        if (i < m_numColumns - 1) {
          result << ",";
        }
      }
      result << "\n";
    }
    return result.str();
  }

  bool CSVFile_Impl::save() const {
    boost::optional<openstudio::path> p = path();
    if (!p) {
      return false;
    }

    if (makeParentFolder(*p)) {
      std::ofstream outFile(openstudio::toSystemFilename(*p));

      if (outFile) {
        try {
          outFile << string();
          outFile.close();
          return true;
        } catch (...) {
          LOG(Error, "Unable to write file to path '" << toString(*p) << "'.");
          return false;
        }
      }
    }

    LOG(Error, "Unable to write file to path '" << toString(*p) << "', because parent directory " << "could not be created.");

    return false;
  }

  bool CSVFile_Impl::saveAs(const openstudio::path& p) {
    setPath(p);
    return save();
  }

  boost::optional<openstudio::path> CSVFile_Impl::path() const {
    return m_path;
  }

  bool CSVFile_Impl::setPath(const openstudio::path& path) {
    m_path = path;
    return true;
  }

  void CSVFile_Impl::resetPath() {
    m_path.reset();
  }

  unsigned CSVFile_Impl::numColumns() const {
    return m_numColumns;
  }

  unsigned CSVFile_Impl::numRows() const {
    return m_rows.size();
  }

  std::vector<std::vector<Variant>> CSVFile_Impl::rows() const {
    return m_rows;
  }

  void CSVFile_Impl::addRow(const std::vector<Variant>& row) {
    m_rows.push_back(row);
    if (row.size() > m_numColumns) {
      m_numColumns = row.size();
      padRows();
    }

    while (m_rows[m_rows.size() - 1].size() < m_numColumns) {
      m_rows[m_rows.size() - 1].push_back(Variant(""));
    }
  }

  void CSVFile_Impl::setRows(const std::vector<std::vector<Variant>>& rows) {
    m_rows = rows;
    assignNumColumns();
  }

  void CSVFile_Impl::clear() {
    m_rows.clear();
    m_path.reset();
    m_numColumns = 0;
  }

  unsigned CSVFile_Impl::addColumn(const std::vector<DateTime>& dateTimes) {
    unsigned n = dateTimes.size();
    ensureNumRows(n);

    unsigned numRows = m_rows.size();
    for (unsigned i = 0; i < numRows; ++i) {
      if (i < n) {
        m_rows[i].push_back(Variant(dateTimes[i].toISO8601()));
      } else {
        m_rows[i].push_back(Variant(""));
      }
    }

    ++m_numColumns;
    return m_numColumns;
  }

  unsigned CSVFile_Impl::addColumn(const Vector& values) {
    unsigned n = values.size();
    ensureNumRows(n);

    unsigned numRows = m_rows.size();
    for (unsigned i = 0; i < numRows; ++i) {
      if (i < n) {
        m_rows[i].push_back(Variant(values[i]));
      } else {
        m_rows[i].push_back(Variant(""));
      }
    }

    ++m_numColumns;
    return m_numColumns;
  }

  unsigned CSVFile_Impl::addColumn(const std::vector<double>& values) {
    unsigned n = values.size();
    ensureNumRows(n);

    unsigned numRows = m_rows.size();
    for (unsigned i = 0; i < numRows; ++i) {
      if (i < n) {
        m_rows[i].push_back(Variant(values[i]));
      } else {
        m_rows[i].push_back(Variant(""));
      }
    }

    ++m_numColumns;
    return m_numColumns;
  }

  unsigned CSVFile_Impl::addColumn(const std::vector<std::string>& values) {
    unsigned n = values.size();
    ensureNumRows(n);

    unsigned numRows = m_rows.size();
    for (unsigned i = 0; i < numRows; ++i) {
      if (i < n) {
        m_rows[i].push_back(Variant(values[i]));
      } else {
        m_rows[i].push_back(Variant(""));
      }
    }

    ++m_numColumns;
    return m_numColumns;
  }

  std::vector<DateTime> CSVFile_Impl::getColumnAsDateTimes(unsigned columnIndex) const {
    if (columnIndex >= m_numColumns) {
      LOG(Warn, "Column index " << columnIndex << " invalid for number of columns " << m_numColumns);
      return {};
    }

    std::vector<DateTime> result;

    unsigned numRows = m_rows.size();
    for (unsigned i = 0; i < numRows; ++i) {
      if (m_rows[i][columnIndex].variantType() != VariantType::String) {
        LOG(Warn, "Value at row " << i << " and column " << columnIndex << " is not a DateTime string");
        return {};
      }

      boost::optional<DateTime> dateTime = DateTime::fromISO8601(m_rows[i][columnIndex].valueAsString());
      if (!dateTime) {
        LOG(Warn, "Value at row " << i << " and column " << columnIndex << " is not a DateTime string");
        return {};
      }
      result.push_back(*dateTime);
    }

    return result;
  }

  std::vector<double> CSVFile_Impl::getColumnAsDoubleVector(unsigned columnIndex) const {
    if (columnIndex >= m_numColumns) {
      LOG(Warn, "Column index " << columnIndex << " invalid for number of columns " << m_numColumns);
      return {};
    }

    std::vector<double> result;

    unsigned numRows = m_rows.size();
    for (unsigned i = 0; i < numRows; ++i) {

      boost::optional<double> value;
      if (m_rows[i][columnIndex].variantType() == VariantType::Double) {
        value = m_rows[i][columnIndex].valueAsDouble();
      } else if (m_rows[i][columnIndex].variantType() == VariantType::Integer) {
        value = m_rows[i][columnIndex].valueAsInteger();
      }

      if (!value) {
        LOG(Warn, "Value at row " << i << " and column " << columnIndex << " is not a numeric value");
        return {};
      }
      result.push_back(*value);
    }

    return result;
  }

  std::vector<std::string> CSVFile_Impl::getColumnAsStringVector(unsigned columnIndex) const {
    if (columnIndex >= m_numColumns) {
      LOG(Warn, "Column index " << columnIndex << " invalid for number of columns " << m_numColumns);
      return {};
    }

    std::vector<std::string> result;

    unsigned numRows = m_rows.size();
    for (unsigned i = 0; i < numRows; ++i) {

      if (m_rows[i][columnIndex].variantType() == VariantType::String) {
        result.push_back(m_rows[i][columnIndex].valueAsString());
      } else if (m_rows[i][columnIndex].variantType() == VariantType::Double) {
        std::stringstream ss;
        ss << m_rows[i][columnIndex].valueAsDouble();
        result.push_back(ss.str());
      } else if (m_rows[i][columnIndex].variantType() == VariantType::Integer) {
        std::stringstream ss;
        ss << m_rows[i][columnIndex].valueAsInteger();
        result.push_back(ss.str());
      }
    }

    return result;
  }

  // throws on error
  std::vector<std::vector<Variant>> CSVFile_Impl::parseRows(std::istream& input) {
    std::vector<std::vector<Variant>> result;

    // DLM: what conditions should make this throw?

    // Excel formated CSV regex, \A[^,"]*(?=,)|(?:[^",]*"[^"]*"[^",]*)+|[^",]*"[^"]*\Z|(?<=,)[^,]*(?=,)|(?<=,)[^,]*\Z|\A[^,]*\Z
    static const boost::regex csvRegex(
      "\\A[^,\"]*(?=,)|(?:[^\",]*\"[^\"]*\"[^\",]*)+|[^\",]*\"[^\"]*\\Z|(?<=,)[^,]*(?=,)|(?<=,)[^,]*\\Z|\\A[^,]*\\Z");
    static const boost::regex intRegex("^[-0-9]+$");
    static const boost::regex doubleRegex("^[+-]?\\d+\\.?(\\d+)?$");
    static const boost::regex quoteRegex("^\"(.*)\"$");

    std::string line;
    while (std::getline(input, line)) {
      std::vector<Variant> row;
      boost::regex_token_iterator<std::string::iterator> it{line.begin(), line.end(), csvRegex, 0};
      boost::regex_token_iterator<std::string::iterator> end;
      while (it != end) {
        std::string value = *it;
        if (boost::regex_match(value, intRegex)) {
          row.push_back(Variant(std::stoi(value)));
        } else if (boost::regex_match(value, doubleRegex)) {
          row.push_back(Variant(std::stod(value)));
        } else {
          boost::smatch valueMatch;
          if (boost::regex_search(value, valueMatch, quoteRegex)) {
            row.push_back(Variant(valueMatch[1]));
          } else {
            row.push_back(Variant(value));
          }
        }
        ++it;
      }
      result.push_back(row);
    }

    return result;
  }

  void CSVFile_Impl::assignNumColumns() {
    m_numColumns = 0;
    for (const auto& row : m_rows) {
      m_numColumns = std::max<unsigned>(m_numColumns, row.size());
    }
  }

  void CSVFile_Impl::ensureNumRows(unsigned numRows) {
    // add empty cells to existing columns if needed
    if (numRows > m_rows.size()) {
      unsigned numRowsToAdd = numRows - m_rows.size();
      std::vector<Variant> blankRow(m_numColumns, Variant(""));
      for (unsigned i = 0; i < numRowsToAdd; ++i) {
        m_rows.push_back(blankRow);
      }
    }
    OS_ASSERT(m_rows.size() >= numRows);
  }

  void CSVFile_Impl::padRows() {
    for (auto& row : m_rows) {
      while (row.size() < m_numColumns) {
        row.push_back(Variant(""));
      }
    }
  }

}  // namespace detail

CSVFile::CSVFile() : m_impl(std::shared_ptr<detail::CSVFile_Impl>(new detail::CSVFile_Impl())) {}

CSVFile::CSVFile(const std::string& s) : m_impl(std::shared_ptr<detail::CSVFile_Impl>(new detail::CSVFile_Impl(s))) {}

CSVFile::CSVFile(const openstudio::path& p) : m_impl(std::shared_ptr<detail::CSVFile_Impl>(new detail::CSVFile_Impl(p))) {}

CSVFile::CSVFile(std::shared_ptr<detail::CSVFile_Impl> impl) : m_impl(impl) {}

CSVFile CSVFile::clone() const {
  return getImpl<detail::CSVFile_Impl>()->clone();
}

boost::optional<CSVFile> CSVFile::load(const std::string& s) {
  boost::optional<CSVFile> result;
  try {
    result = CSVFile(s);
  } catch (const std::exception&) {
  }
  return result;
}

boost::optional<CSVFile> CSVFile::load(const openstudio::path& p) {
  boost::optional<CSVFile> result;
  try {
    result = CSVFile(p);
  } catch (const std::exception&) {
  }
  return result;
}

std::string CSVFile::string() const {
  return getImpl<detail::CSVFile_Impl>()->string();
}

bool CSVFile::save() const {
  return getImpl<detail::CSVFile_Impl>()->save();
}

bool CSVFile::saveAs(const openstudio::path& p) {
  return getImpl<detail::CSVFile_Impl>()->saveAs(p);
}

boost::optional<openstudio::path> CSVFile::path() const {
  return getImpl<detail::CSVFile_Impl>()->path();
}

bool CSVFile::setPath(const openstudio::path& path) {
  return getImpl<detail::CSVFile_Impl>()->setPath(path);
}

void CSVFile::resetPath() {
  getImpl<detail::CSVFile_Impl>()->resetPath();
}

unsigned CSVFile::numColumns() const {
  return getImpl<detail::CSVFile_Impl>()->numColumns();
}

unsigned CSVFile::numRows() const {
  return getImpl<detail::CSVFile_Impl>()->numRows();
}

std::vector<std::vector<Variant>> CSVFile::rows() const {
  return getImpl<detail::CSVFile_Impl>()->rows();
}

void CSVFile::addRow(const std::vector<Variant>& row) {
  getImpl<detail::CSVFile_Impl>()->addRow(row);
}

void CSVFile::setRows(const std::vector<std::vector<Variant>>& rows) {
  getImpl<detail::CSVFile_Impl>()->setRows(rows);
}

void CSVFile::clear() {
  getImpl<detail::CSVFile_Impl>()->clear();
}

unsigned CSVFile::addColumn(const std::vector<DateTime>& dateTimes) {
  return getImpl<detail::CSVFile_Impl>()->addColumn(dateTimes);
}

unsigned CSVFile::addColumn(const Vector& values) {
  return getImpl<detail::CSVFile_Impl>()->addColumn(values);
}

unsigned CSVFile::addColumn(const std::vector<double>& values) {
  return getImpl<detail::CSVFile_Impl>()->addColumn(values);
}

unsigned CSVFile::addColumn(const std::vector<std::string>& values) {
  return getImpl<detail::CSVFile_Impl>()->addColumn(values);
}

std::vector<DateTime> CSVFile::getColumnAsDateTimes(unsigned columnIndex) const {
  return getImpl<detail::CSVFile_Impl>()->getColumnAsDateTimes(columnIndex);
}

std::vector<double> CSVFile::getColumnAsDoubleVector(unsigned columnIndex) const {
  return getImpl<detail::CSVFile_Impl>()->getColumnAsDoubleVector(columnIndex);
}

std::vector<std::string> CSVFile::getColumnAsStringVector(unsigned columnIndex) const {
  return getImpl<detail::CSVFile_Impl>()->getColumnAsStringVector(columnIndex);
}

std::ostream& operator<<(std::ostream& os, const CSVFile& csvFile) {
  os << csvFile.string();
  return os;
}

}  // namespace openstudio
