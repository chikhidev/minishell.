#/!/bash

# ask for the path of the minishell
$minishell = read -p "Enter the path of the minishell: " minishell
echo "alias minishell='$minishell/minishell'" >> ~/.zshrc
echo "alias minishell='$PWD/minishell'" >> ~/.zshrc

make fclean
git add .