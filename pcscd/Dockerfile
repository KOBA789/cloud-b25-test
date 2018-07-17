FROM alpine:3.8

RUN apk add --no-cache \
    pcsc-lite

ENTRYPOINT ["pcscd", "-f", "--error"]
