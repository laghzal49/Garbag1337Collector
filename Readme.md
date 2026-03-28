Mini Garbage Collector (42 Project Edition) 🧹
A lightweight memory management system for C projects. It uses a Central Registry (linked list) and a Static Manager to track all allocations, allowing for a single-point cleanup to prevent memory leaks.

🛠️ The Functions

1. get_manger (The Manager) 🎮
   What it does: Houses a static pointer to the head of your linked list.
   How it works: In C, a static variable inside a function is initialized only once and stays alive until the program ends.
   Why we need it: It acts as a "hidden" global variable. This allows any part of your program to access the same memory list without you having to pass a pointer (t_list_gc \*\*) through every single function in your project.

2. ft_malloc (The Wrapper) 🎁
   What it does: Replaces the standard malloc.

It calls the real malloc for the size you requested.

It creates a new "Node" (a small piece of memory) to store that address.

It links that Node into the list managed by get_manger.
Why we need it: It automates the tracking. You don't have to remember which pointers you've created; the collector "registers" them the moment they are born.

3. free_all (The Janitor) 🧹
   What it does: The "Kill Switch."

It loops through the linked list.

For every node, it calls free() on the user's data (address).

It then free()s the node itself.

Finally, it sets the manager's head to NULL.
Why we need it: Instead of writing 100 lines of free() at the end of minishell or cub3d, you call this one function. It ensures a 100% clean heap, even if your program exits unexpectedly due to an error.
