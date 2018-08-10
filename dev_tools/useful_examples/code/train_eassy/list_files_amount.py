#coding=utf-8

import os
import sys

def check_if_exist(path):
    return os.path.exists(path)

def make_if_not_exist(path):
    if not os.path.exists(path):
        os.makedirs(path)


def main(data_dir):
	c_amount = {}

	classes = os.listdir(data_dir)	
	for c in classes:
		c_full = os.path.join(data_dir, c)
		amount = os.listdir(c_full)
		c_amount[c]=len(amount)

	return c_amount

if __name__ == '__main__':
	data_dir = ""

	if len(sys.argv) < 2:
		print "\ndata_dir needed here\n"
		print "Usage: %s data_dir\n" % sys.argv[0]
		sys.exit(1)
	else:
		data_dir = sys.argv[1]

	c_amount = main(data_dir)
	c_amount = sorted(c_amount.items(), lambda x, y: cmp(x[1], y[1]))
	for c, amount in c_amount:
		print "class: %s | %s" %(c,amount)