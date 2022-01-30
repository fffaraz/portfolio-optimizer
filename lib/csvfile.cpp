/*
 * Copyright (c) 2021  Faraz Fallahi <fffaraz@gmail.com>
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file
 */

#include "csvfile.hpp"

#include <algorithm> // For: std::remove
#include <fstream> // For: std::ifstream
#include <iostream> // For: std::cout
#include <sstream> // For: std::istringstream

using namespace Farazlib;

CsvFile::CsvFile(std::string filePath, bool hasHeader, uint64_t maxRecords)
    : m_filePath { std::move(filePath) }
{
    std::cout << "CsvFile::CsvFile " << m_filePath << std::endl;
    loadFile(hasHeader, maxRecords);
}

void CsvFile::loadFile(bool hasHeader, uint64_t maxRecords)
{
    std::ifstream inFile { m_filePath };
    if (!inFile.is_open()) {
        std::cout << "CsvFile::loadFile [Failed to open file] " << m_filePath << std::endl;
    }

    constexpr char delimiter = ',';
    constexpr char quotation = '"';

    if (maxRecords > 0) {
        m_data.reserve(maxRecords);
    } else {
        m_data.reserve(100'000);
    }

    std::vector<std::string> items;
    items.reserve(10); // number of columns

    std::string line;
    line.reserve(100); // chars in a line

    std::string item;
    item.reserve(100); // chars in a column

    int lineNum {};

    while (std::getline(inFile, line)) {
        lineNum++;
        std::istringstream iss { line };

        items.clear();
        while (std::getline(iss, item, delimiter)) {
            item.erase(std::remove(item.begin(), item.end(), quotation), item.end());
            items.push_back(item);
        }

        if (hasHeader && lineNum == 1) {
            m_header = items;
        } else {
            m_data.push_back(items);
        }
    }
}
