# MeowSQL

MeowSQL is an attempt to port a very useful GUI database client [HeidiSQL](https://www.heidisql.com/) to C++/Qt (that can be run natively at least on Linux)

![MeowSQL Gif](screenshots/meowsql.gif)

## How to build (for developers)

1. You need gcc (or other compiler) with c++11 support
2. Qt and QMake (Optionally: Qt Creator)
3. Clone the repo
4. libmysqlclient library, for deb-based distros: apt-get install libmysqlclient-dev
5. (Optional) Debian: sudo apt-get install mysql-server
6. (Optional) Install test db: https://dev.mysql.com/doc/sakila/en/

## Version

Current stable version is 0.0.0, but it can connect, show some database info (read-only) and perform your queries

Current development version is 0.0.1

See [roadmap](ROADMAP.md)

## License

This project is licensed under the GPL 2.0 License

## Acknowledgments
* HeidiSQL developers - now on [github](https://github.com/HeidiSQL/HeidiSQL)
