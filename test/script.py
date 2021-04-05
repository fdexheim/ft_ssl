import os
import sys

def compare(command, testfile):
    os.system("./ft_ssl " + command + " " + testfile)
    os.system("openssl " + command + " " + testfile)

compare(sys.argv[1], sys.argv[2])
