
#pragma once

#include <drogon/drogon.h>
#include <rfl/json.hpp>

#include <charconv>
#include <functional>
#include <string>
#include <type_traits>
#include <utility>

#include "JsonSerializer.hpp"
#include "HttpResult.hpp"
#include "HttpError.hpp"

namespace karrenkontrolle::server
{
    class Router
    {
        using Callback = std::function<void(const drogon::HttpResponsePtr &)>;

    public:
        explicit Router(std::string prefix = "") : prefix_(std::move(prefix))
        {
        }

        // GET with no path paarmeters
        template <typename Func>
        void get(const std::string &path, Func handler)
        {
            drogon::app().registerHandler(
                fullPath(path),
                [handler](const drogon::HttpRequestPtr &, Callback &&cb)
                {
                    reply(std::move(cb), [&]
                          { return handler(); });
                },
                {drogon::Get});
        }

        // GET with a single path parameter
        template <typename Id, typename Func>
        void get(const std::string &path, Func handler)
        {
            drogon::app().registerHandler(
                fullPath(path),
                [handler](const drogon::HttpRequestPtr &,
                          Callback &&cb,
                          const std::string &rawId)
                {
                    reply(std::move(cb), [&]
                          { return handler(parsePath<Id>(rawId)); });
                },
                {drogon::Get});
        }

        // POST with JSON body
        template <typename Body, typename Func>
        void post(const std::string &path, Func handler)
        {
            drogon::app().registerHandler(
                fullPath(path),
                [handler](const drogon::HttpRequestPtr &req,
                          Callback &&cb)
                {
                    reply(std::move(cb), [&]
                          { return handler(parseBody<Body>(req)); });
                },
                {drogon::Post});
        }

        // PUT with path parameter and JSON body
        template <typename Id, typename Body, typename Func>
        void put(const std::string &path, Func handler)
        {
            drogon::app().registerHandler(
                fullPath(path),
                [handler](const drogon::HttpRequestPtr &req,
                          Callback &&cb,
                          const std::string &rawId)
                {
                    reply(std::move(cb), [&]
                          { return handler(
                                parsePath<Id>(rawId),
                                parseBody<Body>(req)); });
                },
                {drogon::Put});
        }

        // DELETE with one path parameter
        template <typename Id, typename Func>
        void del(const std::string &path, Func handler)
        {
            drogon::app().registerHandler(
                fullPath(path),
                [handler](const drogon::HttpRequestPtr &,
                          Callback &&cb,
                          const std::string &rawId)
                {
                    reply(std::move(cb), [&]
                          { return handler(parsePath<Id>(rawId)); });
                },
                {drogon::Delete});
        }

    private:
        std::string prefix_;

        std::string fullPath(const std::string &path) const
        {
            return prefix_ + path;
        }

        // Convert a path parameter to a C++ type
        template <typename T>
        static T parsePath(const std::string &raw)
        {
            if constexpr (std::is_same_v<T, std::string>)
            {
                return raw;
            }
            else if constexpr (std::is_integral_v<T> &&
                               !std::is_same_v<T, bool>)
            {
                T value{};

                auto [ptr, ec] = std::from_chars(
                    raw.data(),
                    raw.data() + raw.size(),
                    value);

                if (ec != std::errc{} ||
                    ptr != raw.data() + raw.size())
                {
                    throw HttpError(400, "Invalid path parameter");
                }

                return value;
            }
            else
            {
                static_assert(std::is_same_v<T, std::string> ||
                                  std::is_integral_v<T>,
                              "Unsupported path parameter type");
            }
        }

        // Convert JSON request body to a C++ struct
        template <typename T>
        static T parseBody(const drogon::HttpRequestPtr &req)
        {
            auto result = rfl::json::read<T>(req->body());

            if (!result)
            {
                throw HttpError(400, result.error().what());
            }

            return std::move(result).value();
        }

        // Standard JSON response
        template <typename T>
        static drogon::HttpResponsePtr makeResponse(const T &data)
        {
            auto response = drogon::HttpResponse::newHttpResponse();

            response->setStatusCode(drogon::k200OK);
            response->setContentTypeCode(drogon::CT_APPLICATION_JSON);
            response->setBody(JsonSerializer<T>::serialize(data));

            return response;
        }

        // Response with custom status code
        template <typename T>
        static drogon::HttpResponsePtr makeResponse(
            const HttpResult<T> &result)
        {
            if (result.status == 204)
            {
                auto response = drogon::HttpResponse::newHttpResponse();
                response->setStatusCode(drogon::k204NoContent);
                return response;
            }

            auto response = makeResponse(result.body);

            response->setStatusCode(
                static_cast<drogon::HttpStatusCode>(result.status));

            return response;
        }

        // Error JSON response
        static drogon::HttpResponsePtr errorResponse(
            int status,
            const std::string &message)
        {
            Json::Value json;
            json["error"] = message;

            auto response =
                drogon::HttpResponse::newHttpJsonResponse(json);

            response->setStatusCode(
                static_cast<drogon::HttpStatusCode>(status));

            return response;
        }

        // Execute handler and convert result to HTTP
        template <typename Work>
        static void reply(Callback &&callback, Work &&work)
        {
            try
            {
                if constexpr (std::is_void_v<std::invoke_result_t<Work>>)
                {
                    std::forward<Work>(work)();

                    auto response =
                        drogon::HttpResponse::newHttpResponse();

                    response->setStatusCode(drogon::k204NoContent);
                    callback(response);
                }
                else
                {
                    auto result = std::forward<Work>(work)();
                    callback(makeResponse(result));
                }
            }
            catch (const HttpError &e)
            {
                callback(errorResponse(e.status(), e.what()));
            }
            catch (const std::exception &e)
            {
                LOG_ERROR << "Request failed: " << e.what();

                callback(errorResponse(
                    500,
                    "Internal server error"));
            }
        }
    };
}
