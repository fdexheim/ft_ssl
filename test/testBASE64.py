import os
import sys

def compare(testfile):
#    my = testfile + "Eoutput"
#    myd = testfile + "Doutput"
#    ref = testfile + "REFEoutput"
#    refd = testfile + "REFDoutput"

    my = "my"
    myd = "myd"
    ref = "ref"
    refd = "refd"

    os.system("./ft_ssl base64 -i " + testfile + " -o " + my)
    os.system("./ft_ssl base64 -d -i " + my + " -o " + myd)
    os.system("openssl base64 -in " + testfile + " -out " + ref)
    os.system("openssl base64 -d -in " + ref + " -out " + refd)

    print("[ENCRYPT] diff " + my + " VS " + ref)
    os.system("diff " + my + " " + ref)
    print("[DECRYPT] diff " + myd + " VS " + refd)
    os.system("diff " + myd + " " + refd)

    os.system("rm " + my)
    os.system("rm " + myd)
    os.system("rm " + ref)
    os.system("rm " + refd)

compare(sys.argv[1])
