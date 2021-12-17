import os
import sys

def compare(mode, testfile, testflags):
    my = "my"
    myd = "myd"
    ref = "ref"
    refd = "refd"

    refcmd = "openssl " + mode + " -in " + testfile + " -out " + ref
    refdcmd = "openssl " + mode + " -d -in " + ref + " -out " + refd
    mycmd = "./ft_ssl " + mode + " -i " + testfile + " -o " + my
    mydcmd = "./ft_ssl " + mode + " -d -i " + my + " -o " + myd

    key  = "133457799bbcdff1"
    iv   = "0000000000000001"
    salt = "6363636363636363"
    password = "a"

    if (testflags.find('P') != -1):
        refcmd += " -p "
        refdcmd += " -p "
    if (testflags.find('k') != -1):
        refcmd += " -K " + key
        refdcmd += " -K " + key
        mycmd += " -k " + key
        mydcmd += " -k " + key
    if (testflags.find('s') != -1):
        refcmd += " -S " + salt
        refdcmd += " -S " + salt
        mycmd += " -s " + salt
        mydcmd += " -s " + salt
    if (testflags.find('v') != -1):
        refcmd += " -iv " + iv
        refdcmd += " -iv " + iv
        mycmd += " -v " + iv
        mydcmd += " -v " + iv
    if (testflags.find('p') != -1):
        refcmd += " -pass pass:" + password
        refdcmd += " -pass pass:" + password
        mycmd += " -p " + password
        mydcmd += " -p " + password
    if (testflags.find('b') != -1):
        refcmd += " -a "
        refdcmd += " -a "
        mycmd += " -a "
        mydcmd += " -a "

    os.system("rm " + my)
    os.system("rm " + ref)
    os.system("rm " + myd)
    os.system("rm " + refd)


    print("\n======[ OPENSSL ENCRYPT ]======")
    print(refcmd)
    os.system(refcmd)
    print("\n======[ OPENSSL DECRYPT ]======")
    print(refdcmd)
    os.system(refdcmd)
    print("\n======[ FT_SSL  ENCRYPT ]======")
    print(mycmd)
    os.system(mycmd)
    print("\n======[ FT_SSL  DECRYPT ]======")
    print(mydcmd)
    os.system(mydcmd)

    print("\n===============================\n")
    print("[ENCRYPT] diff " + my + " (" + str(os.path.getsize(my))  + " bytes)   VS   " + ref + " (" + str(os.path.getsize(ref))  + " bytes)")
    os.system("diff " + my + " " + ref)
    print("[DECRYPT] diff " + myd + " (" + str(os.path.getsize(myd))  + " bytes)   VS   " + refd + " (" + str(os.path.getsize(refd))  + " bytes)")
    os.system("diff " + myd + " " + refd)

    print("\n===============================\n")

    if (testflags.find('c') != -1):
        print("cat my  (" + str(os.path.getsize(my)) + " bytes)")
        os.system("cat " + my)
        print("\ncat ref (" + str(os.path.getsize(ref)) + " bytes)")
        os.system("cat " + ref)
        print("\ncat myd (" + str(os.path.getsize(myd)) + " bytes)")
        os.system("cat " + myd)
        print("\ncat refd (" + str(os.path.getsize(refd)) + " bytes)")
        os.system("cat " + refd)
        print("");

compare(sys.argv[1], sys.argv[2], sys.argv[3])
