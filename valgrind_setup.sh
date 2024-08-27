echo 'alias vl="valgrind --suppressions=readline.supp --leak-check=full --show-leak-kinds=all" ' >> ~/.bashrc

# create readline.supp if it doesn't exist
if [ ! -f readline.supp ]; then
    echo "Creating readline.supp"
    echo "{ " >> readline.supp
    echo "   ignore_readline_leaks" >> readline.supp
    echo "   Memcheck:Leak" >> readline.supp
    echo "   ..." >> readline.supp
    echo "   obj:/**/libreadline.so*" >> readline.supp
    echo "}" >> readline.supp
fi