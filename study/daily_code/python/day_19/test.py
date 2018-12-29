#!/usr/bin/env python
# coding=utf-8

#体育竞技问题分析

from random import random



#打印帮助信息
def printInfo():
    print("这个程序模拟两个选手A和B的某种竞技比赛\n")
    print("程序运行需要A和B的能力值(以0到1之间小数标示)\n")



#输入相关的数据
def getInputs():
    proA=input("请输入选手A的能力值(0-1):")
    proB=input("请输入选手B的能力值(0-1):")
    N=input("请输入模拟比赛的场次:")
    
    return proA,proB,N


#模拟一场比赛结束的条件，A和B谁先到15分
def gameOver(a,b):
    return a==15 or b==15

#模拟一场
def simOnegames(proA,proB):
    scoreA,scoreB=0,0
    server="A"

    #核心，游戏规则，A发球局，赢了得分，输了为B的发球局
    while not gameOver(scoreA,scoreB):
        if server=="A":
            if random() < proA:
                scoreA += 1
            else :
                server="B"
        else :
            if random() < proB:
                scoreB += 1
            else :
                server="A"
    
    return scoreA,scoreB


#模拟N场比赛
def simNgames(proA,proB,N):
    winA,winB=0,0
    
    for i in range(N):
        scoreA,scoreB = simOnegames(proA,proB)    
        if scoreA == 15:
            winA += 1
        elif scoreB == 15:
            winB += 1    
    
    return winA,winB


def main():
    printInfo()
    proA,proB,N=getInputs()
    #scoreA,scoreB=simOnegames(proA,proB)
    winA,winB=simNgames(proA,proB,N)
    print("proA:{:},proB:{:}\n".format(winA,winB))
    print("A的胜率:{:%},B的胜率:{:%}\n".format(1.0*winA/N,1.0*winB/N))


main()    
