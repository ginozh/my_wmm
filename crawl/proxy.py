#!/usr/bin/python3
# -*- coding: utf-8 -*- 
'''
@author: storm
@date: 2020-05-22
@description: 
'''

import sys
from selenium import webdriver
from selenium.common.exceptions import TimeoutException
from selenium.webdriver.support.ui import WebDriverWait 
from selenium.webdriver.support import expected_conditions as EC
from selenium.webdriver.common.by import By
from selenium.common.exceptions import TimeoutException
from selenium.common.exceptions import NoSuchElementException
from selenium.common.exceptions import ElementClickInterceptedException
from selenium.common.exceptions import WebDriverException
from selenium.common.exceptions import TimeoutException
from scrapy.selector import HtmlXPathSelector
from scrapy.http import Response 
from scrapy.http import TextResponse 
from datetime import datetime
from selenium.webdriver.common.proxy import Proxy, ProxyType
import time
import collections
import io
import re

#proxylasttime=0
#proxyintervaltime=1*60 #1个小时更新一次ip
proxyidx=0
proxystruct = collections.namedtuple('struct', 'ip port')
proxyips=[]
def addproxy():
  try:
    #global proxystruct
    global proxyips
    proxy = webdriver.Firefox()
    proxy.get("https://www.hide-my-ip.com/proxylist.shtml")
    #text_html=proxy.page_source.encode('utf-8')
    rows = proxy.find_elements_by_xpath('//*[@id="sort-list"]/tbody/tr[*]')
    for row in rows:
      #col = row.find_elements(By.TAG_NAME, "td")[1] #note: index start from 0, 1 is col 2
      cols = row.find_elements(By.TAG_NAME, "td") #note: index start from 0, 1 is col 2
      ip = cols[0].text
      port = cols[1].text
      ttype = cols[4].text
      print ("ip: ", ip, " port: ", port, " type: ", ttype, " speed: ", cols[3].text) #prints text from the element
      if ttype.lower().strip() == "https" :
        proxyips.append(proxystruct(ip, port))
    proxy.quit()
  except NoSuchElementException:
    print("error proxy. no tr. time: ", datetime.now())
  except WebDriverException:
    print("error proxy. WebDriverException. time: ", datetime.now())

def addproxy2():
  try:
    global proxyips
    proxy = webdriver.Firefox()
    proxy.get("https://free-proxy-list.net/")
    #text_html=proxy.page_source.encode('utf-8')
    typeselect = proxy.find_element_by_xpath('//*[@id="proxylisttable"]/tfoot/tr/th[7]/select')
    for option in typeselect.find_elements_by_tag_name('option'):
      if option.text == "yes":
        option.click() # select() in earlier versions of webdriver
        break
    rows = proxy.find_elements_by_xpath('//*[@id="proxylisttable"]/tbody/tr[*]')
    for row in rows:
      #col = row.find_elements(By.TAG_NAME, "td")[1] #note: index start from 0, 1 is col 2
      cols = row.find_elements(By.TAG_NAME, "td") #note: index start from 0, 1 is col 2
      ip = cols[0].text
      port = cols[1].text
      ttype = cols[6].text #https
      print ("ip: ", ip, " port: ", port, " type: ", ttype, " speed: ", cols[3].text) #prints text from the element
      if ttype.lower().strip() == "yes" :
        proxyips.append(proxystruct(ip, port))
    proxy.quit()
  except NoSuchElementException:
    print("error proxy. no tr. time: ", datetime.now())
  except WebDriverException:
    print("error proxy. WebDriverException. time: ", datetime.now())

def addproxy3():
  try:
    global proxyips
    proxy = webdriver.Firefox()
    proxy.get("http://list.didsoft.com/get?email=hqzhang1983@gmail.com&pass=e927jt&pid=http1000&https=yes&showcountry=yes&excludedcountry=CN&level=1|2")
    #text_html=proxy.page_source.encode('utf-8')
    ips = proxy.find_element_by_xpath('/html/body/pre')
    buf = io.StringIO(ips.text)
    for line in buf.readlines():
      m = re.match('([^:]+):([^#]+)#(.*)', line.strip())
      #print (line.strip())
      ip = m.group(1)
      port = m.group(2)
      country = m.group(3)
      print ("ip: ", ip, " port: ", port, " country: ", country) 
      proxyips.append(proxystruct(ip, port))
    proxy.quit()
  except NoSuchElementException:
    print("error proxy. no tr. time: ", datetime.now())
  except WebDriverException:
    print("error proxy. WebDriverException. time: ", datetime.now())

def proxydriver(PROXY_HOST,PROXY_PORT):
  fp = webdriver.FirefoxProfile()
  # Direct = 0, Manual = 1, PAC = 2, AUTODETECT = 4, SYSTEM = 5
  print ("proxy proxydriver ip: ", PROXY_HOST, " port: ", PROXY_PORT)
  fp.set_preference("network.proxy.type", 1)
  fp.set_preference("network.proxy.http",PROXY_HOST)
  fp.set_preference("network.proxy.http_port",int(PROXY_PORT))
  fp.set_preference("network.proxy.https",PROXY_HOST)
  fp.set_preference("network.proxy.https_port",int(PROXY_PORT))
  fp.set_preference("network.proxy.ssl",PROXY_HOST)
  fp.set_preference("network.proxy.ssl_port",int(PROXY_PORT)) 
  fp.set_preference("general.useragent.override","whater_useragent")
  fp.update_preferences()
  return webdriver.Firefox(firefox_profile=fp)

def getproxy():
  global proxyips
  #timestamp = int(time.time())
    #use proxyips[proxyidx] and judge valid
  while True:
    global proxyidx
    if proxyidx >= len(proxyips):
      #1, clean and get new ip list
      proxyidx = 0;
      proxyips.clear()
      #proxyips.append(proxystruct("162.159.244.149", "80")) # for test wrong ip
      proxyips.append(proxystruct("139.162.18.98", "30128")) # for test wrong ip
      addproxy3()
      #addproxy2()
      #addproxy()
    if len(proxyips) >= 2 and proxyidx < len(proxyips) :
      ip = proxyips[proxyidx].ip
      port = proxyips[proxyidx].port
      driver = proxydriver(ip, port)
      print("proxy. try proxy ip: ", ip, " port: ", port, " proxyidx: ", proxyidx, " proxycnt: ", len(proxyips), ". time: ", datetime.now() )
      proxyidx += 1
      try:
        # verify proxy
        #driver.get("https://whatismyipaddress.com/ip-lookup")
        driver.set_page_load_timeout(30)
        #driver.get("https://www.google.com")
        driver.get("https://www.wikipedia.org/")
      except TimeoutException:
        print("proxy. error the proxy is timeout. ip: ", ip, " port: ", port, " time: ", datetime.now())
        driver.quit()
        continue
      except WebDriverException:
        print("proxy. error the proxy is invalid. ip: ", ip, " port: ", port)
        driver.quit()
        time.sleep(2)
        continue
      return driver
    else:
      print("error proxy. no ips. time: ", datetime.now() )
      time.sleep(60*5)
      continue
      #return webdriver.Firefox()
    
def getproxytest():
  return proxydriver("139.162.18.98", "30128")

if __name__ == '__main__':
  #try:
    #getproxy()
    driver=getproxytest()
    driver.get("https://www.wikipedia.org/")
    print ("proxy len: ", len(proxyips))
    for ip in proxyips:
      print ("proxy ip: ", ip.ip, " port: ", ip.port)
  #except:
  #  print("proxy. Unexpected error:", sys.exc_info()[0])

