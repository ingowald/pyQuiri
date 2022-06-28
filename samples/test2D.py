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
        print("closest value for point "+str(similarPoint)+" = "+str(tree.find_closest(similarPoint)))
    #queryBoxes=([[-10,-10],[10,10]],[[20,20],[40,40]])
    queryBoxes=[((-10,-10),(10,10)),((20,20),(40,40))]
    print("########### Test query of a given box (values only):")
    for box in queryBoxes:
        print("all_values_in_range("+str(box)+") = "+str(tree.all_values_in_range(box[0],box[1])))
    print("########### Test query of a given box (key:value pairs):")
    for box in queryBoxes:
        print("all_points_in_range("+str(box)+") = "+str(tree.all_points_in_range(box[0],box[1])))
    
main()
