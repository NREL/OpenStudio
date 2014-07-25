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
  bool connected;
  connected = connect(mGroupEdit, SIGNAL(textEdited(const QString&)),
    this, SLOT(on_groupTextEdited(const QString&)));
  OS_ASSERT(connected);

  connected = connect(mObjectEdit, SIGNAL(textEdited(const QString&)),
    this, SLOT(on_objectTextEdited(const QString&)));
  OS_ASSERT(connected);

  connected = connect(mGroupList, SIGNAL(itemSelectionChanged()),
    this, SLOT(on_groupItemSelectionChanged()));
  OS_ASSERT(connected);

  connected = connect(mObjectList, SIGNAL(itemSelectionChanged()),
    this, SLOT(on_objectItemSelectionChanged()));
  OS_ASSERT(connected);
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
