cmd_/shared_folder/test_mod00.mod := printf '%s\n'   test_mod00.o | awk '!x[$$0]++ { print("/shared_folder/"$$0) }' > /shared_folder/test_mod00.mod
