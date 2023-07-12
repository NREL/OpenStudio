/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "PrjReader.hpp"
#include <iostream>
#include <stdlib.h>

#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/FilesystemHelpers.hpp"
#include "../../utilities/core/StringHelpers.hpp"

namespace openstudio {
namespace contam {

  Reader::Reader(openstudio::filesystem::ifstream& file) : m_stream(openstudio::filesystem::read_as_string(file)), m_lineNumber(0) {}

  Reader::Reader(const std::string& string, int starting) : m_lineNumber(starting) {
    m_stream.str(string);
  }

  double Reader::readDouble() {
    const auto string = readString();
    try {
      return openstudio::string_conversions::to<double>(string);
    } catch (const std::bad_cast&) {
      LOG_AND_THROW("Floating point (double) conversion error at line " << m_lineNumber << " for \"" << string << "\"");
    }
  }

  std::string Reader::readString() {
    while (true) {
      while (m_entries.empty()) {
        std::string input;
        std::getline(m_stream, input);
        LOG(Debug, "Line read: " << input);
        if (!m_stream) {
          LOG_AND_THROW("Failed to read input at line " << m_lineNumber);
        }
        m_lineNumber++;
        while (input[0] == '!') {
          std::getline(m_stream, input);
          LOG(Debug, "Line read: " << input);
          if (!m_stream) {
            LOG_AND_THROW("Failed to read input at line " << m_lineNumber);
          }
          m_lineNumber++;
        }

        std::vector<std::string> strs;
        boost::split(strs, input, boost::is_any_of(" "));

        m_entries.clear();
        for (const auto& s : strs) {
          std::string s2 = boost::replace_all_copy(s, " ", "");
          boost::replace_all(s2, "\r", "");
          boost::replace_all(s2, "\n", "");
          boost::replace_all(s2, "\t", "");
          if (!s2.empty()) {
            m_entries.push_back(s2);
          }
        }
      }
      std::string out = m_entries.front();
      m_entries.pop_front();
      if (out[0] == '!') {
        m_entries.clear();
      } else {
        LOG(Debug, "String return: " << out);
        return out;
      }
    }
  }

  int Reader::readInt() {
    std::string string = readString();
    int value = 0;
    try {
      value = std::stoi(string);
    } catch (const std::exception&) {
      LOG_AND_THROW("Integer conversion error at line " << m_lineNumber << " for \"" << string << "\"");
    }
    return value;
  }

  unsigned int Reader::readUInt() {
    std::string string = readString();
    int value = 0;
    try {
      value = std::stoul(string);
    } catch (const std::exception&) {
      LOG_AND_THROW("Unsigned Integer conversion error at line " << m_lineNumber << " for \"" << string << "\"");
    }
    return value;
  }

  std::string Reader::readLine() {
    /* Dump any other input */
    if (!m_entries.empty()) {
      m_entries.clear();
    }
    std::string input;
    std::getline(m_stream, input);
    LOG(Debug, "Line read: " << input);
    if (!m_stream) {
      LOG_AND_THROW("Failed to read input at line " << m_lineNumber);
    }
    m_lineNumber++;
    while (input[0] == '!') {
      std::getline(m_stream, input);
      LOG(Debug, "Line read: " << input);
      if (!m_stream) {
        LOG_AND_THROW("Failed to read input at line " << m_lineNumber);
      }
      m_lineNumber++;
    }
    return input;
  }

  void Reader::read999() {
    std::string input = readLine();
    if (!boost::starts_with(input, "-999")) {
      LOG_AND_THROW("Failed to read -999 at line " << m_lineNumber);
    }
  }

  void Reader::read999(std::string mesg) {
    std::string input = readLine();
    if (!boost::starts_with(input, "-999")) {
      LOG_AND_THROW(mesg << " at line " << m_lineNumber);
    }
  }

  void Reader::readEnd() {
    std::string input = readLine();
    if (!boost::starts_with(input, "* end project file.")) {
      LOG_AND_THROW("Failed to read file end at line " << m_lineNumber);
    }
  }

  void Reader::skipSection() {
    readSection();
  }

  std::string Reader::readSection() {
    std::string section;
    while (true) {
      std::string input;
      std::getline(m_stream, input);
      if (!m_stream) {
        LOG_AND_THROW("Failed to read input at line " << m_lineNumber);
      }
      m_lineNumber++;
      section += std::string(input + '\n');
      if (input.find("-999") == 0) {
        break;
      }
    }
    return section;
  }

  std::vector<int> Reader::readIntVector(bool terminated) {
    int n = readInt();
    std::vector<int> vector;
    for (int i = 0; i < n; i++) {
      vector.push_back(readInt());
    }
    if (terminated) {
      read999("Failed to find section termination");
    }
    return vector;
  }

  template <>
  int Reader::read<int>() {
    return readInt();
  }

  template <>
  unsigned int Reader::read<unsigned int>() {
    return readUInt();
  }

  template <>
  double Reader::read<double>() {
    return readDouble();
  }

  template <>
  std::string Reader::read<std::string>() {
    return readString();
  }

  template <>
  double Reader::readNumber<double>() {
    return readInt();
  }

  template <>
  std::string Reader::readNumber<std::string>() {
    std::string string = readString();
    if (!openstudio::contam::is_valid<double>(string)) {
      LOG_AND_THROW("Invalid number \"" << string << "\" on line " << m_lineNumber);
    }
    return string;
  }

}  // namespace contam
}  // namespace openstudio
