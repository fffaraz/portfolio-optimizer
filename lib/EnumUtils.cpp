/*
 * Copyright (c) 2021  Faraz Fallahi <fffaraz@gmail.com>
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file
 */

#include "EnumUtils.hpp"

#include <iostream>
#include <unordered_map>

using namespace portopt;

std::set<AssetClass> EnumUtils::assetTag(const std::string& key)
{
    static const std::unordered_map<std::string, AssetClass> map {
        { "AGG", AssetClass::TotalBond }, // iShares Core U.S. Aggregate Bond ETF
        { "BND", AssetClass::TotalBond }, // Vanguard Total Bond Market Index Fund ETF Shares
        { "BIV", AssetClass::TotalBond }, // VANGUARD INTERMEDIATE-TERM BOND ETF
        { "BSV", AssetClass::TotalBond }, // VANGUARD SHORT-TERM BOND ETF
        { "ILTB", AssetClass::TotalBond }, //
        { "IUSB", AssetClass::TotalBond }, // iShares Core Total USD Bond Market ETF
        { "BLV", AssetClass::TotalBond }, // VANGUARD LONG-TERM BOND ETF
        { "IMTB", AssetClass::TotalBond }, //
        { "VMBS", AssetClass::TotalBond }, //
        { "ISTB", AssetClass::TotalBond }, //

        { "IAGG", AssetClass::IntlBond }, // iShares Core International Aggregate Bond ETF
        { "BNDX", AssetClass::IntlBond }, // Vanguard Total International Bond Index Fund ETF

        { "EDV", AssetClass::LongTermBond }, // Vanguard Extended Duration Treasury ETF
        { "TLT", AssetClass::LongTermBond }, //
        { "SPTL", AssetClass::LongTermBond }, //

        { "ITOT", AssetClass::TotalMarket }, // iShares Core S&P Total US Stock Market ETF
        { "VTI", AssetClass::TotalMarket }, // Vanguard Total Stock Market Index Fund ETF
        { "SCHB", AssetClass::TotalMarket }, // Schwab US Broad Market ETF
        { "IWV", AssetClass::TotalMarket }, // iShares Russell 3000 ETF
        { "VT", AssetClass::TotalMarket }, //

        { "SPY", AssetClass::SP500 }, // SPDR S&P 500 ETF Trust
        { "IVV", AssetClass::SP500 }, // iShares Core S&P 500 ETF
        { "VOO", AssetClass::SP500 }, // Vanguard 500 Index Fund ETF
        { "SPLG", AssetClass::SP500 }, // SPDR Portfolio S&P 500 ETF
        { "VV", AssetClass::SP500 }, // Vanguard Large-Cap Index Fund ETF
        { "RSP", AssetClass::SP500 }, // Invesco S&P 500 Eql Wght ETF

        { "VONE", AssetClass::Russell1000 }, // Vanguard Russell 1000 Index Fund ETF
        { "IWB", AssetClass::Russell1000 }, // iShares Russell 1000 ETF
        { "SCHK", AssetClass::Russell1000 }, // Schwab 1000 Index ETF
        { "SCHX", AssetClass::Russell1000 }, // Schwab US Large-Cap ETF
        { "SPTM", AssetClass::Russell1000 }, // SPDR Portfolio S&P 1500 Composite Stock Market ETF

        { "IXUS", AssetClass::TotalIntl }, // iShares Core MSCI Total International Stock ETF
        { "VXUS", AssetClass::TotalIntl }, // Vanguard Total International Stock Index Fund ETF
        { "VEU", AssetClass::TotalIntl }, // Vanguard FTSE All World ex US ETF
        { "VEA", AssetClass::TotalIntl }, // Vanguard Developed Markets Index Fund ETF
        { "IEUR", AssetClass::TotalIntl }, // iShares Core MSCI Europe ETF
        { "IPAC", AssetClass::TotalIntl }, //
        { "VSS", AssetClass::TotalIntl },
        { "VWO", AssetClass::TotalIntl },

        { "VTV", AssetClass::LargeValue }, // Vanguard Value Index Fund ETF
        { "MGV", AssetClass::LargeValue }, // Vanguard Mega Cap Value Index Fund ETF
        { "IUSV", AssetClass::LargeValue }, // iShares Core S&P US Value ETF
        { "DTD", AssetClass::LargeValue }, //
        { "DJD", AssetClass::LargeValue }, //
        { "VOE", AssetClass::LargeValue }, //

        { "IVW", AssetClass::LargeGrowth }, // iShares S&P 500 Growth ETF
        { "VUG", AssetClass::LargeGrowth }, // Vanguard Growth Index Fund ETF
        { "SCHG", AssetClass::LargeGrowth },
        { "IUSG", AssetClass::LargeGrowth }, // iShares Core S&P US Growth ETF
        { "MGK", AssetClass::LargeGrowth },
        { "QQQ", AssetClass::LargeGrowth },
        { "QQQE", AssetClass::LargeGrowth },
        { "VGT", AssetClass::LargeGrowth },
        { "VOT", AssetClass::LargeGrowth },
        { "VHT", AssetClass::LargeGrowth },

        { "SLQD", AssetClass::ShortCorpBd }, // iShares 0-5 Year Investment Grade Corporate Bd ETF
        { "IGSB", AssetClass::ShortCorpBd }, // iShares 1-5 Year Investment Grade Corporate Bd ETF
        { "IGIB", AssetClass::ShortCorpBd },

        { "IGLB", AssetClass::LongCorpBd }, // iShares 10+ Year Investment Grade Corp Bond ETF
        { "VCLT", AssetClass::LongCorpBd }, // Vanguard Long-Term Corporate Bond Idx Fund ETF
        { "VCIT", AssetClass::IntermediateCorpBd }, // VANGUARD INTERMEDIATE-TERM CORPORATE BOND ETF

        { "REET", AssetClass::REIT }, //
        { "RWR", AssetClass::REIT }, //
        { "SCHH", AssetClass::REIT }, //
        { "USRT", AssetClass::REIT }, //
        { "VNQ", AssetClass::REIT }, //
        { "VNQI", AssetClass::REIT }, //

        { "SGOL", AssetClass::PreciousMetal }, //
        { "SIVR", AssetClass::PreciousMetal }, //

        { "TECL", AssetClass::Technology }, // Direxion Daily Technology Bull 3X Shares ETF
        { "CXSE", AssetClass::China }, // WisdomTree Trust China ex State Owned Enterprises ETF
        { "DGRO", AssetClass::HighYield }, // iShares Core Dividend Growth ETF
        { "IPO", AssetClass::ActiveETF }, // Renaissance IPO ETF
        { "STIP", AssetClass::ShortTermBond }, // iShares 0-5 Year TIPS Bond ETF
        { "USHY", AssetClass::CorporateBond }, //
        { "VAW", AssetClass::NaturalResources }, //
        { "VCR", AssetClass::ConsumerCyclical }, //
        { "VDC", AssetClass::ConsumerDefensive }, //
        { "VFH", AssetClass::FinancialServices }, //
        { "VIG", AssetClass::HighYield }, //
        { "VIS", AssetClass::Industrials }, //
        { "VPU", AssetClass::Utilities }, //
        { "SOXL", AssetClass::Technology }, //
        { "SPHY", AssetClass::HighYield }, //

        { "VDE", AssetClass::Energy }, //
        { "XLE", AssetClass::Energy }, //

        { "VOX", AssetClass::Communication }, //
        { "XLC", AssetClass::Communication }, //

        { "DEM", AssetClass::IntlHighYield }, //
        { "VYMI", AssetClass::IntlHighYield }, //

        { "ARKF", AssetClass::ActiveETF }, //
        { "ARKG", AssetClass::ActiveETF }, //
        { "ARKK", AssetClass::ActiveETF }, //
        { "ARKQ", AssetClass::ActiveETF }, //
        { "ARKW", AssetClass::ActiveETF }, //
        { "PFF", AssetClass::ActiveETF }, //
        { "QYLD", AssetClass::ActiveETF }, //

        { "MUB", AssetClass::MuniBond }, //
        { "VTEB", AssetClass::MuniBond }, //

        { "SPYD", AssetClass::HighYield }, //
        { "SPHD", AssetClass::HighYield }, //
        { "HDV", AssetClass::HighYield }, //
        { "VYM", AssetClass::HighYield }, //
        { "SCHD", AssetClass::HighYield }, //

        { "VO", AssetClass::MidCap }, //
        { "JHMM", AssetClass::MidCap }, //

        { "VB", AssetClass::SmallCap }, //
        { "VBK", AssetClass::SmallCap }, //
        { "VBR", AssetClass::SmallCap }, //
        { "VXF", AssetClass::SmallCap }, //
        { "JPSE", AssetClass::SmallCap }, //

        { "Intermediate-Term Bond", AssetClass::IntermediateBond },
        { "Technology", AssetClass::Technology },
        { "Health", AssetClass::Healthcare },
        { "Healthcare", AssetClass::Healthcare },
        { "Communications", AssetClass::Communication },
        { "Mid-Cap Growth", AssetClass::MidCapGrowth },
        { "Long-Term Bond", AssetClass::LongTermBond },
        { "Short-Term Bond", AssetClass::ShortTermBond },
        { "China Region", AssetClass::China },
        { "Diversified Emerging Mkts", AssetClass::Emerging },
        { "Large Value", AssetClass::LargeValue },
        { "Large Growth", AssetClass::LargeGrowth },
        { "Long Government", AssetClass::LongTermBond },
        { "Europe Stock", AssetClass::Europe },
        { "Corporate Bond", AssetClass::CorporateBond },
        { "Diversified Pacific/Asia", AssetClass::China },
        { "Large Blend", AssetClass::LargeBlend },
        { "Foreign Large Blend", AssetClass::ForeignLargeBlend },
        { "Foreign Large Value", AssetClass::ForeignLargeValue },
        { "Mid-Cap Blend", AssetClass::MidCapBlend },
        { "Mid-Cap Value", AssetClass::MidCapValue },
        { "Small Blend", AssetClass::SmallBlend },
        { "Small Growth", AssetClass::SmallGrowth },
        { "Small Value", AssetClass::SmallValue },
        { "Muni National Interm", AssetClass::MuniBond },
        { "Real Estate", AssetClass::REIT },
        { "Global Real Estate", AssetClass::REIT },
        { "Utilities", AssetClass::Utilities },
        { "Industrials", AssetClass::Industrials },
        { "Consumer Cyclical", AssetClass::ConsumerCyclical },
        { "Consumer Defensive", AssetClass::ConsumerDefensive },
        { "Financial Services", AssetClass::FinancialServices },
        { "Financial", AssetClass::FinancialServices },
        { "Communication Services", AssetClass::Communication },
        { "Inflation-Protected Bond", AssetClass::ShortTermBond },
        { "Trading--Leveraged Equity", AssetClass::Leveraged },
        { "High Yield Bond", AssetClass::CorporateBond },
        { "Natural Resources", AssetClass::NaturalResources },
        { "Equity Energy", AssetClass::Energy },
        { "Energy", AssetClass::Energy },
        { "Intermediate Government", AssetClass::IntermediateBond },
        { "Foreign Small/Mid Blend", AssetClass::MidCapBlend },
        { "World Stock", AssetClass::TotalMarket },
        { "Preferred Stock", AssetClass::ActiveETF }, // $PFF
    };
    if (map.contains(key)) {
        return std::set<AssetClass> { map.at(key) };
    }
    if (!key.empty()) {
        std::cerr << "EnumUtils::getAssetTags [Not Found] " << key << "\n";
    }
    return {};
}

std::string EnumUtils::to_string(AssetClass tag)
{
    static const std::unordered_map<AssetClass, std::string> map {
        { AssetClass::Unclassified, "Unclassified" },

        { AssetClass::ETF, "ETF" },
        { AssetClass::ActiveETF, "Active ETF" },
        { AssetClass::NotETF, "Not ETF" },
        { AssetClass::Foreign, "Foreign" },
        { AssetClass::REIT, "REIT" },

        { AssetClass::BlackRock, "BlackRock" },
        { AssetClass::Vanguard, "Vanguard" },
        { AssetClass::Schwab, "Schwab" },
        { AssetClass::SPDR, "SPDR" },
        { AssetClass::Invesco, "Invesco" },

        { AssetClass::Bond, "Bond" },
        { AssetClass::TotalBond, "Total Bond" },
        { AssetClass::IntlBond, "Intl Bond" },
        { AssetClass::MuniBond, "Muni Bond" },
        { AssetClass::ShortCorpBd, "Short Corp Bond" },
        { AssetClass::IntermediateCorpBd, "Intermediate Corp Bond" },
        { AssetClass::LongCorpBd, "Long Corp Bond" },
        { AssetClass::IntermediateBond, "Intermediate Bond" },
        { AssetClass::LongTermBond, "Long Term Bond" },
        { AssetClass::ShortTermBond, "Short Term Bond" },
        { AssetClass::CorporateBond, "Corporate Bond" },
        { AssetClass::WorldBondUSDHedged, "World Bond USD Hedged" },

        { AssetClass::SP500, "S&P 500" },
        { AssetClass::TotalMarket, "Total Market" },
        { AssetClass::TotalIntl, "Total Intl" },

        { AssetClass::PreciousMetal, "Precious Metal" },
        { AssetClass::Russell1000, "Russell 1000" },

        { AssetClass::HighYield, "High Yield" },
        { AssetClass::DividendGrowth, "Dividend Growth" },
        { AssetClass::IntlHighYield, "Intl High Yield" },

        { AssetClass::ForeignLargeBlend, "Foreign Large Blend" },
        { AssetClass::ForeignLargeGrowth, "Foreign Large Growth" },
        { AssetClass::ForeignLargeValue, "Foreign Large Value" },

        { AssetClass::SmallCap, "Small Cap" },
        { AssetClass::SmallBlend, "Small Blend" },
        { AssetClass::SmallGrowth, "Small Growth" },
        { AssetClass::SmallValue, "Small Value" },

        { AssetClass::MidCap, "Mid Cap" },
        { AssetClass::MidCapGrowth, "Mid Cap Growth" },
        { AssetClass::MidCapBlend, "Mid Cap Blend" },
        { AssetClass::MidCapValue, "Mid Cap Value" },

        { AssetClass::LargeCap, "Large Cap" },
        { AssetClass::LargeBlend, "Large Blend" },
        { AssetClass::LargeValue, "Large Value" },
        { AssetClass::LargeGrowth, "Large Growth" },

        { AssetClass::Energy, "Energy" },
        { AssetClass::Technology, "Technology" },
        { AssetClass::Healthcare, "Healthcare" },
        { AssetClass::Utilities, "Utilities" },
        { AssetClass::Communication, "Communication" },
        { AssetClass::ConsumerCyclical, "Consumer Cyclical" },
        { AssetClass::ConsumerDefensive, "Consumer Defensive" },
        { AssetClass::Industrials, "Industrials" },
        { AssetClass::FinancialServices, "Financial Services" },
        { AssetClass::NaturalResources, "Natural Resources" },

        { AssetClass::China, "China" },
        { AssetClass::Emerging, "Emerging" },
        { AssetClass::Europe, "Europe" },
        { AssetClass::Leveraged, "Leveraged" },
    };
    return map.contains(tag) ? map.at(tag) : "AssetTag::" + std::to_string(static_cast<int>(tag));
}
