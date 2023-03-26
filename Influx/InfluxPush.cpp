//
// Created by richard on 29/12/22.
//

/*
 * InfluxPush.cpp Created by Richard Buckley (C) 29/12/22
 */

#include <ctime>
#include <cstring>
#include <iostream>
#include "InfluxPush.h"

bool InfluxPush::pushData() {
    std::stringstream buildUrl{};
    
    buildUrl << (connectTls ? "https" : "http")
             << "://" << influxHost << ':' << influxPort << "/write?db=" << influxDataBase;
    
    auto postData = measurements.str();

    if (!postData.empty())
        try {
            cURLpp::Cleanup cleaner;
            cURLpp::Easy request;
            request.setOpt(new cURLpp::Options::Url(buildUrl.str()));
            request.setOpt(new cURLpp::Options::Verbose(false));
            
            std::list<std::string> header;
            header.emplace_back("Content-Type: application/octet-stream");
            request.setOpt(new cURLpp::Options::HttpHeader(header));
            request.setOpt(new cURLpp::Options::PostFieldSize(static_cast<long>(postData.length())));
            request.setOpt(new cURLpp::Options::PostFields(postData));

            request.perform();
        } catch (cURLpp::LogicError &e) {
            std::cerr << e.what() << '\n';
            return false;
        } catch (cURLpp::RuntimeError &e) {
            std::cerr << e.what() << '\n';
            return false;
        }
    return true;
}

void InfluxPush::setMeasurementEpoch(const std::string &date, const std::string &time) {
    std::tm localDateTime{};
    time_t epoch;
    ::time(&epoch);
    localDateTime = *(localtime(&epoch));

    struct tm epocDateTime{};
    memset(&epocDateTime, 0, sizeof(epocDateTime));

    std::string timeString{date};
    timeString.append("T").append(time).append(localDateTime.tm_zone);
    strptime(timeString.c_str(), TimeFmtISO.c_str(), &epocDateTime);
    epocDateTime.tm_gmtoff = localDateTime.tm_gmtoff;
    epocDateTime.tm_zone = localDateTime.tm_zone;
    epocDateTime.tm_isdst = localDateTime.tm_isdst;
    timeStamp = static_cast<unsigned long long>(mktime(&epocDateTime)) * 1000000000;
}

void InfluxPush::showData() {
    auto postData = measurements.str();
    std::cout << '\n' << postData << "\n" << std::endl;
}
