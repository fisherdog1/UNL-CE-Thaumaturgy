gdb is most often used for debugging compiled programs. However, it is also very useful as a first-line investigative tool. In this activity, we will use gdb to find a secret integer that is generated randomly at runtime.

Start by compiling secret.c using the following command. Note the use of the debug flag `-g`

	gcc -g -o secret secret.c
	
We will use gdb to extract the secret integer and enter it when prompted. At this point we can either start the program directly from gdb, or attach gdb to the already running process. We will do the latter first, to better get a hold of gdb's investigative power. After all, in a real use case, we may need to start debugging after something has already gone wrong.

Start the executable with `./secret` in its own terminal. We need to know the PID of the process we just started, so run `ps -a` and look for the name "secret", the number to the left is the running executable's PID.

Start gdb in a separate terminal. You will likely need to run it with `sudo` as we will be attaching to a process that gdb doesn't "own". For this activity, we will use the TUI mode. This mode may not be available depending on your terminal, and this is especially common when debugging embedded systems over serial, so always be prepared to work a bit harder in CLI mode if that is the case.

	gdb --tui
	
Now, attach to the running secret process using the attach command in gdb. Provide the PID that we just looked up.

	(gdb) attach 1337
	
When we attach to a process in this fashion, we will not initially have source code available, because gdb doesn't know what file the process was loaded from. We know where it is though, so we just need to use the symbol-file command to load debugging information.

	(gdb) symbol-file secret
	
Note that gdb uses the directory it was launched from as the *working directory*. If you were one level up in this repository when you launched gdb, your command would instead look like:

	(gdb) symbol-file p1_secret/secret
	
You can see the current working directory using pwd, and change it using cd, just like in a regular old bash terminal. If you started gdb with `sudo` there's a high chance your working directory is currently `/root` You can skip this minor annoyance by running gdb with the `--cd=` option.

	gdb --tui --cd=.

Refresh the source code window by running `list`

	(gdb) list
	
Now we should see the source code almost exactly as it appears in the source file secret.c.

However, the program is not actually in this part of the code. Since we started debugging an in-progress process, we are likely paused several levels deep in the scanf function. Use the backtrace (`bt`) command to see where we are.

At the bottom of our stack trace we can see main. We would like to get the heck out of here, back to main, so we can have access to `int secret`. We can't access it right now even though we can see it in the source code, because it is outside the current stack frame. Remember that it is not valid to access a local variable outside of the function that created it!

Let's put a breakpoint on the line containing the if statement.

	(gdb) b 17
	
Now we can run `continue` to unpause program execution. scanf will still expect us to enter something, so go back to the terminal running secret and enter something, then press enter. Our program will break before the if statement executes.

Now we can see from the gdb window that our program has stopped again. The highlighted line shows what will be run next. Crucially, we are now back in the main function, and can simply read the secret int using the `print` command.

	print secret

Note the number for later. The $1 on the left side of the output is a name for quick reference, which you can use in later commands instead of typing the full number.

Now we just need to continue the program again, and enter our secret number. The `delete` command can be used to delete the breakpoint we created, calling it with no arguments will let you delete all breakpoints.

Go back to the terminal running secret and enter your secret number. Congratulations!

--misc--

If you want to pause a program being debugged by gdb, press Ctrl-C.

--From start--

Start debugging with the `start` command, then use the classic debugging commands to advance to *after* `int secret` is assigned a random number. You can use the \[s\]tep or \[n\]ext instructions, or a \[b\]reakpoint if you're feeling fancy.

