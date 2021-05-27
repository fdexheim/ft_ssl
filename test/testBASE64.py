import os
import sys

def compare(testfile):
    my = testfile + "Eoutput"
    myd = testfile + "Doutput"
    ref = testfile + "REFEoutput"
    refd = testfile + "REFDoutput"

    os.system("./ft_ssl base64 -i " + testfile + " -o " + my)
    os.system("./ft_ssl base64 -d -i " + my + " -o " + myd)
    os.system("openssl base64 -in " + testfile + " -out " + ref)
    os.system("openssl base64 -d -in " + ref + " -out " + refd)

    print(my + " VS " + ref)
    print(myd + " VS " + refd)
    os.system("diff " + my + " " + ref)
    os.system("diff " + myd + " " + refd)

    os.system("rm " + my)
    os.system("rm " + myd)
    os.system("rm " + ref)
    os.system("rm " + refd)

compare(sys.argv[1])
