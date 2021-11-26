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

symbols = ["ABR","AGNC","AGG","DGRO","AI","ARKF","AMBA","VHT","BB","AMT","BNDX","AMD","IPAC","ARKG","CNDT","ARKK","IGIB","CXSE","BIV","VDE","ARKQ","BSV","VDC","NSTB","ARKW","BA","BABA","BBIO","BITO","INTC","BLV","ILTB","BLZE","XLNX","BND","STAG","BRK.B","CCI","GOGO","SPTM","MNDY","CIBR","CRM","EQR","VONE","CRWD","VBK","DAL","FB","DBX","VAW","DEM","IMTB","IAGG","EMD","VIS","DHR","HDV","Z","DLR","DJD","DOCN","IEUR","DOCU","ILMN","DTD","EDV","VEA","FRT","FSLY","IECS","GE","GFS","GTLB","HA","IVW","HOOD","IXUS","ICE","IGLB","IGSB","IPO","ISTB","ITOT","IUSB","IUSG","IUSV","ZM","IVV","IWB","IWV","JHMM","SCHH","JPSE","LAZR","VRM","LUV","MCHP","MGK","MGV","MNDT","MSFT","MU","MUB","NET","NFLX","NIO","NNN","RSP","NVTA","TWTR","O","ORCL","PANW","PD","PFE","PFF","VXF","PLTR","PYPL","QQQ","QQQE","QYLD","REET","RWR","SCHB","SCHD","SCHG","SCHK","SCHX","SCHY","SGOL","SIVR","SLQD","SPTL","SOFI","SPHD","SPHY","SPLG","SPOT","SPY","VMBS","SPYD","SQ","STIP","STOR","STX","T","TIP","TSM","USHY","USRT","VB","VBR","VCIT","VCLT","VCR","VCSH","WPC","VEU","VFH","VGT","VIG","VLDR","VNQ","VNQI","VO","VOE","VSS","VOO","VOT","VOX","VPU","VT","VTEB","VTI","VTV","VV","VWO","VXUS","VYM","VYMI","WDC","WMT","XLC","XLE","ZEN","ZG"]

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
