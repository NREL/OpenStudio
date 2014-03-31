/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
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

#include "JobErrorsDisplay.hpp"

namespace openstudio {
namespace runmanager {

  JobErrorsDisplay::JobErrorsDisplay(QWidget *parent, Qt::WindowFlags flags, const JobErrors &joberror)
    : QDialog(parent, flags)
  {
    ui.setupUi(this);

    std::string s;

    const std::vector<std::string> &warnings = joberror.warnings();
    const std::vector<std::string> &errors = joberror.errors();

    if (!warnings.empty())
    {
      s += "<h1>Warnings</h1>\n";
      s += "<ul>";
      for (size_t i = 0; i < warnings.size(); ++i)
      {
        s += "<li>" + warnings[i] + "</li>";
      }
      s += "</ul>";
    }

    if (!errors.empty())
    {
      s += "<h1>Errors</h1>\n";
      s += "<ul>";
      for (size_t i = 0; i < errors.size(); ++i)
      {
        s += "<li>" + errors[i] + "</li>";
      }
      s += "</ul>";
    }

    setWindowTitle("Job Warnings and Errors");
    ui.textEdit->setHtml(toQString(s));

  }

  JobErrorsDisplay::~JobErrorsDisplay()
  {
  }


} // runmanager
} // openstudio


