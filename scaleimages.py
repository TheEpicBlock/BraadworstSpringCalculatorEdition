#!/usr/bin/env python
#-*- coding:utf-8 -*-

import sys
import subprocess
import os

gfx = sys.argv[1]

with open(gfx+"/scaling.properties") as f:
    file = f.read()
    for line in file.split("\n"):
        pair = line.split("=")
        if len(pair) == 2:
            key = pair[0]
            val = pair[1]
            
            subprocess.run([
                "ffmpeg", 
                "-i",
                gfx+"/source/"+key.strip(),
                "-vf",
                "scale=-1:"+val.strip(),
                "-y",
                "./"+gfx+"/scaled/"+key.strip(),
            ])
            
