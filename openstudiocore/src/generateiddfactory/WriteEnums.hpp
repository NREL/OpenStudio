/**********************************************************************
*  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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
#ifndef GENERATEIDDFACTORY_WRITEENUMS_HPP
#define GENERATEIDDFACTORY_WRITEENUMS_HPP

#include <iostream>
#include <iomanip>
#include <cmath>
#include <sstream>
#include <exception>

namespace openstudio {

template<typename Container>
void writeDomain(std::ostream &t_os, const std::string &t_name, const Container &t_values, bool in_class)
{
  if (in_class)
  {
    t_os << 
      "  enum domain " << std::endl;
  } else {
    t_os << 
      "  enum " << t_name << "::domain : int" << std::endl;
  }

  t_os << 
  "  {" << std::endl;

  for (const auto &val : t_values)
  {
    t_os << val.first << ", ";
  }
  t_os <<
  "  };" << std::endl;
}

template<typename Container>
void writeBuildStringVec(std::ostream &t_os, const std::string &t_name, const Container &t_values, bool in_class)
{
  if (in_class)
  {
    t_os << "    static VecType buildStringVec(bool isd)" << std::endl;
  } else {
    t_os << "    " << t_name << "::VecType " << t_name << "::buildStringVec(bool isd)" << std::endl;
  }

  t_os << 
  "    {" << std::endl <<
  "      struct evalue" << std::endl <<
  "      {" << std::endl <<
  "        int value; const char *name; const char *description;" << std::endl <<
  "      };" << std::endl <<
  "      const evalue a[] =" << std::endl <<
  "      {" << std::endl;

  for (const auto &val : t_values)
  {
    t_os << "{ " << t_name << "::" << val.first << ", \"" << val.first << "\", \"" << val.second << "\"}," << std::endl;
  }

  t_os << 
  "        { 0,0,0 }" << std::endl <<
  "      };" << std::endl <<
 
  "      VecType v;" << std::endl <<
  "      int i = 0;" << std::endl <<
  "      while (!(a[i].value == 0 && a[i].name == 0 && a[i].description == 0))" << std::endl <<
  "      {" << std::endl <<
  "        if (isd)" << std::endl <<
  "        {" << std::endl <<
  "          std::string description = a[i].description; " << std::endl <<
  "          if (!description.empty())" << std::endl <<
  "          {" << std::endl <<
  "            v.push_back(PT(description, a[i].value));" << std::endl <<
  "          }" << std::endl <<
  "        } else {" << std::endl <<
  "          v.push_back(PT(a[i].name, a[i].value));" << std::endl <<
  "        }" << std::endl <<
  "        ++i;" << std::endl <<
  "      }" << std::endl <<
  "      return v;" << std::endl <<
  "    }" << std::endl;
}


template<typename Container>
void writeEnumFast(std::ostream &t_os, const std::string &t_name, const Container &t_values)
{
  t_os <<
  "#ifdef SWIG " << std::endl <<
  "  OPENSTUDIO_ENUM(" << t_name << ", )" << std::endl <<
  "#else" << std::endl <<
  "class " << t_name << ": public ::EnumBase<" << t_name << "> {" << std::endl <<
  " public: " << std::endl;

  writeDomain(t_os, t_name, t_values, true);

  t_os <<
  "  " << t_name << "()" << std::endl <<
  "   : EnumBase<" << t_name << ">(" << t_values.front().first << ") {} " << std::endl <<
  "  " << t_name << "(const std::string &t_name) " << std::endl <<
  "   : EnumBase<" << t_name << ">(t_name) {} " << std::endl <<
  "  " << t_name << "(int t_value) " << std::endl <<
  "   : EnumBase<" << t_name << ">(t_value) {} " << std::endl <<
  "  static std::string enumName() " << std::endl <<
  "  { return \"" << t_name << "\"; }" << std::endl <<
  "  domain value() const { return static_cast<domain>(EnumBase<" << t_name << ">::value()); }" << std::endl <<
 
  "   private:" << std::endl <<
  "    friend class EnumBase<" << t_name << ">;" << std::endl <<
  "    typedef std::pair<std::string, int> PT;" << std::endl <<
  "    typedef std::vector<PT> VecType;" << std::endl;

  writeBuildStringVec(t_os, t_name, t_values, true);

  t_os <<
  "  " << std::endl <<
//  "    friend class boost::serialization::access;" << std::endl <<
 
//  "    template<class Archive> void serialize(Archive & ar, const unsigned int version)" << std::endl <<
//  "    {" << std::endl <<
//  "      ar & boost::serialization::make_nvp(\"value\", m_value);" << std::endl <<
//  "    }" << std::endl <<
  "  };" << std::endl <<
 
  "  inline std::ostream &operator<<(std::ostream &os, const " << t_name << " &e)" << std::endl <<
  "  {" << std::endl <<
  "    return os << e.valueName() << \"(\" << e.value() << \")\";" << std::endl <<
  "  }" << std::endl <<
 
  "  typedef boost::optional<" << t_name << "> Optional" << t_name << " ;" << std::endl <<
  "#endif";
}

template<typename Container>
void writeEnum(std::ostream &t_os, const std::string &t_name, const Container &t_values)
{
  // write IddObjectType enum. is very large, so split into 7 groups.
  unsigned groupSize = static_cast<unsigned>(std::ceil(static_cast<double>(t_values.size())/7.0));
  unsigned n = 0; // number of objects written so far--will start with Catchall and UserCustom
  t_os
    << "OPENSTUDIO_ENUM7( " << t_name << " ," << std::endl;

  for (const auto &val : t_values)
  {
    if (n > 0 && (n % groupSize) == 0) {
      t_os << "  ," << std::endl;
    }

    // writes the enum value (name and description)
    t_os
      << "  ((" << val.first << ")";

    if (!val.second.empty()) {
      t_os << "(" << val.second << ")";
    }

    t_os << ")" << std::endl;
    ++n;
  }

  t_os << ");" << std::endl;
}

} // openstudio

#endif // GENERATEIDDFACTORY_WRITEENUMS_HPP
