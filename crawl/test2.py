#!/usr/bin/python
# -*- coding: utf-8 -*- 
'''
@author: storm
@date: 2020-03-24
@description: 
'''

import sys
# stripped down BoltBus script 
from selenium import webdriver
from selenium.common.exceptions import TimeoutException
from selenium.webdriver.support.ui import WebDriverWait 
from selenium.webdriver.support import expected_conditions as EC
from selenium.webdriver.common.keys import Keys
from scrapy.selector import HtmlXPathSelector
from scrapy.http import Response 
from scrapy.http import TextResponse 
import time

# set dates, origin, destination 
cityOrigin="Baltimore"
cityDeparture="New York"
day_array=[0]
browser = webdriver.Firefox()

# we are going the day of the days of the month from 15,16,...,25
# there is a discrepancy between the index of the calendar days and the day itself: for example day[10] may correspond to Feb 7th
for day in day_array:

    # Create a new instance of the Firefox driver
    browser.get("https://filmora.wondershare.com/video-editing-tips/")
    text_html=browser.page_source.encode('utf-8')
    html_str=str(text_html)

    print ("html_str: " + html_str)
    # <h5 class="card-title"><a class="d-block" href="/video/free-luts-for-premiere-pro.html">73 Best Free Luts for Premiere Pro to Make Cinematic Videos</a>\n                                    </h5>
    # <p class="card-text"><a class="d-block" href="/video/free-luts-for-premiere-pro.html">Do you want to find free luts for Premiere Pro? In this article, we will let you know 75 free to use cinematic lut for Premiere. Check it out!</a></p>
    results = browser.find_elements_by_css_selector("h3>a")
    for i, result in results.iteritems():
      print("#{}: {} ({})".format(i, result.text, result.get_property("href")))
'''
    # this is a hack that initiates a "TextResponse" object (taken from the Scrapy module)
    resp_for_scrapy=TextResponse('none',200,{},html_str,[],None)

    # takes a "TextResponse" object and feeds it to a scrapy function which will convert the raw HTML to a XPath document tree
    hxs=HtmlXPathSelector(resp_for_scrapy)

    # the | sign means "or"
    table_rows=hxs.select('//tr[@class="fareviewrow"] | //tr[@class="fareviewaltrow"]')
    row_ct=len(table_rows)

    for x in xrange(row_ct):

        cur_node_elements=table_rows[x]
        travel_price=cur_node_elements.select('.//td[@class="faresColumn0"]/text()').re("\d{1,3}\.\d\d")

        # I use a mixture of xpath selectors to get me to the right location in the document, and regular expressions to get the exact data

        # actual digits of time 
        depart_time_num=cur_node_elements.select('.//td[@class="faresColumn1"]/text()').re("\d{1,2}\:\d\d")

        # AM or PM (time signature)
        depart_time_sig=cur_node_elements.select('.//td[@class="faresColumn1"]/text()').re("[AP][M]")

        # actual digits of time 
        arrive_time_num=cur_node_elements.select('.//td[@class="faresColumn2"]/text()').re("\d{1,2}\:\d\d")

        # AM or PM (time signature)
        arrive_time_sig=cur_node_elements.select('.//td[@class="faresColumn2"]/text()').re("[AP][M]")

        print ("Depart date: " + depart_date)
        print ("Depart time: " + depart_time_num[0] + " " + depart_time_sig[0]   )
        print ("Arrive time: " + arrive_time_num[0] + " " + arrive_time_sig[0])
        print ("Cost: " + "$" + travel_price[0] )
        print ("\n")
'''
