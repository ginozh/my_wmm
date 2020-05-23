#!/usr/bin/python
# -*- coding: utf-8 -*- 
'''
@author: storm
@date: 2020-05-21
@description: 
sudo pip3 install playsound
因为第二个页面无法给出发地、目的地直接赋值，因此只能每次在首页赋值查询, 请看nanhang.py
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
from selenium.common.exceptions import ElementClickInterceptedException
from selenium.common.exceptions import WebDriverException
from selenium.webdriver.common.keys import Keys
from scrapy.selector import HtmlXPathSelector
from scrapy.http import Response 
from scrapy.http import TextResponse 
from datetime import datetime
from playsound import playsound
import time

arrfromcity =      ["温哥华机场", "温哥华机场", "伦敦希思罗机场", "伦敦希思罗机场", "伦敦希思罗机场", "伦敦希思罗机场", "韩国仁川"]
arrtocity =        ["广州"      , "广州"      , "广州"          , "广州"          , "广州"          , "广州"          , "沈阳"]
arrDepartureDate = ["2020-06-03", "2020-05-27", "2020-06-18"    , "2020-05-28"    , "2020-06-04"    ,             "2020-06-11"    , "2020-06-07"]
driver = webdriver.Firefox()
iIndex = 0
while True:
  found = 0
  try:
    #driver.get("http://139.162.18.98/crawl.html")
    driver.get("https://www.csair.com/cn/index.shtml")
    text_html=driver.page_source.encode('utf-8')
    html_str=str(text_html)
    #print ("html_str: " + html_str)

    #time.sleep(5000)
    try:
      result = driver.find_element_by_css_selector("a.approve-setting")
      result.click()
      time.sleep(5)
    except NoSuchElementException:
      print("no a.approve-setting")
    except ElementClickInterceptedException:
      print("no a.approve-setting ElementClickInterceptedException")

    delay = 15 # seconds
    try:
      WebDriverWait(driver, delay).until(EC.presence_of_element_located((By.ID, 'fDepCity')))
      print ("Page is ready!")
    except TimeoutException:
      print ("Loading took too much time!")

    '''
    while True:
      try:
        print("1");
        fDepCity = driver.find_element_by_id("fDepCity")
        print("2");
      except NoSuchElementException:
        print("3");
        time.sleep(5)
        continue
      print("4");
      break
    '''

    fDepCity = driver.find_element_by_id("fDepCity")
    fDepCity.clear()
    fDepCity.send_keys(arrfromcity[2])

    #fArrCity = driver.find_element_by_id("fArrCity")
    #fArrCity.clear()
    #fArrCity.send_keys("广州")
    #fArrCity.send_keys("广州白云国际机场")
    driver.execute_script("document.getElementById('fArrCity').value = '"+arrtocity[2]+"'")

    driver.execute_script("document.getElementById('fDepDate').value = '"+arrDepartureDate[2]+"'")

    time.sleep(1)
    result = driver.find_element_by_css_selector("a.fr.searchBtn.searchFlight")
    result.click()

    #text_html=driver.page_source.encode('utf-8')
    #html_str=str(text_html)
    #print ("html_str: " + html_str)

    #time.sleep(2)
    while found == 0:
      try:
        time.sleep(20)
        if iIndex >= len(arrfromcity):
          iIndex = 0
        fromcity = arrfromcity[iIndex]
        tocity = arrtocity[iIndex]
        DepartureDate = arrDepartureDate[iIndex]
        print(" fromcity: ", fromcity, " tocity: ",tocity, " DepartureDate: ", DepartureDate)
        driver.execute_script("document.getElementById('fromcity').value = '"+fromcity+"'")
        driver.execute_script("document.getElementById('tocity').value = '"+tocity+"'")
        driver.execute_script("document.getElementById('DepartureDate').value = '"+DepartureDate+"'")
        time.sleep(1)
        iIndex += 1

        submit = driver.find_element_by_id("bk-form-submit")
        submit.click()
        text_html=driver.page_source.encode('utf-8')
        html_str=str(text_html)
        #time.sleep(1)
        #print("1")
      except NoSuchElementException:
        print("no bk-form-submit")
        driver.get(driver.current_url)
        continue
      except ElementClickInterceptedException:
        print("ElementClickInterceptedException") #验证码
        time.sleep(20)
        #driver.get(driver.getCurrentUrl());
        driver.get(driver.current_url)
        #driver.find_element_by_id("bk-form-submit").send_keys(Keys.F5)
        #driver.find_element_by_css_selector("input.input.ca-input").send_keys(Keys.F5)
        #driver.find_element_by_id("fromcity").send_keys(Keys.F5)
        #driver.findElement(By textboxLocator).send_keys(Keys.F5)
        continue
      try:
        #print("2")
        result = driver.find_element_by_css_selector("div.sh-cabins")
        print ("found div.sh-cabins time: ", datetime.now())
        try:
          result = driver.find_element_by_css_selector("div.cell.cab-0.sold-out")
          print ("found div.cell.cab-0.sold-out text: ", result.text)
          if result.text != '售罄':
            found = 1
        except NoSuchElementException:
          print("no div.cell.cab-0.sold-out")
      except NoSuchElementException:
        print("no div.sh-cabins time: ", datetime.now())

      if found == 1 :
        print ("found")
        break;
  except WebDriverException:
    print("WebDriverException")
  if found == 1 :
    print ("found")
    break;

playsound('/media/psf/Home/Desktop/upan/qqrtx/QQ/Spring_In_My_Step.mp3')
print("end time: ", datetime.now())
