#!/usr/bin/env python
# coding=utf-8



import numpy as np
import matplotlib.pyplot as plt

plt.figure(1)
ax=plt.subplot(111)
x=np.linspace(0,np.pi*2,200)

r=2*np.cos(x)
ax.plot(r*np.cos(x),r*np.sin(x))


r=1
ax.plot(r*np.cos(x),r*np.sin(x))


plt.show()
