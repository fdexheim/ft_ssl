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

    os.system("./ft_ssl des -i " + testfile + " -o " + my + " -k 133457799bbcdff1 ")
#    os.system("./ft_ssl des -d -i " + my + " -o " + myd)
    os.system("openssl des -in " + testfile + " -out " + ref + " -K 133457799bbcdff1 -iv 0000000000000000")
#    os.system("openssl des -d -in " + ref + " -out " + refd)

    print("[ENCRYPT] diff " + my + " VS " + ref)
    os.system("diff " + my + " " + ref)
#    print("[DECRYPT] diff " + myd + " VS " + refd)
#    os.system("diff " + myd + " " + refd)

#    os.system("rm " + my)
#    os.system("rm " + myd)
#    os.system("rm " + ref)
#    os.system("rm " + refd)

compare(sys.argv[1])
