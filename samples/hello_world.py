#!/usr/bin/env python3

import pyQuiri as piq

def main():
    tree = piq.kd_tree(3)
    key = (1.4,2,3)
    tree.add(key,"first point")
    tree.add((-1,-2,-2),"second point")
    tree.add((+1,+2,+2),"third point")
    tree.add(key,"first point (copy)")

    tree.build();

    value = tree.find(key)
    print('kdtree.find('+str(key)+') = '+str(value))
    key = (1,1,1)
    value = tree.find(key)
    print('kdtree.find('+str(key)+') = '+str(value))
    
main()
