#!/usr/bin/env python3

#
# Copyright (c) 2021  Faraz Fallahi <fffaraz@gmail.com>
#
# Use of this source code is governed by an MIT-style
# license that can be found in the LICENSE file
#

import json
import time
import yfinance as yf

symbols = ["ABR","AGG","AGNC","AMBA","AMD","AMT","APTV","ARKF","ARKG","ARKK","ARKQ","ARKW","BA","BABA","BB","BBIO","BIV","BLV","BND","BNDX","BSV","BYND","CCI","CGEN","CNDT","CRM","CRSP","CRWD","CSCO","CXSE","DAL","DBX","DEM","DFS","DGRO","DHR","DJD","DLR","DTD","DXC","EDIT","EDV","EMD","EQR","FB","FEYE","FRT","HA","HDV","IAGG","ICE","IEUR","IGIB","IGLB","IGSB","ILMN","ILTB","IMTB","INTC","IPAC","IPO","ISTB","ITOT","IUSB","IUSG","IUSV","IVV","IVW","IWB","IWV","IXUS","JHMM","JPSE","KO","LAZR","LUV","LYFT","M","MCD","MCHP","MGK","MGM","MGV","MO","MRNA","MSFT","MU","MUB","NFLX","NIO","NSTB","NTLA","NVDA","NVTA","O","ORCL","PANW","PEP","PFE","PFF","PLTR","PM","QQQ","QQQE","QS","QYLD","REET","RSP","RWR","SCHB","SCHD","SCHG","SCHH","SCHK","SCHX","SGOL","SIVR","SLQD","SOXL","SPHD","SPHY","SPLG","SPOT","SPTL","SPTM","SPY","SPYD","SQ","STAG","STIP","STOR","STX","T","TEAM","TECL","TLT","TSLA","TSM","TWTR","UBER","USHY","USRT","VAW","VB","VBK","VBR","VCLT","VCR","VDC","VDE","VEA","VEU","VFH","VGT","VHT","VIG","VIS","VLDR","VMBS","VNQ","VNQI","VO","VOE","VONE","VOO","VOT","VOX","VPU","VSS","VT","VTEB","VTI","VTV","VUG","VV","VWO","VXF","VXUS","VYM","VYMI","WDC","WMT","WPC","XLC","XLE","XLNX","XOM","ZM"]

for symbol in symbols:
    time.sleep(5)

    print(symbol)
    ticker = yf.Ticker(symbol)

    # get stock info
    with open("../data/symbols/" + symbol + '.json', 'w') as outfile:
        json.dump(ticker.info, outfile, indent=4, sort_keys=True)

    # get historical market data
    history = ticker.history(period="max")
    history.to_csv("../data/symbols/" + symbol + ".csv")
