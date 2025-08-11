/*
 * Copyright (c) 2021  Faraz Fallahi <fffaraz@gmail.com>
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file
 */

#include "CsvFile.hpp"

#include <algorithm> // For: std::remove
#include <cassert>
#include <fstream> // For: std::ifstream
#include <iostream> // For: std::cerr
#include <sstream> // For: std::istringstream

using namespace portopt;

CsvFile::CsvFile(const std::filesystem::path& filePath, bool hasHeader)
{
    std::cerr << "CsvFile::CsvFile [filePath] " << filePath << "\n";

    std::ifstream file { filePath };
    assert(file.is_open());

    m_header.reserve(16); // reserve 16 columns
    m_data.reserve(100'000); // reserve 100,000 rows

    // line buffer
    std::string line;
    line.reserve(256);

    // cell buffer
    std::string cell;
    cell.reserve(128);

    // row buffer
    std::vector<std::string> row;
    row.reserve(16);

    int lineNum {};

    while (std::getline(file, line)) {
        lineNum++;
        std::istringstream iss { line };

        row.clear();
        while (std::getline(iss, cell, ',')) {
            cell.erase(std::remove(cell.begin(), cell.end(), '"'), cell.end()); // remove quotation marks
            cell.erase(std::remove(cell.begin(), cell.end(), '\r'), cell.end()); // remove carriage return
            cell.erase(std::remove(cell.begin(), cell.end(), '\n'), cell.end()); // remove newline
            row.push_back(cell);
        }

        if (hasHeader && lineNum == 1) {
            m_header = row; // first row is the header
        } else {
            m_data.push_back(row);
        }
    }
}
