#!/bin/bash

<<COMMENT
@author: storm
@date: 2020-03-25
@description: 
COMMENT

xvfb-run -a '--server-args=-screen 0 1024x768x16' python3 testcrawl.py
