#include "AuthGuard.h"

#include <drogon/drogon.h>
#include <string>

namespace karrenkontrolle::server::middleware
{
    void registerAuthGuard()
    {
        drogon::app().registerPreRoutingAdvice(
            [](const drogon::HttpRequestPtr &request,
               drogon::AdviceCallback &&callback,
               drogon::AdviceChainCallback next)
            {
                const auto &path = request->path();

                // TODO Remove and Replace with auth check... Demo!!!! Only rejects reqeusts based on path. if path contains 5, then reject.
                if (path.find('5') != std::string::npos)
                {
                    Json::Value error;
                    error["message"] = "Unauthorized! No 5s allowed. Just go home...";

                    auto response = drogon::HttpResponse::newHttpJsonResponse(error);

                    response->setStatusCode(drogon::k401Unauthorized);

                    callback(response);
                    return;
                }
                next();
            });
    }
}