/*
 * Copyright (c) 2021  Faraz Fallahi <fffaraz@gmail.com>
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file
 */

#include "OhlcList.hpp"
#include "Utils.hpp"

#include <cassert>
#include <fstream>
#include <iostream>

using namespace Farazlib;

namespace {

auto loadData(const CsvFile& csv)
{
    std::cerr << "OhlcList::loadData\n";
    const auto& data = csv.data();

    OhlcList::OhlcVector result;
    result.reserve(data.size());

    const auto maxDate = Utils::toTimePoint("2021-12-01"); // TODO(faraz): parameterize
    const auto minDate = Utils::toTimePoint("2010-01-01"); // TODO(faraz): parameterize

    for (auto itr = data.rbegin(); itr != data.rend(); ++itr) {
        const Ohlc item { *itr };

        if (!item.isValid) {
            continue;
        }
        if (item.timepoint > maxDate) {
            // std::cerr << "OhlcList::loadData [ignoring] " << itr->at(0) << "\n";
            continue;
        }
        if (item.timepoint < minDate) {
            break;
        }

        // Fill missing dates with last record
        const std::chrono::duration<int, std::ratio<86400>> one_day(1);
        if (!result.empty()) {
            int missingDays = 0;
            const auto lastRecord = result.at(result.size() - 1);
            for (auto date = lastRecord.timepoint - one_day; date > item.timepoint; date -= one_day) {
                auto missingRecord = lastRecord;
                missingRecord.timepoint = date;
                missingRecord.isFake = true;
                result.push_back(missingRecord);
                missingDays++;
            }
            constexpr bool logMissing = false;
            if (logMissing && missingDays > 0) {
                std::cerr << "OhlcList::loadData [missing] " << itr->at(0) << " " << missingDays << "\n";
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

size_t OhlcList::size() const noexcept
{
    return m_data.size();
}

const Ohlc& OhlcList::at(size_t i) const
{
    assert(!m_data.empty());
    if (i >= m_data.size()) {
        i = m_data.size() - 1;
    }
    return m_data.at(i);
}

void OhlcList::save(const std::filesystem::path& filePath) const
{
    std::ofstream outFile(filePath, std::ios::out | std::ios::trunc);
    assert(outFile.is_open());

    outFile << "Date,Open,High,Low,Close,Volume,Dividends,Stock Splits,isFake,";
    outFile << "priceChange,allTimeHigh,percentFromAth,percentToAth\n";

    outFile.setf(std::ios::fixed);
    // outFile.precision(8);

    const auto ath = allTimeHigh();
    const auto pfAth = percentFrom(ath);
    const auto ptAth = percentTo(ath);

    for (size_t i = 0; i < m_data.size(); ++i) {
        const auto& itr = m_data.at(i);
        outFile << Utils::to_string(itr.timepoint) << ",";
        outFile << itr.open << ",";
        outFile << itr.high << ",";
        outFile << itr.low << ",";
        outFile << itr.close << ",";
        outFile << std::fixed << std::noshowpoint << itr.volume << ",";
        outFile << std::noshowpoint << itr.dividends << ",";
        outFile << std::noshowpoint << itr.splits << ",";
        outFile << itr.isFake << ",";
        outFile << priceChange(i) << ",";
        outFile << ath.at(i) << ",";
        outFile << pfAth.at(i) << ",";
        outFile << ptAth.at(i) << "\n";
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

    std::cerr << "OhlcList::priceDirection [PriceDirection::Invalid] "
              << today.to_string() << "\t" << yesterday.to_string()
              << "\n";
    assert(false);
    return PriceDirection::Narrow; // Invalid
}

double OhlcList::priceChange(size_t i) const
{
    assert(i < m_data.size());
    const auto& today = at(i);
    assert(today.open > 0);
    return (today.high - today.low) / today.low;
}

double OhlcList::priceChange(size_t i, size_t offset, PriceType type) const
{
    assert(i < m_data.size());
    if (offset == 0) { // Same day case
        return priceChange(i);
    }
    const auto today = at(i).get(type);
    const auto yesterday = at(i + offset).get(type);
    assert(yesterday > 0);
    return (today - yesterday) / yesterday;
}

std::vector<double> OhlcList::toVector(size_t size, size_t offset, PriceType type) const
{
    assert(offset + size <= m_data.size());
    if (offset + size > m_data.size()) {
        return {};
    }
    std::vector<double> result;
    result.reserve(size);
    for (size_t i = 0; i < size; ++i) {
        result.push_back(m_data.at(offset + i).get(type));
    }
    return result;
}

double OhlcList::allTimeHigh(const size_t skip) const
{
    double result {};
    for (size_t i = skip; i < m_data.size(); ++i) {
        result = std::max(result, m_data[i].high);
    }
    return result;
}

std::vector<double> OhlcList::allTimeHigh() const
{
    double ath {};
    std::vector<double> result;
    result.resize(m_data.size());
    for (int64_t i = static_cast<int64_t>(m_data.size()) - 1; i >= 0; --i) {
        ath = std::max(ath, m_data[i].high);
        result[i] = ath;
    }
    return result;
}

double OhlcList::percentFromAth(const size_t i) const
{
    const double lastPrice = m_data.at(i).low;
    const double ath = allTimeHigh(i);
    assert(ath > 0);
    assert(ath >= lastPrice);
    return (lastPrice - ath) / ath * 100;
}

std::vector<double> OhlcList::percentFrom(const std::vector<double>& ath) const
{
    std::vector<double> result;
    result.reserve(m_data.size());
    for (size_t i = 0; i < m_data.size(); ++i) {
        const double lastPrice = m_data.at(i).low;
        assert(ath[i] >= lastPrice);
        result.push_back((lastPrice - ath[i]) / ath[i] * 100);
    }
    return result;
}

double OhlcList::percentToAth(const size_t i) const
{
    const double lastPrice = m_data.at(i).low;
    assert(lastPrice > 0);
    const double ath = allTimeHigh(i);
    assert(ath >= lastPrice);
    return (ath - lastPrice) / lastPrice * 100;
}

std::vector<double> OhlcList::percentTo(const std::vector<double>& ath) const
{
    std::vector<double> result;
    result.reserve(m_data.size());
    for (size_t i = 0; i < m_data.size(); ++i) {
        const double lastPrice = m_data.at(i).low;
        assert(ath[i] >= lastPrice);
        result.push_back((ath[i] - lastPrice) / lastPrice * 100);
    }
    return result;
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
        result += priceChange(i, length, PriceType::HL2);
    }
    return result / static_cast<double>(size);
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
        vector.push_back(priceChange(i, length, PriceType::HL2));
    }

    return Utils::stdDev(vector);
}

bool OhlcList::matchTimePoint(const OhlcList& other, size_t maxSize) const
{
    assert(maxSize <= m_data.size());
    assert(maxSize <= other.m_data.size());
    for (size_t i = 0; i < maxSize; ++i) {
        const auto& date1 = m_data.at(i).timepoint;
        const auto& date2 = other.m_data.at(i).timepoint;
        if (date1 != date2) {
            std::cerr << "OhlcList::matchTimePoint [timepoint mismatch]" << i
                      << Utils::to_string(date1) << Utils::to_string(date2);
            return false;
        }
    }
    return true;
}
