import os
import sys

def t(command):
    print("> " + command)
    os.system(command)
    print(">")

def run(command):
    exec_command = "./ft_ssl " + command
    t("echo \"42 is nice\" | " + " openssl " + command)

    t("echo \"42 is nice\" | " + exec_command)

    t("echo \"42 is nice\" | " + exec_command + " -p ")

    t("echo \"Pity the living.\" | " + exec_command + " -q -r ")

    t("echo \"And above all,\" > test/file")

    t(exec_command + " test/file")

    t(exec_command + " -r test/file")

    t(exec_command + " -s \"pity those that aren't following baerista on spotify.\"")

    t("echo \"be sure to handle edge cases carefully\" | " + exec_command + " -p test/file")

    t("echo \"some of this will not make sense at first\" | " + exec_command + " test/file")

    t("echo \"but eventually you will understand\" | " + exec_command + " -p -r test/file")

    t("echo \"GL HF let's go\" | " + exec_command + " -p -s \"foo\" test/file")

    t("echo \"one more thing\" | " + exec_command + " -r -p -s \"foo\" test/file -s \"bar\"")

    t("echo \"just to be extra clear\" | " + exec_command + " -r -q -p -s \"foo\" test/file")

run(sys.argv[1])
