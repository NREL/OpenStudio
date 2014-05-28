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

#ifndef PATAPP_EXPORTXMLDIALOG_HPP
#define PATAPP_EXPORTXMLDIALOG_HPP

#include "../shared_gui_components/OSDialog.hpp"

#include "../utilities/bcl/BCLMeasure.hpp"

class QLineEdit;
class QTextEdit;
class QLabel;
class QComboBox;
class QRadioButton;

namespace openstudio{

class ExportXMLDialog : public OSDialog
{
  Q_OBJECT

  public:

    /// Constructor of the dialog to create a new BCL Measure
    ExportXMLDialog(const std::vector<std::string>& alternatives, QWidget* parent = nullptr);

    virtual ~ExportXMLDialog();

    /// Returns the name of the EDA baseline model
    std::string getEDABaseline();

    /// Returns the name of the proposed baseline model
    std::string getProposedBaseline();

    /// Returns the name of the certification baseline model
    std::string getCertificationBaseline();

  private:

    QComboBox* m_edaBaselineComboBox;
    QComboBox* m_proposedBaselineComboBox;
    QComboBox* m_certificationBaselineComboBox;

    boost::optional<BCLMeasure> m_bclMeasureToCopy;
};


} // openstudio

#endif // PATAPP_EXPORTXMLDIALOG_HPP
