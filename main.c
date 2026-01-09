/********* main.c ********
* Student Name: Noor Karabala
* Purpose: Runs the menu-driven CLI for registering, logging in, and managing users.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "a2_nodes.h"
#include "a2_functions.h"

int main()
{
    FILE *csv_file = fopen("user_details.csv", "r");
    if (csv_file == NULL)
    {
        perror("Error opening the CSV file");
        return 1;
    }

    // Parse CSV data and create users
    user_t *users = read_CSV_and_create_users(csv_file, 50);

    fclose(csv_file);

    unsigned short int choice = 0;

    char logged_in_username[30];  // Variable to store logged-in username


    while(choice != 3){

        print_menu();
        choice = get_input_usi(3);

        if (choice == 1){

            char username_[30];
            printf("\nEnter a username: ");
            scanf("%s", username_);
            to_lowercase(username_);

            if (find_user(users, username_) != NULL) {

                printf("\nUsername already exists. Please choose a different one.\n");

            } else {

                char password_[15];
                do {
                    printf("Enter a password (from 8 to 15 characters): ");
                    scanf("%s", password_);

                    if (strlen(password_) < 8) {
                        printf("\nPassword must be at least 8 characters long. Please try again.\n\n");
                    }
                } while (strlen(password_) < 8);

                users = add_user(users, username_, password_);
                printf("\n              **** User Added! ****\n");
            }
            
        }else if (choice == 2){    //login

            
            char username_[30];
            char password_[15];

            printf("\nEnter your username in lowercase: ");
            scanf("%s", username_);
            printf("Enter your password: ");
            scanf("%s", password_);

            user_t *user = find_user(users, username_);

            // If user not found or password does not match

            if (user == NULL || strcmp(user->password, password_) != 0) {
            printf("\nInvalid username or password.\n");

            } else {

                unsigned short int login_choice = 0;
                while (login_choice != 5){
                
                    strcpy(logged_in_username, username_);
                    
                    print_login_menu(username_);
                    login_choice = get_input_usi(5);
                
                    

                    if (login_choice == 1){

                        char old_password[15];
                        char new_password[15];

                        printf("Enter the old password: ");
                        scanf("%s", old_password);

                        if (strcmp(user->password, old_password) == 0) {
                            printf("Enter the new password: ");
                            scanf("%s", new_password);
                            strcpy(user->password, new_password);
                            printf("\n   **** Password updated successfully! ****\n");
                        } else {
                            printf("\nError: Incorrect old password.\n");
                        }

                    }else if (login_choice == 2){
                        
                        // Manage posts
                        printf("------------------------------------------------\n");
                        printf("                Managing %s posts            \n", user);
                        printf("------------------------------------------------\n");
                        if (user->posts == NULL) {
                            printf("\nNote: No posts available.\n");
                        }
                        printf("1. Add a new post\n");
                        printf("2. Remove a post\n");
                        printf("3. Return to main menu\n");
                        printf("Enter your choice: ");
                        unsigned short int choice_ = get_input_usi(3);

                        if (choice_ == 1) {
                            char new_post[250];
                            printf("Enter your post content: ");
                            scanf(" %[^\n]s", new_post);
                            add_post(user, new_post);
                            printf("------------------------------------------------\n");
                            printf("                Your posts                     \n");
                            display_all_user_posts(user);
                            printf("------------------------------------------------\n");
                        } 
                        else if (choice_ == 2) {
                            delete_post(user);
                            display_all_user_posts(user);
                        } 
                        else if (choice_ == 3) {
                            printf("\nReturning to the main menu...\n");
                        }
                    }else if (login_choice == 3){

                        unsigned short int choice_ = 0;
                        do {
                            printf("------------------------------------------------\n");
                            printf("          Managing %s friends                \n", user);
                            printf("------------------------------------------------\n");
                            printf("1. Add a new friend\n");
                            printf("2. Remove a friend\n");
                            printf("3. Return to main menu\n");
                            printf("Enter your choice: ");
                            choice_ = get_input_usi(3);

                            if (choice_ == 1) {

                                char new_friend[30];
                                printf("Enter name of friend you want to add: ");
                                scanf("%s", new_friend);

                                user_t *friend_user = find_user(users, new_friend);

                                if (friend_user == NULL) {
                                    // The user doesn't exist in the database
                                    printf("\nError: '%s' does not exist in the database.\n", new_friend);
                                } else {
                                    // The user exists, so add them as a friend
                                    add_friend(user, new_friend);
                                    printf("Friend added!\n");
                                }
                            } 
                            else if (choice_ == 2) {
                                printf("List of your friends: \n");
                                display_user_friends(user);
                                char friend_to_delete[30];

                                if (user->friends != NULL) {
                                    printf("Enter name of friend you want to delete: ");
                                    scanf(" %s", friend_to_delete);
                                }

                                if (delete_friend(user, friend_to_delete) == 1) {
                                    printf("\nUpdated List of your friends: \n");
                                    display_user_friends(user);
                                }
                            } 
                            else if (choice_ == 3) {
                                printf("\nReturning to the main menu...\n");
                                break;
                            }
                        } while (choice_ != 3);

                    }else if (login_choice == 4){

                            char username_to_view_posts[30];
                            printf("Enter a friend name to view their posts: ");
                            scanf("%s", username_to_view_posts);

                            // Find the user in the database
                            user_t *friend_to_view_posts = find_user(users, username_to_view_posts);

                            if (friend_to_view_posts == NULL) {
                                
                                printf("\n------------------------------------------------\n");
                                printf("                User not found                    \n");
                                printf("------------------------------------------------\n");
                            }else{
                            
                                // Traverse through the logged-in user's friends list
                                friend_t *user_friends = user->friends;
                                int friend_found = 0;

                                while (user_friends != NULL) {
                                    // If the entered friend name matches one of the user's friends
                                    if (strcmp(user_friends->username, username_to_view_posts) == 0) {
                                        friend_found = 1;
                                        printf("\nDisplaying posts for friend: %s\n", username_to_view_posts);

                                        // Display the posts from the friend's posts pointer 
                                        display_all_user_posts(friend_to_view_posts); // Display posts from the user 
                    
                                    }
                                    user_friends = user_friends->next;
                                }

                                if (friend_found == 0) {
                                    // If the user is not a friend
                                    printf("\n------------------------------------------------\n");
                                    printf("            %s is not your friend.\n", username_to_view_posts);
                                    printf("------------------------------------------------\n");
                                }
                            }
                    }else if (login_choice == 5){
                        printf("\n                GOODBYE       \n\n");
                    }
                }
            }
    }else if(choice == 3){
            printf("\n                GOODBYE       \n\n");
            teardown(users);
            users = NULL; 
            
        }
}
}