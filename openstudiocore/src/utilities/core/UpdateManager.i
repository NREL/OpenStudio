#ifndef UTILITIES_CORE_UPDATEMANAGER_I
#define UTILITIES_CORE_UPDATEMANAGER_I

%{
  #include <utilities/core/UpdateManager.hpp>
%}

%feature("director") UpdateManager;

// use replyProcessed
ignore UpdateManager::replyFinished(QNetworkReply* reply);

%include <utilities/core/UpdateManager.hpp>

#endif //UTILITIES_CORE_UPDATEMANAGER_I
