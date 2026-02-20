# holbertonschool-simple_shell
#Hello World!

# Simple Shell (hsh)

A **C-based UNIX command line interpreter** that mimics the behavior of `/bin/sh`.  
Supports executing commands, PATH handling, and built-in commands like `exit` and `env`.

---

## ✨ Features
- 💻 **Interactive Prompt** – Displays `:)` and waits for user input.
- 📂 **Execute Commands** – Run commands using absolute paths or searching the `PATH`.
- 🚪 **exit Built-in** – Exit the shell cleanly.
- 🌐 **env Built-in** – Display all current environment variables.
- ⚠️ **Error Handling** – Correctly prints error if command is not found.
- ⌨️ **End-of-file Handling** – Handles Ctrl+D gracefully.
- 🧹 **Memory Safe** – No memory leaks (tested with valgrind).

---

## ⚙️ Usage

### Interactive mode
```bash
$ ./hsh
:) /bin/ls
file1  file2
:) env
PATH=/usr/local/bin:/usr/bin:/bin
HOME=/home/user
...
:) exit
$
````

### Non-interactive mode

```bash
$ echo "/bin/ls" | ./hsh
file1  file2
```

---

## 🔧 Built-in Commands

* **exit** – exits the shell.
* **env** – prints all current environment variables.

---

## 💻 Installation

Clone the repository and compile:

```bash
git clone https://github.com/yourusername/holbertonschool-simple_shell.git
cd holbertonschool-simple_shell
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 shell.c -o hsh
```

Run:

```bash
./hsh
```

---

## 📝 Authors

See the [AUTHORS](AUTHORS) file for contributors.

---
```

