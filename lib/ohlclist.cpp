/*
 * Copyright (c) 2021  Faraz Fallahi <fffaraz@gmail.com>
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file
 */

#include "ohlclist.hpp"
#include "utils.hpp"

#include <QDebug>

#include <fstream>
#include <iomanip>
#include <iostream>

using namespace Farazlib;

namespace {

auto loadData(const CsvFile& csv)
{
    std::cout << "OhlcList::loadData " << csv.filePath() << std::endl;
    const auto& data = csv.data();

    OhlcList::OhlcVector result;
    result.reserve(data.size());

    const auto maxDateTime = Utils::toDateTime("2021-09-03"); // TODO: parameterize
    const auto minDateTime = Utils::toDateTime("2000-01-01"); // TODO: parameterize

    for (auto itr = data.rbegin(); itr != data.rend(); ++itr) {
        Ohlc item { *itr };

        if (!item.isValid) {
            continue;
        }
        if (item.datetime > maxDateTime) {
            // std::cout << "OhlcList::loadData [ignoring] " << itr->at(0) << std::endl;
            continue;
        }
        if (item.datetime < minDateTime) {
            break;
        }

        // Fill missing dates with last record
        if (!result.empty()) {
            int missingDays = 0;
            const auto lastRecord = result.at(result.size() - 1);
            for (auto date = lastRecord.datetime.addDays(-1); date > item.datetime; date = date.addDays(-1)) {
                auto missingRecord = lastRecord;
                missingRecord.datetime = date;
                missingRecord.isFake = true;
                result.push_back(missingRecord);
                missingDays++;
            }
            if (false && missingDays > 0) {
                std::cout << "OhlcList::loadData [missing] " << itr->at(0) << " " << missingDays << std::endl;
            }
        }

        result.push_back(item);
    }
    assert(!result.empty());
    return result;
}

} // anonymous namespace

OhlcList::OhlcList(double price)
    : m_data { Ohlc { price } }
    , m_timeFrame { OhlcTimeFrame::Daily }
{
}

OhlcList::OhlcList(OhlcList::OhlcVector data)
    : m_data { std::move(data) }
    , m_timeFrame { OhlcTimeFrame::Daily }
{
}

OhlcList::OhlcList(const CsvFile& csv, OhlcTimeFrame timeFrame)
    : m_data { loadData(csv) }
    , m_timeFrame { timeFrame }
{
}

const Ohlc& OhlcList::at(size_t i) const
{
    assert(!m_data.empty());
    if (i >= m_data.size()) {
        i = m_data.size() - 1;
    }
    return m_data.at(i);
}

void OhlcList::save(const std::string& filePath) const
{
    std::ofstream outFile(filePath, std::ios::out | std::ios::trunc);
    if (!outFile.is_open()) {
        std::cout << "OhlcList::save [FAILED TO OPEN FILE] " << filePath << std::endl;
    }

    outFile << "Date,Open,High,Low,Close,Volume,Dividends,Stock Splits,isFake\n";

    outFile.setf(std::ios::fixed);
    // outFile.precision(8);

    for (auto itr = m_data.rbegin(); itr != m_data.rend(); ++itr) {
        outFile << itr->datetime.toString("yyyy-MM-dd").toStdString() << ",";
        outFile << itr->open << ",";
        outFile << itr->high << ",";
        outFile << itr->low << ",";
        outFile << itr->close << ",";
        outFile << std::fixed << std::noshowpoint << itr->volume << ",";
        outFile << std::noshowpoint << itr->dividends << ",";
        outFile << std::noshowpoint << itr->splits << ",";
        outFile << itr->isFake << "\n";
    }
}

PriceDirection OhlcList::priceDirection(size_t i, size_t offset) const
{
    assert(i + offset < m_data.size());

    if (offset == 0) { // Same day case
        const auto& today = m_data.at(i);
        return today.close > today.open ? PriceDirection::Up : PriceDirection::Down;
    }

    const auto& today = m_data.at(i);
    const auto& yesterday = m_data.at(i + offset);

    if (today.high >= yesterday.high && today.low >= yesterday.high) {
        return PriceDirection::VeryUp;
    }
    if (today.high <= yesterday.low && today.low < yesterday.low) {
        return PriceDirection::VeryDown;
    }
    if (today.high <= yesterday.high && today.low >= yesterday.low) {
        return PriceDirection::Narrow;
    }
    if (today.high > yesterday.high && today.low < yesterday.low) {
        return PriceDirection::Widen;
    }
    if (today.high >= yesterday.high) {
        return PriceDirection::Up;
    }
    if (today.low <= yesterday.low) {
        return PriceDirection::Down;
    }

    std::cout << "OhlcList::priceDirection [PriceDirection::Invalid] "
              << today.to_string() << "\t" << yesterday.to_string()
              << std::endl;
    assert(false);
    return PriceDirection::Narrow; // Invalid
}

double OhlcList::priceChange(size_t i, size_t offset) const
{
    assert(i < m_data.size());

    if (offset == 0) { // Same day case
        const auto& today = at(i);
        return (today.close - today.open) / today.open;
    }

    const auto today = at(i).hl2();
    const auto yesterday = at(i + offset).hl2();
    assert(yesterday > 0);
    return (today - yesterday) / yesterday;
}

std::vector<double> OhlcList::vector(size_t size, size_t offset, PriceType type) const
{
    assert(offset + size <= m_data.size());
    std::vector<double> result;
    if (offset + size > m_data.size()) {
        return result;
    }
    result.reserve(size);
    for (size_t i = 0; i < size; ++i) {
        result.push_back(m_data.at(offset + i).get(type));
    }
    return result;
}

double OhlcList::allTimeHigh() const
{
    double result {};
    for (const auto& item : m_data) {
        if (item.high > result) {
            result = item.high;
        }
    }
    return result;
}

double OhlcList::percentFromAth() const
{
    const double lastPrice = m_data.at(0).close;
    const double ath = allTimeHigh();
    assert(ath > 0);
    assert(ath >= lastPrice);
    return (ath - lastPrice) / ath * 100;
}

double OhlcList::percentToAth() const
{
    const double lastPrice = m_data.at(0).close;
    assert(lastPrice > 0);
    const double ath = allTimeHigh();
    assert(ath >= lastPrice);
    return (ath - lastPrice) / lastPrice * 100;
}

double OhlcList::avgReturn(size_t length) const
{
    assert(!m_data.empty());
    // assert(length < m_data.size());

    if (length >= m_data.size()) {
        length = m_data.size() - 1;
    }

    double result {};
    const size_t size = m_data.size() - length;
    for (size_t i = 0; i < size; ++i) {
        result += priceChange(i, length);
    }
    return result / size;
}

double OhlcList::avgRisk(size_t length) const
{
    assert(!m_data.empty());
    // assert(length < m_data.size());

    if (length >= m_data.size()) {
        length = m_data.size() - 1;
    }

    const size_t size = m_data.size() - length;
    std::vector<double> vector;
    vector.reserve(size);

    for (size_t i = 0; i < size; ++i) {
        vector.push_back(priceChange(i, length));
    }

    return Utils::stdDev(vector);
}

bool OhlcList::matchDatetime(const OhlcList& other, size_t maxSize) const
{
    assert(maxSize <= m_data.size());
    assert(maxSize <= other.m_data.size());
    for (size_t i = 0; i < maxSize; ++i) {
        const auto& date1 = m_data.at(i).datetime;
        const auto& date2 = other.m_data.at(i).datetime;
        if (date1 != date2) {
            qDebug() << "OhlcList::matchDatetime [datetime mismatch]" << i
                     << date1.toString("yyyy-MM-dd") << date2.toString("yyyy-MM-dd");
            return false;
        }
    }
    return true;
}
