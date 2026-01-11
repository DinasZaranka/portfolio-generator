#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Extracts the directory path from the executable's full path.
 * 
 * For example, if argv[0] is "C:\Users\Me\Desktop\folder\generator.exe",
 * this function extracts "C:\Users\Me\Desktop\folder\" and stores it in executable_directory.
 * 
 */
void extract_executable_directory(const char *argument_zero);

/*
 * Builds a full file path by combining the executable directory with a filename.
 * 
 * For example, if executable_directory is "C:\folder\" and filename is "template.html",
 * the result will be "C:\folder\template.html".
 */
void build_file_path(char *destination, const char *filename);

int check_file_exists(const char *filepath);

char *read_file_contents(const char *filepath);

int write_file_contents(const char *filepath, const char *content);

/*
 * Replaces the first occurrence of a placeholder with a replacement string.
 * 
 * This is the core function for generating HTML - finds placeholders like "{{NAME}}"
 * and replaces them with actual user data.
 * 
 * Returns a string (caller must free it).
 */
char *replace_placeholder(const char *source, const char *placeholder, const char *replacement);

void read_line_from_user(char *buffer, int max_length);

void clear_input_buffer(void);

/*
 * Reads an integer with validation.
 * Keeps asking until user enters a valid number within the range.
 */
int read_integer_with_validation(const char *prompt, int minimum, int maximum);

/*
 * Displays instructions for using the program.
 */
void display_instructions(void);

/*
 * Checks all required files at startup and warns about missing ones.
 * Gives user a chance to add files before entering all their data.
 */
void check_required_files(const char *template_path, const char *css_path, 
                          const char *profile_picture_path, const char *default_picture_path);

#endif