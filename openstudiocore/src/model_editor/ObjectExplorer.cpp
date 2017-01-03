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

#include <QBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>

#include "ListWidget.hpp"

#include "ObjectExplorer.hpp"

#include "../utilities/core/Assert.hpp"

namespace modeleditor
{

ObjectExplorer::ObjectExplorer(openstudio::IddFile& iddFile, QWidget * parent)
  : QWidget(parent),
  mGroupEdit(nullptr),
  mObjectEdit(nullptr),
  mGroupList(nullptr),
  mObjectList(nullptr),
  mIddFile(iddFile)
{
  createWidgets();
  connectSignalsAndSlots();
  createLayout();
}

ObjectExplorer::~ObjectExplorer()
{
}

void ObjectExplorer::createWidgets()
{
  mGroupEdit = new QLineEdit(this);
  mObjectEdit = new QLineEdit(this);
  mGroupList = new QListWidget(this);
  mObjectList = new ListWidget(this);

  mGroupList->setEditTriggers(QAbstractItemView::NoEditTriggers);
  mGroupList->setSelectionMode(QAbstractItemView::ExtendedSelection);
  mGroupList->setDragEnabled(false);
  mGroupList->setSortingEnabled(true);

  mObjectList->setEditTriggers(QAbstractItemView::NoEditTriggers);
  mObjectList->setSelectionMode(QAbstractItemView::ExtendedSelection);
  mObjectList->setDragEnabled(true);
  mObjectList->setSortingEnabled(true);
}

void ObjectExplorer::connectSignalsAndSlots()
{
  connect(mGroupEdit, &QLineEdit::textEdited, this, &ObjectExplorer::on_groupTextEdited);

  connect(mObjectEdit, &QLineEdit::textEdited, this, &ObjectExplorer::on_objectTextEdited);

  connect(mGroupList, &QListWidget::itemSelectionChanged, this, &ObjectExplorer::on_groupItemSelectionChanged);

  connect(mObjectList, &ListWidget::itemSelectionChanged, this, &ObjectExplorer::on_objectItemSelectionChanged);
}

void ObjectExplorer::createLayout()
{
  QLabel * groupLabel = new QLabel("Search groups:", this);
  groupLabel->setBuddy(mGroupEdit);

  QLabel * objectLabel = new QLabel("Search classes:", this);
  objectLabel->setBuddy(mObjectEdit);

  auto groupEditLayout = new QHBoxLayout();
  groupEditLayout->addWidget(groupLabel);
  groupEditLayout->addWidget(mGroupEdit);

  auto objectEditLayout = new QHBoxLayout();
  objectEditLayout->addWidget(objectLabel);
  objectEditLayout->addWidget(mObjectEdit);

  auto groupLayout = new QVBoxLayout();
  groupLayout->addLayout(groupEditLayout);
  groupLayout->addWidget(mGroupList);
  QGroupBox * groupBox = new QGroupBox(tr("Groups"));
  groupBox->setLayout(groupLayout);

  auto objectLayout = new QVBoxLayout();
  objectLayout->addLayout(objectEditLayout);
  objectLayout->addWidget(mObjectList);
  QGroupBox * objectBox = new QGroupBox(tr("Classes"));
  objectBox->setLayout(objectLayout);

  auto hLayout = new QHBoxLayout();
  hLayout->addWidget(groupBox);
  hLayout->addWidget(objectBox);

  setLayout(hLayout);
}

void ObjectExplorer::updateIddFile()
{
  ///! update displayed info
  if(!mObjectList || !mGroupList){
    return;
  }

  mObjectList->clear();
  mGroupList->clear();

  std::string group;
  std::vector<std::string> groups = mIddFile.groups();
  openstudio::IddObject object;
  std::vector<openstudio::IddObject> objects;

  QListWidgetItem * newItem = nullptr;

  for(unsigned i=0 ; i<groups.size(); i++){
    group = groups.at(i);
    if(group.empty()) continue;
    objects = mIddFile.getObjectsInGroup(group);
    newItem = new QListWidgetItem();
    newItem->setText(tr(group.c_str()));
    mGroupList->addItem(newItem);
    for(unsigned j=0 ; j<objects.size(); j++){
      object = objects.at(j);
      newItem = new QListWidgetItem();
      newItem->setText(tr(object.name().c_str()));
      newItem->setWhatsThis(tr(group.c_str()));
      mObjectList->addItem(newItem);
    }
  }
  mGroupList->sortItems();
  mObjectList->sortItems();
}

void ObjectExplorer::groupTextEdited(const QString& text)
{
  for(int i=0; i<mGroupList->count(); i++){
    QListWidgetItem * item = mGroupList->item(i);
    item->setHidden(!item->text().contains(text, Qt::CaseInsensitive));
  }
}

void ObjectExplorer::objectTextEdited(const QString& text)
{
  QListWidgetItem * item = nullptr;
  for(int i=0; i<mObjectList->count(); i++){
    item = mObjectList->item(i);
    item->setHidden(!item->text().contains(text, Qt::CaseInsensitive));
  }

  QList<QListWidgetItem *> items;
  items = mGroupList->selectedItems();
  if(items.size()){
    for(int i=0; i<items.size(); i++){
      item = items.at(i);
    }
  }
  else{
    for(int i=0; i<mGroupList->count(); i++){
      item = mGroupList->item(i);
      if(!item->isHidden()){
        items.push_back(item);
      }
    }
  }

  bool match = false;
  for(int i=0; i<mObjectList->count(); i++){
    if(!mObjectList->item(i)->isHidden()){
      for(int j=0; j<items.size(); j++){
        match = false;
        if(items.at(j)->text() == mObjectList->item(i)->whatsThis()){
          match = true;
          break;
        }
      }
      if(!match){
        mObjectList->item(i)->setHidden(true);
      }
    }
  }
}

void ObjectExplorer::groupItemSelectionChanged()
{
  for(int i=0; i<mObjectList->count(); i++){
    mObjectList->item(i)->setHidden(true);
  }

  QString groupName;
  QListWidgetItem * item = nullptr;
  QList<QListWidgetItem *> items;
  items = mGroupList->selectedItems();
  for(int i=0; i<items.size(); i++)
  {
    groupName = items.at(i)->text();
    for(int j=0; j<mObjectList->count(); j++){
      item = mObjectList->item(j);
      if(item->whatsThis() == groupName){
        item->setHidden(false);
      }
    }
  }
}

void ObjectExplorer::objectItemSelectionChanged()
{
  //QListWidgetItem * item = NULL;
  QList<QListWidgetItem *> items;
  items = mObjectList->selectedItems();
  for(int i=0; i<items.size(); i++)
  {
    ///! TODO do something
  }
}

///! Slots
void ObjectExplorer::on_groupTextEdited(const QString& text)
{
  groupTextEdited(text);
  objectTextEdited(mObjectEdit->text());
}

void ObjectExplorer::on_objectTextEdited(const QString& text)
{
  objectTextEdited(text);
}

void ObjectExplorer::on_groupItemSelectionChanged()
{
  groupItemSelectionChanged();
}

void ObjectExplorer::on_objectItemSelectionChanged()
{
  objectItemSelectionChanged();
}

} // namespace modeleditor
