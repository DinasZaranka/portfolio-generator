#include "functions.h"

#define MAX_INPUT_LENGTH 1000      // Maximum characters for a single user input (name, bio, etc.)
#define MAX_FILE_SIZE 50000        // Maximum size for the HTML file content
#define MAX_SKILLS 10              // Maximum number of skills the user can enter
#define MAX_PROJECTS 10            // Maximum number of projects the user can enter
#define MAX_EDUCATION_ENTRIES 10   // Maximum number of education entries the user can enter
#define MAX_PATH_LENGTH 512        // Maximum length for file paths

char executable_directory[MAX_PATH_LENGTH] = "";

void extract_executable_directory(const char *argument_zero) {
    // Copy the full path to our global variable
    strcpy(executable_directory, argument_zero);
    
    /* 
     * Find the last slash in the path. We check both forward slash (/) and backslash (\)
     * because Windows uses backslash and Linux/Mac use forward slash.
     */
    char *last_forward_slash = strrchr(executable_directory, '/');
    char *last_backslash = strrchr(executable_directory, '\\');
    
    // Use whichever slash appears last in the string
    char *last_separator = last_forward_slash > last_backslash ? last_forward_slash : last_backslash;
    
    if (last_separator) {
        // Cut the string right after the last slash, removing the filename
        *(last_separator + 1) = '\0';
    } else {
        // No slash found means the exe is in the current directory
        executable_directory[0] = '\0';
    }
}

void build_file_path(char *destination, const char *filename) {
    strcpy(destination, executable_directory);
    strcat(destination, filename);
}

int check_file_exists(const char *filepath) {
    FILE *file = fopen(filepath, "rb");
    if (file) {
        fclose(file);
        return 1;
    }
    return 0;
}

char *read_file_contents(const char *filepath) {
    FILE *file = fopen(filepath, "rb");
    if (!file) {
        printf("Error: Cannot open %s\n", filepath);
        return NULL;
    }
    
    // Find file size by seeking to the end
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);
    
    // Allocate memory: file_size + 1 for the null terminator
    char *content = malloc(file_size + 1);
    if (!content) {
        fclose(file);
        return NULL;
    }
    
    // Read the file and add null terminator
    fread(content, 1, file_size, file);
    content[file_size] = '\0';
    
    fclose(file);
    return content;
}

int write_file_contents(const char *filepath, const char *content) {
    FILE *file = fopen(filepath, "w");
    if (!file) {
        printf("Error: Cannot write to %s\n", filepath);
        return 0;
    }
    
    fputs(content, file);
    fclose(file);
    return 1;
}

char *replace_placeholder(const char *source, const char *placeholder, const char *replacement) {
    char *position = strstr(source, placeholder);
    
    if (!position) {
        // Placeholder not found - return a copy of original
        char *copy = malloc(strlen(source) + 1);
        strcpy(copy, source);
        return copy;
    }
    
    // Calculate lengths of each part
    size_t before_length = position - source;
    size_t placeholder_length = strlen(placeholder);
    size_t replacement_length = strlen(replacement);
    size_t after_length = strlen(position + placeholder_length);
    
    // Allocate memory for new string: before + replacement + after + null terminator
    char *result = malloc(before_length + replacement_length + after_length + 1);
    if (!result) return NULL;
    
    // Build the new string
    strncpy(result, source, before_length);
    strcpy(result + before_length, replacement);
    strcpy(result + before_length + replacement_length, position + placeholder_length);
    
    return result;
}

void read_line_from_user(char *buffer, int max_length) {
    if (fgets(buffer, max_length, stdin)) {
        // strcspn finds the newline position, we replace it with null terminator
        buffer[strcspn(buffer, "\n")] = '\0';
    }
}

void clear_input_buffer(void) {
    int character;
    while ((character = getchar()) != '\n' && character != EOF);
}

int read_integer_with_validation(const char *prompt, int minimum, int maximum) {
    int value;
    int scan_result;
    
    while (1) {
        printf("%s", prompt);
        scan_result = scanf("%d", &value);
        clear_input_buffer();
        
        if (scan_result != 1) {
            printf("[Error] Please enter a valid number.\n");
            continue;
        }
        
        if (value < minimum || value > maximum) {
            printf("[Error] Please enter a number between %d and %d.\n", minimum, maximum);
            continue;
        }
        
        return value;
    }
}

void display_instructions(void) {
    printf("\n");
    printf("=== HOW TO USE ===\n\n");
    printf("This program generates a personal portfolio website.\n\n");
    printf("You will be asked to enter:\n");
    printf("  - Your name and a short bio\n");
    printf("  - An 'About Me' description\n");
    printf("  - Your skills (up to %d)\n", MAX_SKILLS);
    printf("  - Your education (up to %d entries)\n", MAX_EDUCATION_ENTRIES);
    printf("  - Your projects with descriptions (up to %d)\n", MAX_PROJECTS);
    printf("  - Your contact information (email, socials, etc.)\n\n");
    printf("PROFILE PICTURE:\n");
    printf("  - Place a file named \"pfp.jpg\" in the assets folder\n");
    printf("  - If no \"pfp.jpg\" is found, a default picture 'defaultpfp.jpg' will be used\n\n");
    printf("OUTPUT:\n");
    printf("  - The program creates \"index.html\" in the current directory\n");
    printf("  - Open \"index.html\" in your browser to see your portfolio\n");
    printf("  - Make sure the \"pfp.jpg\" and \"style.css\" files are in the assets folders.\n\n");
    printf("==================\n\n");
}

void check_required_files(const char *template_path, const char *css_path, 
                          const char *profile_picture_path, const char *default_picture_path) {
    printf("=== Checking Files ===\n\n");
    
    // Check template.html - required, only warn (checked again at the end, if still no template then terminate)
    if (check_file_exists(template_path)) {
        printf("[OK] template.html found\n");
    } else {
        printf("[WARNING] template.html NOT FOUND - This file is required!\n");
        printf("          Make sure template.html is in the assets folder.\n");
    }
    
    // Check main.css 
    if (check_file_exists(css_path)) {
        printf("[OK] style.css found\n");
    } else {
        printf("[WARNING] style.css NOT FOUND!\n");
        printf("          Make sure style.css is in the assets folder.\n");
    }
    
    // Check profile pictures
    if (check_file_exists(profile_picture_path)) {
        printf("[OK] pfp.jpg found\n");
    } else {
        printf("[INFO] pfp.jpg not found - will use default picture\n");
        
        if (check_file_exists(default_picture_path)) {
            printf("[OK] defaultpfp.jpg found\n");
        } else {
            printf("[WARNING] defaultpfp.jpg NOT FOUND - No profile picture will be displayed!\n");
            printf("          Add pfp.jpg or defaultpfp.jpg to the assets folder.\n");
        }
    }
    
    printf("\n");
}