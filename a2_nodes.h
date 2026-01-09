/********* a2_nodes.h ********
 * Student Name: Noor Karabala
 * Purpose: Defines node structures for users, friends, and posts.
 */
#ifndef __A2_nodes_H__
#define __A2_nodes_H__

// Structure to represent a linked list of users
typedef struct user
{
    char username[30];
    char password[15];
    struct friend *friends;
    struct post *posts;
    struct user *next;
} user_t;


// Structure to represent a linked list of a user's friends
typedef struct friend
{
    char username[30];
    struct post ** posts; //changes as suggested in the instructions
    struct friend *next;
}
friend_t;


// Structure to represent a linked list of a user's posts
typedef struct post
{
    char content[250];
    struct post *next;
} post_t;

#endif