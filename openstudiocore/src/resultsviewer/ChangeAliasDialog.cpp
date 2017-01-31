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

#include "ChangeAliasDialog.hpp"
#include <QFile>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

namespace resultsviewer{

ChangeAliasDialog::ChangeAliasDialog(const QString& oldAlias, const QString& filename, QWidget* parent):QDialog(parent)
{
  m_newAlias = "";

  int boxWidth = fontMetrics().width(filename) + 20;
  QPalette p(palette());
  p.setColor(QPalette::Base, Qt::lightGray);    

  QLabel *fileLabel = new QLabel(tr("&EnergyPlus file:"));
  auto fileName = new QLineEdit(filename);
  fileLabel->setBuddy(fileName);
  fileName->setReadOnly(true);
  fileName->setPalette(p);
  fileName->setMinimumWidth(boxWidth);

  QLabel *oldAliasLabel = new QLabel(tr("&Old alias:"));
  auto oldAliasName = new QLineEdit(oldAlias);
  oldAliasLabel->setBuddy(oldAliasName);
  oldAliasName->setReadOnly(true);
  oldAliasName->setPalette(p);
  oldAliasName->setMinimumWidth(boxWidth);

  QLabel *newAliasLabel = new QLabel(tr("&New alias:"));
  m_newAliasName = new QLineEdit;
  newAliasLabel->setBuddy(m_newAliasName);
  m_newAliasName->setMinimumWidth(boxWidth);

  m_updateButton = new QPushButton(tr("Update"));
  m_updateButton->setDefault(true);
  m_updateButton->setEnabled(false);

  QPushButton *cancelButton = new QPushButton(tr("Cancel"));

  connect(m_updateButton, &QPushButton::clicked, this, &ChangeAliasDialog::updateClicked);
  connect(cancelButton, &QPushButton::clicked, this, &ChangeAliasDialog::close);
  connect(m_newAliasName, &QLineEdit::textChanged, this, &ChangeAliasDialog::enableUpdateButton);
// layout
  auto gridLayout = new QGridLayout;
  gridLayout->addWidget(fileLabel, 0, 0);
  gridLayout->addWidget(fileName, 0, 1);
  gridLayout->addWidget(oldAliasLabel, 1, 0);
  gridLayout->addWidget(oldAliasName, 1, 1);
  gridLayout->addWidget(newAliasLabel, 2, 0);
  gridLayout->addWidget(m_newAliasName, 2, 1);

  auto buttonLayout = new QHBoxLayout;
  buttonLayout->addStretch();
  buttonLayout->addWidget(m_updateButton);
  buttonLayout->addWidget(cancelButton);

  auto mainLayout = new QVBoxLayout;
  mainLayout->addLayout(gridLayout);
  mainLayout->addLayout(buttonLayout);

  setLayout(mainLayout);

  setWindowTitle(tr("Change alias"));
  setFixedHeight(sizeHint().height());
}

void ChangeAliasDialog::enableUpdateButton(const QString& alias)
{
  m_updateButton->setEnabled(!m_newAliasName->text().isEmpty());
}

void ChangeAliasDialog::updateClicked()
{
  m_newAlias = m_newAliasName->text();
  if (!m_newAlias.isEmpty()) accept();
}

};
