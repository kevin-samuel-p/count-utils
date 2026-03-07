# Count

**Count** is a command-line copypaste counting utility.
It supports multiple counting formats for speedrunning counting forums on Discord.

---

# Usage

1. Install the package corresponding to your operating system from Releases.
Supported operating systems include Windows, macOS, and Debian (Ubuntu, Mint).

2. After verifying installation by following the installation steps mentioned below, open a terminal window.

3. Type "count" in the terminal and press Enter. If the application was successfully installed, you'll get a response from the terminal.

---

# Installation

Prebuilt installers and packages are available on the **Releases** page.

Download the file that matches your operating system and follow the instructions below.

---

## Windows

### Install

1. Download the Windows installer:

```
counting-tool-installer.exe
```

2. Run the installer.

3. Follow the installation wizard.

The installer will:

* Install the program
* Install the documentation files
* Add `count` to the system **PATH**

After installation, open **Command Prompt** or **PowerShell** and run:

```
count 
```

If help text appears, the installation succeeded.

---

### Uninstall

1. Open **Settings → Apps → Installed Apps**

2. Find **Counting Tool**

3. Click **Uninstall**

Alternatively use:

```
Control Panel → Programs → Uninstall a program
```

---

### NOTE

For the best experience, it is strongly recommended to run **Counting Tool** in [Windows Terminal](https://aka.ms/terminal).

The traditional **Comand Prompt (`cmd`)** and **Powershell (`powershell.exe`)** consoles often have limited Unicode (special character) rendering and may fail to correctly display certain characters used by this program, such as:

- Japanese characters (Kanji)
- Combining overlines (used in Roman numerals)
- Other extended Unicode symbols

**Windows Terminal** provides proper modern Unicode support and handles these characters correctly.

#### Install Windows Terminal

1. Visit the official page:
https://aka.ms/terminal

2. Install **Windows Terminal** from the Microsoft Store.

3. Launch **Windows Terminal**.

To ensure all characters render correctly:

1. Open **Windows Terminal**.
2. Press **Ctrl + ,** to open **Settings**.
3. Select your default profile (for example **Powershell** or **Command Prompt**).
4. Navigate to **Appearance**.
5. Set **Font face** to a font with strong Unicode coverage, such as:

```
Cascadia Mono
Cascadia Code
Noto Sans Mono
MS Gothic
```

After applying the changes, restart the terminal.

The application should now correctly display all special characters and Unicode output.

## Linux

Two installation methods are available.

---

### Debian / Ubuntu (.deb)

Download:

```
counting_tool_1.0.0.deb
```

Install:

```
sudo dpkg -i count_1.0.0.deb
```

If dependency repair is required:

```
sudo apt -f install
```

Verify installation:

```
count 
```

---

### Portable Archive

Download:

```
counting-tool-linux-x64.tar.gz
```

Extract:

```
tar -xzf counting-tool-linux-x64.tar.gz
```

System-wide installation:

```
sudo mv count /usr/local/bin/
sudo mkdir -p /usr/local/share/count/docs
sudo cp docs/* /usr/local/share/count/docs/
```

Run directly:

```
count
```

---

### Uninstall (Linux)

If installed via `.deb`:

```
sudo dpkg -r count
```

If installed manually:

```
sudo rm /usr/local/bin/count
sudo rm -r /usr/local/share/count
```

---

## macOS

Download:

```
counting_tool_1.0.0.pkg
```

Install:

```
sudo installer -pkg counting_tool_1.0.0.pkg -target /
```

Verify installation:

```
count 
```

---

### Uninstall (macOS)

Remove installed files:

```
sudo rm /usr/local/bin/count
sudo rm -r /usr/local/share/count
```

---

# Usage

Basic command:

```
count
```

View documentation:

```
count help
```

Different counting modes can be selected depending on the desired format.

---

# Documentation

Help pages are bundled with the program and can be accessed through the built-in help system:

```
count help
```

---

# Building From Source

Clone the repository and run:

```
make
```

Platform-specific builds are handled automatically by the provided Makefiles.

---

# License

This project is released under the terms of the license included in the `LICENSE` file.
