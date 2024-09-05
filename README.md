## RElational DB created with C++!

bullseye is the mock DB file I added to showcase some values. Here are the steps to run the program.

1. Make sure you have the G++ and GCC compilers installed on your system. Find it in your system [here](https://www.mingw-w64.org/downloads/)

2. Run the g++ compiler on main and give the file a name (make sure everything is in the same folder!)
   
   ```g++ .\main.cpp -l sqlite3 -o Database```
This will create a file named Database.exe

3. Run the program. Feel free to find it in file explorer and double click to execute, OR run the following
   ```.\Database.exe```

   The program is setup for ease of use to the user. They are given menu options to add, update, delete, and show certain tables. By following the on screen prompts you will be entering SQL queries without even knowing it! This lets you manipulate the data in various ways.

   I believe in the future it would be more efficient to embed more menus within the system, and give it a GUI. This would make updating tables extremely quick and could have a real world efficient use. Feel free to play around with it.
