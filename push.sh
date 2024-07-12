#/!/bash

echo "alias minishell='$PWD/minishell'" >> ~/.zshrc

make fclean
git add .
if [ -z "$1" ]
then
    echo "No argument supplied"
    exit 1
else
    git commit -m "$1"
fi
git push origin parsing # while parsing is the current branch =------------------------
