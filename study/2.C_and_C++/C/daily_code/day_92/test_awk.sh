#!/bin/bash

awk 'BEGIN{info="this is a test!";print index(info,"test")?"ok":"no found";}' test.c

