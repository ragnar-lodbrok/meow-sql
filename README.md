# MeowSQL

MeowSQL is an attempt to port a very useful GUI database client [HeidiSQL](https://www.heidisql.com/) to C++/Qt (with aim to be run natively at least on Linux)

![MeowSQL Gif](screenshots/meowsql.gif)

## How to build (for developers)

Linux:

1. You need gcc (or other compiler) with c++11 support
2. Qt (tested with 5.5-5.9) and QMake (Optionally: Qt Creator)
3. Clone the repo
4. libmysqlclient library, for deb-based distros: apt-get install libmysqlclient-dev
5. (Optional) Debian: sudo apt-get install mysql-server
6. (Optional) Install test db: https://dev.mysql.com/doc/sakila/en/

Windows:

1. Download and install Qt via online-installer https://www1.qt.io/download-open-source/#section-2
Version that worked for me is Qt 5.6.2 win32-msvc2013
2. You would need a cpp compiler of course, I've got MS Visual Studio 2013 (Community)
3. You should have QMake (plus I used Qt Creator)
4. I've downloaded MySQL Connector C 6.1 (C not C++), and seems put all necessary files into third_party/
5. After build (only release worked for me), copy these dlls to the .exe folder:
  Qt5Widgets.dll
  Qt5Gui.dll
  Qt5Core.dll
  libmysql.dll
  platforms/qwindows.dll

## Version

Current stable version is 0.0.0, but it can connect, show some database info (read-only) and perform your queries

Current development version is 0.0.1

See [roadmap](ROADMAP.md)

## License

This project is licensed under the GPL 2.0 License

## Acknowledgments
* HeidiSQL developers - now on [github](https://github.com/HeidiSQL/HeidiSQL)
