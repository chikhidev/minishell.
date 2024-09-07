#include "../includes/exec.h"
#include "../includes/main.h"
#include "../includes/parsing.h"
#include "../includes/string.h"

// bool match(const char *pattern, const char *string) {
//     while (*pattern != '\0' && *string != '\0')
//     {
//         if (*pattern == '*')
//         {
//             // Skip consecutive '*'
//             while (*pattern == '*')
//                 pattern++;
//             // If '*' is the last character in the pattern, it's a match
//             if (*pattern == '\0')
//                 return (true);
//             // Try matching the rest of the pattern with every substring
//             while (*string != '\0')
//             {
//                 if (match(pattern, string))
//                     return (true);
//                 string++;
//             }
//             return (false);
//         }
//         else if (*pattern != *string)
//             return (false);
//         pattern++;
//         string++;
//     }
//     // Skip any remaining '*' in the pattern
//     while (*pattern == '*')
//         pattern++;
//     // If we've reached the end of both strings, it's a match
//     return (*pattern == '\0' && *string == '\0');
// }

// int handle_wildcard(t_db *db, char  **result, char *pattern)
// {
//     DIR *curr_dir;
//     struct dirent *entry;
//     curr_dir = opendir(".");
//     if (curr_dir)
//     {
//         entry = readdir(curr_dir);
//         while (entry)
//         {
//             if (!starts_with(entry->d_name, ".") &&
//                 !starts_with(entry->d_name, "..") &&
//                 match(entry->d_name, pattern))

//                 result = append_word(db, result, entry->d_name, false);
//             entry = readdir(curr_dir);
//         }
//     }
//     return (SUCCESS);

// }

// char    *wildcard(t_db *db, char *pattern)
// {
//     (void)db;
//     int i;
//     char    *files;
//     char   *cwd = ".";
//     i = 0;
//     while (pattern[i])
//     {
//         if (pattern[i] == '*' && !is_inside_quotes_line(pattern, i))
//             files = get_dir_files(db, cwd);
//         i++;
//     }
//     return (NULL);
// }
