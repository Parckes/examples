#! /usr/bin/env python
# -*- coding: utf-8 -*-

import xlrd
import os
import csv
import sys
from pathlib import Path
import collections

def csv_from_excel(excel_file):
    wb = xlrd.open_workbook(excel_file)
    sh = wb.sheet_by_name('Sheet1')

    your_csv_file = open(f"{excel_file[:-4:1]}.csv", 'wb')
    wr = csv.writer(your_csv_file, quoting=csv.QUOTE_ALL)

    for rownum in xrange(sh.nrows):
        wr.writerow(sh.row_values(rownum))

    your_csv_file.close()

amount_of_files = len(sys.argv) #amount of file where we need to delete duplicates
print(amount_of_files)

list_of_doi = [] #set of unique DOI

for i in range(1, amount_of_files): #going throw all files

    with open(sys.argv[i], encoding='utf-8') as csv_fd:

        with open('temp.csv', 'w', encoding='utf-8', newline='') as temp: #new file without duplicates

            print("the begin")
            writer = csv.writer(temp)
            reader = csv.reader(csv_fd, delimiter=',')

            row = next(reader)
            writer.writerow(row)
            doi_column = 0

            for name_of_column in row:                  #which column contains DOI information
                if 'DOI' in name_of_column:
                    doi_column = row.index(name_of_column)
                    break

            for row in reader:                       #checking DOI

                    #writer.writerow(row)
                    list_of_doi.append(row[doi_column])
            print("the end")

print (len(list_of_doi))
print (len(set(list_of_doi)))
uniqueList = []

for_stack = lambda array: sorted(list(set([x for x in array if array.count(x) > 1])))

print(for_stack(list_of_doi))


with open("log.txt", "w", encoding='utf-8') as f:
    f.write(str([item for item, count in collections.Counter(list_of_doi).items() if count > 1]))

    #p = Path('temp.csv')
    #p.replace(sys.argv[i])
    #os.remove('temp.csv')
    #os.rename('temp.csv', sys.argv[i])
