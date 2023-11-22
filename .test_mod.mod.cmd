cmd_/shared_folder/test_mod.mod := printf '%s\n'   test_mod.o | awk '!x[$$0]++ { print("/shared_folder/"$$0) }' > /shared_folder/test_mod.mod
