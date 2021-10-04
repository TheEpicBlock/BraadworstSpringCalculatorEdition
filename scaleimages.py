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
                "-y",
                "-filter_complex",
                "[0]split=2[bg][fg];[bg]drawbox=c=#294d79@1:replace=1:t=fill[bg];[bg][fg]overlay=format=auto",
                gfx+"/backgrounded/"+key.strip(),
            ])
            subprocess.run([
                "ffmpeg", 
                "-i",
                gfx+"/backgrounded/"+key.strip(),
                "-vf",
                "scale=-1:"+val.strip(),
                "-y",
                gfx+"/scaled/"+key.strip(),
            ])
