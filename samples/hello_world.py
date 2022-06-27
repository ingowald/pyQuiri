#!/usr/bin/env python3

import pyQuiri as piq

def main():
    kd = piq.kd_tree(3)
    key = (1.4,2,3)
    kd.add(key,5.)
    value = kd.find(key)
    print('kdtree.find('+str(key)+') = '+str(value))
    key = (1,1,1)
    value = kd.find(key)
    print('kdtree.find('+str(key)+') = '+str(value))

main()
