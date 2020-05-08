#!/usr/bin/python
# -*- coding: utf-8 -*- 
'''
@author: storm
@date: 2020-03-25
@description: 
'''

import sys
from selenium import webdriver
from selenium.common.exceptions import TimeoutException
from selenium.webdriver.support.ui import WebDriverWait 
from selenium.webdriver.support import expected_conditions as EC
from selenium.webdriver.common.keys import Keys
from scrapy.selector import HtmlXPathSelector
from scrapy.http import Response 
from scrapy.http import TextResponse 
import time

driver = webdriver.Firefox()
#driver.get("http://139.162.18.98/crawl.html")
driver.get("https://filmora.wondershare.com/video-editing-tips/")
text_html=driver.page_source.encode('utf-8')
html_str=str(text_html)

#print ("html_str: " + html_str)
icnt = 0;
for i in range(1, 53):
  results = driver.find_elements_by_css_selector("div.card-body")
  for result in results:
    card_title = result.find_element_by_css_selector("h5 > a")
    card_text = result.find_element_by_css_selector("p.card-text")
    if not card_title.text or not card_text.text:
      continue
    icnt += 1;
    print("{} card_title: '{}' url: '{}' card_text: '{}'".format(icnt, card_title.text, card_title.get_property("href")
      , card_text.text))

  #<span class="next curr">Next</span>
  result = driver.find_element_by_css_selector("span.next.curr")
  #print("next_curr: {} ".format(result.text))
  #result.send_keys(Keys.CONTROL + Keys.RETURN)
  result.click()
  time.sleep(5)

exit()

results = driver.find_elements_by_css_selector("div.card-body")
icnt = 0;
for result in results:
  icnt += 1;
  #print("#{} ({})".format(result.text, result.get_property("href")))
  card_titles = result.find_elements_by_css_selector("h5 > a")
  for card_title in card_titles:
    print("{} card_title: {} ({})".format(icnt, card_title.text, card_title.get_property("href")))
  card_texts = result.find_elements_by_css_selector("p.card-text")
  for card_text in card_texts:
    print("{} card_text: {} ({})".format(icnt, card_text.text, card_text.get_property("href")))
  '''
    # right
    card_titles = result.find_elements_by_css_selector("h5 > a")
    for card_title in card_titles:
      print("{} card_title: {} ({})".format(icnt, card_title.text, card_title.get_property("href")))
    card_texts = result.find_elements_by_css_selector("p.card-text")
    for card_text in card_texts:
      print("{} card_text: {} ".format(icnt, card_text.text))
  '''
exit()
#results = driver.find_element_by_name('continue')
#results = driver.find_elements_by_css_selector("div.row art-list")
results = driver.find_elements_by_css_selector("h5 > a")
for result in results:
  print("#{} ({})".format(result.text, result.get_property("href")))
exit()
results = driver.find_elements_by_css_selector("h5.card-title")
for result in results:
  print("#{} ({})".format(result.text, result.get_property("href")))

results = driver.find_elements_by_css_selector("p.card-text")
for result in results:
  print("#{} ({})".format(result.text, result.get_property("href")))
