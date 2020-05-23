#!/usr/bin/python3
# -*- coding: utf-8 -*- 
'''
@author: storm
@date: 2020-05-22
@description: 
sudo python3 -m pip install -U playsound
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
#from selenium.webdriver.common.keys import Keys
from scrapy.selector import HtmlXPathSelector
from scrapy.http import Response 
from scrapy.http import TextResponse 
from datetime import datetime
from sys import platform
if platform == "linux" or platform == "linux2":
  # linux
  from playsound import playsound
elif platform == "darwin":
  # OS X
  from pydub import AudioSegment
  from pydub.playback import play
          
import time
import proxy

arrfromcity =      ["温哥华机场", "温哥华机场", "伦敦希思罗机场", "伦敦希思罗机场", "伦敦希思罗机场", "伦敦希思罗机场", "首尔仁川机场"  , "洛杉矶机场", "温哥华"    , "洛杉矶"]
arrtocity =        ["广州"      , "广州"      , "广州"          , "广州"          , "广州"          , "广州"          , "沈阳"          , "广州"      , "广州"      , "广州"]
arrDepartureDate = ["2020-06-03", "2020-05-27", "2020-06-18"    , "2020-05-28"    , "2020-06-04"    ,             "2020-06-11"    , "2020-06-07"    , "2020-05-31", "2020-06-04", "2020-05-31"]

'''
# test
arrfromcity =      ["温哥华机场"]
arrtocity =        ["广州"]
arrDepartureDate = ["2020-07-29"]
'''

def searchTicket(driver, fDepCity, fArrCity, fDepDate):
  delay = 15 # seconds
  try:
    WebDriverWait(driver, delay).until(EC.presence_of_element_located((By.ID, 'fDepCity')))
    #print ("Page is ready!")
  except TimeoutException:
    print ("error Loading took too much time!")
    return

  print("assign depcity  time: ", datetime.now())
  driver.execute_script("document.getElementById('fDepCity').value = '"+fDepCity+"'")
  driver.execute_script("document.getElementById('fArrCity').value = '"+fArrCity+"'")
  driver.execute_script("document.getElementById('fDepDate').value = '"+fDepDate+"'")
  print("fromcity: ", fDepCity, " tocity: ",fArrCity, " DepartureDate: ", fDepDate, " time: ", datetime.now())
  time.sleep(5)

  try:
    '''
    search = driver.find_element_by_css_selector("a.searchBtn:nth-child(2)")
    search.click()
    print("search 1 time: ", datetime.now())
    search = driver.find_element_by_xpath('/html/body/div[4]/div[1]/div/div/div[1]/div/div[2]/div[4]/a')
    search.click()
    print("search 2 time: ", datetime.now())
    '''
    search = driver.find_element_by_css_selector("a.fr.searchBtn.searchFlight")
    search.click()
    '''
    print("search 3 time: ", datetime.now())
    search = driver.find_element_by_xpath('//*[@id="commonbox"]/div[1]/div/div/div[1]/div/div[2]/div[4]/a')
    search.click()
    print("search 4 time: ", datetime.now())
    '''
  except WebDriverException:
    time.sleep(1)
  print("click search  time: ", datetime.now())

iIndex = 0
found = 0
while True:
  #driver = webdriver.Firefox()
  #found = 1
  driver = proxy.getproxy()
  #driver = proxy.getproxytest()
  driver.set_page_load_timeout(60*3)
  try:
    #if True:
    #driver.get("http://139.162.18.98/crawl.html")
    driver.get("https://www.csair.com/cn/index.shtml")
    text_html=driver.page_source.encode('utf-8')
    html_str=str(text_html)
    #print ("html_str: " + html_str)

    #time.sleep(5000)
    try:
      result = driver.find_element_by_css_selector("a.approve-setting")
      result.click()
      time.sleep(2)
    except NoSuchElementException:
      time.sleep(1)
      #print("no a.approve-setting")
    except ElementClickInterceptedException:
      print("error. no a.approve-setting ElementClickInterceptedException")

    print("click yes time: ", datetime.now())
    if iIndex >= len(arrfromcity):
      iIndex = 0
    fDepCity = arrfromcity[iIndex]
    fArrCity = arrtocity[iIndex]
    fDepDate = arrDepartureDate[iIndex]
    iIndex += 1

    searchTicket(driver, fDepCity, fArrCity, fDepDate)
    time.sleep(5)
    #break # test
    Button=''
    iSearchCnt = 0
    iLoopCnt = 0
    while not Button and iLoopCnt <10:
      try:
        Button=driver.find_element_by_id('bk-form-submit')
        print ("find bk-form-submit:", Button, " time: ", datetime.now() )
        #Button.click()
      except TimeoutException:
        print ("error Loading took too much time!")
        break
      except WebDriverException:
      #except:
        time.sleep(5)
        if iSearchCnt < 5 :
          searchTicket(driver, fDepCity, fArrCity, fDepDate)
          iSearchCnt += 1
      iLoopCnt += 1

    '''
    time.sleep(30)

    delay = 30 # seconds
    try:
      WebDriverWait(driver, delay).until(EC.presence_of_element_located((By.ID, 'fromcity')))
      print ("Query Page is ready! time: ", datetime.now() )
    except TimeoutException:
      print ("error Loading took too much time!")
    '''
    try:
      retry = driver.find_element_by_xpath('/html/body/div[3]/div[2]/div[3]/div/div[2]/div[1]/div/a')
      retry.click()
      time.sleep(5)
    except NoSuchElementException:
      print("no retry. result time: ", datetime.now())

    try:
      retry = driver.find_element_by_xpath('/html/body/div[3]/div[2]/div[3]/div/div[2]/div[5]/div/a')
      retry.click()
      time.sleep(5)
    except NoSuchElementException:
      print("no retry. result time: ", datetime.now())

    try:
      price = driver.find_element_by_xpath('/html/body/div[3]/div[2]/div[3]/div/div[2]/div[1]/div/div[1]/div[3]/ul/li[4]/div[1]/div')
    except NoSuchElementException:
      print("no price time: ", datetime.now())
    #found = 1 #test
    #break;

    try:
      #print("2")
      print ("Query Page is ready! time: ", datetime.now() )
      result = driver.find_element_by_css_selector("div.sh-cabins")
      print ("found div.sh-cabins time: ", datetime.now())
      try:
        result = driver.find_element_by_css_selector("div.cell.cab-0.sold-out")
        print ("found div.cell.cab-0.sold-out text: ", result.text)
        if result.text != '售罄':
          found = 1
      except NoSuchElementException:
        print("no div.cell.cab-0.sold-out")
        found = 1 #test
        break;
    except NoSuchElementException:
      print("no result time: ", datetime.now())
      #found = 1 #test
      #break;
      #time.sleep(100)

    if found == 1 :
      print ("found")
      break;
  except TimeoutException:
    print("timeout. time: ", datetime.now())
  except WebDriverException:
    print("WebDriverException")
  if found == 1 :
    print ("found")
    break;
  driver.quit()
  #time.sleep(15)

if found == 1 :
  if platform == "linux" or platform == "linux2":
    # linux
    playsound('/media/psf/Home/Desktop/upan/qqrtx/QQ/Spring_In_My_Step.mp3')
  elif platform == "darwin":
    # OS X
    play(AudioSegment.from_mp3("/Users/user/Desktop/upan/qqrtx/QQ/Spring_In_My_Step.mp3"))

print("end time: ", datetime.now())
