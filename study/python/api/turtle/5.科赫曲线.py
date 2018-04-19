#!/usr/bin/env python
# coding=utf-8

#绘制科赫雪花
import turtle
import time

count = 0

def koch(size,n):  
    global count  
    if n == 0:
        turtle.fd(size)
        count += 1
    else:
        for angle in [0,60,-120,60]:
            turtle.left(angle)
            count += 1
            koch(size/3,n-1)
    
def drawsnow():
    for i in range(0,3):
        koch(400,3)  
        turtle.right(120)

          
def main():
    #绘制基本背景
    turtle.speed(0)
    turtle.setup(1200,1200)
    turtle.penup()
    turtle.goto(-300,-50)
    turtle.pendown()
    turtle.pensize(2)
    turtle.color('blue')
    start =time.clock()
    #绘制雪花
    drawsnow()
    #记录时间和绘制次数  
    end = time.clock()
    print(count)
    dur = end - start
    print(dur)
    turtle.hideturtle()
    turtle.done()
    
main()
