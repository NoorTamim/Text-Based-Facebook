# Text-Based Facebook

A command-line social network application written in C that allows users to register, log in, manage friends, create posts, and view posts using linked lists and stacks.

## Features
- User registration with unique usernames
- Login system with password validation
- Add and remove friends (sorted linked list)
- Add and remove posts (stack-based)
- View posts from friends
- Load initial users from a CSV file
- Menu-driven command-line interface

## Data Structures
- **Users**: Stored in a sorted linked list
- **Friends**: Stored in a sorted linked list per user
- **Posts**: Stored as a stack (last-in, first-out)

## Files
- `main.c` — Program entry point and menu logic
- `a2_functions.c` — User, friend, and post operations
- `a2_functions.h` — Function declarations
- `a2_nodes.h` — Data structure definitions
- `user_details.csv` — Initial user data
