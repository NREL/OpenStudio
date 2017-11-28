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

#include "PageNavigator.hpp"

#include "../utilities/core/Assert.hpp"

#include <QBoxLayout>
#include <QButtonGroup>
#include <QPushButton>

namespace openstudio {

PageNavigator::PageNavigator(int numPages,
                             int numPagesListed,
                             QWidget * parent)
  : QWidget(parent),
  m_numPagesListed(numPagesListed),
  m_numPages(numPages),
  m_checkedTextColor(QColor(Qt::black)),
  m_uncheckedTextColor(QColor(104,104,104)),
  m_buttonGroup(nullptr),
  m_buttonLayout(nullptr)
{
  if(m_numPages < m_numPagesListed){
    m_numPagesListed = m_numPages;
  }

  createLayout();
}

PageNavigator::~PageNavigator()
{
}

void PageNavigator::createLayout()
{
  auto layout = new QHBoxLayout();
  layout->setContentsMargins(0,0,0,0);
  layout->setSpacing(2);

  setLayout(layout);

  m_buttonGroup = new QButtonGroup(this);
  connect(m_buttonGroup, static_cast<void (QButtonGroup::*)(QAbstractButton *)>(&QButtonGroup::buttonClicked), this, &PageNavigator::on_buttonClicked);

  QPushButton * button = nullptr;

  ///! button first page
  button = new QPushButton(this);
  button->setIcon(QIcon(":/shared_gui_components/images/fast_reverse.png"));
  button->setFixedSize(40,20);
  button->setCheckable(false);
  connect(button, &QPushButton::clicked, this, &PageNavigator::on_firstPage);
  layout->addWidget(button);

  ///! button previous page
  button = new QPushButton(this);
  button->setIcon(QIcon(":/shared_gui_components/images/reverse.png"));
  button->setFixedSize(40,20);
  button->setCheckable(false);
  connect(button, &QPushButton::clicked, this, &PageNavigator::on_previousPage);
  layout->addWidget(button);

  ///! buttons for pages listed
  m_buttonLayout = new QHBoxLayout();
  m_buttonLayout->setContentsMargins(0,0,0,0);
  m_buttonLayout->setSpacing(2);
  layout->addLayout(m_buttonLayout);
  makeNumberedButtons();

  ///! button next page
  button = new QPushButton(this);
  button->setIcon(QIcon(":/shared_gui_components/images/forward.png"));
  button->setFixedSize(40,20);
  button->setCheckable(false);
  connect(button, &QPushButton::clicked, this, &PageNavigator::on_nextPage);
  layout->addWidget(button);

  ///! button last page
  button = new QPushButton(this);
  button->setIcon(QIcon(":/shared_gui_components/images/fast_forward.png"));
  button->setFixedSize(40,20);
  button->setCheckable(false);
  connect(button, &QPushButton::clicked, this, &PageNavigator::on_lastPage);
  layout->addWidget(button);

}

void PageNavigator::makeNumberedButtons()
{
  for (QAbstractButton * button : m_buttonGroup->buttons().toVector().toStdVector()) {
    m_buttonGroup->removeButton(button);
    if(button){
      delete button;
      button = nullptr;
    }
  }

  int id = 0;
  QString num;
  for(int i = 0; i < m_numPagesListed; ++i)
  {
    auto button = new QPushButton();
    button->setText(num.setNum(i+1));
    button->setFixedSize(40,20);
    button->setCheckable(true);
    m_buttonLayout->addWidget(button);
    m_buttonGroup->addButton(button);
    m_buttonGroup->setId(button, id++);
  }
  if(m_buttonGroup->button(0)){
    m_buttonGroup->button(0)->setChecked(true);
  }
  OS_ASSERT(m_numPagesListed == m_buttonGroup->buttons().size());
}

void PageNavigator::setButtonPageNumbers(int lowestPageNum)
{
  OS_ASSERT(lowestPageNum > 0 && lowestPageNum <= m_numPages);
  QString num;
  for(int i = 0; i < m_numPagesListed; ++i){
    QAbstractButton * button = m_buttonGroup->button(i);
    button->setText(num.setNum(lowestPageNum + i));
    if(i == m_numPages) break;
  }
}

void PageNavigator::firstPage()
{
  on_firstPage(true);
}

void PageNavigator::setNumPages(int numPages)
{
  m_numPages = numPages;
  setNumPagesListed();
}

void PageNavigator::setNumPagesListed(int numPagesListed)
{
  m_numPagesListed = numPagesListed;

  // check that the correct number of pages are listed
  if(m_numPages < m_numPagesListed){
    m_numPagesListed = m_numPages;
  }

  // adjust the number of buttons used, if need be
  if(m_buttonGroup->buttons().size() != m_numPagesListed){
    makeNumberedButtons();
  }

}

///! Slots

void PageNavigator::on_buttonClicked(QAbstractButton * button)
{
  /// Get the new page of components (idx zero)
  emit getComponentsByPage(button->text().toInt() - 1);
}

void PageNavigator::on_firstPage(bool checked)
{
  // Verify that there are pages to work with
  if(m_numPagesListed == 0) return;

  /// for efficiency, check if the first page is already being displayed
  if(m_buttonGroup->checkedButton() && m_buttonGroup->checkedButton()->text().toInt() == 1) return;

  /// Get the page number held by the first button
  if(m_buttonGroup->button(0)){
    int firstPageNum = m_buttonGroup->button(0)->text().toInt();

    /// page num = 1, page idx = 0
    m_buttonGroup->button(0)->setChecked(true);

    /// for efficiency, check if the first page nums are already being displayed
    if(firstPageNum != 1){
      setButtonPageNumbers(1);
    }

    emit getComponentsByPage(0);
  }

}

void PageNavigator::on_previousPage(bool checked)
{
  // Verify that there are pages to work with
  if(m_numPagesListed == 0) return;

  /// If the first button of the group is selected...
  if(m_buttonGroup->checkedId() == 0){
    ///...and if it can still be decreased...
    if(m_buttonGroup->button(0)->text().toInt() > 1){
      /// ...subtract 1 from the page numbers listed...
      setButtonPageNumbers(m_buttonGroup->button(0)->text().toInt() - 1);
      /// ...and get the new page of components (idx zero)...
      emit getComponentsByPage(m_buttonGroup->checkedButton()->text().toInt() - 1);
    }
    /// ...and be done
    return;
  }

  /// If greater than the first button of the group is selected...
  if(m_buttonGroup->checkedId() > 0){
    /// ...decrease the selected button...
    m_buttonGroup->button(m_buttonGroup->checkedId() - 1)->setChecked(true);
    /// ...and get the new page of components (idx zero)...
    emit getComponentsByPage(m_buttonGroup->checkedButton()->text().toInt() - 1);
    /// ...and be done
    return;
  }

  /// You should never get here
  OS_ASSERT(false);
  return;
}

void PageNavigator::on_nextPage(bool checked)
{
  // Verify that there are pages to work with
  if(m_numPagesListed == 0) return;

  /// If the last button of the group is selected...
  if(m_buttonGroup->checkedId() == m_numPagesListed - 1){
    ///...and if it can still be increased...
    if(m_buttonGroup->button(m_numPagesListed - 1)->text().toInt() < m_numPages){
      /// ...add 1 to the page numbers listed...
      setButtonPageNumbers(m_buttonGroup->button(1)->text().toInt());
      /// ...and get the new page of components (idx zero)...
      emit getComponentsByPage(m_buttonGroup->checkedButton()->text().toInt() - 1);
    }
    /// ...and be done
    return;
  }

  /// If less than the last button of the group is selected...
  if(m_buttonGroup->checkedId() < m_numPagesListed - 1){
    /// ...advance the selected button...
    m_buttonGroup->button(m_buttonGroup->checkedId() + 1)->setChecked(true);
    /// ...and get the new page of components (idx zero)...
    emit getComponentsByPage(m_buttonGroup->checkedButton()->text().toInt() - 1);
    /// ...and be done
    return;
  }

  /// You should never get here
  OS_ASSERT(false);
  return;
}

void PageNavigator::on_lastPage(bool checked)
{
  // Verify that there are pages to work with
  if(m_numPagesListed == 0) return;

  /// for efficiency, check if the last page is already being displayed
  if(m_buttonGroup->checkedButton() && m_buttonGroup->checkedButton()->text().toInt() == m_numPages) return;

  /// Get the page number held by the last button
  if(m_buttonGroup->button(m_numPagesListed - 1)){
    int lastPageNum = m_buttonGroup->button(m_numPagesListed - 1)->text().toInt();

    /// page num = m_numPages, page idx = m_numPages - 1
    m_buttonGroup->button(m_numPagesListed - 1)->setChecked(true);

    /// for efficiency, check if the last page nums are already being displayed
    if(lastPageNum != m_numPages){
      setButtonPageNumbers(m_numPages - m_numPagesListed + 1);
    }

    emit getComponentsByPage(m_numPages - 1);
  }

}

} // namespace openstudio
