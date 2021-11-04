import os
import sys

def compare(mode, testfile):
    my = "my"
    myd = "myd"
    ref = "ref"
    refd = "refd"

    key = "133457799bbcdff1"
    iv = "0000000000000000"

    os.system("rm " + my)
    os.system("rm " + ref)
    os.system("rm " + myd)
    os.system("rm " + refd)

    print("--- OPENSSL ENCRYPT ---")
    os.system("openssl " + mode + " -in " + testfile + " -out " + ref + " -K " + key + " -iv " + iv)
    print("--- OPENSSL DECRYPT ---")
    os.system("openssl " + mode + " -d -in " + ref + " -out " + refd + " -K " + key + " -iv " + iv)
    print("--- FT_SSL ENCRYPT ---")
    os.system("./ft_ssl " + mode + " -i " + testfile + " -o " + my + " -k " + key + " -v " + iv)
    print("--- FT_SSL DECRYPT ---")
    os.system("./ft_ssl " + mode + " -d -i " + my + " -o " + myd + " -k " + key + " -v " + iv)

    print("")
    print("[ENCRYPT] diff " + my + " VS " + ref)
    os.system("diff " + my + " " + ref)
    print("[DECRYPT] diff " + myd + " VS " + refd)
    os.system("diff " + myd + " " + refd)
    print("")

    print("cat my  (" + str(os.path.getsize(my)) + " bytes)")
    os.system("cat " + my)
    print("\ncat ref (" + str(os.path.getsize(ref)) + " bytes)")
    os.system("cat " + ref)
    print("\ncat myd (" + str(os.path.getsize(myd)) + " bytes)")
    os.system("cat " + myd)
    print("\ncat refd (" + str(os.path.getsize(refd)) + " bytes)")
    os.system("cat " + refd)
    print("");

#    os.system("rm " + my)
#    os.system("rm " + myd)
#    os.system("rm " + ref)
#    os.system("rm " + refd)

compare(sys.argv[1], sys.argv[2])
