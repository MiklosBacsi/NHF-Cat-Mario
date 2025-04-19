valgrind --leak-check=full \
         --show-leak-kinds=definite,indirect,possible,possible \
         --errors-for-leak-kinds=definite \
         --suppressions=valgrind.supp \
         ./Cat-Mario