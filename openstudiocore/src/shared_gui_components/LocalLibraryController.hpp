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

#ifndef SHAREDGUICOMPONENTS_LOCALLIBRARYCONTROLLER_HPP
#define SHAREDGUICOMPONENTS_LOCALLIBRARYCONTROLLER_HPP

#include "OSListView.hpp"
#include "OSListController.hpp"
#include "OSCollapsibleView.hpp"
#include "HeaderViews.hpp"
#include "LocalLibrary.hpp"
#include "BaseApp.hpp"

#include "../utilities/bcl/BCLMeasure.hpp"

#include <QHBoxLayout>
#include <QLabel>
#include <QObject>
#include <QPointer>
#include <QSettings>
#include <QSharedPointer>
#include <QVariant>

namespace openstudio{

class OSListView;
class OSDragPixmapData;
  

class LocalLibraryView;
class LibraryTypeListController;
class LibraryGroupListController;
class LibrarySubGroupListController;
class LibraryListController;
class LibraryItem;

// TODO: Perhaps generalize LibraryGroupItem, LibrarySubGroupItem, LibraryItem and related classes into one tree like set of class.

class LocalLibraryController : public QObject
{
  Q_OBJECT

  friend class LibraryListController;

  public:


  QPointer<LocalLibraryView> localLibraryView;

  LocalLibraryController(openstudio::BaseApp *t_app, bool onlyShowModelMeasures = false);

  virtual ~LocalLibraryController();

  // Returns the selected item from either my measures or bcl measures.  Returns NULL if there is no selection.
  QPointer<LibraryItem> selectedItem() const;

  void reset();

  public slots:

  void showMeasures();

  void showMyMeasuresFolder();

  private slots:
  void addMeasure();
  void duplicateSelectedMeasure();
  void downloadUpdatedBCLMeasures();
  void openBclDlg();

  private:
  REGISTER_LOGGER("openstudio.LocalLibraryController");

  BaseApp *m_app;

  QPointer<OSListView> libraryView;

  QSharedPointer<LibraryTypeListController> createLibraryListController(const QDomDocument & taxonomy,LocalLibrary::LibrarySource source);

  QSettings m_settings;

  bool m_onlyShowModelMeasures;

};

class LibraryTypeItem : public OSListItem
{
  Q_OBJECT

  public:

  LibraryTypeItem(const QString & name);
  virtual ~LibraryTypeItem() {}

  QString name() const { return m_name; }

  QSharedPointer<LibraryGroupListController> libraryGroupListController() const { return m_libraryGroupListController; }

  private:

  QString m_name;

  QSharedPointer<LibraryGroupListController> m_libraryGroupListController;
};

class LibraryTypeItemDelegate : public OSItemDelegate
{
  QWidget * view(QSharedPointer<OSListItem> dataSource);

  public:
  LibraryTypeItemDelegate(BaseApp *t_app);
  virtual ~LibraryTypeItemDelegate() {}

  private:
  BaseApp *m_app;
};

class LibraryTypeListController : public OSListController
{
  Q_OBJECT;

  public:
  virtual ~LibraryTypeListController() {}

  QSharedPointer<OSListItem> itemAt(int i);

  int count();

  void addItem(QSharedPointer<OSListItem> item);

  void reset();

  private:

  QList<QSharedPointer<LibraryTypeItem> > m_items;
};

class LibraryGroupItem : public OSListItem
{
  Q_OBJECT

  public:

  LibraryGroupItem(const QString & name, BaseApp *t_app);
  virtual ~LibraryGroupItem() {}

  QString name() const { return m_name; }

  QSharedPointer<LibrarySubGroupListController> librarySubGroupListController() const { return m_librarySubGroupListController; }

  private:

  QString m_name;

  QSharedPointer<LibrarySubGroupListController> m_librarySubGroupListController;
};

class LibraryGroupItemDelegate : public OSItemDelegate
{
  QWidget * view(QSharedPointer<OSListItem> dataSource);

  public:
  LibraryGroupItemDelegate(BaseApp *t_app);
  virtual ~LibraryGroupItemDelegate() {}

  private:
  BaseApp *m_app;
};

class LibraryGroupListController : public OSListController
{
  Q_OBJECT;

  public:
  virtual ~LibraryGroupListController() {}

  QSharedPointer<OSListItem> itemAt(int i);

  int count();

  void addItem(QSharedPointer<OSListItem> item);

  void reset();

  private:

  QList<QSharedPointer<LibraryGroupItem> > m_items;
};

class LibrarySubGroupItem : public OSListItem
{
  Q_OBJECT

  public:

  LibrarySubGroupItem(const QString & name, const QString & taxonomyTag, 
                      LocalLibrary::LibrarySource source,
                      BaseApp *t_app,
                      bool onlyShowModelMeasures);

  virtual ~LibrarySubGroupItem() {}

  QString name() const { return m_name; }

  QSharedPointer<LibraryListController> libraryListController() const { return m_libraryListController; }

  signals:

  void libraryItemCountChanged(int count);

  private:

  BaseApp *m_app;
  QString m_name;

  QSharedPointer<LibraryListController> m_libraryListController;
};

class LibrarySubGroupItemDelegate : public OSItemDelegate
{
  QWidget * view(QSharedPointer<OSListItem> dataSource);

  public:
  LibrarySubGroupItemDelegate(BaseApp *t_app);
  virtual ~LibrarySubGroupItemDelegate() {}

  private:
  BaseApp *m_app;
};

class LibrarySubGroupListController : public OSListController
{
  Q_OBJECT;

  public:
  LibrarySubGroupListController(BaseApp *t_app);
  virtual ~LibrarySubGroupListController() {}

  QSharedPointer<OSListItem> itemAt(int i);

  int count();

  void addItem(QSharedPointer<OSListItem> item);

  void reset();

  int libraryItemCount();

  signals:

  void libraryItemCountChanged(int libraryItemCount);

  private:
  BaseApp *m_app;
  QList<QSharedPointer<LibrarySubGroupItem> > m_items;
};

class LibraryItem : public OSListItem
{
  Q_OBJECT

  public:

  LibraryItem(const BCLMeasure & bclMeasure, LocalLibrary::LibrarySource source, BaseApp *t_app);

  virtual ~LibraryItem() {}

  Q_PROPERTY(QString name READ name)

  QString name() const { return QString::fromStdString(m_bclMeasure.name()); }

  UUID uuid() const { return m_bclMeasure.uuid(); }

  BCLMeasure m_bclMeasure;

  LocalLibrary::LibrarySource m_source;

  bool isAvailable() const { return m_available; }

  public slots:

  void dragItem(const OSDragPixmapData & data);

  private:

  bool m_available;
  BaseApp *m_app;
  
};

class LibraryItemDelegate : public OSItemDelegate
{
  Q_OBJECT;

  public:
  LibraryItemDelegate(BaseApp *t_app);
  virtual ~LibraryItemDelegate() {}
  QWidget * view(QSharedPointer<OSListItem> dataSource);


  private slots:

  void selectedChanged();

  private:

  BaseApp *m_app;
};

class LibraryListController : public OSListController
{
  Q_OBJECT;

  public:

  LibraryListController(const QString & taxonomyTag, 
                        LocalLibrary::LibrarySource source,
                        BaseApp *t_app,
                        bool onlyShowModelMeasures);

  virtual ~LibraryListController() {}

  QSharedPointer<OSListItem> itemAt(int i);

  int count();

  void reset();

  signals:

  void countChanged(int count);


  private:

  void createItems();

  BaseApp *m_app;

  QString m_taxonomyTag;

  LocalLibrary::LibrarySource m_source;

  std::vector<QSharedPointer<LibraryItem> > m_items;

  bool m_onlyShowModelMeasures;
};


} // openstudio

#endif // SHAREDGUICOMPONENTS_LOCALLIBRARYCONTROLLER_HPP

