/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "ExportXMLDialog.hpp"

#include "../utilities/core/Assert.hpp"

#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QGroupBox>

namespace openstudio {
  namespace pat {

    ExportXMLDialog::ExportXMLDialog(const std::vector<std::string>& alternatives, QWidget* parent)
      : OSDialog(false, parent)
    {
      this->setWindowTitle("Define Baselines");

      QPushButton* cancelButton = this->cancelButton();
      cancelButton->setText("Cancel");
      cancelButton->setMinimumHeight(34);
      cancelButton->setDefault(true);

      QPushButton* okButton = this->okButton();
      okButton->setText("Ok");
      okButton->setMinimumHeight(34);

      auto vLayout = new QVBoxLayout;
      vLayout->setContentsMargins(QMargins(0, 0, 0, 0));

      auto label = new QLabel;
      label->setText("EDA Baseline:");
      label->setObjectName("H2");
      vLayout->addWidget(label);
      m_edaBaselineComboBox = new QComboBox(this);
      //make all alternatives available in the dropdown box
      m_edaBaselineComboBox->addItem(QString("No EDA Baseline"));
      for (const std::string & alternativeName : alternatives) {
        m_edaBaselineComboBox->addItem(toQString(alternativeName));
      }
      m_edaBaselineComboBox->setCurrentIndex(0);
      vLayout->addWidget(m_edaBaselineComboBox);
      vLayout->addSpacing(10);

      label = new QLabel;
      label->setText("Proposed (Cost) Baseline:");
      label->setObjectName("H2");
      vLayout->addWidget(label);
      m_proposedBaselineComboBox = new QComboBox(this);
      m_proposedBaselineComboBox->addItem(QString("No Proposed Baseline"));
      //make all alternatives available in the dropdown box
      for (const std::string & alternativeName : alternatives) {
        m_proposedBaselineComboBox->addItem(toQString(alternativeName));
      }
      m_proposedBaselineComboBox->setCurrentIndex(0);
      vLayout->addWidget(m_proposedBaselineComboBox);
      vLayout->addSpacing(10);

      label = new QLabel;
      label->setText("Certification (LEED, etc) Baseline:");
      label->setObjectName("H2");
      vLayout->addWidget(label);
      m_certificationBaselineComboBox = new QComboBox(this);
      //make all alternatives available in the dropdown box
      m_certificationBaselineComboBox->addItem(QString("No Certification Baseline"));
      for (const std::string & alternativeName : alternatives) {
        m_certificationBaselineComboBox->addItem(toQString(alternativeName));
      }
      m_certificationBaselineComboBox->setCurrentIndex(0);
      vLayout->addWidget(m_certificationBaselineComboBox);
      vLayout->addSpacing(10);

      auto hLayout = new QHBoxLayout;
      hLayout->addStretch();
      vLayout->addLayout(hLayout);

      QBoxLayout* upperLayout = this->upperLayout();
      upperLayout->addLayout(vLayout);
      upperLayout->addStretch();

      this->setWindowModality(Qt::ApplicationModal);
      //this->setSizeGripEnabled(true);

#ifdef Q_OS_MAC
      setWindowFlags(Qt::FramelessWindowHint);
#else
      setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
#endif


    }

    ExportXMLDialog::~ExportXMLDialog()
    {
    }


    std::string ExportXMLDialog::getEDABaseline()
    {
      return m_edaBaselineComboBox->currentText().toStdString();

    }

    std::string ExportXMLDialog::getProposedBaseline()
    {
      return m_proposedBaselineComboBox->currentText().toStdString();

    }

    std::string ExportXMLDialog::getCertificationBaseline()
    {
      return m_certificationBaselineComboBox->currentText().toStdString();

    }

} //  pat
} // openstudio
