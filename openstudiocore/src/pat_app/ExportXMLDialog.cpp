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
  vLayout->setContentsMargins(QMargins(0,0,0,0));

  auto label = new QLabel;
  label->setText("EDA Baseline:");
  label->setObjectName("H2");
  vLayout->addWidget(label);
  m_edaBaselineComboBox = new QComboBox(this);
  //make all alternatives available in the dropdown box
  m_edaBaselineComboBox->addItem(QString("No EDA Baseline"));
  for ( const std::string & alternativeName : alternatives) {
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
  for ( const std::string & alternativeName : alternatives) {
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
  for ( const std::string & alternativeName : alternatives) {
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


} // openstudio
