/*
 * Copyright (c) 2021  Faraz Fallahi <fffaraz@gmail.com>
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file
 */

#pragma once

#include <cstdint>

namespace portopt {

enum class AssetClass : std::uint8_t {
    Unclassified,

    ETF, // passive or active
    ActiveETF, // actively managed portfolio
    NotETF, // Common Stock, corporate equity, ...
    Foreign,
    REIT, // Real estate investment trust
    Gold, // Physical gold
    Crypto, // Cryptocurrency
    Commodities,
    Cash,

    BlackRock, // iShares
    Vanguard,
    Schwab,
    SPDR,
    Invesco,

    Bond,
    TotalBond, // Total Bond Market
    IntlBond, // International Bond
    MuniBond, // Municipal Bond
    InvestGradeBond, // Investment Grade Bond
    HighYieldBond,

    ShortCorpBond,
    IntermediateCorpBond,
    LongCorpBond,
    CorporateBond,

    ShortTermBond, // Short Treasuries
    IntermediateBond, // Intermediate Treasuries
    LongTermBond, // Long Treasuries

    WorldBondUSDHedged,

    SP500, // S&P 500 Index
    TotalMarket,
    TotalIntl, // International stocks

    PreciousMetal, // Gold, Silver, ...
    Russell1000,

    HighYield, // HighDividend
    DividendGrowth,
    IntlHighYield,

    ForeignLargeBlend,
    ForeignLargeGrowth,
    ForeignLargeValue,

    SmallCap,
    SmallBlend,
    SmallGrowth,
    SmallValue,

    MidCap,
    MidCapBlend,
    MidCapGrowth,
    MidCapValue,

    LargeCap,
    LargeBlend,
    LargeGrowth,
    LargeValue,

    Energy,
    Technology,
    Healthcare,
    Utilities,
    Communication,
    ConsumerCyclical,
    ConsumerDefensive,
    Industrials,
    FinancialServices,
    NaturalResources,

    US, // United States Stock Market
    China,
    Emerging, // Emerging Markets
    Europe,
    Leveraged,

    LastTag,
};

enum class AssetAllocation : std::uint8_t // Asset Class
{
    Unclassified,

    USStocks,
    InternationalStocks, // Non-US Stocks

    USBonds, // Bonds
    InternationalBonds, // Bonds

    Cash,
    Alternatives, // Other
};

enum class WorldRegions : std::uint8_t // Asset Geography
{
    Unclassified,

    US,
    EmergingMarkets,
    DevelopedInternational, // Intl Developed Markets

    NorthAmerica, // Americas
    LatinAmerica, // Americas

    UnitedKingdom, // Greater Europe
    EuropeDeveloped, // Greater Europe
    EuropeEmerging, // Greater Europe
    AfricaMiddleEast, // Greater Europe

    Japan, // Greater Asia
    Australasia, // Greater Asia
    AsiaDeveloped, // Greater Asia
    AsiaEmerging, // Greater Asia
};

enum class StockSector : std::uint8_t // Industry
{
    Unclassified,

    // Sensitive: not as immune to economic downturns as their defensive counterparts, but they are also not as profoundly affected as cyclical stocks
    CommunicationServices, 
    Energy,
    Industrials,
    Technology,

    // Defensive: demand remains relatively constant even during recessions
    ConsumerStaples, 
    HealthCare,
    Utilities,

    // Cyclical: thriving during periods of expansion and contracting during recessions
    BasicMaterials, 
    ConsumerDiscretionary,
    FinancialServices,
    RealEstate,
};

enum class StockStyle : std::uint8_t {
    Unclassified,

    LargeCapBlend,
    LargeCapGrowth,
    LargeCapValue,

    MidCapBlend,
    MidCapGrowth,
    MidCapValue,

    SmallCapBlend,
    SmallCapGrowth,
    SmallCapValue,
};

enum class BondStyle : std::uint8_t {
    Unclassified,

    HighLtd,
    HighMod,
    HighExt,

    MedLtd,
    MedMod,
    MedExt,

    LowLtd,
    LowMod,
    LowExt,
};

enum class StockType : std::uint8_t {
    Unclassified,

    HighYield,
    Distressed,
    HardAsset,
    Cyclical,
    SlowGrowth,
    ClassicGrowth,
    AggressiveGrowth,
    SpeculativeGrowth,
};

enum class MarketCap : std::uint8_t {
    Giant,
    Large,
    Medium,
    Small,
    Micro,
};

enum class AlternativeType : std::uint8_t {
    Unclassified,

    DomesticRealEstate,
    ForeignRealEstate,

    Gold,
    Silver,

    Energy,
    AgricultureFood,
    OtherMetals,
};

enum class BondType : std::uint8_t {
    USGov,
    USCorp,
    International,
    USInflationProtected,
};

} // namespace portopt
