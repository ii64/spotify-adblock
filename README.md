# Spotify AdBlock

Windows Spotify Ad Block, in `C` !

## Build

Open an issue with information related if any error occurs.

```bash
mingw32-make all
```

Tested gcc:

```
# gcc --version
gcc (MinGW-W64 x86_64-posix-seh, built by Brecht Sanders) 11.2.0
Copyright (C) 2021 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```

## Config example

name it `adblock.yaml`

```yaml
name: Config
cef_acl:
  - method: POST
    substr: /ads/
  - method: POST
    substr: /ad-logic/
block_host:
  - ad.spotify.com
```

## Preview

![preview app](assets/preview.png)
![config file](assets/config.png)
![terminal preview](assets/terminal.png)

## License

[MinHook](https://github.com/TsudaKageyu/minhook/blob/master/LICENSE.txt)

[log.c](https://github.com/rxi/log.c/blob/master/LICENSE)

[cef](https://github.com/chromiumembedded/cef/blob/master/LICENSE.txt)

[chromium](https://chromium.googlesource.com/chromium/src/+/refs/heads/main/LICENSE)

[libcyaml](https://github.com/tlsa/libcyaml/blob/main/LICENSE)

[libyaml](https://github.com/yaml/libyaml/blob/master/License)

## License

ii64. 2021
