#!/usr/bin/env python3

import pyQuiri as piq
import csv

def main():
    tree = piq.kd_tree(2)
    dataPoints = ([])
    # this is a test data set in CSV format: "country, capital, lon, lat"
    with open('../samples/testData2D.csv', newline='') as csvfile:
       tuples = csv.reader(csvfile, delimiter=',', quotechar='|')
       for tuple in tuples:
           s_coords = tuple[-2:]
           print('s_coords '+str(s_coords))
           f_coords = [float(x) for x in s_coords]
           dataPoints.append(f_coords);
           capital  = tuple[:2]
           tree.add(f_coords,capital)
    tree.build()
    print("########### Test query actual data points:")
    for point in dataPoints:
        print("tree value for point "+str(point)+" = "+str(tree.find(point)))
        
    print("########### Test query closest similar point:")
    for point in dataPoints:
        similarPoint=[x+.1 for x in point]
        print("closest value for point "+str(similarPoint)+" = "+str(tree.findClosest(similarPoint)))
    
main()
