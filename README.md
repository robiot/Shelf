<h1 align="center">
  <br>
  <img src="https://raw.githubusercontent.com/robi0t/Shelf/main/img/shelf.png" alt="Markdownify" width="300">
  <br>
  Shelf Programming Language
  <br>
</h1>

<h4 align="center">A very simple interpreter that currently supports a few functions ↓ See <a href="https://github.com/robi0t/Shelf/tree/main/examples">Examples</a></h4>

<p align="center">
  <a href="#making-a-hello-world-program">Hello World</a> •
  <a href="#set-a-variable-and-say-it">Set Variable</a> •
  <a href="#define-a-function-and-call-it">Define Function</a> •
  <a href="#wait-until-any-key-is-pressed">Buildin Functions</a> •
  <a href="./LICENSE">License</a>
</p>

<p align="center"><i>--Pre-Alpha</i></p>
<br>




Making a "Hello World" program 
------------------------
First create a .shf file\
The function to output text in the terminal is `say()`
```
say("Hello World");
```
Then you can run it with `./shelf(.exe) file.shf` 

Set a variable and say it
------------------------
The syntax to make a variable is `set variablename = "content";`
```
set name = "John Doe";
say(name);
```

Define a function and call it
------------------------
The syntax to make a function is `define functionname(arguments){};`
```
define _function(name){
  say(name);
};

_function("John Doe");
```
At the moment it does not support no arguments in a function, so you can use none, `define functionname(none)`

Wait until any key is pressed
------------------------
The function to wait until a key is pressed is `waitfkey()`
Example:
```
say("before");
waitfkey("Press any key to continue");
say("after");
```

Get user input
------------------------
The function to get user input is `readl()`
Example:
```
readl("Type your name > ");
```
Currently you cant store this into a variable

---
Under progress, see [Projects](https://github.com/robi0t/Shelf/projects/1) 
