# Test task for ALT

This program compares two branches and outputs the result in JSON format in file

```json
{
  "only_in_first":{
    "length": 0,
    "array": []
  },
  "only_in_second":{
    "length": 0,
    "array": []
  },
  "in_first_release_greater":{
    "length": 0,
    "array": []
  }
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


## Preparation for Build

Install [Boost C++ Libraries](boost) and [OpenSSL](https://www.openssl.org/)

Overwrite OPENSSL_ROOT_DIR and BOOST_ROOT in [CMakeLists.txt](https://github.com/rautyrauty/multithreaded-anti-family-sharing/blob/main/CMakeLists.txt#L10)

## Credits
I am grateful to the creators of the following open source libraries:
  - [Boost C++ Libraries](http://www.boost.org/) (Boost Software License)
  - [OpenSSL](https://www.openssl.org/) (Apache License 2.0)
  - [json](https://github.com/nlohmann/json) (MIT License)
 
 I am grateful to  [company ALT](https://www.basealt.ru/)  for a good test assignment and consideration of my resume :)
