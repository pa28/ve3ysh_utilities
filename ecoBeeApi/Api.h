//
// Created by richard on 30/01/23.
//

/*
 * Api.h Created by Richard Buckley (C) 30/01/23
 */

/**
 * @file Api.h
 * @author Richard Buckley <richard.buckley@ieee.org>
 * @version 1.0
 * @date 30/01/23
 * @brief 
 * @details
 */

#ifndef ECOBEEDATA_API_H
#define ECOBEEDATA_API_H

#include <string>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>
#include <curlpp/Infos.hpp>
#include <nlohmann/json.hpp>

namespace ecoBee {

    static constexpr std::string_view Thermostat = "421866388280";
    static constexpr std::string_view ApiKey = "fpsld2HqnigU3P4vnsuP5zZ8pLlpsfup";
    static constexpr std::string_view AppPin = "LVMP-KJRP";

    struct AccessToken {
        std::string mAccessToken{}, mTokenType{}, mScope{}, mRefreshToke{};
        uint32_t mExpiresIn{};
    };

    struct App {
        std::string mApiKey, mApiPin, mAuthCode;
        AccessToken mAccessToken{};
    };

    /**
     * @class Api
     */
    struct Api {
        std::string mApiKey, mApiPin, mAuthCode;
    };

    void runtimeReport(const std::string& token);

    void refreshAccessToken(nlohmann::json& accessToken, const std::string& url, const std::string& apiKey,
                            const std::string& token);

    void thermostat(const std::string &token);

} // ecoBee

#endif //ECOBEEDATA_API_H
