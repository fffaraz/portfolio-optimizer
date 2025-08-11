#!/usr/bin/env python3

#
# Copyright (c) 2021  Faraz Fallahi <fffaraz@gmail.com>
#
# Use of this source code is governed by an MIT-style
# license that can be found in the LICENSE file
#

# pip3 install --upgrade --user yfinance

import json
import os
import time
import yfinance as yf

print("yfinance version:", yf.__version__)

if not os.path.exists("./data/yf"):
    print("Directory ./data/yf does not exist")
    exit(1)

symbols = ["AGG", "AMD", "ARKK", "BIV", "BLV", "BLZE", "BND", "BNDX", "BSV", "CIBR", "CRWD", "DGRO", "DIVB", "DIVO", "DJD", "DOCN", "DTD", "EDV", "HDV", "IAGG", "IEUR", "IGIB", "IGLB", "IGSB", "IJR", "ILTB", "INTC", "IPAC", "ISTB", "ITOT", "IUSB", "IUSG", "IUSV", "IVE", "IVV", "IVW", "IWB", "IWV", "IXUS", "JEPI", "JEPQ", "JPSE", "MGK", "MGV", "MUB", "NET", "PFF", "QQQ", "QQQE", "QQQM", "QYLD", "REET", "RSP", "SCHB", "SCHD", "SCHG", "SCHH", "SCHK", "SCHX", "SCHY", "SCYB", "SGOL", "SGOV", "SIVR", "SLQD", "SPHD", "SPHY", "SPLG", "SPLV", "SPTL", "SPTM", "SPY", "SPYD", "SPYV", "STIP", "TFLO", "TIP", "TLT", "USHY", "USRT", "VAW", "VB", "VBK", "VBR", "VCIT", "VCLT", "VCR", "VCSH", "VDC", "VDE", "VEA", "VEU", "VFH", "VGSH", "VGT", "VHT", "VIG", "VIS", "VMBS", "VNQ", "VNQI", "VO", "VOE", "VONE", "VOO", "VOOV", "VOT", "VOX", "VPU", "VSS", "VT", "VTEB", "VTI", "VTIP", "VTV", "VUG", "VUSB", "VV", "VWO", "VXF", "VXUS", "VYM", "VYMI", "XLC", "XLE"]
symbols = ["VOO", "VTI", "VXUS", "BND", "BNDX", "QQQ", "VUG", "VEA"]

for symbol in symbols:
    print(symbol)
    ticker = yf.Ticker(symbol)

    # get stock info
    with open("./data/yf/" + symbol + '.json', 'w') as outfile:
        json.dump(ticker.info, outfile, indent=4, sort_keys=True)

    # get historical market data
    history = ticker.history(period="max")
    history.to_csv("./data/yf/" + symbol + ".csv")

    # wait 5 seconds to avoid rate limit
    time.sleep(5)
