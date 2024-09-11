/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgouzi <sgouzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 21:16:36 by sgouzi            #+#    #+#             */
/*   Updated: 2024/09/11 22:13:34 by sgouzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

#define SGL_QUOTE '\''
#define DBL_QUOTE '\"'

char		*concat(t_db *db, char *s, char single_char);
void		skip_spaces(char *line, int *i);
int			count(char *line, char c);
int			are_all(char *str, int (*f)(int));
int			all_whitespaces(char *line, int start, int end);
int			is_whitespace(char c);
int			strongest_operator(char *line, t_quote *q);
int			count_between_op(t_db *db, char *line, int op, t_tracker *tracker);
int			count_array_len(char **args);
char		*sub(t_db *db, char *line, int i, int j);
char		*whithout_quotes_free_db(t_db *db, char *line);
bool		contains(char *str, char *sub);
int			find_c(char *str, char c);
int			length_til(char *str, char stop);
int			ft_strcmp(const char *s1, const char *s2);
char		*ft_strdup_ec(t_db *db, const char *s1);
char		*ft_strjoin_ec(t_db *db, char const *s1, char const *s2);
int			is_str_empty(t_db *db, char *str);
bool		is_inside_quotes_line(char *line, int index);
bool		starts_with(char *str, char *sub);
bool		same_s(char *s1, char *s2);
char		*without_quotes(t_db *db, char *line, t_quote *q);
bool		is_quote(t_quote *head, int i);
void		put_fd(int fd, char *s);
void		skip_atoll(char *str, int *i, int *sign);
long long	ft_atoll(char *str, bool *error);