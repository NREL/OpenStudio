#ifndef OPENSTUDIO_RUNMANAGER_SSHCONNECTION_HPP__
#define OPENSTUDIO_RUNMANAGER_SSHCONNECTION_HPP__



#include <string>
#include <fstream>

#include <utilities/core/Path.hpp>
#include <utilities/core/Logger.hpp>

#include <QThread>
#include <QMutex>

#include "RunManagerAPI.hpp"


#ifdef RUNMANAGER_HAVE_OPENSSL
#include <libssh/libssh.h>
#include <libssh/sftp.h>
#endif

#ifdef min
#undef min
#endif

namespace openstudio {
namespace runmanager {

namespace detail {


  /// Thread for executing a remote command and reading the results back from the ssh_session object
  class ExecThread : public QThread
  {
    Q_OBJECT; 

#ifdef RUNMANAGER_HAVE_OPENSSL
    public: 
      ExecThread(int id, ssh_session t_session, QMutex *t_mutex, const std::string &t_command, int t_timeout_msecs);
      virtual ~ExecThread();

    protected:
      void run();

    private:
      REGISTER_LOGGER("openstudio.runmanager.ExecThread");

      int m_id;

      ssh_session m_session;
      ssh_channel m_channel;

      QMutex *m_mutex;
      std::string m_command;
      int m_timeout_msecs;
      bool m_error_occurred;
#endif

    signals:
      void dataRead(int id, QString cmd, QString data);
      void errDataRead(int id, QString cmd, QString data);
      void finished(int id, QString cmd);
      void timeout(int id, QString cmd);

  };

  /// Thread for executing remote file transfer to local and reading the data as it is available
  class GetFileThread : public QThread
  {
    Q_OBJECT; 

#ifdef RUNMANAGER_HAVE_OPENSSL
    public: 
      GetFileThread(int id, ssh_session t_session, QMutex *t_mutex, const openstudio::path &t_fromremotepath,
          const openstudio::path &t_tolocalpath, int t_timeout_msecs);
      virtual ~GetFileThread();

    protected:
      void run();

    private:
      REGISTER_LOGGER("openstudio.runmanager.GetFileThread");

      int m_id;

      void handle_error(const std::string &desc, ssh_session t_session, sftp_session t_file);
      ssh_session m_session;
      sftp_session m_sftp_session;
      sftp_file m_sftp_file;

      QMutex *m_mutex;
      openstudio::path m_fromremote;
      openstudio::path m_tolocal;
      int m_timeout_msecs;
      bool m_error_occurred;
      std::ofstream m_outfile;
#endif

    signals:
      void finished(int id, QString fromremotepath, QString tolocalpath);
      void timeout(int id, QString fromremotepath, QString tolocalpath);

  };

  /// Thread for executing local file transfer to remote and feeding the file data
  class PutFileThread : public QThread
  {
    Q_OBJECT; 

#ifdef RUNMANAGER_HAVE_OPENSSL
    public: 
      PutFileThread(int id, ssh_session t_session, QMutex *t_mutex, const openstudio::path &t_fromlocalpath,
          const openstudio::path &t_toremotepath, int t_timeout_msecs);

      virtual ~PutFileThread();

    protected:
      void run();

    private:
      REGISTER_LOGGER("openstudio.runmanager.PutFileThread");

      int m_id;

      void handle_error(const std::string &desc, ssh_session t_session, sftp_session t_file);
      ssh_session m_session;
      sftp_session m_sftp_session;
      sftp_file m_sftp_file;
      QMutex *m_mutex;
      openstudio::path m_fromlocal;
      openstudio::path m_toremote;
      int m_timeout_msecs;
      bool m_error_occurred;
      std::ifstream m_infile;

#endif

    signals:
      void finished(int id, QString fromlocalpath, QString toremotepath);
      void timeout(int id, QString fromlocalpath, QString toremotepath);

  };

}

  /// Credentials object to be used for specifying how to connect to a remote SSH system
  class RUNMANAGER_API SSHCredentials
  {
    public:
      SSHCredentials(const std::string &host, const std::string &t_username, const std::string &t_password, 
          int t_port=22);

      SSHCredentials(const std::string &host, const std::string &t_username, const openstudio::path &t_publickey,
          const openstudio::path &t_privatekey, int t_port = 22);

      SSHCredentials(const std::string &host, const std::string &t_username, int t_port = 22);

      SSHCredentials(const std::string &host, int t_port = 22);

      SSHCredentials();

      bool operator==(const SSHCredentials &t_rhs) const;
      bool operator!=(const SSHCredentials &t_rhs) const
      {
        return !(*this == t_rhs);
      }

      boost::optional<std::string> host() const;
      boost::optional<std::string> username() const;
      boost::optional<std::string> password() const;
      boost::optional<openstudio::path> publickey() const;
      boost::optional<openstudio::path> privatekey() const;
      int port() const;

      void setHost(const std::string &t_host);
      void setUsername(const std::string &t_username);
      void setPassword(const std::string &t_password);
      void setPublickey(const openstudio::path &t_pubkey);
      void setPrivatekey(const openstudio::path &t_privkey);
      void setPort(int t_port);

    private:
      boost::optional<std::string> m_host;
      boost::optional<std::string> m_username;
      boost::optional<std::string> m_password;
      boost::optional<openstudio::path> m_publickey;
      boost::optional<openstudio::path> m_privatekey;
      int m_port;

      template<typename T>
      static bool compareOptionals(const boost::optional<T> &t_lhs, const boost::optional<T> &t_rhs)
      {
        if (!t_lhs && !t_rhs)
        {
          return true;
        }

        if (t_lhs && t_rhs)
        {
          return *t_lhs == *t_rhs;
        }

        return false;
      }

  };


  /**
   * Class for connecting to an SSH server, executing remote commands on it, sending and receiving files
   * with it.
   * \todo move this to utilities library?
   */
  class RUNMANAGER_API SSHConnection : public QObject
  {
    Q_OBJECT;

    public:
      SSHConnection(const SSHCredentials &t_creds, bool t_allow_ui);
      virtual ~SSHConnection();


      /// Returns the credentials used to successfully connect to the server
      SSHCredentials getCredentials() const;

      /// execute a command. Only one may be running at a time.
      void execCommand(int id, const std::string &command, int t_timeout_msecs);

      /// execute a command out of band. Only one may be running at a time.
      void execOOBCommand(int id, const std::string &command, int t_timeout_msecs);

      /// Download a file from the ssh server. Only one may be running at a time.
      void getFile(int id, const openstudio::path &remotefrom, const openstudio::path &localto, int t_timeout_msecs);

      /// Upload a file to the ssh server. Only one may be running at a time.
      void putFile(int id, const openstudio::path &localfrom, const openstudio::path &remoteto, int t_timeout_msecs);

      /// \returns true if the main execution channel is idle
      bool idle();

      /// \returns true if the outofband execution channel is idle
      bool oobIdle();

    signals:
      /// Emitted when stdout on main command channel is read
      void execCommandStandardOut(int id, const std::string &command, const std::string &data);
      /// Emitted when stderr on main command channel is read
      void execCommandStandardErr(int id, const std::string &command, const std::string &data);
      /// Emitted when main command channel exec is completed
      void execCommandFinished(int id, const std::string &command, bool timedout);

      /// Emitted when stdout on OOB command channel is read
      void execOOBCommandStandardOut(int id, const std::string &command, const std::string &data);
      /// Emitted when stderr on OOB command channel is read
      void execOOBCommandStandardErr(int id, const std::string &command, const std::string &data);
      /// Emitted when oob command channel exec is completed
      void execOOBCommandFinished(int id, const std::string &command, bool timedout);

      /// Emitted when current get file operation has completed
      void getFileFinished(int id, const openstudio::path &remotefrom, const openstudio::path &localto, bool timedout);
      /// Emitted when current put file operation has completed
      void putFileFinished(int id, const openstudio::path &localfrom, const openstudio::path &remoteto, bool timedout);

    private slots:
      //Note: using all QString's for signal parameters to avoid problems with 
      // cross-thread signals
      // used for reading data from threads
      void execDataRead(int id, QString command, QString data);
      void execErrDataRead(int id, QString command, QString data);
      void execFinished(int id, QString command);
      void execTimeout(int id, QString command);

      void execOOBDataRead(int id, QString command, QString data);
      void execOOBErrDataRead(int id, QString command, QString data);
      void execOOBFinished(int id, QString command);
      void execOOBTimeout(int id, QString command);

      void fileGetFinished(int id, QString remotefrom, QString localto);
      void fileGetTimeout(int id, QString remotefrom, QString localto);

      void filePutFinished(int id, QString remotefrom, QString localto);
      void filePutTimeout(int id, QString remotefrom, QString localto);

    private:
      REGISTER_LOGGER("openstudio.runmanager.SSHConnection");

      SSHCredentials m_credentials;

      bool m_command_timedout;
      bool m_oob_command_timedout;
      bool m_get_timedout;
      bool m_put_timedout;

#ifdef RUNMANAGER_HAVE_OPENSSL
      /// deciphers an error and throws an exception
      static void handle_error(const std::string &t_message, ssh_session t_session);

      mutable QMutex m_mutex;
      ssh_session m_session;
      boost::shared_ptr<detail::ExecThread> m_exec_thread;
      boost::shared_ptr<detail::GetFileThread> m_get_file_thread;
      boost::shared_ptr<detail::PutFileThread> m_put_file_thread;

      boost::shared_ptr<detail::ExecThread> m_oob_exec_thread;
#endif

  };

}
}

#endif
