/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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

#ifndef OPENSTUDIO_PLUGIN_HPP
#define OPENSTUDIO_PLUGIN_HPP

#include <plugin/PluginAPI.hpp>
#include <openstudio_lib/SOConstants.hpp>

#include <utilities/idf/Handle.hpp>
#include <utilities/core/Logger.hpp>

#include <string>
#include <map>

#include <boost/shared_ptr.hpp>

#include <QApplication>

namespace openstudio {
namespace model {
  namespace detail {
    class Model_Impl;
  }
}

class OSDocument;
class StartupView;
class StartupMenu;

namespace plugin {

class OSObjectInterface;

class PLUGIN_API OSPlugin : public QApplication
{

  Q_OBJECT

  public:

    OSPlugin( int & argc, char ** argv );

    virtual ~OSPlugin();

    /// Calling this the first creates a new thread with this object, calls exec on the thread, and returns this object.
    static OSPlugin* instance();

  public slots:
    
    /// Quit the application.
    void quit();

    /// Show the help
    void showHelp();

    /// Prompt user to start a new model or open one, if neccesary prompt to save current model.  
    /// Creates a new model if user does not cancel.
    void startupPrompt(const std::string& key);

    /// Import an Idf and add a document referenced by key if successful.
    void importIdfForLastKey(); 

    /// Open an OSM and add a document referenced by key if successful.
    void openForLastKey();

    /// Create a new document from a template and referenced by key if successful.
    void newForLastKey( NewFromTemplateEnum newFromTemplateEnum );

    /// Hide the sending document
    void hideSender(); 

    /// Import an Idf and add a document referenced by key if successful.
    void importIdfForSender(); 

    /// Open an OSM and add a document referenced by key if successful.
    void openForSender();

    /// Create a new document from a template and referenced by key if successful.
    void newForSender();

    /// Show document for key
    void showDocument(const std::string& key);

    /// Hide document for key
    void hideDocument(const std::string& key);

  private slots:

    /// Remove the document refered to by key if it exists.
    bool closeDocument(const std::string& key);

  signals:
    
    void newModel(const std::string& key, openstudio::model::detail::Model_Impl* model);

    void modelClosed(const std::string& key);

    void documentShown(const std::string& key);

    void documentHidden(const std::string& key);

  private:

    REGISTER_LOGGER("OSPlugin");

    void connectDocumentSignals(OSDocument* osDocument);

    boost::optional<std::string> getKeyForDocument(OSDocument* osDocument) const;

    boost::shared_ptr<openstudio::StartupView> m_startupView;

    std::map<std::string, boost::shared_ptr<openstudio::OSDocument> > m_osDocuments;

    std::string m_lastKey;

    /// QWidget wrapper around SketchUp window
    QWidget* m_sketchUpWidget;

};

} // plugin
} // openstudio

#endif // OPENSTUDIO_PLUGIN_HPP

