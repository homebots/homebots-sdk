docker pull ghcr.io/homebots/xtensa-gcc:latest
docker run --rm -v$PWD/tests/hello-world:/home/project -v$PWD:/home/homebots-sdk ghcr.io/homebots/xtensa-gcc:latest make