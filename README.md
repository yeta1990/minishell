# minishell (42 project)

<p align="center">
<img src="minishell-record.gif">
</p>

## Description

Shell with these features:

-   Able to handle N pipes, redirections to stdin and stdout (including append mode `>>`) and heredocs (`<< EOF`)
-   Error handling (message & error codes `$?`)
-   Parsing closed simple and double quotes as bash does.
-   Set, unset and expand environment variables.
-   Execute commands in 3 ways: call as itself `ls`, relative path and absolute path
-   Bash builtins re-written: echo, cd, env, export, unset and exit builtins
-   Signal handling (Ctrl-C, Ctrl-D)

## Instructions

-   `make && ./minishell`
-   In case a _rl_replace_line_ error appeared, make sure
    1.  you have readline library installed (`brew install readline`)
    2.  it's properly linked in Makefile
