//
// Created by richard on 30/01/23.
//

/*
 * Api.cpp Created by Richard Buckley (C) 30/01/23
 */

/**
 * @file Api.cpp
 * @author Richard Buckley <richard.buckley@ieee.org>
 * @version 1.0
 * @date 30/01/23
 */

#include <Info.hpp>
#include "Api.h"
#include "fmt/format.h"
#include "nlohmann/json.hpp"

namespace ecoBee {
    void runtimeReport(const std::string& token) {
        std::stringstream response{};
        cURLpp::Cleanup cleaner;
        cURLpp::Easy request;

        auto url = fmt::format(R"({}{}{}{}{}{}{})",
                               R"(https://api.ecobee.com/1/runtimeReport?format=json&body={"startDate":")",
                               R"(2023-01-31","startInterval":"10)",
                               R"(","endDate":")",
                               R"(2023-01-31","endInterval":"20)",
                               R"(","columns":"auxHeat1,compCool1,fan,zoneHeatTemp,zoneCoolTemp,outdoorTemp,outdoorHumidity,wind,hvacMode,zoneHvacMode,zoneClimate,zoneAveTemp","includeSensors":true,)"
                               R"("selection":{"selectionType":"thermostats","selectionMatch":")",
                               Thermostat,
                               R"("}})");
//        auto url = R"(https://api.ecobee.com/1/runtimeReport?format=json&body=\{"startDate":"2015-01-01","endDate":"2015-01-03","columns":"auxHeat1,compCool1,fan,outdoorTemp,zoneAveTemp","selection":\{"selectionType":"thermostats","selectionMatch":"318324702718"\}\}')"
        std::cout << url << '\n';
        request.setOpt(new cURLpp::Options::Url(url));
        request.setOpt(new cURLpp::Options::Verbose(false));

        std::list<std::string> header;
        header.emplace_back("content-type: text/json;charset=UTF-8");
        auto authHeader = fmt::format("Authorization: Bearer {}", token);
        header.emplace_back(authHeader);
        request.setOpt(new cURLpp::Options::HttpHeader(header));

        request.setOpt(new curlpp::options::WriteStream(&response));

        request.perform();
        [[maybe_unused]] auto code = curlpp::infos::ResponseCode::get(request);
        auto report = nlohmann::json::parse(response);
        std::cout << report.dump(4) << '\n';
    }

    void thermostat(const std::string &token) {
        std::stringstream response{};
        cURLpp::Cleanup cleaner;
        cURLpp::Easy request;

        auto url = R"(https://api.ecobee.com/1/thermostat?format=json&body={"selection":{"selectionType":"registered",)"
                   R"(selectionMatch":"","includeRuntime":true}})";

        request.setOpt(new cURLpp::Options::Url(url));
        request.setOpt(new cURLpp::Options::Verbose(false));

        std::list<std::string> header;
        header.emplace_back("content-type: text/json");
        auto authHeader = fmt::format("Authorization: Bearer {}", token);
        header.emplace_back(authHeader);
        request.setOpt(new cURLpp::Options::HttpHeader(header));

        request.setOpt(new curlpp::options::WriteStream(&response));

        request.perform();

        auto thermostat = nlohmann::json::parse(response);
//        std::cout << thermostat.dump(4) << '\n';
    }

    void refreshAccessToken(nlohmann::json &accessToken, const std::string &url, const std::string &apiKey,
                            const std::string &token) {
        auto postData = fmt::format("grant_type=refresh_token&&code={}&client_id={}", token, apiKey);
        try {
            std::stringstream response{};
            cURLpp::Cleanup cleaner;
            cURLpp::Easy request;
            request.setOpt(new cURLpp::Options::Url(url));
            request.setOpt(new cURLpp::Options::Verbose(false));

            std::list<std::string> header;
            header.emplace_back("Content-Type: application/x-www-form-urlencoded");
            request.setOpt(new cURLpp::Options::HttpHeader(header));

            request.setOpt(new cURLpp::Options::PostFieldSize(static_cast<long>(postData.length())));
            request.setOpt(new cURLpp::Options::PostFields(postData));
            request.setOpt(new curlpp::options::WriteStream(&response));

            request.perform();

            accessToken = nlohmann::json::parse(response);
        } catch (cURLpp::LogicError &e) {
            std::cerr << e.what() << '\n';
        } catch (cURLpp::RuntimeError &e) {
            std::cerr << e.what() << '\n';
        }
    }
} // ecoBee