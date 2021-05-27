import os
import sys

def run(exec_command):
    print("> echo \"pickle rick\" | " + exec_command)
    os.system("echo \"pickle rick\" | " + exec_command)

    print("\n> echo \"Do not pity the dead, Harry.\" | " + exec_command + " -p")
    os.system("echo \"Do not pity the dead, Harry.\" | " + exec_command + " -p")

    print("\n> echo \"Pity the living.\" | " + exec_command + " -p -r")
    os.system("echo \"Pity the living.\" | " + exec_command + " -p -r")

    print("\n> echo \"And above all,\" > test/file")
    os.system("echo \"And above all,\" > test/file")

    print("\n> " + exec_command + " test/file")
    os.system(exec_command + " test/file")

    print("\n> " + exec_command + " -r test/file")
    os.system(exec_command + " -r test/file")

    print("\n> " + exec_command + " -s \"pity those that aren't following baerista on spotify.\"")
    os.system(exec_command + " -s \"pity those that aren't following baerista on spotify.\"")

    print("\n> echo \"be sure to handle edge cases carefully\" | " + exec_command + " -p test/file")
    os.system("echo \"be sure to handle edge cases carefully\" | " + exec_command + " -p test/file")

    print("\n> echo \"some of this will not make sense at first\" | " + exec_command + " test/file")
    os.system("echo \"some of this will not make sense at first\" | " + exec_command + " test/file")

    print("\n> echo \"but eventually you will understand\" | " + exec_command + " -p -r test/file")
    os.system("echo \"but eventually you will understand\" | " + exec_command + " -p -r f test/file")

    print("\n> echo \"GL HF let's go\" | " + exec_command + " -p -s \"foo\" test/file")
    os.system("echo \"GL HF let's go\" | " + exec_command + " -p -s \"foo\" test/file")

    print("\n> echo \"one more thing\" | " + exec_command + " -r -p -s \"foo\" test/file -s \"bar\"")
    os.system("echo \"one more thing\" | " + exec_command + " -r -p -s \"foo\" test/file -s \"bar\"")

    print("\n> echo \"just to be extra clear\" | " + exec_command + " -r -q -p -s \"foo\" test/file")
    os.system("echo \"just to be extra clear\" | " + exec_command + " -r -q -p -s \"foo\" test/file")

run("./ft_ssl " + sys.argv[1])
