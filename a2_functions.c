/*****************
* Student Name: Noor Karabala
*
* Purpose: Implements user, friend, and post operations for the CLI social network.
*/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include <ctype.h>
#include "a2_nodes.h"
#include "a2_functions.h"



/*
   Function that creates a new user and adds it to a sorted (ascending order) linked list at
   the proper sorted location. Return the head of the list.
*/
user_t *add_user(user_t *users, const char *username, const char *password){
    
    user_t *new_user = (user_t *) malloc(sizeof(user_t));
    assert(new_user != NULL);
    
    
    strcpy(new_user->username, username);
    strcpy(new_user->password, password);
    new_user->friends = NULL;  
    new_user->posts = NULL;    
    new_user->next = NULL;

    if (users == NULL || strcmp(username,users ->username) < 0){
        new_user -> next = users;
        users = new_user;
        return new_user;
    }

    user_t *current = users;
    while (current->next != NULL && strcmp(username, current->next->username) > 0) {
        current = current->next;
    }

    new_user->next = current->next;
    current->next = new_user;

    return users;
}


/*
   Function that searches if the user is available in the database 
   Return a pointer to the user if found and NULL if not found.
*/
user_t *find_user(user_t *users, const char *username){

    while (users != NULL) {
        
        if (strcmp(users->username, username) == 0) {
            return users;
        }
        users = users->next;
    }
    return NULL;
}


/*
   Function that creates a new friend's node.
   Return the newly created node.
*/
friend_t *create_friend(const char *username){

    friend_t* new_friend = malloc(sizeof(friend_t));
    assert(new_friend != NULL);
    strcpy(new_friend -> username, username);
    new_friend->posts = NULL;  // No posts initially
    new_friend -> next = NULL;
    return new_friend; 
}


/*
   Function that links a friend to a user. The friend's name should be added into
   a sorted (ascending order) linked list.
*/
void add_friend(user_t *user, const char *friend){


    friend_t* new_friend = create_friend(friend);

    // Linking the friend's posts to this new friend
    user_t *friend_user = find_user(user, friend);
    new_friend->posts = &(friend_user->posts);  // Point to the friend's posts list

    if (user->friends == NULL || strcmp(friend , user->friends->username) < 0){
        new_friend -> next = user -> friends;
        user -> friends = new_friend;
        return;
    }
    
    friend_t *current = user->friends;
    while (current->next != NULL && strcmp(friend, current->next->username) > 0) {
        current = current->next;
    }

    new_friend->next = current->next;
    current->next = new_friend;

}


/*
   Function that removes a friend from a user's friend list.
   Return true if the friend was deleted and false otherwise.
*/
_Bool delete_friend(user_t *user, char *friend_name){
    
    if (user == NULL || user->friends == NULL) {
        return 0;
    }

    friend_t *current = user->friends;
    friend_t *previous = NULL;

    if (strcmp(current->username, friend_name) == 0) {
        user->friends = current->next; 
        free(current); 
        return 1; 
    }

    while (current != NULL && strcmp(current->username, friend_name) != 0) {
        previous = current;
        current = current->next;
    }

    if (current == NULL) {
        printf("\nInvalid friend's name\n");
        return 0; 
    }

    previous->next = current->next; 
    free(current);
    return 1; 
}


/*
   Function that creates a new user's post.
   Return the newly created post.
*/
post_t *create_post(const char *text){

    post_t *new_post = malloc(sizeof(post_t));
    assert(new_post != NULL);
    strncpy(new_post->content, text, sizeof(new_post->content) - 1);
    new_post->content[sizeof(new_post->content) - 1] = '\0';
    new_post->next = NULL;
    return new_post;
}


/*
   Function that adds a post to a user's timeline following a stack.
*/
void add_post(user_t *user, const char *text){

    post_t *new_post = create_post(text);
    new_post -> next = user -> posts;
    user -> posts = new_post;
    
}


/*
   Function that removes a post from a user's list of posts.
   Return true if the post was deleted and false otherwise.
*/
_Bool delete_post(user_t *user){

    if (user == NULL || user->posts == NULL) {
        return 0;
    }

    post_t *to_delete = user->posts;
    user -> posts = (user -> posts)->next;
    free(to_delete);
    return 1;
}


/*
   Function that  displays a specific user's posts (All posts)
*/
void display_all_user_posts(user_t *user){
        
    if (user == NULL || user->posts == NULL) {
        printf("\nNo posts to display.\n");
        return;
    }

    
    post_t *current_post = user->posts;
    int i = 1;
    while (current_post != NULL) {
        printf("\n%d- %s\n",i, current_post->content);  
        current_post = current_post->next;
        i += 1;
    }
}


/*
   Function that displays a specific user's friends (All friends)
*/
void display_user_friends(user_t *user){
    
    if (user == NULL || user->friends == NULL) {
        printf("No friends to display.\n");
        return;
    }

    friend_t *current_friend = user->friends;
    
    int i = 0;

    while (current_friend != NULL) {
        i += 1;
        printf("%d-  %s\n", i,current_friend->username);
        current_friend = current_friend->next;     
    }
}


/*
   Function that displays "number" of posts for a given user.
   After displaying "number" posts, it prompts if you want to display
   more posts.
   If there are no more post or the user types “n” or “N”, the function returns. 
*/
void display_posts_by_n(user_t *users, int number)
{
    static post_t *current_post = NULL; // Static pointer to track progress across recursive calls

    if (users == NULL) {
        printf("User not found.\n");
        return;
    }

    
    if (current_post == NULL) {
        current_post = users->posts;
    }

    int count = 0;

    // Display up to `number` posts from the current position
    while (current_post != NULL && count < number) {
        printf("\n%s\n", current_post->content);
        current_post = current_post->next;
        count++;
    }

    // Check if there are more posts to display
    if (current_post != NULL) {
        char choice;
        printf("\nDo you want to see more posts? (y/n): ");
        scanf(" %c", &choice);

        if (choice == 'y' || choice == 'Y') {
            
            display_posts_by_n(users, number);
        } else {
            
            current_post = NULL;
            printf("Returning to main menu...\n");
        }
    } else {
        printf("\nNo more posts to display.\n");
        
        current_post = NULL;
    }
}


/*
   Function that free all users from the database before quitting the application.
*/
void teardown(user_t *users)
{
    while (users != NULL) 
    {

        user_t *user_to_free = users;

        // Free all friends of the current user
        while (user_to_free->friends != NULL) {
            friend_t *to_free = user_to_free->friends;
            user_to_free->friends = user_to_free->friends->next;
            free(to_free);
            to_free = NULL;
        }

        // Free all posts of the current user
        while (user_to_free->posts != NULL) {
            post_t *to_free_ = user_to_free->posts;
            user_to_free->posts = user_to_free->posts->next;
            free(to_free_);
            to_free_ = NULL;
            
        }

        // Free the current user and move to the next
        
        users = users->next;
        free(user_to_free);
        user_to_free = NULL;
        
    } 
}


/*
   Function that prints the main menu with a list of options for the user to choose from
*/
void print_menu(){

    printf("\n***********************************************\n");
    printf("        Welcome to Text-Based Facebook:                 \n");
    printf("***********************************************\n\n");
    printf("\n***********************************************\n");
    printf("                    MAIN MENU:                 \n");
    printf("***********************************************\n");
    printf("1. Register a new user\n");
    printf("2. Login with existing user's information\n");
    printf("3. Exit\n");
    printf("\nEnter your choice: ");
    
}


/* 
   Get unsigned short integer user input.
   The value entered by the user must be from 1 to parameter n.
   If the user enters a wrong value, the function asks user to re-enter a correct value.
*/
unsigned short int get_input_usi(int n){
	signed int user_input;
	scanf("%i", &user_input);

	while(user_input <= 0 || user_input > n){
		printf("\n-----------------------------------------------------\n");
		printf("The number you entered is not from 1 to %d\n",n);
		printf("Enter a number from 1 to %d: ",n);
		scanf("%i", &user_input);
	}

	return (unsigned short int) user_input;
}


/*
   Function that prints the main menu with a list of options for the user to choose from
*/
void print_login_menu(const char * username){

    printf("\n***********************************************\n");
    printf("                Welcome %s:                 \n",username);
    printf("***********************************************\n");
    
    printf("1. Manage a user's profile (change password)\n");
    printf("2. Manage a user's posts (add/remove)\n");
    printf("3. Manage a user's friends (add/remove)\n");
    printf("4. Display a friends's posts\n");
    printf("5. Exit\n");
    printf("\nEnter your choice: ");
    
}

// Function to convert a string to lowercase
void to_lowercase(char *username) {
    
    for (int i = 0; username[i] != '\0'; i++) {
        username[i] = tolower(username[i]);
    }

}


user_t *read_CSV_and_create_users(FILE *file, int num_users)
{
    srand(time(NULL));
    user_t *users = NULL;
    char buffer[500];
    fgets(buffer, sizeof(buffer), file); // Read and discard the header line
    int count = 0;
    for (int i = 0; i < num_users; i++)
    {
        fgets(buffer, sizeof(buffer), file);
        buffer[strcspn(buffer, "\r\n")] = 0; // Remove newline characters

        char *token = strtok(buffer, ",");
        char *token2 = strtok(NULL, ",");
        users = add_user(users, token, token2);
        char *username = token;

        token = strtok(NULL, ",");

        user_t *current_user = users;
        for (; current_user != NULL && strcmp(current_user->username, username) != 0; current_user = current_user->next)
            ;

        while (token != NULL && strcmp(token, ",") != 0 && count < 3)
        {
            if (strcmp(token, " ") != 0)
            {
                add_friend(current_user, token);
            }
            token = strtok(NULL, ",");
            count++;
        }
        count = 0;

        // token = strtok(NULL, ",");
        while (token != NULL && strcmp(token, ",") != 0)
        {
            add_post(current_user, token);
            token = strtok(NULL, ",");
        }
    }
    return users;
}
