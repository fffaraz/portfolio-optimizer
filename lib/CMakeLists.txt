add_library(
  portopt
  asset.cpp
  asset.hpp
  assetenums.hpp
  assetinfo.cpp
  assetinfo.hpp
  assetratio.cpp
  assetratio.hpp
  csvfile.cpp
  csvfile.hpp
  enumutils.cpp
  enumutils.hpp
  market.cpp
  market.hpp
  ohlc.hpp
  ohlc.cpp
  ohlcenums.hpp
  ohlclist.cpp
  ohlclist.hpp
  portfolio.cpp
  portfolio.hpp
  utils.cpp
  utils.hpp)

target_link_libraries(portopt Threads::Threads Qt${QT_VERSION_MAJOR}::Core)
