/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef GENERATEIDDFACTORY_WRITEENUMS_HPP
#define GENERATEIDDFACTORY_WRITEENUMS_HPP

#include <iostream>
#include <iomanip>
#include <cmath>
#include <sstream>
#include <exception>

namespace openstudio {

template <typename Container>
void writeDomain(std::ostream& t_os, const std::string& t_name, const Container& t_values, bool in_class) {
  if (in_class) {
    t_os << "  enum domain " << '\n';
  } else {
    t_os << "  enum " << t_name << "::domain : int" << '\n';
  }

  t_os << "  {" << '\n';

  for (const auto& val : t_values) {
    t_os << val.first << ", ";
  }
  t_os << "  };" << '\n';
}

template <typename Container>
void writeBuildStringVec(std::ostream& t_os, const std::string& t_name, const Container& t_values, bool in_class) {
  if (in_class) {
    t_os << "    static VecType buildStringVec(bool isd)" << '\n';
  } else {
    t_os << "    " << t_name << "::VecType " << t_name << "::buildStringVec(bool isd)" << '\n';
  }

  t_os << "    {" << '\n'
       << "      struct evalue" << '\n'
       << "      {" << '\n'
       << "        int value; const char *name; const char *description;" << '\n'
       << "      };" << '\n'
       << "      const evalue a[] =" << '\n'
       << "      {" << '\n';

  for (const auto& val : t_values) {
    t_os << "{ " << t_name << "::" << val.first << ", \"" << val.first << "\", \"" << val.second << "\"}," << '\n';
  }

  t_os << "        { 0,0,0 }" << '\n'
       << "      };" << '\n'
       <<

    "      VecType v;" << '\n'
       << "      int i = 0;" << '\n'
       << "      while (!(a[i].value == 0 && a[i].name == 0 && a[i].description == 0))" << '\n'
       << "      {" << '\n'
       << "        if (isd)" << '\n'
       << "        {" << '\n'
       << "          std::string description = a[i].description; " << '\n'
       << "          if (!description.empty())" << '\n'
       << "          {" << '\n'
       << "            v.push_back(PT(description, a[i].value));" << '\n'
       << "          }" << '\n'
       << "        } else {" << '\n'
       << "          v.push_back(PT(a[i].name, a[i].value));" << '\n'
       << "        }" << '\n'
       << "        ++i;" << '\n'
       << "      }" << '\n'
       << "      return v;" << '\n'
       << "    }" << '\n';
}

template <typename Container>
void writeEnumFast(std::ostream& t_os, const std::string& t_name, const Container& t_values) {
  t_os << "#ifdef SWIG " << '\n'
       << "  OPENSTUDIO_ENUM(" << t_name << ", )" << '\n'
       << "#else" << '\n'
       << "class " << t_name << ": public ::EnumBase<" << t_name << "> {" << '\n'
       << " public: " << '\n';

  writeDomain(t_os, t_name, t_values, true);

  t_os << "  " << t_name << "()" << '\n'
       << "   : EnumBase<" << t_name << ">(" << t_values.front().first << ") {} " << '\n'
       << "  " << t_name << "(const std::string &t_name) " << '\n'
       << "   : EnumBase<" << t_name << ">(t_name) {} " << '\n'
       << "  " << t_name << "(int t_value) " << '\n'
       << "   : EnumBase<" << t_name << ">(t_value) {} " << '\n'
       << "  static std::string enumName() " << '\n'
       << "  { return \"" << t_name << "\"; }" << '\n'
       << "  domain value() const { return static_cast<domain>(EnumBase<" << t_name << ">::integer_value()); }" << '\n'
       <<

    "   private:" << '\n'
       << "    friend class EnumBase<" << t_name << ">;" << '\n'
       << "    typedef std::pair<std::string, int> PT;" << '\n'
       << "    typedef std::vector<PT> VecType;" << '\n';

  writeBuildStringVec(t_os, t_name, t_values, true);

  t_os << "  " << '\n'
       <<
    //  "    friend class boost::serialization::access;" << '\n' <<

    //  "    template<class Archive> void serialize(Archive & ar, const unsigned int version)" << '\n' <<
    //  "    {" << '\n' <<
    //  "      ar & boost::serialization::make_nvp(\"value\", m_value);" << '\n' <<
    //  "    }" << '\n' <<
    "  };" << '\n'
       <<

    "  inline std::ostream &operator<<(std::ostream &os, const " << t_name << " &e)" << '\n'
       << "  {" << '\n'
       << "    return os << e.valueName() << \"(\" << e.value() << \")\";" << '\n'
       << "  }" << '\n'
       <<

    "  typedef boost::optional<" << t_name << "> Optional" << t_name << " ;" << '\n'
       << "#endif";
}

template <typename Container>
void writeEnum(std::ostream& t_os, const std::string& t_name, const Container& t_values) {
  // write IddObjectType enum. is very large, so split into 7 groups.
  auto groupSize = static_cast<unsigned>(std::ceil(static_cast<double>(t_values.size()) / 7.0));
  unsigned n = 0;  // number of objects written so far--will start with Catchall and UserCustom
  t_os << "OPENSTUDIO_ENUM7( " << t_name << " ," << '\n';

  for (const auto& val : t_values) {
    if (n > 0 && (n % groupSize) == 0) {
      t_os << "  ," << '\n';
    }

    // writes the enum value (name and description)
    t_os << "  ((" << val.first << ")";

    if (!val.second.empty()) {
      t_os << "(" << val.second << ")";
    }

    t_os << ")" << '\n';
    ++n;
  }

  t_os << ");" << '\n';
}

}  // namespace openstudio

#endif  // GENERATEIDDFACTORY_WRITEENUMS_HPP
