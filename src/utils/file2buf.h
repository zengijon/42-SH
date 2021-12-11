#pragma once

// take the filename in parameter, read him, and return the string of the
// content of the file
char *file2buf(char *filename);
char *expand_special_var(char *buffer, char *val_list);
char *stdin2buf(void);
