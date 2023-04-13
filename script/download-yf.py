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


symbols = ["ABR","AGG","AGNC","AI","AMBA","AMD","AMT","ARKF","ARKG","ARKK","ARKQ","ARKW","BA","BABA","BB","BBIO","BITO","BIV","BLV","BLZE","BND","BNDX","BSV","CCI","CIBR","CNDT","CRM","CRWD","CXSE","DAL","DBX","DEM","DGRO","DHR","DJD","DLR","DOCN","DOCU","DTD","EDV","EMD","EQR","FB","FRT","FSLY","GE","GFS","GOGO","GTLB","HA","HDV","HOOD","IAGG","ICE","IECS","IEUR","IGIB","IGLB","IGSB","ILMN","ILTB","IMTB","INTC","IPAC","IPO","ISTB","ITOT","IUSB","IUSG","IUSV","IVV","IVW","IWB","IWV","IXUS","JHMM","JPSE","LAZR","LUV","MCHP","MGK","MGV","MNDT","MNDY","MSFT","MU","MUB","NET","NFLX","NIO","NNN","NSTB","NVTA","O","ORCL","PANW","PD","PFE","PFF","PLTR","PYPL","QQQ","QQQE","QYLD","REET","RSP","RWR","SCHB","SCHD","SCHG","SCHH","SCHK","SCHX","SCHY","SGOL","SIVR","SLQD","SOFI","SPHD","SPHY","SPLG","SPOT","SPTL","SPTM","SPY","SPYD","SQ","STAG","STIP","STOR","STX","T","TIP","TSM","TWTR","USHY","USRT","VAW","VB","VBK","VBR","VCIT","VCLT","VCR","VCSH","VDC","VDE","VEA","VEU","VFH","VGT","VHT","VIG","VIS","VLDR","VMBS","VNQ","VNQI","VO","VOE","VONE","VOO","VOT","VOX","VPU","VRM","VSS","VT","VTEB","VTI","VTV","VV","VWO","VXF","VXUS","VYM","VYMI","WDC","WMT","WPC","XLC","XLE","XLNX","Z","ZEN","ZG","ZM"]

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
