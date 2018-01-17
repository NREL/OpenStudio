/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "./LibraryDialog.hpp"
#include "../utilities/core/Path.hpp"
#include <QVBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QFileDialog>
#include <QDir>

#include <iostream>


namespace openstudio {

LibraryDialog::LibraryDialog(std::vector<openstudio::path> paths, std::vector<openstudio::path> defaultPaths)
  : QDialog(), m_defaultPaths(defaultPaths) 
{
  auto mainLayout = new QVBoxLayout();
  setLayout(mainLayout);

  auto title = new QLabel("Change Default Libraries");
  title->setObjectName("H1");
  mainLayout->addWidget(title);

  m_list = new QListWidget();
  m_list->setTextElideMode(Qt::ElideLeft);
  m_list->setMinimumWidth(600);
  m_list->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  m_list->setFocusPolicy(Qt::NoFocus);
  mainLayout->addWidget(m_list);

  auto addRemoveLayout = new QHBoxLayout();
  mainLayout->addLayout(addRemoveLayout);

  auto add = new QPushButton("Add");
  addRemoveLayout->addWidget(add,0);
  connect(add,&QPushButton::clicked,this,&LibraryDialog::onAdd);

  auto remove = new QPushButton("Remove");
  addRemoveLayout->addWidget(remove,0);
  connect(remove,&QPushButton::clicked,this,&LibraryDialog::onRemove);

  addRemoveLayout->addStretch(1);

  auto restore = new QPushButton("Restore Defaults");
  addRemoveLayout->addWidget(restore,0,Qt::AlignRight);
  connect(restore,&QPushButton::clicked,this,&LibraryDialog::onRestore);

  auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                   | QDialogButtonBox::Cancel);
  mainLayout->addWidget(buttonBox);

  connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
  connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

  for( auto path : paths ) {
    m_list->addItem(QString::fromStdString(toString(path)));
  }
}

void LibraryDialog::onAdd() {
  auto fileName = QFileDialog::getOpenFileName(nullptr,
      tr("Select OpenStudio Library"), QDir::homePath(), tr("OpenStudio Files (*.osm)"));

  if( ! fileName.isNull() ) {
    m_list->addItem(fileName);
  }
}

void LibraryDialog::onRemove() {
  for( auto item : m_list->selectedItems() ) {
    delete item;
  }
}

void LibraryDialog::onRestore() {
  m_list->clear();

  for( const auto path : m_defaultPaths ) {
    m_list->addItem(QString::fromStdString(toString(path)));
  }
}

std::vector<openstudio::path> LibraryDialog::paths() const {
  std::vector<openstudio::path> result;

  for ( int i = 0; i != m_list->count(); ++i ) {
    auto stringValue = m_list->item(i)->text();
    result.push_back(toPath(stringValue));
  }

  return result;
}

} // openstudio

