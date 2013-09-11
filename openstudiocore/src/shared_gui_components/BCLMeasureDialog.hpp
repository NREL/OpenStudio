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

#ifndef OPENSTUDIO_BCLMEASUREDIALOG_H
#define OPENSTUDIO_BCLMEASUREDIALOG_H

#include <shared_gui_components/OSDialog.hpp>

#include <utilities/bcl/BCLMeasure.hpp>

class QLineEdit;
class QTextEdit;
class QLabel;
class QComboBox;
class QRadioButton;

namespace openstudio{

class BCLMeasureDialog : public OSDialog
{
  Q_OBJECT

  public:

    /// Constructor of the dialog to create a new BCL Measure
    BCLMeasureDialog(QWidget* parent = 0);

    /// Constructor of the dialog to create a copy of an existing BCL Measure
    BCLMeasureDialog(const BCLMeasure& bclMeasure, QWidget* parent = 0);

    virtual ~BCLMeasureDialog();

    /// After calling exec() and getting appropriate return code
    /// Call createMeasure to create the new measure
    boost::optional<openstudio::BCLMeasure> createMeasure();

  private slots:

    void nameChanged(const QString& newName);

    void firstLevelTaxonomyChanged(const QString& newName);

  private:

    void init();

    QLineEdit* m_nameLineEdit;
    QLabel* m_classNameLabel;
    QTextEdit* m_descriptionTextEdit;
    QTextEdit* m_modelerDescriptionTextEdit;
    QComboBox* m_measureTypeComboBox;
    QComboBox* m_taxonomyFirstLevelComboBox;
    QComboBox* m_taxonomySecondLevelComboBox;
    QRadioButton* m_usesSketchUpAPI;

    boost::optional<BCLMeasure> m_bclMeasureToCopy;
};


} // openstudio

#endif // OPENSTUDIO_BCLMEASUREDIALOG_H
