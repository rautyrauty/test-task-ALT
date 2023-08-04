# Test task for ALT
This program is a dynamic library and a command line interface that uses this library. It compares two branches from [altrepo](https://rdb.altlinux.org/api/)  and outputs the result in JSON format to a file
```json
{
  "arch":{
    "only_in_first":{
      "length": 0,
      "packages": []
    },
    "only_in_second":{
      "length": 0,
      "packages": []
    },
    "in_first_release_greater":{
      "length": 0,
      "packages": []
    }
  },
  ...
}
```
"only_in_first"  - all packages that are in the 1st but not in the 2nd

"only_in_second" - all packages that are in the 2nd but they are not in the 1st

"in_first_release_greater" - all packages whose version-release is greater in the 1st than in the 2nd

---

"length" - number of packages

"array" - array of packages

---
packages json format
```json
{
  "name": "string",
  "epoch": 0,
  "version": "string",
  "release": "string",
  "arch": "string",
  "disttag": "string",
  "buildtime": 0,
  "source": "string"
}
```
## Build Linux ALT workstation 10.1

0. Install [CMake](https://cmake.org/), g++, git and clone repository

```
sudo apt-get update
```
```
sudo apt-get install gcc-g++
```
```
sudo apt-get install cmake
```
```
sudo apt-get install git
```
```
sudo git clone https://github.com/rautyrauty/test-task-ALT

```

1. Install [Boost C++ Libraries](boost) and [OpenSSL](https://www.openssl.org/)

```
sudo apt-get install boost-devel
```
```
sudo apt-get install boost-asio-devel
```
```
sudo apt-get install libssl-devel
```

2. Build
```
cd test-task-ALT
sudo cmake .
sudo cmake --build ./
```
If OpenSSL or Boost is installed in a non-standard location you can set options:
```
sudo cmake --build ./ -DOPENSSL_ROOT_DIR=some_dir -DBOOST_ROOT=some_dir
```
to get the paths to boost and openssl, use the "whereis" command:
```
whereis boost
```
```
whereis openssl
```

## Build Linux Ubuntu 22.04

0. Install [CMake](https://cmake.org/)

```bash
sudo apt-get update
sudo apt-get install cmake
```

1. Install [Boost C++ Libraries](boost) and [OpenSSL](https://www.openssl.org/)

```bash
sudo apt-get install libboost-all-dev
sudo apt-get install libssl-dev
```

2. Build
```
cd [root-directory-of-project]
sudo cmake .
sudo cmake --build ./
```
If OpenSSL or Boost is installed in a non-standard location you can set options:
```
sudo cmake --build ./ -DOPENSSL_ROOT_DIR=some_dir -DBOOST_ROOT=some_dir
```
to get the paths to boost and openssl, use the "whereis" command:
```
whereis boost
whereis openssl
```


## Usage
1. Enter the name of the two branches
2. Get the file

## Credits
I am grateful to the creators of the following open source libraries:
  - [Boost C++ Libraries](http://www.boost.org/) (Boost Software License)
  - [OpenSSL](https://www.openssl.org/) (Apache License 2.0)
  - [json](https://github.com/nlohmann/json) (MIT License)
  - [altrepo API v1](https://rdb.altlinux.org/api/) (GNU AGPLv3)
 
 I am grateful to  [ALT](https://www.basealt.ru/)  for a good test assignment and consideration of my resume :)
