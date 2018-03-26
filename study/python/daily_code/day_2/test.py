#!/usr/bin/env python
# coding=utf-8

import turtle

def drawSnake(rad,angle,len,neckrad):
    for i in range(len):
         turtle.circle(rad,angle)
         turtle.circle(-rad,angle)
    
    turtle.circle(rad,angle/2)
    turtle.fd(rad)
    turtle.circle(neckrad+1,120)
    turtle.fd(rad*5)

def main():
    turtle.setup(1300,800,0,0)
    pythonsize = 30
    turtle.pensize(pythonsize)
    turtle.pencolor("blue")
    turtle.seth(-40)
    drawSnake(40,80,5,pythonsize/2)

main() 

turtle.done()   
