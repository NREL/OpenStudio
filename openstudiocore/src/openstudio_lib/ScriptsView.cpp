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

#include <openstudio_lib/ScriptsView.hpp>
#include <openstudio_lib/ScriptFolderListView.hpp>
#include <openstudio_lib/BuildingStoryInspectorView.hpp>
#include <openstudio_lib/OSAppBase.hpp>
#include <openstudio_lib/OSDocument.hpp>

#include <runmanager/lib/RunManager.hpp>
#include <runmanager/lib/ConfigOptions.hpp>

#include <ruleset/OSArgument.hpp>

#include <model/Model_Impl.hpp>

#include <utilities/idf/ImfFile.hpp>
#include <utilities/core/ApplicationPathHelpers.hpp>
#include <utilities/document/DocumentRegex.hpp>
#include <utilities/core/StringHelpers.hpp>

#include <QStyleOption>
#include <QPainter>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QScrollArea>
#include <QTextBrowser>
#include <QProcess>
#include <QMessageBox>
#include <QFileDialog>

#include <boost/foreach.hpp>

#include <sstream>
#include <iomanip>

namespace openstudio {

  ScriptsInspectorView::ScriptsInspectorView( QWidget * parent )
    : OSInspectorView( false, parent),
      m_scriptTypeLabel(new QLabel()),
      m_scriptNameLabel(new QLabel()),
      m_locationLabel(new QLabel()),
      m_openInEditorButton(new QPushButton("Open Script in\nText Editor")),
      m_isUserScriptSwitch(new QPushButton()),
      m_refreshOSArgumentsButton(new QPushButton("Refresh User Script\nArguments")),
      m_argumentsLabel(new QLabel("User Script Arguments")),
      m_argumentEditsLayout(new QVBoxLayout()),
      m_scriptItem(NULL)
  {
    QVBoxLayout *layout = new QVBoxLayout();

    m_rubyInterpreterWarning = new QLabel("<h2><b>Warning</b>: Ruby script interpreter not yet located</h2>");
    m_rubyInterpreterWarning->setVisible(false);
    layout->addWidget(m_rubyInterpreterWarning);

    QHBoxLayout *titleLayout = new QHBoxLayout();
    titleLayout->addWidget(m_scriptTypeLabel);
    m_scriptTypeLabel->setStyleSheet("QLabel { font: bold; } ");
    titleLayout->addWidget(m_scriptNameLabel);
    layout->addLayout(titleLayout);

    QLabel *locationLabelName = new QLabel("Location:");
    locationLabelName->setStyleSheet("QLabel { font: bold; } ");
    layout->addWidget(locationLabelName);
    layout->addWidget(m_locationLabel);

    QHBoxLayout *actionsLayout = new QHBoxLayout();
    actionsLayout->addWidget(m_openInEditorButton);
    QVBoxLayout *isUserScriptLayout = new QVBoxLayout();
    QLabel *isUserScriptLabel = new QLabel("Formal User Script");
    isUserScriptLayout->addWidget(isUserScriptLabel,0,Qt::AlignHCenter);
    isUserScriptLayout->addWidget(m_isUserScriptSwitch,0,Qt::AlignHCenter);
    actionsLayout->addLayout(isUserScriptLayout);
    actionsLayout->addWidget(m_refreshOSArgumentsButton);
    layout->addLayout(actionsLayout);

    m_openInEditorButton->setEnabled(false);

    m_isUserScriptSwitch->setObjectName("StandardGrayButton");
    m_isUserScriptSwitch->setCheckable(true);
    m_isUserScriptSwitch->setEnabled(false);

    m_refreshOSArgumentsButton->setEnabled(false);

    connect(m_openInEditorButton, SIGNAL(clicked(bool)), this, SLOT(openScriptFile()));
    connect(m_isUserScriptSwitch, SIGNAL(toggled(bool)), this, SLOT(onIsUserScriptClicked(bool)));
    connect(m_refreshOSArgumentsButton,SIGNAL(clicked(bool)),this,SLOT(refreshOSArguments()));

    m_argumentsLabel->setStyleSheet("QLabel { font: bold; } ");
    layout->addWidget(m_argumentsLabel);
    layout->addLayout(m_argumentEditsLayout);
    m_argumentsLabel->setVisible(false);

    layout->addStretch();

    QWidget *w = new QWidget();
    w->setLayout(layout);
    stackedWidget()->addWidget(w);

    connect(&m_fsWatcher, SIGNAL(fileChanged(const QString &)), this, SLOT(update()));
    updateRubyInterpreterWarning();
  }

  void ScriptsInspectorView::updateRubyInterpreterWarning()
  {
    LOG(Debug, "updateRubyInterpreterWarning called");

    openstudio::runmanager::ConfigOptions co(true);
    bool ruby_installed = co.getTools().getAllByName("ruby").tools().size() > 0;
    m_rubyInterpreterWarning->setVisible(!ruby_installed);
  }

  void ScriptsInspectorView::openScriptFile() {
    if (m_scriptItem) {
      QStringList args;
      args.push_back(openstudio::toQString(m_scriptItem->path()));
#if defined(Q_OS_WIN32)
      if (!QProcess::startDetached("notepad", args))
      {
          QMessageBox::critical(this, "Unable to launch editor", "Notepad could not be launched.");
      }
#elif defined(Q_OS_MAC)
      if (!QProcess::startDetached("/Applications/TextEdit.app/Contents/MacOS/TextEdit", args))
      {
          QMessageBox::critical(this, "Unable to launch editor", "TextEdit could not be launched.");
      }
#else
      if (!QProcess::startDetached("gedit", args))
      {
        if (!QProcess::startDetached("kedit", args))
        {
          QMessageBox::critical(this, "Unable to launch editor", "No appropriate editor found.");
        }
      }
#endif
    }
  }

  void ScriptsInspectorView::onIsUserScriptClicked(bool checked) {
    if (m_scriptItem) {
      m_scriptItem->setIsUserScript(checked);
      if (m_scriptItem->isUserScript()) {
        m_isUserScriptSwitch->setText("Yes");
        m_refreshOSArgumentsButton->setVisible(true);
        m_refreshOSArgumentsButton->setEnabled(true);
      }
      else {
        m_isUserScriptSwitch->setText("No");
        m_refreshOSArgumentsButton->setVisible(false);
        m_refreshOSArgumentsButton->setEnabled(false);
      }
    }
  }

  void ScriptsInspectorView::refreshOSArguments() {
    if (m_scriptItem) {
      // find tools
      runmanager::RunManager rm;
      openstudio::runmanager::ConfigOptions co = rm.getConfigOptions();
      if (co.findTools(true, true, true, true))
      {
        emit toolsUpdated();
        rm.setConfigOptions(co);
      }

      updateRubyInterpreterWarning();

      // refresh args
      try {
        m_scriptItem->refreshArgumentsFromScript(rm);
      }
      catch (...) {
        LOG(Error, "Exception thrown while calling refreshArgumentsFromScript");
        // TODO: Let user know what happened here.
      }
      refreshArgumentWidgets();
    }
  }

  void ScriptsInspectorView::onClearSelection() {
    if (m_scriptItem && (!m_scriptItem->removed())) {
      m_scriptItem->saveArgumentsToDb();
    }
    
    m_scriptItem = NULL;
    m_scriptTypeLabel->setText("Script Type:");
    m_scriptNameLabel->setText("No file selected.");
    m_locationLabel->setText("");
    m_openInEditorButton->setEnabled(false);
    m_isUserScriptSwitch->setChecked(true);
    m_isUserScriptSwitch->setEnabled(false);
    m_refreshOSArgumentsButton->setVisible(true);
    m_refreshOSArgumentsButton->setEnabled(false);
    m_argumentsLabel->setVisible(false);
    clearArgumentWidgets();

    if (!m_fsWatcher.files().empty())
    {
      m_fsWatcher.removePaths(m_fsWatcher.files());
    }
  }

  void ScriptsInspectorView::scriptItemDeleting()
  {
    // Make sure we don't try to use it
    if (m_scriptItem)
    {
      LOG(Warn, "setting m_scriptItem to 0, because we never got a deselect");
    }

    m_scriptItem = 0;
  }

  void ScriptsInspectorView::onSelectItem(OSItem *item)
  {
    if (m_scriptItem && (!m_scriptItem->removed())) {
      // ETH@20120910 Crash happens from here. Somehow m_scriptItem has been corrupted. 
      // Uninitialized path data.
      m_scriptItem->saveArgumentsToDb();
    }

    m_scriptItem = qobject_cast<ScriptItem*>(item);
    connect(m_scriptItem, SIGNAL(destroyed()), this, SLOT(scriptItemDeleting()));
    assert(m_scriptItem);
    const ScriptFolderListView* scriptFolders =
        OSAppBase::instance()->currentDocument()->scriptFolderListView();

    m_scriptTypeLabel->setText(toQString(scriptFolders->folderName(toPath(m_scriptItem->itemId().sourceId()))) + ":");
    m_scriptNameLabel->setText(toQString(m_scriptItem->path().filename()));
    m_locationLabel->setText(m_scriptItem->itemId().itemId());
    m_openInEditorButton->setEnabled(true);
    m_isUserScriptSwitch->setEnabled(true);
    m_scriptItem->updateArgumentsFromDb();
    refreshArgumentWidgets();

    if (!m_fsWatcher.files().empty()) {
      m_fsWatcher.removePaths(m_fsWatcher.files());
    }

    m_fsWatcher.addPath(openstudio::toQString(m_scriptItem->path()));
  }

  void ScriptsInspectorView::onUpdate() {
    refreshArgumentWidgets();
  }

  void ScriptsInspectorView::clearArgumentWidgets() {
    BOOST_FOREACH(ArgumentEdit& argEdit,m_argumentEdits) {
      m_argumentEditsLayout->removeWidget(argEdit.first);
      delete argEdit.first;
      m_argumentEditsLayout->removeWidget(argEdit.second);
      delete argEdit.second;
    }
    m_argumentEdits.clear();
  }

  void ScriptsInspectorView::refreshArgumentWidgets() {
    clearArgumentWidgets();
    if (m_scriptItem) {
      m_isUserScriptSwitch->setChecked(m_scriptItem->isUserScript());
      if (m_scriptItem->isUserScript()) {
        m_isUserScriptSwitch->setText("Yes");
        m_refreshOSArgumentsButton->setVisible(true);
        m_refreshOSArgumentsButton->setEnabled(true);
        OSArgumentPathEdit* lastImfFile;
        ruleset::OSArgumentVector args = m_scriptItem->osArguments();
        BOOST_FOREACH(ruleset::OSArgument& arg,args) {
//          LOG(Debug, "Populating argument: " << arg.name << " " << arg.displayName << " " << arg.type().
          std::string name = arg.displayName();
          if (name.empty()) { name = arg.name(); }
          QLabel* argLabel = new QLabel(toQString(name));
          argLabel->setStyleSheet("QLabel { font: bold; } ");
          OSArgumentBooleanEdit* argBoolEdit;
          OSArgumentDoubleEdit* argDoubleEdit;
          OSArgumentIntegerEdit* argIntegerEdit;
          OSArgumentStringEdit* argStringEdit;
          OSArgumentChoiceEdit* argChoiceEdit;
          OSArgumentPathEdit* argPathEdit;
          bool connected(false);
          switch (arg.type().value()) {
            case ruleset::OSArgumentType::Boolean :
              argBoolEdit = new OSArgumentBooleanEdit(arg);
              connected = connect(argBoolEdit,
                                  SIGNAL(newArgumentValue(const ruleset::OSArgument&)),
                                  m_scriptItem,
                                  SLOT(setOSArgument(const ruleset::OSArgument&)));
              m_argumentEdits.push_back(std::pair<QLabel*,QWidget*>(argLabel,argBoolEdit));
              break;
            case ruleset::OSArgumentType::Double :
              argDoubleEdit = new OSArgumentDoubleEdit(arg);
              connected = connect(argDoubleEdit,
                                  SIGNAL(newArgumentValue(const ruleset::OSArgument&)),
                                  m_scriptItem,
                                  SLOT(setOSArgument(const ruleset::OSArgument&)));
              m_argumentEdits.push_back(std::pair<QLabel*,QWidget*>(argLabel,argDoubleEdit));
              break;
            case ruleset::OSArgumentType::Integer :
              argIntegerEdit = new OSArgumentIntegerEdit(arg);
              connected = connect(argIntegerEdit,
                                  SIGNAL(newArgumentValue(const ruleset::OSArgument&)),
                                  m_scriptItem,
                                  SLOT(setOSArgument(const ruleset::OSArgument&)));
              m_argumentEdits.push_back(std::pair<QLabel*,QWidget*>(argLabel,argIntegerEdit));
              break;
            case ruleset::OSArgumentType::String :
              // check for special default values
              if (arg.hasDefaultValue()) {
                if (arg.defaultValueAsString() == "listImfFileSectionNames") {
                  if (lastImfFile) {
                    argChoiceEdit = new OSArgumentChoiceEdit(arg,true);
                    connected = connect(lastImfFile,
                                        SIGNAL(imfFileSections(std::vector<std::string>)),
                                        argChoiceEdit,
                                        SLOT(setChoices(std::vector<std::string>)));
                    connected = connect(argChoiceEdit,
                                        SIGNAL(newArgumentValue(const ruleset::OSArgument&)),
                                        m_scriptItem,
                                        SLOT(setOSArgument(const ruleset::OSArgument&)));
                    lastImfFile->loadImfFileSections();
                    argChoiceEdit->connectOnCurrentIndexChanged();
                    m_argumentEdits.push_back(std::pair<QLabel*,QWidget*>(argLabel,argChoiceEdit));
                    break;
                  }
                  else {
                    QMessageBox::information(
                        this,
                        "Unexpected User Script Argument",
                        "String arguments with the default value 'listImfFileSectionNames' should be preceded by a path argument with extension 'imf'.",
                        QMessageBox::Ok);
                  }
                }
              }
              argStringEdit = new OSArgumentStringEdit(arg);
              connected = connect(argStringEdit,
                                  SIGNAL(newArgumentValue(const ruleset::OSArgument&)),
                                  m_scriptItem,
                                  SLOT(setOSArgument(const ruleset::OSArgument&)));
              m_argumentEdits.push_back(std::pair<QLabel*,QWidget*>(argLabel,argStringEdit));
              break;
            case ruleset::OSArgumentType::Choice :
              argChoiceEdit = new OSArgumentChoiceEdit(arg);
              connected = connect(argChoiceEdit,
                                  SIGNAL(newArgumentValue(const ruleset::OSArgument&)),
                                  m_scriptItem,
                                  SLOT(setOSArgument(const ruleset::OSArgument&)));
              m_argumentEdits.push_back(std::pair<QLabel*,QWidget*>(argLabel,argChoiceEdit));
              break;
            case ruleset::OSArgumentType::Path :
              argPathEdit = new OSArgumentPathEdit(arg,m_scriptItem->resourcesPath());
              connected = connect(argPathEdit,
                                  SIGNAL(newArgumentValue(const ruleset::OSArgument&)),
                                  m_scriptItem,
                                  SLOT(setOSArgument(const ruleset::OSArgument&)));
              m_argumentEdits.push_back(std::pair<QLabel*,QWidget*>(argLabel,argPathEdit));
              if (arg.extension() == "imf") {
                lastImfFile = argPathEdit;
              }
              break;
            default:
              continue;
          }
          m_argumentEditsLayout->addWidget(m_argumentEdits.back().first);
          m_argumentEditsLayout->addWidget(m_argumentEdits.back().second);
          BOOST_ASSERT(connected);
        }
        if (m_argumentEdits.empty()) {
          m_argumentsLabel->setVisible(false);
        }
        else {
          m_argumentsLabel->setVisible(true);
        }
      }
      else {
        m_argumentsLabel->setVisible(false);
        m_isUserScriptSwitch->setText("No");
        m_refreshOSArgumentsButton->setVisible(false);
        m_refreshOSArgumentsButton->setEnabled(false);
      }
    }
  }

ScriptsView::ScriptsView(const openstudio::path &t_scriptsPath,
                         QWidget * parent)
  : SubTabView(new ScriptFolderListView(t_scriptsPath, true, false, true, OSItem::COLLAPSIBLE_LIST_HEADER, parent),
               new ScriptsInspectorView(parent),
               parent)
{
  ScriptFolderListView* scriptFolderListView = qobject_cast<ScriptFolderListView*>(itemSelector());
  connect(this, SIGNAL(updateRubyInterpreterWarning()), 
      inspectorView(), SLOT(updateRubyInterpreterWarning()));
  connect(inspectorView(), SIGNAL(toolsUpdated()),
      this, SIGNAL(toolsUpdated()));

//  connect(this, 
  BOOST_ASSERT(scriptFolderListView);
}

OSArgumentBooleanEdit::OSArgumentBooleanEdit(ruleset::OSArgument& arg, QWidget * parent)
  : OSSwitch(parent), m_arg(arg)
{
  BOOST_ASSERT(arg.type() == ruleset::OSArgumentType::Boolean);
//  setObjectName("StandardGrayButton");
//  this->setCheckable(true);
//  this->setAcceptDrops(false);
  bool value(false);
  if (arg.hasValue()) {
    value = arg.valueAsBool();
  }
  else if (arg.hasDefaultValue()) {
    value = arg.defaultValueAsBool();
  }
  else {
    m_arg.setValue(value);
  }
  this->setChecked(value);

  bool isConnected = connect(this,SIGNAL(clicked(bool)),this,SLOT(onToggled(bool)));
  BOOST_ASSERT(isConnected);
}

void OSArgumentBooleanEdit::onToggled(bool checked) {
  m_arg.setValue(checked);
  emit newArgumentValue(m_arg);
}

OSArgumentDoubleEdit::OSArgumentDoubleEdit(ruleset::OSArgument& arg,
                                                           QWidget * parent)
  : QLineEdit(parent), m_arg(arg), m_isScientific(false)
{
  BOOST_ASSERT(arg.type() == ruleset::OSArgumentType::Double);
  this->setAcceptDrops(false);
  refreshText();

  bool isConnected = connect(this,SIGNAL(editingFinished()),this,SLOT(onEditingFinished()));
  BOOST_ASSERT(isConnected);
}

void OSArgumentDoubleEdit::onEditingFinished()
{
  try {
    std::string str = this->text().toStdString();
    double value = boost::lexical_cast<double>(str);
    setPrecision(str);
    m_arg.setValue(value);
    refreshText();
    emit newArgumentValue(m_arg);
  }
  catch (...) {}
}

void OSArgumentDoubleEdit::refreshText() {
  OptionalDouble value;
  if (m_arg.hasValue()) {
    value = m_arg.valueAsDouble();
    setStyleSheet("color:black");
  }
  else if (m_arg.hasDefaultValue()) {
    value = m_arg.defaultValueAsDouble();
    setStyleSheet("color:green");
  }
  QString textValue;
  if (value) {
    std::stringstream ss;
    if (m_isScientific) {
      ss << std::scientific;
    }
    else {
      ss << std::fixed;
    }
    if (m_precision) {
      ss << std::setprecision(*m_precision);
    }
    ss << *value;
    textValue = toQString(ss.str());
  }
  setText(textValue);
}

void OSArgumentDoubleEdit::setPrecision(const std::string& str) {
  boost::regex rgx("-?([[:digit:]]*)(\\.)?([[:digit:]]+)([EDed][-\\+]?[[:digit:]]+)?");
  boost::smatch m;
  if (boost::regex_match(str,m,rgx)) {
    std::string sci, prefix, postfix;
    if (m[1].matched) {
      prefix = std::string(m[1].first,m[1].second);
    }
    if (m[3].matched) {
      postfix = std::string(m[3].first,m[3].second);
    }
    if (m[4].matched) {
      sci = std::string(m[4].first,m[4].second);
    }
    m_isScientific = !sci.empty();

    if (m_isScientific) {
      m_precision = prefix.size() + postfix.size() - 1;
    }
    else {
      if (m[2].matched) {
        m_precision = postfix.size();
      }
      else {
        m_precision = 0;
      }
    }
  }
  else {
    m_isScientific = false;
    m_precision.reset();
  }
}

OSArgumentIntegerEdit::OSArgumentIntegerEdit(ruleset::OSArgument& arg,
                                                             QWidget * parent)
  : QLineEdit(parent), m_arg(arg), m_isScientific(false)
{
  BOOST_ASSERT(arg.type() == ruleset::OSArgumentType::Integer);
  this->setAcceptDrops(false);
  refreshText();

  bool isConnected = connect(this,SIGNAL(editingFinished()),this,SLOT(onEditingFinished()));
  BOOST_ASSERT(isConnected);
}

void OSArgumentIntegerEdit::onEditingFinished() {
  try {
    std::string str = this->text().toStdString();
    int value = boost::lexical_cast<int>(str);
    setPrecision(str);
    m_arg.setValue(value);
    refreshText();
    emit newArgumentValue(m_arg);
  }
  catch (...) {}
}

void OSArgumentIntegerEdit::refreshText() {
  OptionalInt value;
  if (m_arg.hasValue()) {
    value = m_arg.valueAsInteger();
    setStyleSheet("color:black");
  }
  else if (m_arg.hasDefaultValue()) {
    value = m_arg.defaultValueAsInteger();
    setStyleSheet("color:green");
  }
  QString textValue;
  if (value) {
    std::stringstream ss;
    if (m_isScientific) {
      ss << std::scientific;
    }
    else {
      ss << std::fixed;
    }
    if (m_precision) {
      ss << std::setprecision(*m_precision);
    }
    ss << *value;
    textValue = toQString(ss.str());
  }
  setText(textValue);
}

void OSArgumentIntegerEdit::setPrecision(const std::string& str) {
  boost::regex rgx("-?([[:digit:]]*)(\\.)?([[:digit:]]+)([EDed][-\\+]?[[:digit:]]+)?");
  boost::smatch m;
  if (boost::regex_match(str,m,rgx)) {
    std::string sci, prefix, postfix;
    if (m[1].matched) {
      prefix = std::string(m[1].first,m[1].second);
    }
    if (m[3].matched) {
      postfix = std::string(m[3].first,m[3].second);
    }
    if (m[4].matched) {
      sci = std::string(m[4].first,m[4].second);
    }
    m_isScientific = !sci.empty();

    if (m_isScientific) {
      m_precision = prefix.size() + postfix.size() - 1;
    }
    else {
      m_precision = 0;
    }
  }
  else {
    m_isScientific = false;
    m_precision.reset();
  }
}

OSArgumentStringEdit::OSArgumentStringEdit(ruleset::OSArgument& arg,
                                                           QWidget * parent)
  : QLineEdit(parent), m_arg(arg)
{
  BOOST_ASSERT(arg.type() == ruleset::OSArgumentType::String);
  this->setAcceptDrops(false);
  refreshText();

  bool isConnected = connect(this,SIGNAL(editingFinished()),this,SLOT(onEditingFinished()));
  BOOST_ASSERT(isConnected);
}

void OSArgumentStringEdit::onEditingFinished() {
  std::string str = this->text().toStdString();
  m_arg.setValue(str);
  emit newArgumentValue(m_arg);
}

void OSArgumentStringEdit::refreshText() {
  OptionalString value;
  if (m_arg.hasValue()) {
    value = m_arg.valueAsString();
    setStyleSheet("color:black");
  }
  else if (m_arg.hasDefaultValue()) {
    value = m_arg.defaultValueAsString();
    setStyleSheet("color:green");
  }
  QString textValue;
  if (value) { textValue = toQString(*value); }
  setText(textValue);
}

OSArgumentChoiceEdit::OSArgumentChoiceEdit(ruleset::OSArgument& arg,
                                                           bool delayIndexConnect,
                                                           QWidget * parent)
  : QComboBox(parent), m_arg(arg)
{
  BOOST_ASSERT((arg.type() == ruleset::OSArgumentType::Choice) || (arg.type() == ruleset::OSArgumentType::String));
  this->setAcceptDrops(false);
  refreshChoices();

  if (!delayIndexConnect) {
    connectOnCurrentIndexChanged();
  }
}

void OSArgumentChoiceEdit::connectOnCurrentIndexChanged() {
  bool isConnected = connect(this,
                             SIGNAL(currentIndexChanged(const QString&)),
                             this,
                             SLOT(onCurrentIndexChanged(const QString&)));
  BOOST_ASSERT(isConnected);
}

void OSArgumentChoiceEdit::setChoices(const std::vector<std::string>& choices) {
  m_choices = choices;
  refreshChoices();
}

void OSArgumentChoiceEdit::onCurrentIndexChanged(const QString & text) {
  if (!m_choices.empty()) {
    m_arg.setValue(toString(text));
    emit newArgumentValue(m_arg);
  }
}

void OSArgumentChoiceEdit::refreshChoices() {
  clear();
  OptionalString currentValue;
  if (m_arg.type() == ruleset::OSArgumentType::Choice) {
    m_choices.clear();
    m_choices = m_arg.choiceValues();
    if (m_arg.hasValue()) {
      currentValue = m_arg.valueAsString();
    }
    else if (m_arg.hasDefaultValue()) {
      currentValue = m_arg.defaultValueAsString();
    }
  }
  else if (m_arg.hasDefaultValue() && (m_arg.defaultValueAsString() == "listImfFileSectionNames")) {
    if (m_choices.empty()) {
      m_choices.push_back("Please select an Imf file.");
    }
    else if (m_arg.hasValue()) {
      currentValue = m_arg.valueAsString();
    }
  }
  BOOST_FOREACH(const std::string& choice, m_choices) {
    addItem(QString::fromStdString(choice));
  }
  if (currentValue) {
    std::string value = currentValue.get();
    if (!value.empty()) {
      int index = static_cast<int>(std::find(m_choices.begin(),m_choices.end(),value) - m_choices.begin());
      if (index < int(m_choices.size())) {
        this->setCurrentIndex(index);
      }
    }
  }
}

OSArgumentPathEdit::OSArgumentPathEdit(ruleset::OSArgument& arg,
                                                       const openstudio::path& resourcesPath,
                                                       QWidget * parent)
  : QWidget(parent), m_arg(arg), m_resourcesPath(resourcesPath)
{
  m_fileLbl = new QLabel();
  m_fileLbl->setStyleSheet("border: 2px solid black");

  QPushButton * btn = new QPushButton("Browse",this);
  bool isConnected = connect(btn,SIGNAL(clicked()),this,SLOT(onFileBtnClicked()));
  BOOST_ASSERT(isConnected);
  btn->setFixedWidth(100);

  QVBoxLayout * vLayout = new QVBoxLayout();
  setLayout(vLayout);
  vLayout->addWidget(m_fileLbl);
  vLayout->addWidget(btn);

  refreshText();
}

void OSArgumentPathEdit::loadImfFileSections() {
  if (m_arg.hasValue() && m_arg.extension() == "imf") {
    openstudio::path filePath = m_arg.valueAsPath();
    OptionalImfFile imfFile = ImfFile::load(filePath,IddFileType::EnergyPlus);
    if (imfFile) {
      emit imfFileSections(imfFile->sectionNames());
    }    
  }
}

void OSArgumentPathEdit::onFileBtnClicked() {
  std::stringstream ss;
  ss << "Files (*." << m_arg.extension() << ")";
  QString fileTypes = toQString(ss.str());
  ss.str("");

  ss << "Open " << m_arg.extension() << " File";
  QString fileName = QFileDialog::getOpenFileName(this,toQString(ss.str()),toQString(m_resourcesPath),fileTypes);
  if(!fileName.isEmpty()) {
    openstudio::path filePath = openstudio::toPath(fileName);
    m_arg.setValue(filePath);
    refreshText();
    loadImfFileSections();
    emit newArgumentValue(m_arg);
  }
}

void OSArgumentPathEdit::refreshText() {
  QString value;
  if (m_arg.hasValue()) {
    value = toQString(m_arg.valueAsPath());
    m_fileLbl->setStyleSheet("border: 2px solid black; color:black");
  }
  else if (m_arg.hasDefaultValue()) {
    value = toQString(m_arg.defaultValueAsPath());
    m_fileLbl->setStyleSheet("border: 2px solid black; color:green");
  }
  m_fileLbl->setText(value);
}

} // openstudio

