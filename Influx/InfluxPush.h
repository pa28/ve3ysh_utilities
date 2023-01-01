//
// Created by richard on 29/12/22.
//

/*
 * InfluxPush.h Created by Richard Buckley (C) 29/12/22
 */

#ifndef ECOBEEDATA_INFLUXPUSH_H
#define ECOBEEDATA_INFLUXPUSH_H


#include <string>
#include <utility>
#include <cmath>
#include <sstream>
#include <optional>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>

/**
 * @class InfluxPush
 *
 * https://archive.docs.influxdata.com/influxdb/v1.2/guides/writing_data/
 */
class InfluxPush {
private:
    const std::string TimeFmt{"%Y-%m-%dT%k:%M:%S%z"};   ///< Timestamp conversion format.
    unsigned long long timeStamp{};
    std::string influxHost{};
    bool connectTls{false};
    long influxPort{0};
    std::string influxDataBase{};

    std::stringstream measurements{};

public:
    InfluxPush() = delete;

    InfluxPush(std::string host, bool tls, long port, std::string dataBase)
        : influxHost(std::move(host)), connectTls(tls), influxPort(port), influxDataBase(std::move(dataBase)) {}

    /**
     * @brief Set measurement time stamp from a date, time and zone strings. See @refitem TimeFmt.
     * @param date The data string YYYY-MM-DD.
     * @param time The time string HH:MM:SS.
     * @param zone The zone string sHH:MM offset from GMT.
     */
    void setMeasurementEpoch(const std::string& date, const std::string& time, const std::string& zone);

    /**
     * @brief Get the current time stamp in nanoseconds from epoch.
     */
    [[maybe_unused]] [[nodiscard]] auto getMeasurementEpoch() const { return timeStamp; }

    /**
     * @brief Clear the measurements store.
     */
    void newMeasurements() {
        measurements.str("");
    }

    /**
     * @brief Add a measurement to the measurement store using the internal time stamp.
     * @param prefix The measurement prefix.
     * @param name The measurement value name.
     * @param value The measurement value.
     */
    bool addMeasurement(const std::string& prefix, const std::optional<std::string>& name, const std::optional<std::string>& value) {
        return addMeasurement(prefix, name, value, timeStamp);
    }

    /**
     * @brief Add a measurement to the measurement store using a specified time stamp in nanoseconds from epoch.
     * @param prefix The measurement prefix.
     * @param name The measurement value name.
     * @param value The measurement value.
     */
    bool addMeasurement(const std::string& prefix, const std::optional<std::string>& name,
                        const std::optional<std::string>& value, unsigned long long useTimeStamp) {
        if (name.has_value() && !name.value().empty() && value.has_value() && !value.value().empty()) {
            measurements << prefix << name.value() << '=' << value.value() << ' ' << useTimeStamp << '\n';
            return true;
        }
        return false;
    }

    /**
     * @brief Push the measurement store to the InfluxDB.
     * @return true if successful.
     */
    bool pushData();

    [[maybe_unused]] void showData();

    [[maybe_unused]] auto getMeasurements() const {
        return measurements.str();
    }
};


#endif //ECOBEEDATA_INFLUXPUSH_H

