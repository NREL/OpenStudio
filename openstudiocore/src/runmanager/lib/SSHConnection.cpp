
#include "SSHConnection.hpp"
#include <utilities/core/String.hpp>
#include <QInputDialog>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <algorithm>
#include <fcntl.h>

namespace openstudio {
namespace runmanager {

  SSHCredentials::SSHCredentials(const std::string &t_host, const std::string &t_username,
      const std::string &t_password, int t_port)
    : m_host(t_host), m_username(t_username), m_password(t_password), m_port(t_port)
  {
  }

  SSHCredentials::SSHCredentials(const std::string &t_host, const std::string &t_username, const openstudio::path &t_publickey,
      const openstudio::path &t_privatekey, int t_port)
    : m_host(t_host), m_username(t_username), m_publickey(t_publickey), m_privatekey(t_privatekey),
      m_port(t_port)
  {
  }

  SSHCredentials::SSHCredentials(const std::string &t_host, const std::string &t_username, int t_port)
    : m_host(t_host), m_username(t_username), m_port(t_port)
  {
  }

  SSHCredentials::SSHCredentials(const std::string &t_host, int t_port)
    : m_host(t_host), m_port(t_port)
  {
  }

  SSHCredentials::SSHCredentials()
  {
  }

  bool SSHCredentials::operator==(const SSHCredentials &t_rhs) const
  {
    return compareOptionals(m_host, t_rhs.m_host)
      && compareOptionals(m_password, t_rhs.m_password)
      && m_port == t_rhs.m_port
      && compareOptionals(m_privatekey, t_rhs.m_privatekey)
      && compareOptionals(m_publickey, t_rhs.m_publickey)
      && compareOptionals(m_username, t_rhs.m_username);
  }


  boost::optional<std::string> SSHCredentials::host() const
  {
    return m_host;
  }

  boost::optional<std::string> SSHCredentials::username() const
  {
    return m_username;
  }

  boost::optional<std::string> SSHCredentials::password() const
  {
    return m_password;
  }

  boost::optional<openstudio::path> SSHCredentials::publickey() const
  {
    return m_publickey;
  }

  boost::optional<openstudio::path> SSHCredentials::privatekey() const
  {
    return m_privatekey;
  }

  int SSHCredentials::port() const
  {
    return m_port;
  }

  void SSHCredentials::setHost(const std::string &t_host)
  {
    m_host = t_host;
  }

  void SSHCredentials::setUsername(const std::string &t_username)
  {
    m_username = t_username;
  }

  void SSHCredentials::setPassword(const std::string &t_password)
  {
    m_password = t_password;
  }

  void SSHCredentials::setPublickey(const openstudio::path &t_pubkey)
  {
    m_publickey = t_pubkey;
  }

  void SSHCredentials::setPrivatekey(const openstudio::path &t_privkey)
  {
    m_privatekey = t_privkey;
  }

  void SSHCredentials::setPort(int t_port)
  {
    m_port = t_port;
  }

#ifdef RUNMANAGER_HAVE_OPENSSL

  void SSHConnection::handle_error(const std::string &desc, ssh_session t_session)
  {
    std::string err = ssh_get_error(t_session);
    ssh_disconnect(t_session);
    ssh_free(t_session);
    throw std::runtime_error(desc + ": " + err);
  }

  SSHConnection::SSHConnection(const SSHCredentials &t_creds, bool t_allow_ui)
    : m_credentials(t_creds)
  {
    m_session = ssh_new();

    if (m_session == NULL)
    {
      throw std::runtime_error("Unable to allocate ssh session");
    }

    LOG(Debug, "SSH Port Set" << m_credentials.port());
    int port = m_credentials.port();
    if (ssh_options_set(m_session, SSH_OPTIONS_PORT, &port) < 0)
    {
      handle_error("Error setting ssh port", m_session);
    }

    if (m_credentials.username())
    {
      std::string user = *m_credentials.username();
      LOG(Debug, "SSHUserNameSet " << user);

      if (ssh_options_set(m_session, SSH_OPTIONS_USER, user.c_str()) < 0)
      {
        handle_error("Error setting ssh username", m_session);
      }
    }


    if (m_credentials.host())
    {
      std::string host = *m_credentials.host();
      LOG(Debug, "SSHHost set " << host);

      if (ssh_options_set(m_session, SSH_OPTIONS_HOST, host.c_str()) < 0)
      {
        handle_error("Error setting ssh hostname", m_session);
      }
    }

    LOG(Debug, "Initiating SSH Connection");
    if (ssh_connect(m_session))
    {
      handle_error("Error establishing connection", m_session);
    }

    /* for the future
    if (verify_knownhost(m_session) < 0)
    {
      ssh_disconnect(m_session);
      ssh_free(m_session);
      throw std::runtime_error("Unable to verify ssh host");
    }
    */


    LOG(Debug, "Attempting userauth none");
    int rc = ssh_userauth_none(m_session, NULL);

    if (rc == SSH_AUTH_ERROR)
    {
      handle_error("Error with userauth none", m_session);
    }

    if (rc == SSH_AUTH_SUCCESS)
    {
      //yay
      return;
    }

    int method = ssh_auth_list(m_session);

    if (method & SSH_AUTH_METHOD_PUBLICKEY)
    {
      LOG(Debug, "Attempting userauth autopubkey");
      rc = ssh_userauth_autopubkey(m_session, NULL);
      if (rc == SSH_AUTH_ERROR)
      {
        handle_error("Error with userauth autopubkey", m_session);
      }

      if (rc == SSH_AUTH_SUCCESS)
      {
        //yay
        return;
      }
    }

    if (m_credentials.publickey() || m_credentials.privatekey())
    {
      throw std::runtime_error("public/private key authentication not yet supported");
    }

    LOG(Debug, "Attempting userauth password");
    if (method & SSH_AUTH_METHOD_PASSWORD)
    {
      if (m_credentials.password())
      {
        std::string password = *m_credentials.password();
        rc = ssh_userauth_password(m_session, NULL, password.c_str());

        if (rc == SSH_AUTH_ERROR)
        {
          handle_error("Error with userauth password", m_session);
        }

      }

      while (rc != SSH_AUTH_SUCCESS)
      {
        LOG(Debug, "Attempting userauth password - prompting for password");

        QString txt;
        if (t_allow_ui)
        {
          txt = QInputDialog::getText(0, "Enter Password", toQString("Enter password for SSH connection to " + *m_credentials.host()), QLineEdit::Password);
        }

        if (txt.isEmpty())
        {
          ssh_disconnect(m_session);
          ssh_free(m_session);
          throw std::runtime_error("Unable to authenticate via password (invalid username/password)");
        } else {
          std::string password = openstudio::toString(txt);
          rc = ssh_userauth_password(m_session, NULL, password.c_str());
          m_credentials.setPassword(password);
          if (rc == SSH_AUTH_ERROR)
          {
            handle_error("Error with userauth password", m_session);
          }
        }
      }

      // If we make it here, we've succeeded with a password authentication
      return;
    }

    // If we make it here, no authentication style succeeded
    ssh_disconnect(m_session);
    ssh_free(m_session);

    throw std::runtime_error("No authentication can succeed");

  }

  SSHCredentials SSHConnection::getCredentials() const
  {
    QMutexLocker l(&m_mutex);
    return m_credentials;
  }


  SSHConnection::~SSHConnection()
  {

    m_exec_thread.reset();
    m_get_file_thread.reset();
    m_put_file_thread.reset();
    m_oob_exec_thread.reset();

    QMutexLocker l(&m_mutex);
    ssh_disconnect(m_session);
    ssh_free(m_session);
  }

  void SSHConnection::execDataRead(int id, QString command, QString data)
  {
    LOG(Debug, "Receiving execDataRead in thread: " << QThread::currentThreadId());
    emit execCommandStandardOut(id, toString(command), toString(data));
  }

  void SSHConnection::execErrDataRead(int id, QString command, QString data)
  {
    LOG(Debug, "Receiving execErrDataRead in thread: " << QThread::currentThreadId());
    emit execCommandStandardErr(id, toString(command), toString(data));
  }


  void SSHConnection::execFinished(int id, QString command)
  {
    LOG(Debug, "Receiving execFinished in thread: " << QThread::currentThreadId());
    emit execCommandFinished(id, toString(command), m_command_timedout);
  }

  void SSHConnection::execTimeout(int id, QString command)
  {
    m_command_timedout = true;
  }

  void SSHConnection::execOOBDataRead(int id, QString command, QString data)
  {
    LOG(Debug, "Receiving execOOBDataRead in thread: " << QThread::currentThreadId());
    emit execOOBCommandStandardOut(id, toString(command), toString(data));
  }

  void SSHConnection::execOOBErrDataRead(int id, QString command, QString data)
  {
    LOG(Debug, "Receiving execOOBDataRead in thread: " << QThread::currentThreadId());
    emit execOOBCommandStandardErr(id, toString(command), toString(data));
  }


  void SSHConnection::execOOBFinished(int id, QString command)
  {
    LOG(Debug, "Receiving execOOBFinished in thread: " << QThread::currentThreadId());
    emit execOOBCommandFinished(id, toString(command), m_oob_command_timedout);
  }

  void SSHConnection::execOOBTimeout(int id, QString command)
  {
    m_oob_command_timedout = true;
  }

  void SSHConnection::fileGetFinished(int id, QString remotefrom, QString localto)
  {
    LOG(Debug, "Receiving fileGetFinished (" << toString(remotefrom) << " to " << toString(localto) << ") in thread: " << QThread::currentThreadId());
    emit getFileFinished(id, toPath(remotefrom), toPath(localto), m_get_timedout);
  }

  void SSHConnection::fileGetTimeout(int id, QString remotefrom, QString localto)
  {
    m_get_timedout = true;
  }

  void SSHConnection::filePutFinished(int id, QString localfrom, QString remoteto)
  {
    LOG(Debug, "Receiving filePutFinished (" << toString(localfrom) << " to " << toString(remoteto) << ") in thread: " << QThread::currentThreadId());
    emit putFileFinished(id, toPath(localfrom), toPath(remoteto), m_put_timedout);
  }

  void SSHConnection::filePutTimeout(int id, QString remotefrom, QString localto)
  {
    m_put_timedout = true;
  }

  void SSHConnection::execCommand(int t_id, const std::string &t_command, int t_timeout_msecs)
  {
    LOG(Debug, "Starting remote command: " << t_command << " with timeout " << t_timeout_msecs);
    QMutexLocker l(&m_mutex);
    if (m_exec_thread)
    {
      if (m_exec_thread->isRunning())
      {
        throw std::runtime_error("Already running execute");
      } else {
        QObject::disconnect(m_exec_thread.get(), 0, 0, 0);
        m_exec_thread.reset();
      }
    }

    m_command_timedout = false;

    m_exec_thread = boost::shared_ptr<detail::ExecThread>(new detail::ExecThread(
          t_id, m_session, &m_mutex, t_command, t_timeout_msecs));

    QObject::connect(m_exec_thread.get(), SIGNAL(dataRead(int, QString, QString)),
            this, SLOT(execDataRead(int, QString, QString)));
    QObject::connect(m_exec_thread.get(), SIGNAL(errDataRead(int, QString, QString)),
            this, SLOT(execErrDataRead(int, QString, QString)));
    QObject::connect(m_exec_thread.get(), SIGNAL(finished(int, QString)),
        this, SLOT(execFinished(int, QString)));
    QObject::connect(m_exec_thread.get(), SIGNAL(timeout(int, QString)),
        this, SLOT(execTimeout(int, QString)));

    m_exec_thread->start();
  }

  void SSHConnection::execOOBCommand(int t_id, const std::string &t_command, int t_timeout_msecs)
  {
    LOG(Debug, "Starting OOB remote command: " << t_command << " with timeout " << t_timeout_msecs);
    QMutexLocker l(&m_mutex);
    if (m_oob_exec_thread)
    {
      if (m_oob_exec_thread->isRunning())
      {
        throw std::runtime_error("Already running OOB execute");
      } else {
        QObject::disconnect(m_oob_exec_thread.get(), 0, 0, 0);
        m_oob_exec_thread.reset();
      }
    }

    m_oob_command_timedout = false;

    m_oob_exec_thread = boost::shared_ptr<detail::ExecThread>(new detail::ExecThread(
          t_id, m_session, &m_mutex, t_command, t_timeout_msecs));

    QObject::connect(m_oob_exec_thread.get(), SIGNAL(dataRead(int, QString, QString)),
            this, SLOT(execOOBDataRead(int, QString, QString)));
    QObject::connect(m_oob_exec_thread.get(), SIGNAL(errDataRead(int, QString, QString)),
            this, SLOT(execOOBErrDataRead(int, QString, QString)));
    QObject::connect(m_oob_exec_thread.get(), SIGNAL(finished(int, QString)),
        this, SLOT(execOOBFinished(int, QString)));
    QObject::connect(m_oob_exec_thread.get(), SIGNAL(timeout(int, QString)),
        this, SLOT(execOOBTimeout(int, QString)));

    m_oob_exec_thread->start();
  }


  void SSHConnection::getFile(int t_id, const openstudio::path &t_fromremote, const openstudio::path &t_tolocal, int t_timeout_msecs)
  {
    QMutexLocker l(&m_mutex);
    if (m_get_file_thread)
    {
      if (m_get_file_thread->isRunning())
      {
        throw std::runtime_error("Already running get file operation");
      } else {
        QObject::disconnect(m_get_file_thread.get(), 0, 0, 0);
        m_get_file_thread.reset();
      }
    }

    LOG(Debug, "getFile (" << toString(t_fromremote) << " to " << toString(t_tolocal) << ")");

    m_get_timedout  = false;

    m_get_file_thread = boost::shared_ptr<detail::GetFileThread>(new detail::GetFileThread(
          t_id, m_session, &m_mutex, t_fromremote, t_tolocal, t_timeout_msecs));

    QObject::connect(m_get_file_thread.get(), SIGNAL(finished(int, QString, QString)),
        this, SLOT(fileGetFinished(int, QString, QString)));
    QObject::connect(m_get_file_thread.get(), SIGNAL(timeout(int, QString, QString)),
        this, SLOT(fileGetTimeout(int, QString, QString)));

    m_get_file_thread->start();
  }


  void SSHConnection::putFile(int t_id, const openstudio::path &t_fromlocal, const openstudio::path &t_toremote, int t_timeout_msecs)
  {
    QMutexLocker l(&m_mutex);
    if (m_put_file_thread)
    {
      if (m_put_file_thread->isRunning())
      {
        throw std::runtime_error("Already running put file operation");
      } else {
        QObject::disconnect(m_put_file_thread.get(), 0, 0, 0);
        m_put_file_thread.reset();
      }
    }

    LOG(Debug, "putFile (" << toString(t_fromlocal) << " to " << toString(t_toremote) << ")");

    m_put_timedout  = false;

    m_put_file_thread = boost::shared_ptr<detail::PutFileThread>(new detail::PutFileThread(
          t_id, m_session, &m_mutex, t_fromlocal, t_toremote, t_timeout_msecs));

    QObject::connect(m_put_file_thread.get(), SIGNAL(finished(int, QString, QString)),
        this, SLOT(filePutFinished(int, QString, QString)));
    QObject::connect(m_put_file_thread.get(), SIGNAL(timeout(int, QString, QString)),
        this, SLOT(filePutTimeout(int, QString, QString)));

    m_put_file_thread->start();
  }


namespace detail {

  /**
   * EXECTHREAD
   */

  ExecThread::ExecThread(int t_id, ssh_session t_session, QMutex *t_mutex,
      const std::string &t_command, int t_timeout_msecs)
    : m_id(t_id), m_session(t_session), m_mutex(t_mutex), m_command(t_command), m_timeout_msecs(t_timeout_msecs),
      m_error_occurred(false)
  {
//    QMutexLocker l(m_mutex);

    m_channel = channel_new(m_session);
    if (m_channel == NULL)
    {
      throw std::runtime_error("Unable to allocate channel for SSH command execution");
    }

    int rc = channel_open_session(m_channel);
    if (rc < 0)
    {
      channel_close(m_channel);
      channel_free(m_channel);
      m_channel = 0;
      throw std::runtime_error("Unable to open session for channel for SSH command execution");
    }

    rc = channel_request_exec(m_channel, m_command.c_str());
    if (rc < 0)
    {
      channel_close(m_channel);
      channel_free(m_channel);
      m_channel = 0;
      throw std::runtime_error("Unable to create SSH command execution");
    }

    // m_channel is all set up now to read from, it has begun command execution
  }

  ExecThread::~ExecThread()
  {

    while (!wait(10))
    {
      quit();
    }

    //QMutexLocker l(m_mutex);
    channel_close(m_channel);
    channel_free(m_channel);
  }

  void ExecThread::run()
  {
    boost::posix_time::ptime start = boost::posix_time::microsec_clock::universal_time();

    bool run = true;

    while (run)
    {
      std::vector<char> buffer(10000);
      std::vector<char> errbuffer(10000);

      int bytesread = 0;
      int errbytesread = 0;

      int bytesstillavail = 0;
      int errbytesstillavail = 0;

      { // Scope for mutex lock
        QMutexLocker l(m_mutex);
        if (channel_is_open(m_channel))
        {
//          LOG(Debug, "channel is open");
          const int bytesavail = channel_poll(m_channel, false);
          if (bytesavail == SSH_ERROR)
          {
            run = false;
            m_error_occurred = true;
          } else if (bytesavail > 0) {
            bytesread = channel_read(m_channel, &buffer.front(), std::min(bytesavail, static_cast<int>(buffer.size())), false);
            bytesstillavail = bytesavail - bytesread;
          }

          // and now for stderr
          const int errbytesavail = channel_poll(m_channel, true);
          if (errbytesavail == SSH_ERROR)
          {
            run = false;
            m_error_occurred = true;
          } else if (errbytesavail > 0) {
            errbytesread = channel_read(m_channel, &errbuffer.front(), std::min(errbytesavail, static_cast<int>(errbuffer.size())), true);
            errbytesstillavail = errbytesavail - errbytesread;
          }
        }

        if (channel_is_eof(m_channel))
        {
          LOG(Debug, "channel is eof, stopping");
          run = false;
        }
      }

      assert(bytesread <= static_cast<int>(buffer.size()) && "channel_read overran our buffer");
      assert(errbytesread <= static_cast<int>(errbuffer.size()) && "channel_read overran our err buffer");

      if (bytesread > 0)
      {
        LOG(Debug, "emitting dataRead from thread: " << QThread::currentThreadId());
        emit dataRead(m_id, openstudio::toQString(m_command),
            openstudio::toQString(std::string(&buffer.front(), buffer.size())));
      }

      if (errbytesread > 0)
      {
        LOG(Debug, "emitting errDataRead from thread: " << QThread::currentThreadId());
        emit errDataRead(m_id, openstudio::toQString(m_command),
            openstudio::toQString(std::string(&errbuffer.front(), errbuffer.size())));
      }


      if (!run)
      {
        break;
      }


      const int timeleft = m_timeout_msecs -
        (boost::posix_time::microsec_clock::universal_time() - start).total_milliseconds();

//      LOG(Debug, "timeleft: " << timeleft);

      if (timeleft > 0)
      {
        // Only sleep if there's no data available
        // If data is available, we want the loop to fall through, regain the lock and grab some more data
        if (bytesstillavail <= 0 && errbytesstillavail <= 0)
        {
          const int sleeptime = std::min(timeleft, 5);
          QThread::msleep(sleeptime);
        }
      } else {
        LOG(Debug, "emitting timeout from thread: " << QThread::currentThreadId());
        emit timeout(m_id, openstudio::toQString(m_command));
        run = false;
      }
    }

    LOG(Debug, "emitting finished from thread: " << QThread::currentThreadId());
    emit finished(m_id, openstudio::toQString(m_command));

    QThread::exit();
  }



  /**
   * GETFILETHREAD
   */

  void GetFileThread::handle_error(const std::string &desc, ssh_session t_session, sftp_session t_file)
  {
    std::string err = ssh_get_error(t_session);
    sftp_free(t_file);
    throw std::runtime_error(desc + ": " + err);
  }


  GetFileThread::GetFileThread(int t_id, ssh_session t_session, QMutex *t_mutex,
      const openstudio::path &t_fromremotepath, const openstudio::path &t_tolocalpath, int t_timeout_msecs)
    : m_id(t_id), m_session(t_session), m_mutex(t_mutex), m_fromremote(t_fromremotepath),
      m_tolocal(t_tolocalpath), m_timeout_msecs(t_timeout_msecs),
      m_error_occurred(false),
      m_outfile(openstudio::toString(m_tolocal).c_str(), std::ios_base::trunc | std::ios_base::out | std::ios_base::binary)
  {
    if (m_outfile.fail())
    {
      throw std::runtime_error("Unable to open output file");
    }

  //  QMutexLocker l(m_mutex);

    m_sftp_session = sftp_new(m_session);

    if (m_sftp_session == NULL)
    {
      throw std::runtime_error("Unable to create sftp_session for ssh_connection");
    }

    if (sftp_init(m_sftp_session) < 0)
    {
      handle_error("Unable to init sftp session", m_session, m_sftp_session);
    }

    m_sftp_file = sftp_open(m_sftp_session, toString(m_fromremote).c_str(), O_RDONLY, 0);

    if (m_sftp_file == NULL)
    {
      handle_error("Unable to open remote sftp file for reading", m_session, m_sftp_session);
    }
    // m_sftp_file is all set up now to read from
  }

  GetFileThread::~GetFileThread()
  {
    while (!wait(10))
    {
      quit();
    }

    //QMutexLocker l(m_mutex);
    if (m_sftp_file) {
      sftp_close(m_sftp_file);
    }
    sftp_free(m_sftp_session);
  }

  void GetFileThread::run()
  {
    boost::posix_time::ptime start = boost::posix_time::microsec_clock::universal_time();

    bool run = true;

    while (run)
    {
      std::vector<char> buffer(100000);

      int bytesread = 0;

      { // Scope for mutex lock
        QMutexLocker l(m_mutex);

        bytesread = sftp_read(m_sftp_file, &buffer.front(), buffer.size());
      }

      assert(bytesread <= static_cast<int>(buffer.size()) && "sftp_read overran our buffer");

      if (bytesread < 0)
      {
        run = false;
        m_error_occurred = true;
      } else if (bytesread == 0) {
        //end of file?
        run = false;
        m_outfile.flush();
      } else if (bytesread > 0) {
        LOG(Debug, "bytes read: " << bytesread);

        m_outfile.write(&buffer.front(), bytesread);
      }

      //We've relinguished the lock and wrote out to the file, but data
      //is (presumably) still available, continue with the loop


      const int timeleft = m_timeout_msecs -
        (boost::posix_time::microsec_clock::universal_time() - start).total_milliseconds();

      if (timeleft <= 0)
      {
        // we've overstayed our welcome
        run = false;
        LOG(Debug, "emitting timeout from thread: " << QThread::currentThreadId());
        emit timeout(m_id, openstudio::toQString(m_fromremote), openstudio::toQString(m_tolocal));
      }
    }

    m_outfile.flush();
    m_outfile.close();

    LOG(Debug, "emitting finished from thread: " << QThread::currentThreadId());
    emit finished(m_id, openstudio::toQString(m_fromremote), openstudio::toQString(m_tolocal));

    QThread::exit();
  }

  /**
   * PUTFILETHREAD
   */

  void PutFileThread::handle_error(const std::string &desc, ssh_session t_session, sftp_session t_file)
  {
    std::string err = ssh_get_error(t_session);
    sftp_free(t_file);
    throw std::runtime_error(desc + ": " + err);
  }

  PutFileThread::PutFileThread(int t_id, ssh_session t_session, QMutex *t_mutex,
      const openstudio::path &t_fromlocalpath, const openstudio::path &t_toremotepath, int t_timeout_msecs)
    : m_id(t_id), m_session(t_session), m_mutex(t_mutex), m_fromlocal(t_fromlocalpath),
      m_toremote(t_toremotepath), m_timeout_msecs(t_timeout_msecs),
      m_error_occurred(false),
      m_infile(openstudio::toString(m_fromlocal).c_str(), std::ios_base::in | std::ios_base::binary)
  {
    if (!m_infile.good() || !m_infile.is_open())
    {
      throw std::runtime_error("Unable to open local input file");
    }

//    QMutexLocker l(m_mutex);

    m_sftp_session = sftp_new(m_session);

    if (m_sftp_session == NULL)
    {
      throw std::runtime_error("Unable to create sftp_session for ssh_connection");
    }

    if (sftp_init(m_sftp_session) < 0)
    {
      handle_error("error initializing sftp session", m_session, m_sftp_session);
    }

    m_sftp_file = sftp_open(m_sftp_session, toString(m_toremote).c_str(), O_WRONLY | O_TRUNC | O_CREAT, 0700);

    if (m_sftp_file == NULL)
    {
      handle_error("error opening sftp file for write", m_session, m_sftp_session);
    }
    // m_sftp_file is all set up now to read from
  }

  PutFileThread::~PutFileThread()
  {
    while (!wait(10))
    {
      quit();
    }

    //QMutexLocker l(m_mutex);
    if (m_sftp_file) {
      sftp_close(m_sftp_file);
    }
    sftp_free(m_sftp_session);
  }

  void PutFileThread::run()
  {
    boost::posix_time::ptime start = boost::posix_time::microsec_clock::universal_time();

    bool run = true;

    while (run && m_infile.good())
    {
      std::vector<char> buffer(100000);

      m_infile.read(&buffer.front(), buffer.size());

      int bytesread = m_infile.gcount();

      if (bytesread > 0) { // Scope for mutex lock
        QMutexLocker l(m_mutex);

        int byteswritten = 0;
        while (byteswritten < bytesread)
        {
          const int timeleft = m_timeout_msecs -
            (boost::posix_time::microsec_clock::universal_time() - start).total_milliseconds();

          if (timeleft <= 0)
          {
            // we've overstayed our welcome
            run = false;
            LOG(Debug, "emitting timeout from thread: " << QThread::currentThreadId());
            emit timeout(m_id, openstudio::toQString(m_fromlocal), openstudio::toQString(m_toremote));
            break;
          }

          int written = sftp_write(m_sftp_file, &buffer.front(), bytesread);
          if (written < 0)
          {
            m_error_occurred = true;
          } else {
            byteswritten += written;
          }

        }
      }
    }

    {
      QMutexLocker l(m_mutex);
      sftp_close(m_sftp_file);
    }

    m_sftp_file = NULL;
    LOG(Debug, "emitting finished from thread: " << QThread::currentThreadId());
    emit finished(m_id, openstudio::toQString(m_fromlocal), openstudio::toQString(m_toremote));

    QThread::exit();
  }
} // namespace detail

  bool SSHConnection::idle()
  {
    QMutexLocker l(&m_mutex);
    if (m_exec_thread && !m_exec_thread->isRunning())
    {
      m_exec_thread.reset();
    }

    if (m_get_file_thread && !m_get_file_thread->isRunning())
    {
      m_get_file_thread.reset();
    }

    if (m_put_file_thread && !m_put_file_thread->isRunning())
    {
      m_put_file_thread.reset();
    }

    return !m_exec_thread && !m_get_file_thread && !m_put_file_thread;
  }

  bool SSHConnection::oobIdle()
  {
    QMutexLocker l(&m_mutex);
    if (m_oob_exec_thread && !m_oob_exec_thread->isRunning())
    {
      m_oob_exec_thread.reset();
    }

    return !m_oob_exec_thread;
  }

 
#else

  SSHConnection::SSHConnection(const SSHCredentials &t_creds, bool /*t_allow_ui*/ )
    : m_credentials(t_creds), m_command_timedout(false),
      m_get_timedout(false), m_put_timedout(false)
  {
  }

  SSHConnection::~SSHConnection()
  {
  }

  void SSHConnection::execCommand(int, const std::string &command, int t_timeout_msecs)
  {
  }

  void SSHConnection::execOOBCommand(int, const std::string &command, int t_timeout_msecs)
  {
  }


  void SSHConnection::filePutTimeout(int, QString,QString)
  {
  }

  void SSHConnection::filePutFinished(int, QString,QString)
  {
  }

  void SSHConnection::fileGetTimeout(int, QString,QString)
  {
  }

  void SSHConnection::fileGetFinished(int, QString,QString)
  {
  }

  void SSHConnection::execTimeout(int, QString)
  {
  }

  void SSHConnection::execFinished(int, QString)
  {
  }

  void SSHConnection::execDataRead(int, QString,QString)
  {
  }

  void SSHConnection::execErrDataRead(int, QString,QString)
  {
  }

  void SSHConnection::execOOBTimeout(int, QString)
  {
  }

  void SSHConnection::execOOBFinished(int, QString)
  {
  }

  void SSHConnection::execOOBDataRead(int, QString,QString)
  {
  }

  void SSHConnection::execOOBErrDataRead(int, QString,QString)
  {
  }

  SSHCredentials SSHConnection::getCredentials() const
  {
    return m_credentials;
  }

  void SSHConnection::getFile(int, const openstudio::path&, const openstudio::path&, int)
  {
  }

  void SSHConnection::putFile(int, const openstudio::path&, const openstudio::path&, int)
  {
  }

  bool SSHConnection::idle()
  {
    return true;
  }

  bool SSHConnection::oobIdle()
  {
    return true;
  }

#endif




}
}

