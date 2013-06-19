#ifndef OPENSTUDIO_URLSEARCHPATH
#define OPENSTUDIO_URLSEARCHPATH

#include <utilities/core/Path.hpp>
#include <utilities/idd/IddEnums.hxx>
#include <boost/optional.hpp>
#include <QUrl>

namespace openstudio
{
  class URLSearchPath
  {
    public:
      enum Relative
      {
        ToInputFile,
        ToCurrentWorkingDir
      };

      /// Create a new URLSearchPath.
      /// \param[in] t_url Url to search (local or remote, remote not yet implemented)
      /// \param[in] t_relativity If t_url is a relative path, it is evaluated relative to t_relativity.
      URLSearchPath(const QUrl &t_url, Relative t_relativity = ToInputFile)
        : m_url(t_url), m_relativity(t_relativity)
      {
      }

      /// Create a new URLSearchPath.
      /// \param[in] t_url Url to search (local or remote, remote not yet implemented)
      /// \param[in] t_relativity If t_url is a relative path, it is evaluated relative to t_relativity.
      /// \param[in] t_object_type Object type to apply this search to
      URLSearchPath(const QUrl &t_url, Relative t_relativity, IddObjectType t_object_type)
        : m_url(t_url), m_relativity(t_relativity), m_object_type(t_object_type)
      {
      }


      /// Create a new URLSearchPath.
      /// \param[in] t_path local path to search
      /// \param[in] t_relativity If t_url is a relative path, it is evaluated relative to t_relativity.
      URLSearchPath(const openstudio::path &t_path, Relative t_relativity = ToInputFile)
        : m_url(QUrl::fromLocalFile(toQString(t_path))), m_relativity(t_relativity)
      {
      }

      /// Create a new URLSearchPath.
      /// \param[in] t_path local path to search
      /// \param[in] t_relativity If t_url is a relative path, it is evaluated relative to t_relativity.
      /// \param[in] t_object_type Object type to apply this search to
      URLSearchPath(const openstudio::path &t_path, Relative t_relativity, IddObjectType t_object_type)
        : m_url(QUrl::fromLocalFile(toQString(t_path))), m_relativity(t_relativity), m_object_type(t_object_type)
      {
      }


      QUrl getUrl() const
      {
        return m_url;
      }

      Relative getRelativity() const
      {
        return m_relativity;
      }

      boost::optional<IddObjectType> getIddObjectType() const
      {
        return m_object_type;
      }


    private:
      QUrl m_url;
      Relative m_relativity;
      boost::optional<IddObjectType> m_object_type;

  };

}


#endif
