# gerrit_helper

### dependency
1. [cpr 1.9.9](https://github.com/libcpr/cpr/tree/1.9.9)
2. [nlohmann_json](https://github.com/nlohmann/json)
3. [cxxopts](https://github.com/jarro2783/cxxopts)

### install
1. mkdir build && cd build
2. cmake ..
3. make -jN
4. make install

### usage
```
gerrit_helper <command> [OPTION...]
Usage:
  gerrit_helper [OPTION...] positional parameters

  -h, --help                print help messages
  -u, --user username       username (default: "")
      --url gerrit url      gerrit url (default: "")
  -p, --password password   password (default: "")
  -f, --file file path      assign a file path (default: "")
  -t, --topic               set topic
  -c, --commit              set commit
      --branch branch name  which branches to pick (default: "")
  -d, --detail              display more details
  -o, --output output file  output result to a file
```