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

#ifndef SHAREDGUICOMPONENTS_BCLMEASUREDIALOG_HPP
#define SHAREDGUICOMPONENTS_BCLMEASUREDIALOG_HPP

#include "OSDialog.hpp"

#include "../utilities/bcl/BCLMeasure.hpp"

class QLineEdit;
class QTextEdit;
class QLabel;
class QComboBox;
class QRadioButton;
class QListWidget;

namespace openstudio{

class BCLMeasureDialog : public OSDialog
{
  Q_OBJECT

  public:

    /// Constructor of the dialog to create a new BCL Measure
    BCLMeasureDialog(QWidget* parent = nullptr);

    /// Constructor of the dialog to create a copy of an existing BCL Measure
    BCLMeasureDialog(const BCLMeasure& bclMeasure, QWidget* parent = nullptr);

    virtual ~BCLMeasureDialog();

    QSize sizeHint() const override;

    /// After calling exec() and getting appropriate return code
    /// Call createMeasure to create the new measure
    boost::optional<openstudio::BCLMeasure> createMeasure();

  private slots:

    void nameChanged(const QString& newName);

    void measureTypeChanged(const QString& newName);

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
    QListWidget* m_intendedSoftwareToolListWidget;
    QListWidget* m_intendedUseCaseListWidget;

    boost::optional<BCLMeasure> m_bclMeasureToCopy;
};


} // openstudio

#endif // SHAREDGUICOMPONENTS_BCLMEASUREDIALOG_HPP
