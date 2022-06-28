#!/usr/bin/env python3

import pyQuiri as piq
import csv

def main():
    tree = piq.kd_tree(3)
    dataPoints = ([])
    with open('../samples/testData3D.csv', newline='') as csvfile:
       tuples = csv.reader(csvfile, delimiter=',', quotechar='|')
       for tuple in tuples:
           s_coords = tuple[:-1]
           f_coords = [float(x) for x in s_coords]
           dataPoints.append(f_coords);
           value    = tuple[-1]
           tree.add(f_coords,value)
    tree.build()
    for point in dataPoints:
        print("tree value for point "+str(point)+" = "+str(tree.find(point)))
        
    
main()
