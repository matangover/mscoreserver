/**
  @file
  @author Stefan Frings
*/

#ifndef HTTPREQUESTHANDLER_H
#define HTTPREQUESTHANDLER_H

#include "httprequest.h"
#include "httpresponse.h"

/**
   The request handler generates a response for each HTTP request. Web Applications
   usually have one central request handler that maps incoming requests to several
   controllers (servlets) based on the requested path.
   <p>
   You need to override the service() method or you will always get an HTTP error 501.
   <p>
   @warning Be aware that the main request handler instance must be created on the heap and
   that it is used by multiple threads simultaneously.
   @see StaticFileController which delivers static local files.
*/

class HttpRequestHandler : public QObject {
      Q_OBJECT
      Q_DISABLE_COPY(HttpRequestHandler)

   public:
      HttpRequestHandler(QObject* parent = 0);
      virtual ~HttpRequestHandler();
      virtual void service(HttpRequest& request, HttpResponse& response);
      };

#endif // HTTPREQUESTHANDLER_H
