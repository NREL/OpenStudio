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

#ifndef OPENSTUDIO_SCRIPTSVIEW_HPP
#define OPENSTUDIO_SCRIPTSVIEW_HPP

#include "SubTabView.hpp"
#include "ModelObjectInspectorView.hpp"

#include <ruleset/OSArgument.hpp>

#include <model/Model.hpp>

#include "../shared_gui_components/OSSwitch.hpp"
#include "OSVectorController.hpp"
#include "ScriptItem.hpp"
#include "OSItemList.hpp"

#include <utilities/idf/WorkspaceObject_Impl.hpp>

#include <QFileSystemWatcher>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>

namespace openstudio {

class ScriptFolderListView;

class ScriptsInspectorView : public OSInspectorView {
  Q_OBJECT

  public:

    ScriptsInspectorView(QWidget * parent = 0);
    virtual ~ScriptsInspectorView() {}

  protected:

    virtual void onClearSelection();
    virtual void onSelectItem(OSItem *item);
    virtual void onUpdate();

  signals:
    void toolsUpdated();

  public slots:
    void updateRubyInterpreterWarning();

  private slots:

    void openScriptFile();
    void onIsUserScriptClicked(bool checked);
    void refreshOSArguments();
    void scriptItemDeleting();

  private:
    QLabel *m_scriptTypeLabel;
    QLabel *m_scriptNameLabel;
    QLabel *m_locationLabel;
    QLabel *m_rubyInterpreterWarning;
    QPushButton *m_openInEditorButton;
    QPushButton *m_isUserScriptSwitch;
    QPushButton *m_refreshOSArgumentsButton;
    QLabel *m_argumentsLabel;
    QVBoxLayout *m_argumentEditsLayout;
    typedef std::pair<QLabel*, QWidget*> ArgumentEdit;
    std::vector<ArgumentEdit> m_argumentEdits;

    ScriptItem *m_scriptItem;
    QFileSystemWatcher m_fsWatcher;

    void clearArgumentWidgets();
    void refreshArgumentWidgets();

    REGISTER_LOGGER("openstudio.ScriptsInspectorView");
};


class ScriptsView : public SubTabView {
  Q_OBJECT;
 public:
  ScriptsView(const openstudio::path &t_scriptsPath,
              QWidget * parent = 0);
  virtual ~ScriptsView() {}
 signals:
  void updateRubyInterpreterWarning();
  void toolsUpdated();
};

class OSArgumentBooleanEdit : public OSSwitch {
  Q_OBJECT;
 public:

  OSArgumentBooleanEdit(ruleset::OSArgument& arg, QWidget * parent = 0);
  virtual ~OSArgumentBooleanEdit() {}

 signals:

  void newArgumentValue(const ruleset::OSArgument& arg);

 private slots:

  void onToggled(bool checked);

 private:

  ruleset::OSArgument m_arg;
};

class OSArgumentDoubleEdit : public QLineEdit {
  Q_OBJECT;
 public:
  OSArgumentDoubleEdit(ruleset::OSArgument& arg, QWidget * parent = 0);
  virtual ~OSArgumentDoubleEdit() {}

 signals:

  void newArgumentValue(const ruleset::OSArgument& arg);

 private slots:

  void onEditingFinished();

 private:

  ruleset::OSArgument m_arg;

  bool m_isScientific;
  boost::optional<int> m_precision;

  void refreshText();
  void setPrecision(const std::string& str);
};

class OSArgumentIntegerEdit : public QLineEdit {
  Q_OBJECT;
 public:
  OSArgumentIntegerEdit(ruleset::OSArgument& arg, QWidget * parent = 0);
  virtual ~OSArgumentIntegerEdit() {}

 signals:

  void newArgumentValue(const ruleset::OSArgument& arg);

 private slots:

  void onEditingFinished();

 private:

  ruleset::OSArgument m_arg;

  bool m_isScientific;
  boost::optional<int> m_precision;

  void refreshText();
  void setPrecision(const std::string& str);
};

class OSArgumentStringEdit : public QLineEdit {
  Q_OBJECT;
 public:

  OSArgumentStringEdit(ruleset::OSArgument& arg, QWidget * parent = 0);
  virtual ~OSArgumentStringEdit() {}

 signals:

  void newArgumentValue(const ruleset::OSArgument& arg);

 private slots:

  void onEditingFinished();

 private:
  ruleset::OSArgument m_arg;

  void refreshText();
};

class OSArgumentChoiceEdit : public QComboBox {
  Q_OBJECT;
 public:

  OSArgumentChoiceEdit(ruleset::OSArgument& arg,
                       bool delayIndexConnect = false,
                       QWidget * parent = 0);

  virtual ~OSArgumentChoiceEdit() {}

  void connectOnCurrentIndexChanged();

 signals:

  void newArgumentValue(const ruleset::OSArgument& arg);

 public slots:

  void setChoices(const std::vector<std::string>& choices);

 private slots:

  void onCurrentIndexChanged(const QString & text);

 private:
  ruleset::OSArgument m_arg;
  std::vector<std::string> m_choices;

  void refreshChoices();
};

class OSArgumentPathEdit : public QWidget {
  Q_OBJECT;
 public:
  OSArgumentPathEdit(ruleset::OSArgument& arg,
                     const openstudio::path& resourcesPath,
                     QWidget * parent = 0);

  virtual ~OSArgumentPathEdit() {}

  void loadImfFileSections();

 signals:

  void newArgumentValue(const ruleset::OSArgument& arg);

  void imfFileSections(const std::vector<std::string>& imfFileSections);

 private slots:

  void onFileBtnClicked();

 private:
  ruleset::OSArgument m_arg;
  openstudio::path m_resourcesPath;

  QLabel * m_fileLbl;

  void refreshText();
};

} // openstudio

#endif // OPENSTUDIO_SCRIPTSVIEW_HPP
