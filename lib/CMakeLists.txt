add_library(
  portopt
  Asset.cpp
  Asset.hpp
  AssetEnums.hpp
  AssetInfo.hpp
  AssetRatio.cpp
  AssetRatio.hpp
  CsvFile.cpp
  CsvFile.hpp
  EnumUtils.cpp
  EnumUtils.hpp
  EtradePortfolio.cpp
  EtradePortfolio.hpp
  Market.cpp
  Market.hpp
  Ohlc.cpp
  Ohlc.hpp
  OhlcEnums.hpp
  OhlcList.cpp
  OhlcList.hpp
  Portfolio.cpp
  Portfolio.hpp
  Utils.cpp
  Utils.hpp)

target_link_libraries(portopt PUBLIC nlohmann_json::nlohmann_json)
