#!/usr/bin/env python3

#
# Copyright (c) 2021  Faraz Fallahi <fffaraz@gmail.com>
#
# Use of this source code is governed by an MIT-style
# license that can be found in the LICENSE file
#

# pip3 install --user yfinance

import json
import os
import time
import yfinance as yf

if not os.path.exists("../data/yf"):
    print("Directory ../data/yf does not exist")
    exit(1)

symbols = ["AGG", "ARKF", "ARKG", "ARKK", "BIV", "BLV", "BLZE", "BND", "BNDX", "BSV", "CIBR", "CRWD", "CXSE", "DEM", "DGRO", "DIVB", "DIVO", "DJD", "DOCN", "DTD", "EDV", "HDV", "IAGG", "IEUR", "IGIB", "IGLB", "IGSB", "IJR", "ILTB", "IPAC", "ISTB", "ITOT", "IUSB", "IUSG", "IUSV", "IVV", "IVW", "IWB", "IWV", "IXUS", "JEPI", "JEPQ", "JHMM", "JPSE", "MGK", "MGV", "MUB", "NET", "PFF", "QQQ", "QQQE", "QQQM", "QYLD", "REET", "RSP", "RWR", "SCHB", "SCHD", "SCHG", "SCHH", "SCHK", "SCHX", "SCHY", "SGOL", "SGOV", "SIVR", "SLQD", "SPHD", "SPHY", "SPLG", "SPLV", "SPTL", "SPTM", "SPY", "SPYD", "STIP", "TFLO", "TIP", "TLT", "USHY", "USRT", "VAW", "VB", "VBK", "VBR", "VCIT", "VCLT", "VCR", "VCSH", "VDC", "VDE", "VEA", "VEU", "VFH", "VGSH", "VGT", "VHT", "VIG", "VIS", "VMBS", "VNQ", "VNQI", "VO", "VOE", "VONE", "VOO", "VOT", "VOX", "VPU", "VSS", "VT", "VTEB", "VTI", "VTIP", "VTV", "VUG", "VUSB", "VV", "VWO", "VXF", "VXUS", "VYM", "VYMI", "XLC", "XLE"]

for symbol in symbols:
    print(symbol)
    ticker = yf.Ticker(symbol)

    # get stock info
    with open("../data/yf/" + symbol + '.json', 'w') as outfile:
        json.dump(ticker.info, outfile, indent=4, sort_keys=True)

    # get historical market data
    history = ticker.history(period="max")
    history.to_csv("../data/yf/" + symbol + ".csv")

    # wait 5 seconds to avoid rate limit
    time.sleep(5)
