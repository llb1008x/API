#!/bin/bash

awk '{count++;print $0;} END{print "user count is ",count}' /etc/passwd
