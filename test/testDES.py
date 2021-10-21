import os
import sys

def compare(mode, testfile):
#    my = testfile + "Eoutput"
#    myd = testfile + "Doutput"
#    ref = testfile + "REFEoutput"
#    refd = testfile + "REFDoutput"

    my = "my"
    myd = "myd"
    ref = "ref"
    refd = "refd"

    key = "133457799bbcdff1"
    iv = "0000000000000000"

    os.system("rm " + my)
    os.system("rm " + ref)

    os.system("./ft_ssl " + mode + " -i " + testfile + " -o " + my + " -k " + key)
#    os.system("./ft_ssl des -d -i " + my + " -o " + myd)
    os.system("openssl " + mode + " -in " + testfile + " -out " + ref + " -K " + key + " -iv " + iv)
#    os.system("openssl des -d -in " + ref + " -out " + refd)

    print("[ENCRYPT] diff " + my + " VS " + ref)
    os.system("diff " + my + " " + ref)
#    print("[DECRYPT] diff " + myd + " VS " + refd)
#    os.system("diff " + myd + " " + refd)

    print("cat my  (" + str(os.path.getsize(my)) + " bytes)")
    os.system("cat " + my)
    print("\ncat ref (" + str(os.path.getsize(ref)) + " bytes)")
    os.system("cat " + ref)

#    os.system("rm " + my)
#    os.system("rm " + myd)
#    os.system("rm " + ref)
#    os.system("rm " + refd)

compare(sys.argv[1], sys.argv[2])
