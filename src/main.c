/*
 * Portfolio Generator
 * 
 * This program generates a personal portfolio website (HTML file) based on user input.
 * The user enters their name, bio, about me, skills, education, projects, and contact info.
 * The program then reads a template HTML file, replaces placeholders with the user's data,
 * and outputs the final index.html file.
 * 
 * Required files in the assets directory:
 *   - template.html (the HTML template with placeholders)
 *   - style.css
 *   - pfp.jpg and/or defaultpfp.jpg 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "functions.h"

#define MAX_INPUT_LENGTH 1000      // Maximum characters for a single user input (name, bio, etc.)
#define MAX_FILE_SIZE 50000        // Maximum size for the HTML file content
#define MAX_SKILLS 10              // Maximum number of skills the user can enter
#define MAX_PROJECTS 10            // Maximum number of projects the user can enter
#define MAX_EDUCATION_ENTRIES 10   // Maximum number of education entries the user can enter
#define MAX_PATH_LENGTH 512        // Maximum length for file paths

int main(int argc, char *argv[]) {
    
    // Basic user information
    char name[MAX_INPUT_LENGTH];
    char bio[MAX_INPUT_LENGTH];
    char about_me[MAX_INPUT_LENGTH];
    char contact_info[MAX_INPUT_LENGTH];
    
    // Skills array
    char skills[MAX_SKILLS][MAX_INPUT_LENGTH];
    int skill_count = 0;
    
    // Education: school, degree, and year for each entry
    char education_school[MAX_EDUCATION_ENTRIES][MAX_INPUT_LENGTH];
    char education_degree[MAX_EDUCATION_ENTRIES][MAX_INPUT_LENGTH];
    char education_year[MAX_EDUCATION_ENTRIES][MAX_INPUT_LENGTH];
    int education_count = 0;
    
    // Projects: title and description for each
    char project_titles[MAX_PROJECTS][MAX_INPUT_LENGTH];
    char project_descriptions[MAX_PROJECTS][MAX_INPUT_LENGTH];
    int project_count = 0;
    
    // File paths
    char template_path[MAX_PATH_LENGTH];
    char output_path[MAX_PATH_LENGTH];
    char profile_picture_path[MAX_PATH_LENGTH];
    char css_path[MAX_PATH_LENGTH];
    char default_picture_path[MAX_PATH_LENGTH];
    
    // Other variables
    int menu_choice;
    const char *profile_picture_filename;
    
    
    // ----------------------------------------
    // Setup: Build file paths
    // ----------------------------------------
    
    extract_executable_directory(argv[0]);
    
    build_file_path(template_path, "assets/template.html");
    build_file_path(output_path, "index.html");
    build_file_path(profile_picture_path, "assets/pfp.jpg");
    build_file_path(css_path, "assets/style.css");
    build_file_path(default_picture_path, "assets/defaultpfp.jpg");
    
    
    // ----------------------------------------
    // Welcome screen and file check
    // ----------------------------------------
    
    printf("=== Portfolio Generator ===\n\n");
    
    check_required_files(template_path, css_path, profile_picture_path, default_picture_path);
    
    printf("1 - Start generating portfolio\n");
    printf("2 - Show instructions and then start generating protfolio\n");
    printf("3 - Exit the program\n");
    menu_choice = read_integer_with_validation("Enter your choice: \n", 1, 3);
    
    if (menu_choice == 2) {
        display_instructions();
    }
    if (menu_choice == 3) {
        printf("Terminated program.\n");
        return 0;
    }
    
    
    // ----------------------------------------
    // Collect user input: Basic info
    // ----------------------------------------
    
    printf("Enter your name: ");
    read_line_from_user(name, MAX_INPUT_LENGTH);
    
    printf("Enter your bio (short tagline): ");
    read_line_from_user(bio, MAX_INPUT_LENGTH);
    
    printf("Enter about me text: ");
    read_line_from_user(about_me, MAX_INPUT_LENGTH);
    
    
    // ----------------------------------------
    // Collect user input: Skills
    // ----------------------------------------
    
    printf("\n--- Skills ---\n");
    
    char skill_prompt[100];
    sprintf(skill_prompt, "How many skills? (0-%d): ", MAX_SKILLS);
    skill_count = read_integer_with_validation(skill_prompt, 0, MAX_SKILLS);
    
    for (int i = 0; i < skill_count; i++) {
        printf("Skill %d: ", i + 1);
        read_line_from_user(skills[i], MAX_INPUT_LENGTH);
    }
    
    
    // ----------------------------------------
    // Collect user input: Education
    // ----------------------------------------
    
    printf("\n--- Education ---\n");
    
    char education_prompt[100];
    sprintf(education_prompt, "How many education entries? (0-%d): ", MAX_EDUCATION_ENTRIES);
    education_count = read_integer_with_validation(education_prompt, 0, MAX_EDUCATION_ENTRIES);
    
    for (int i = 0; i < education_count; i++) {
        printf("\nEducation %d school/university: ", i + 1);
        read_line_from_user(education_school[i], MAX_INPUT_LENGTH);
        printf("Education %d degree/field: ", i + 1);
        read_line_from_user(education_degree[i], MAX_INPUT_LENGTH);
        printf("Education %d year(s): ", i + 1);
        read_line_from_user(education_year[i], MAX_INPUT_LENGTH);
    }
    
    
    // ----------------------------------------
    // Collect user input: Projects
    // ----------------------------------------
    
    printf("\n--- Projects ---\n");
    
    char project_prompt[100];
    sprintf(project_prompt, "How many projects? (0-%d): ", MAX_PROJECTS);
    project_count = read_integer_with_validation(project_prompt, 0, MAX_PROJECTS);
    
    for (int i = 0; i < project_count; i++) {
        printf("\nProject %d title: ", i + 1);
        read_line_from_user(project_titles[i], MAX_INPUT_LENGTH);
        printf("Project %d description: ", i + 1);
        read_line_from_user(project_descriptions[i], MAX_INPUT_LENGTH);
    }
    
    
    // ----------------------------------------
    // Collect user input: Contact (last, since it's at the bottom of the website)
    // ----------------------------------------
    
    printf("\n--- Contact ---\n");
    printf("Enter contact info (email, phone, socials): ");
    read_line_from_user(contact_info, MAX_INPUT_LENGTH);
    printf("\n---------------------------------------------\n");
    
    
    // ----------------------------------------
    // Check profile picture
    // ----------------------------------------
    
    if (check_file_exists(profile_picture_path)) {
        profile_picture_filename = "assets/pfp.jpg";
        printf("\n[OK] Profile picture found: pfp.jpg\n");
    } else {
        profile_picture_filename = "assets/defaultpfp.jpg";
        printf("\n[!] No pfp.jpg found - using default picture (defaultpfp.jpg)\n");
    }
    
    
    // ----------------------------------------
    // Build HTML content for each section
    // ----------------------------------------
    
    // Build skills HTML
    // +50 for HTML tags, whitespaces, etc.
    char skills_html[MAX_FILE_SIZE] = "";
    for (int i = 0; i < skill_count; i++) {
        char single_skill_html[MAX_INPUT_LENGTH + 50];
        sprintf(single_skill_html, "<li>%s</li>\n                ", skills[i]);
        strcat(skills_html, single_skill_html);
    }
    
    // Build education HTML
    // *3 for three inputs (school, degree, year), +300 for HTML tags
    char education_html[MAX_FILE_SIZE] = "";
    for (int i = 0; i < education_count; i++) {
        char single_education_html[MAX_INPUT_LENGTH * 3 + 300];
        sprintf(single_education_html, 
            "<div class=\"education-entry\">\n"
            "                <h4>%s</h4>\n"
            "                <p class=\"degree\">%s</p>\n"
            "                <p class=\"year\">%s</p>\n"
            "            </div>\n\n            ",
            education_school[i], education_degree[i], education_year[i]);
        strcat(education_html, single_education_html);
    }
    
    // Build projects HTML
    // *2 for two inputs (title, description), +200 for HTML tags
    char projects_html[MAX_FILE_SIZE] = "";
    for (int i = 0; i < project_count; i++) {
        char single_project_html[MAX_INPUT_LENGTH * 2 + 200];
        sprintf(single_project_html, 
            "<div class=\"project\">\n"
            "                <h4>%d. %s</h4>\n"
            "                <p>%s</p>\n"
            "            </div>\n\n            ",
            i + 1, project_titles[i], project_descriptions[i]);
        strcat(projects_html, single_project_html);
    }
    
    
    // ----------------------------------------
    // Read template and generate final HTML
    // ----------------------------------------
    
    // Try to read template (check again in case user added it after the warning)
    char *html_content = read_file_contents(template_path);
    if (!html_content) {
        printf("\n[ERROR] template.html is still missing!\n");
        printf("Cannot generate portfolio without template.html.\n");
        printf("Press Enter to exit...");
        getchar();
        return 1;
    }
    
    //Replace all placeholders with actual content.

    char *temp;
    
    temp = replace_placeholder(html_content, "{{NAME}}", name);
    free(html_content);
    html_content = temp;
    
    temp = replace_placeholder(html_content, "{{BIO}}", bio);
    free(html_content);
    html_content = temp;
    
    temp = replace_placeholder(html_content, "{{ABOUT}}", about_me);
    free(html_content);
    html_content = temp;
    
    temp = replace_placeholder(html_content, "{{CONTACT}}", contact_info);
    free(html_content);
    html_content = temp;
    
    temp = replace_placeholder(html_content, "{{SKILLS}}", skills_html);
    free(html_content);
    html_content = temp;
    
    temp = replace_placeholder(html_content, "{{EDUCATION}}", education_html);
    free(html_content);
    html_content = temp;
    
    temp = replace_placeholder(html_content, "{{PROJECTS}}", projects_html);
    free(html_content);
    html_content = temp;
    
    temp = replace_placeholder(html_content, "{{PFP}}", profile_picture_filename);
    free(html_content);
    html_content = temp;
    
    if (write_file_contents(output_path, html_content)) {
        printf("\n=== Success! ===\n");
        printf("Your portfolio has been generated: %s\n", output_path);
        printf("Open index.html in your browser to view it.\n");
    } else {
        printf("Error: Could not create index.html\n");
    }
    
    free(html_content);
    
    return 0;
}